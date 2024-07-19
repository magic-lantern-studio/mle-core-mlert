/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleGroup.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleGroup.h"
#include "mle/MleLoad.h"
#include "mle/MleRole.h"
#include "mle/MleSet.h"

#ifdef MLE_DIGITAL_WORKPRINT

// Include system header files.
#include <stdio.h>

// Include all the workprint stuff group loading needs to deal with.
#include "mle/DwpActor.h"
#include "mle/DwpFinder.h"

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Include system header files.
#ifdef WIN32
#include <string.h>
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlFileio.h"
#include "mle/mlMalloc.h"

#include "mle/ppinput.h"
#include "mle/MleTables.h"

#endif /* MLE_DIGITAL_PLAYPRINT */

// Base class implementation for Groups.
//
// This implementation does not anticipate supporting subclasses of
// groups in the workprint or the tools.

//----------------------------------------------------------------------
// Constructors and destructors

// Default constructor makes sure that the actors array has some storage.


MleGroup::MleGroup(int initSize) : MlePtrContainer<MleActor*>(initSize)
{
#ifdef MLE_DIGITAL_WORKPRINT
	m_name = NULL;
	m_groupClass = NULL;
#endif /* MLE_DIGITAL_WORKPRINT */
}

MleGroup::MleGroup(void) : MlePtrContainer<MleActor*>(8)
{
#ifdef MLE_DIGITAL_WORKPRINT
	m_name = NULL;
	m_groupClass = NULL;
#endif /* MLE_DIGITAL_WORKPRINT */
}

MleGroup::~MleGroup(void)
{
	// Delete all of our actors
	for (int i = 0; i < getSize(); i++)
	{
		delete (*this)[i];
	}

#ifdef MLE_DIGITAL_WORKPRINT
	this->unregisterInstance();
	if (m_name) mlFree(m_name);
#endif /* MLE_DIGITAL_WORKPRINT */
}
  
// Do nothing yet
void
MleGroup::init(void)
{}

void *
MleGroup::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleGroup::operator delete(void *p)
{
	mlFree(p);
}

MleGroup *
_mlCreateMleGroup(void)
{
    return new MleGroup;
}

#ifdef MLE_DIGITAL_WORKPRINT
const char *
MleGroup::getTypeName(void) const
{
    return "MleGroup";
}

int
MleGroup::isa(const char *type) const
{
    return !strcmp(type,"MleGroup");
}

void
MleGroup::initClass(void)
{
    new MleGroupClass("MleGroup",_mlCreateMleGroup,"");
}

MleDwpStrKeyDict MleGroup::g_instanceRegistry;

// Registering the instance with rehearsal player.
void
MleGroup::registerInstance(const char* name)
{
#ifdef WIN32
   this->m_name = _strdup(name);
#else
   this->m_name = strdup(name);
#endif
   g_instanceRegistry.set(this->m_name, this);
}

// Unregister ourselves from registry.
void
MleGroup::unregisterInstance()
{
    // Eliminate from instance registry.
    // XXX - must iterate through dict.  We could have registered this twice,
    // or we could put a removeValue in the dict class

    for (MleDwpDictIter iter(g_instanceRegistry); iter.getValue(); iter.next()) {
		if ((MleGroup*) iter.getValue() == this) {
			g_instanceRegistry.remove(iter.getKey());
			break;
		}
    }
}

// This function returns the group class of an group.
// The group class object is defined in mle/MleGroupClass.h.  It contains
// information about the class, like a creation function.
const MleGroupClass *
MleGroup::getClass(void)
{
	// If there is a cached value, return it.
	if ( m_groupClass )
		return m_groupClass;
	
	// Look it up in the registry.
	m_groupClass = MleGroupClass::find(getTypeName());

	return m_groupClass;
}

// Load the group from the relevant item in the DWP.
// Use different binding types depending on whether the DWP is 
// a playprint or workprint.  In the workprint, we use the WP struct.
// Note that this static function calls the mlLoadGroup() function
// in load.cxx, which is able to load groups of various types, eg it 
// is a group "class factory."

extern MleGroup* mlLoadGroup(MleDwpGroup* wpGroup);
extern MleGroup* mlLoadGroup(const char* id);

MleGroup *
MleGroup::load(MleDwpGroup* wpGroup)
{
	if ( wpGroup == NULL ) {
		printf("mlLoadGroup: Cant find group in workprint.\n");
		return NULL;
	}

	return mlLoadGroup( wpGroup );
}

MleGroup *
MleGroup::load(const char* id)
{
	return mlLoadGroup( id );
}

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// In the playprint, we use the compiled table of group identifiers.

extern MleGroup* mlLoadGroup(const int indexToPpTOC);

MleGroup*
MleGroup::load(const int indexToPpTOC)
{
	return mlLoadGroup( indexToPpTOC );
}

#endif /* MLE_DIGITAL_PLAYPRINT */
