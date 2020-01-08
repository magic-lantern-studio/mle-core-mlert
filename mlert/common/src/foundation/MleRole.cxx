/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleRole.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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


// Include Magic Lanter header files.
#include "mle/mlTypes.h"
#include "mle/mlMalloc.h"

// Include Runtime Engine header files.
#include "mle/MleRole.h"


// Declare creation function used by runtime engine.
#ifdef MLE_DIGITAL_PLAYPRINT
extern MleRole *_mleCreateMleRole(MleActor *actor)
{
    return new MleRole(actor);
}
#endif /* MLE_DIGITAL_PLAYPRINT */


void MleRole::init(void)
{
    // Do nothing in the base class.
}


// When an role is destroyed, we ensure the attached actor knows 
// about the deletion.  However, if the actor is already being 
// destroyed we don't want call the notify function.
MleRole::~MleRole()
{
    MLE_ASSERT(m_actor);

    // Check to make sure actor not in process of being destroyed.
    if (  m_actor->getRole() != NULL )
    {
        // Verify that the actor is attached to this role.
        MLE_ASSERT( m_actor->getRole() == this);
        
        m_actor->removeRole();
    }
}


void *
MleRole::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleRole::operator delete(void *p)
{
	mlFree(p);
}


void
MleRole::addChild(MleRole * /* child */)
{
    // Do nothing in the base class.
}

void
MleRole::setAttribute(const char * /* name */, void * /* value */)
{
	// Do nothing in the base class.
}

void *
MleRole::getAttribute(const char * /* name */)
{
	return NULL;
}

#ifdef MLE_DIGITAL_WORKPRINT

const char *
MleRole::getTypeName(void) const
{
	return "MleRole";
}

int
MleRole::isa(const char *type) const
{
	return !strcmp(type,"MleRole");
}

MleDwpDict MleRole::g_pickRegistry;

void 
MleRole::setIdentifier(const char* /*id*/)
{
}

#endif /* MLE_DIGITAL_WORKPRINT */
