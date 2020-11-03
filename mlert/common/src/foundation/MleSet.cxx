/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleSet.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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


// Include standard header files.
#include <stdlib.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Runtime Engine header files.
#include "mle/MleSet.h"


// This static is valid during actor set loads.  Actors and roles
// instantiated from a group may refer to this variable in their
// constructor to get a handle to the set that contains them.
MleSet *MleSet::g_currentSet;


void MleSet::getProperty(MleObject */*object*/, const char */*name*/, unsigned char **value)
{ value = 0; }


void MleSet::setProperty(MleObject */*object*/, const char */*name*/, unsigned char */*value*/)
{}


MleSet::MleSet(void)
  : MleObject()
{
#ifdef MLE_DIGITAL_WORKPRINT
    m_name = NULL;
    m_setClass = NULL;
#endif /* MLE_DIGITAL_WORKPRINT */
}


MleSet::~MleSet(void)
{
#ifdef MLE_DIGITAL_WORKPRINT
    unregisterInstance();
    if (m_name)
		mlFree(m_name);
#endif /* MLE_REHEARSAL */
}


void
MleSet::init(void)
{
	// The base class does nothing.
}


void *
MleSet::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleSet::operator delete(void *p)
{
	mlFree(p);
}


void
MleSet::attach(MleRole * /* parent */, MleRole * /* child */)
{
	// Does nothing in the base class.
}


MleSet *_mlCreateMleSet(void)
{
    return new MleSet;
}

// Include system header files.
#include <stdio.h>
#include <string.h>

int
MleSet::isa(const char *type) const
{
    return !strcmp(type,"MleSet");
}

#ifdef MLE_DIGITAL_WORKPRINT

#include "mle/DwpDataUnion.h"
#include "mle/DwpDatatype.h"

MleDwpStrKeyDict MleSet::g_instanceRegistry;

const char *
MleSet::getTypeName(void) const
{
    return "MleSet";
}

void
MleSet::initClass(void)
{
    new MleSetClass("MleSet", _mlCreateMleSet, "");
}

// This function returns the set class of a set.
// The set class object is defined in mle/MleSetClass.h.  It contains
// information about the class, like a creation function, and it is
// a dictionary of the set properties registered with
// mlRegisterSetMember.
const MleSetClass *
MleSet::getClass(void)
{
    // If there is a cached value, return it.
    if ( m_setClass )
		return m_setClass;

    // Look it up in the registry.
    m_setClass = MleSetClass::find(getTypeName());

    return m_setClass;
}

int
MleSet::poke(const char *property,MleDwpDataUnion *value)
{
    MLE_ASSERT(property);
    MLE_ASSERT(value);

    // Make sure the set class is present.
    if ( getClass() == NULL )
	    return 0;

    // Get the member corresponding to the property name.
    const MleSetMember *member = m_setClass->findMember(property);
    if ( member == NULL )
    {
	    printf("property %s not present on %s.\n",
		    property,getTypeName());
	    return 1;
    }

    // Transfer the value.
    MLE_ASSERT(value->m_datatype);

    if ( value->m_datatype != member->getType() )
    {
		printf("type mismatch in setting property %s on %s (%s and %s).\n",
			property,getTypeName(),
			value->m_datatype->getName(),
			member->getType()->getName());
		return 1;
    }

    //value->m_datatype->get(value,(char *)this + member->getOffset());
	MlePropertyEntry *entry = member->getEntry();
	// TBD: the following will probably not work without some munging of MleDwpDataUnion. Need testing.
	entry->setProperty(this, entry->name, (unsigned char *)value);

    return 0;
}

void
MleSet::resolveEdit(const char *)
{
}

// Set a set's name, which also causes re-registration under new name.
void
MleSet::setName(char *newName)
{
    unregisterInstance();
    
    if (m_name)
		mlFree(m_name);

    registerInstance(newName);
}

/**
 * Register the instance with rehearsal player.
 *
 * @param n The name of the instance.
 */
void
MleSet::registerInstance(const char* n)
{
   m_name = strdup(n);
   g_instanceRegistry.set(m_name, this);
}

/**
 * Unregister ourselves from registry.
 */
void
MleSet::unregisterInstance(void)
{
    // Eliminate from instance registry.
    // XXX - must iterate through dict.  We could have registered this twice,
    // or we could put a removeValue in the dict class

    for (MleDwpDictIter iter(g_instanceRegistry); iter.getValue(); iter.next()) {
		if ((MleSet*) iter.getValue() == this) {
			g_instanceRegistry.remove(iter.getKey());
			break;
		}
    }
}

#endif /* MLE_DIGITAL_WORKPRINT */

#ifdef MLE_REHEARSAL

MleActor*
MleSet::pick(int /* x */, int /* y*/)
{
    return(NULL);
}

const char** MleSet::getFunctions()
{
    return(NULL);
}

const char** MleSet::getFunctionAttributes(char* /*functionName*/)
{
    return(NULL);
}

// Actor layering.
int 
MleSet::pushActor(MleActor* /*actor*/)
{
    return(-1);
}

int 
MleSet::pushActorToBottom(MleActor* /*actor*/)
{
    return(-1);
}

int 
MleSet::popActor(MleActor* /*actor*/)
{
    return(-1);
}

int 
MleSet::popActorToTop(MleActor* /*actor*/)
{
    return(-1);
}


// Actor render modes.
void 
MleSet::setRenderMode(MleActor* /*actor*/, char* /*mode*/)
{
}

char* 
MleSet::getRenderMode(MleActor* /*actor*/)
{
    return(NULL);
}


// Global render mode.
void 
MleSet::setRenderMode(char* /*mode*/)
{
}

char* 
MleSet::getRenderMode()
{
    return(NULL);
}

#endif /* MLE_REHEARSAL */
