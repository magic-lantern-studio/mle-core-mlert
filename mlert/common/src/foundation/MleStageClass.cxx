/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleStageClass.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source code, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Runtime Engine header files.
#include "mle/MleDsoLoader.h"
#include "mle/MleStageClass.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"

//
// Declare the registry for stage classes.  This registry is indexed by
// stage class names and holds a pointer to a function that creates an
// instance of the stage class.
//
#if defined(_WINDOWS)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleStageClassDict MleStageClass::g_registry(16);
#if defined(_WINDOWS)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif

//
// Declare the DSO loader for stage classes.
//
MleDSOLoader MleStageClass::g_loader;


//
// Constructor.
//
MleStageClass::MleStageClass(const char *name, MleStage *(*c)(void),
   const char *superclass)
: MleDwpStrKeyDict()
{
    //
    // Remember the creation function.
    //
    create = c;

    //
    // Look up the superclass info.
    //
    MleStageClass *super = (MleStageClass *) g_registry.find(superclass);

    //
    // Copy the superclass members into the local space.
    //
    if (super)
   {
      MleStageMemberIter iter(super);
      const MleStageMember *member;

        while ( (member = iter.getMember()) )
      {
         set(iter.getName(), new MleStageMember(*member));
         iter.next();
      }
    }

    //
    // Put self into the registry.
    // 
    g_registry.set(name, this);
}


//
// This function returns a new instance of a stage of this class.
//
MleStage *
MleStageClass::createInstance(void) const
{
    return (*create)();
}


//
// This function adds a new property type to the stage class.
//
void
MleStageClass::addMember(const char *name, const char *type, MlePropertyEntry *entry)
{
    //
    // Create the new member object.
    //
    MleStageMember *member = new MleStageMember(type, entry);

    //
    // Put the new property into the dictionary.
    //
    set(name, member);
}


//
// This function looks up a property.
//
const MleStageMember *
MleStageClass::findMember(const char *name) const
{
    //
    // Get the pointer from the registry.
    //
    return (const MleStageMember *) MleDwpStrKeyDict::find(name);
}


//
// This static function looks up an stage class from a name.
//
MleStageClass *
MleStageClass::find(const char *name)
{
    MleStageClass *stageClass;

    //
    // Try to look up the stage class in the registry.
    //
    stageClass = (MleStageClass *) g_registry.find(name);

    //
    // Return the stage class if we found it.
    //
    if(stageClass)
   return stageClass;
    
    //
    // Otherwise, try to load the class from a DSO.
    //
    g_loader.loadClass(name);

    //
    // Now try to look it up again.
    //
    stageClass = (MleStageClass *) g_registry.find(name);

    //
    // Return it whether we succeeded or not.
    return stageClass;
}


//
// This defines the MleStageMemberEntry subclass for a dictionary
// of MleStageMember pointers.  All it knows how to do is delete the
// MleStageMember pointer when the dictionary entry is deleted.
//
class MleStageMemberEntry : public MleDwpStrKeyDictEntry
{
   public:

      ~MleStageMemberEntry()
      { delete ((MleStageMember *) m_value); }
};


//
// This function overrides MleDwpStrKeyDict::makeEntry() to return an entry
// of the MleStageMemberEntry.
//
MleDwpDictEntry *
MleStageClass::makeEntry(void)
{
    return (new MleStageMemberEntry);
}

void *
MleStageClass::operator new(size_t tSize)
{
   void *p = mlMalloc(tSize);
   return p;
}

void
MleStageClass::operator delete(void *p)
{
   mlFree(p);
}

//
// This defines the MleStageClassDictEntry subclass for a dictionary
// of MleStageClass pointers.  All it knows how to do is delete the
// MleStageClass pointer when the dictionary entry is deleted.
//
class MleStageClassDictEntry : public MleDwpStrKeyDictEntry
{
  public:

   /**
    * The desctructor.
    */
   ~MleStageClassDictEntry() { delete ((MleStageClass *) m_value); }

   /**
    * Override operator new.
    *
    * @param tSize The size, in bytes, to allocate.
    */
   void* operator new(size_t tSize);

   /**
    * Override operator delete.
    *
    * @param p A pointer to the memory to delete.
    */
    void  operator delete(void *p);
};

void *
MleStageClassDictEntry::operator new(size_t tSize)
{
   void *p = mlMalloc(tSize);
   return p;
}

void
MleStageClassDictEntry::operator delete(void *p)
{
   mlFree(p);
}

//
// This function overrides MleDwpStrKeyDict::makeEntry() to return an entry
// of the MleStageClassDictEntry.
//
MleDwpDictEntry *
MleStageClassDict::makeEntry(void)
{
    return (new MleStageClassDictEntry);
}

void *
MleStageClassDict::operator new(size_t tSize)
{
   void *p = mlMalloc(tSize);
   return p;
}

void
MleStageClassDict::operator delete(void *p)
{
   mlFree(p);
}

MleStageMember::MleStageMember(const char *t, MlePropertyEntry *e)
{
    m_type = MleDwpDatatype::findType(t);

    if (m_type == NULL)
   {
      fprintf(stderr,
         "The workprint data type \"%s\" could not be found.\n", t);
      MLE_ASSERT(m_type != NULL);
    }
    
    m_entry = e;
}

void *
MleStageMember::operator new(size_t tSize)
{
   void *p = mlMalloc(tSize);
   return p;
}

void
MleStageMember::operator delete(void *p)
{
   mlFree(p);
}

void *
MleStageMemberIter::operator new(size_t tSize)
{
   void *p = mlMalloc(tSize);
   return p;
}

void
MleStageMemberIter::operator delete(void *p)
{
   mlFree(p);
}
