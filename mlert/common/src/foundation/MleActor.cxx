/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActor.cxx
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

// Include system header files.
#include <string.h>
#ifdef MLE_DIGITAL_WORKPRINT
#include <stdio.h>
#endif /* MLE_DIGITAL_WORKPRINT */

// Include Magic Lantern header files.
#include "mle/MleActor.h"
#include "mle/MleRole.h"
#include "mle/mlMalloc.h"

void MleActor::getProperty(MleObject */*object*/, const char */*name*/, unsigned char **value)
{
	value = 0;
    // ToDo: Log that default getter is being used.
}

void MleActor::setProperty(MleObject */*object*/, const char */*name*/, unsigned char */*value*/)
{
    // ToDo: Log that default setter is being used.
}

MleActor::MleActor(void)
  : MleObject()
{
    // variable initialization
    m_role = NULL;

#ifdef MLE_DIGITAL_WORKPRINT
    m_name = NULL;
    m_actorClass = NULL;
#endif /* MLE_DIGITAL_WORKPRINT */
}


// The base class for actors destroys an attached role, if one
// exists.  This is driven by the fact that roles, once constructed, 
// can not be attached to a different actor.
MleActor::~MleActor()
{
    // Check for attached role
    if (m_role) 
    {
        // Verify that the role is attached to this actor
	    MLE_ASSERT (m_role->getActor() == this);

	    // Set role_ to NULL to avoid role attempting
	    // to call the virtual function removeRole on actor.
	    MleRole* curRole = m_role;
	    m_role = NULL;		
	    delete curRole;
    }

#ifdef MLE_DIGITAL_WORKPRINT
    this->unregisterInstance();
    if (m_name) mlFree(m_name);
#endif /* MLE_DIGITAL_WORKPRINT */
}

// This is the virtual init function that is called after property values
// are poked into actor memory.  This does nothing in the base class;
// subclasses should override to make the state consistent.
void
MleActor::init(void)
{
	// Does nothing in base class.
}

void *
MleActor::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActor::operator delete(void *p)
{
	mlFree(p);
}

void MleActor::attachRole(MleRole* newRole)
{ 	
    MLE_ASSERT(!m_role);
    m_role = newRole;
}


void MleActor::removeRole()
{
    MLE_ASSERT(m_role);
    m_role = NULL;
}


MleActor *_mleCreateMleActor(void)
{
    return new MleActor;
}

#ifdef MLE_DIGITAL_WORKPRINT
const char *
MleActor::getTypeName(void) const
{
    return "MleActor";
}

int
MleActor::isa(const char *type) const
{
    return !strcmp(type,"MleActor");
}

void
MleActor::initClass(void)
{
    new MleActorClass("MleActor",_mleCreateMleActor,"");
}
#endif /* MLE_DIGITAL_WORKPRINT */

#ifdef MLE_DIGITAL_WORKPRINT
// Include Digital Workprint header files.
#include "mle/DwpDataUnion.h"
#include "mle/DwpDatatype.h"
#include "mle/DwpFloatArray.h"
#include "mle/DwpIntArray.h"
#include "mle/DwpScalarArray.h"
#include "mle/DwpVector3Array.h"
#include "mle/DwpVector2.h"
#include "mle/DwpVector3.h"
#include "mle/DwpVector4.h"
#include "mle/DwpTransform.h"
#include "mle/DwpRotation.h"
#include "mle/DwpString.h"
#include "math/scalar.h"
#include "math/vector.h"
#include "math/transfrm.h"

#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleDwpStrKeyDict MleActor::g_instanceRegistry;
#if defined(WIN32)
#pragma data_seg()
#pragma comment("linker, /section:.GLOBALS,rws")
#endif

// Registering the instance with rehearsal player.
void
MleActor::registerInstance(const char* name)
{
   this->m_name = strdup(name);
   g_instanceRegistry.set(this->m_name, this);
}

// Unregister ourselves from registry.
void
MleActor::unregisterInstance()
{
    // Eliminate from instance registry.

    // XXX - must iterate through dict.  We could have registered this twice,
    // or we could put a removeValue in the dict class.

    for (MleDwpDictIter iter(g_instanceRegistry); iter.getValue(); iter.next())
	{
		if ((MleActor*) iter.getValue() == this)
		{
			g_instanceRegistry.remove(iter.getKey());
			break;
		}
    }
}

// Change an actor's name, which also causes re-registration under
// new name.
void
MleActor::setName(char *newName)
{
    unregisterInstance();
    
    if (m_name)
		mlFree(m_name);

    registerInstance(newName);
}

// This function returns the actor class of an actor.
//   The actor class object is defined in mle/MleActorClass.h.  It contains
//   information about the class, like a creation function, and it is
//   a dictionary of the Actor properties registered with
//   mleRegisterActorMember.
const MleActorClass *
MleActor::getClass(void)
{
	// If there is a cached value, return it.
	if ( m_actorClass )
		return m_actorClass;
	
	// Look it up in the registry.
	m_actorClass = MleActorClass::find(getTypeName());

	return m_actorClass;
}

int
MleActor::poke(const char *property,MleDwpDataUnion *value)
{
	MLE_ASSERT(property);
	MLE_ASSERT(value);

	// Make sure the actor class is present.
	if ( getClass() == NULL )
		return 0;

	// Get the member corresponding to the property name.
	const MleActorMember *member = m_actorClass->findMember(property);
	if ( member == NULL )
	{
		printf("MleActor: property %s not present on %s.\n",
			property,getTypeName());
		return 1;
	}

	// Transfer the value.
	MLE_ASSERT(value->m_datatype);

	//if (value->m_datatype != member->getType())
	int cmp = strcmp(value->m_datatype->getName(),member->getType()->getName());
	if (cmp != 0)
	{
        printf("MleActor: type mismatch in setting property %s on %s (%s and %s).\n",
               property, getTypeName(),
               value->m_datatype->getName(),
               member->getType()->getName());
	    return 1;
	}

	//value->m_datatype->get(value,(char *)this + member->getOffset());
	MlePropertyEntry *entry = member->getEntry();
	unsigned char *pvalue;
	//if (member->getType()->isa(MleDwpFloatArray::typeId)) {
	if (! strcmp(member->getType()->getName(), "FloatArray")) {
		const MleDwpFloatArray *dwpFloatArray = (MleDwpFloatArray *)member->getType();
		MleArray<float> *array = new MleArray<float>;
		dwpFloatArray->get(value, array);
		pvalue = (unsigned char *)array;
	//} else if (member->getType()->isa(MleDwpIntArray::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "IntArray")) {
		const MleDwpIntArray *dwpIntArray = (MleDwpIntArray *)member->getType();
		MleArray<int> *array = new MleArray<int>;
		dwpIntArray->get(value, array);
		pvalue = (unsigned char *)array;
	//} else if (member->getType()->isa(MleDwpScalarArray::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "ScalarArray")) {
		const MleDwpScalarArray *dwpScalarArray = (MleDwpScalarArray *)member->getType();
		MleArray<MlScalar> *array = new MleArray<MlScalar>;
		dwpScalarArray->get(value, array);
		pvalue = (unsigned char *)array;
	//} else if (member->getType()->isa(MleDwpVector3Array::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "Vector3Array")) {
		const MleDwpVector3Array *dwpVec3Array = (MleDwpVector3Array *)member->getType();
		MleArray<MlVector3> *array = new MleArray<MlVector3>;
		dwpVec3Array->get(value, array);
		pvalue = (unsigned char *)array;
	//} else if (member->getType()->isa(MleDwpVector2::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "MlVector2")) {
        const MleDwpVector2 *dwpVec2 = (MleDwpVector2 *)member->getType();
        MlVector2 *vector = new MlVector2();
        dwpVec2->get(value, vector);
        pvalue = (unsigned char *)vector;
	//} else if (member->getType()->isa(MleDwpVector3::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "MlVector3")) {
        const MleDwpVector3 *dwpVec3 = (MleDwpVector3 *)member->getType();
        MlVector3 *vector = new MlVector3();
        dwpVec3->get(value, vector);
        pvalue = (unsigned char *)vector;
    //} else if (member->getType()->isa(MleDwpVector4::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "MlVector4")) {
        const MleDwpVector4 *dwpVec4 = (MleDwpVector4 *)member->getType();
        MlVector4 *vector = new MlVector4();
        dwpVec4->get(value, vector);
        pvalue = (unsigned char *)vector;
    //} else if (member->getType()->isa(MleDwpTransform::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "MlTransform")) {
        const MleDwpTransform *dwpTran = (MleDwpTransform *)member->getType();
        MlTransform *transfrm = new MlTransform();
        dwpTran->get(value, transfrm);
        pvalue = (unsigned char *)transfrm;
    //} else if (member->getType()->isa(MleDwpRotation::typeId)) {
	} else if (! strcmp(member->getType()->getName(), "MlRotation")) {
        const MleDwpRotation *dwpRot = (MleDwpRotation *)member->getType();
        MlRotation *rotation = new MlRotation();
        dwpRot->get(value, rotation);
        pvalue = (unsigned char *)rotation;
	} else if (! strcmp(member->getType()->getName(), "string")) {
		const MleDwpString *dwpStr = (MleDwpString *)member->getType();
		//char *str = (char *) mlMalloc(dwpStr->getSize() + 1);
		char *str;
		// Memory is allocated for the string by the following get() query.
		dwpStr->get(value, &str);
		pvalue = (unsigned char *)str;
	} else {
		// This should work for anonymous data types (i.e. int, float, etc.)
		pvalue = (unsigned char *)value;
	}

	// Set value from DWP into Actor property. Note: other data types may need to be
	// translated in addition to arrays.
	entry->setProperty(this, entry->name, pvalue);

	// Clean up
	/*
	if (member->getType()->isa(MleDwpVector2::typeId)) { delete ((MlVector2 *)pvalue); }
	else if (member->getType()->isa(MleDwpVector3::typeId)) { delete ((MlVector3 *)pvalue); }
	else if (member->getType()->isa(MleDwpVector4::typeId)) { delete ((MlVector4 *)pvalue); }
	else if (member->getType()->isa(MleDwpTransform::typeId)) { delete ((MlTransform *)pvalue); }
	else if (member->getType()->isa(MleDwpRotation::typeId)) { delete ((MlRotation*)pvalue); }
	*/

	if (! strcmp(member->getType()->getName(), "MlVector2")) { delete ((MlVector2 *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "MlVector3")) { delete ((MlVector3 *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "MlVector4")) { delete ((MlVector4 *)pvalue); }
	else if  (! strcmp(member->getType()->getName(), "MlTransform")) { delete ((MlTransform *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "MlRotation")) { delete ((MlRotation *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "FloatArray")) { delete ((MleArray<float> *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "IntArray")) { delete ((MleArray<int> *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "ScalarArray")) { delete ((MleArray<MlScalar> *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "Vector3Array")) { delete ((MleArray<MlVector3> *)pvalue); }
	else if (! strcmp(member->getType()->getName(), "string")) { mlFree((char *)pvalue); }

	return 0;
}

void
MleActor::resolveEdit(const char *)
{
	// Does nothing in Rehearsal Player.
}

MlePtrArray *
MleActor::getPropNames(const char *propDatasetName) const
{
    return m_actorClass->getPropNames(propDatasetName);
}

MlErr
MleActor::getPropDataset(const char *propDatasetName, void *data)
{
    MlErr status;

    if (strcmp(propDatasetName, MLE_PROP_DATASET_TRANSFORM) == 0)
	{
        getTransform(*((MlTransform *) data));
        status = MLE_S_OK;
    }
    else
        status = MLE_E_ACTOR_NO_PROP_DATASET;

    return status;
}

MlErr
MleActor::setPropDataset(const char *propDatasetName, void *data)
{
    MlErr status;

    if (strcmp(propDatasetName, MLE_PROP_DATASET_TRANSFORM) == 0)
	{
        setTransform(*((MlTransform *) data));
        status = MLE_S_OK;
    }
    else
        status = MLE_E_ACTOR_NO_PROP_DATASET;

    return status;
}

#endif	/* MLE_DIGITAL_WORKPRINT */

// Transformation support.

//   This is compiled MLE_DIGITAL_WORKPRINT in this defining class, but subclasses
//   may reimplement the same interface exposed for the runtime.
#ifdef MLE_DIGITAL_WORKPRINT
// Include system header files.
#include <math.h>

// Include Magic Lantern math header files.
#include "math/transfrm.h"
#include "math/rotation.h"

// Inlucde Digital Workprint header files.
#include "mle/DwpScalar.h"
#include "mle/DwpVector2.h"
#include "mle/DwpVector3.h"
#include "mle/DwpRotation.h"
#include "mle/DwpTransform.h"

int
MleActor::hasTransform(void)
{
	// Make sure the actor class is present.
	if ( getClass() == NULL )
		return 0;
	
	// Look for some well-known representations.
	int rval = 0;
	const MleActorMember *member;

	member = m_actorClass->findMember("position");
	if ( member && (member->getType()->isa(MleDwpVector3::typeId) ||
	     member->getType()->isa(MleDwpVector2::typeId)) )
		rval |= MLE_ACTOR_TRANSLATION;

	member = m_actorClass->findMember("translation");
	if ( member && (member->getType()->isa(MleDwpVector3::typeId) ||
	     member->getType()->isa(MleDwpVector2::typeId)) )
		rval |= MLE_ACTOR_TRANSLATION;
	
	member = m_actorClass->findMember("orientation");
	if ( member && member->getType()->isa(MleDwpRotation::typeId) )
		rval |= MLE_ACTOR_ROTATION;

	member = m_actorClass->findMember("rotation");
	if ( member && member->getType()->isa(MleDwpVector3::typeId) )
		rval |= MLE_ACTOR_ROTATION;
	
	member = m_actorClass->findMember("transform");
	if ( member && member->getType()->isa(MleDwpTransform::typeId) )
		rval |= MLE_ACTOR_AFFINE;
	
	return rval;
}

int
MleActor::setTransform(MlTransform& t)
{
	// Make sure the actor class is present.
	if ( getClass() == NULL )
		return 1;
	
	// Look for some well-known representations.
	int rval = 0;
	const MleActorMember *member;

	member = m_actorClass->findMember("transform");
	if ( member && member->getType()->isa(MleDwpTransform::typeId) )
	{
		//MlTransform *p = (MlTransform *) ((char *)this + member->getOffset());
		MlTransform *p;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&p);

		*p = t;
	}

	member = m_actorClass->findMember("position");
	if (member)
	{
		if (member->getType()->isa(MleDwpVector3::typeId)) {
			//MlScalar *p = (MlScalar *) ((char *) this +	member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			p[0] = t[3][0];
			p[1] = t[3][1];
			p[2] = t[3][2];
		}
		else if (member->getType()->isa(MleDwpVector2::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			p[0] = t[3][0];
			p[1] = t[3][1];
		}
	}

	member = m_actorClass->findMember("translation");
	if (member)
	{
		if (member->getType()->isa(MleDwpVector3::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			p[0] = t[3][0];
			p[1] = t[3][1];
			p[2] = t[3][2];
		}
		else if (member->getType()->isa(MleDwpVector2::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			p[0] = t[3][0];
			p[1] = t[3][1];
		}
	}

	member = m_actorClass->findMember("scale");
	if ( member && member->getType()->isa(MleDwpVector3::typeId) )
	{
		//MlScalar *s = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *s;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&s);

		s[0] = mlSqrt(
			mlSquare(t[0][0]) +
			mlSquare(t[0][1]) +
			mlSquare(t[0][2]));
		s[1] = mlSqrt(
			mlSquare(t[1][0]) +
			mlSquare(t[1][1]) +
			mlSquare(t[1][2]));
		s[2] = mlSqrt(
			mlSquare(t[2][0]) +
			mlSquare(t[2][1]) +
			mlSquare(t[2][2]));

		MlScalar tmp;
		tmp = mlReciprocal(s[0]);
		t[0][0] = mlMul(t[0][0],tmp);
		t[0][1] = mlMul(t[0][1],tmp);
		t[0][2] = mlMul(t[0][2],tmp);
		tmp = mlReciprocal(s[1]);
		t[1][0] = mlMul(t[1][0],tmp);
		t[1][1] = mlMul(t[1][1],tmp);
		t[1][2] = mlMul(t[1][2],tmp);
		tmp = mlReciprocal(s[2]);
		t[2][0] = mlMul(t[2][0],tmp);
		t[2][1] = mlMul(t[2][1],tmp);
		t[2][2] = mlMul(t[2][2],tmp);
	}

	member = m_actorClass->findMember("scale");
	if ( member && member->getType()->isa(MleDwpScalar::typeId) )
	{
		//MlScalar *s = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *s;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&s);


		*s = mlSqrt(
			mlSquare(t[0][0]) +
			mlSquare(t[0][1]) +
			mlSquare(t[0][2]));

		MlScalar tmp = mlReciprocal(*s);
		t[0][0] = mlMul(t[0][0],tmp);
		t[0][1] = mlMul(t[0][1],tmp);
		t[0][2] = mlMul(t[0][2],tmp);
		t[1][0] = mlMul(t[1][0],tmp);
		t[1][1] = mlMul(t[1][1],tmp);
		t[1][2] = mlMul(t[1][2],tmp);
		t[2][0] = mlMul(t[2][0],tmp);
		t[2][1] = mlMul(t[2][1],tmp);
		t[2][2] = mlMul(t[2][2],tmp);
	}

	member = m_actorClass->findMember("orientation");
	if ( member && member->getType()->isa(MleDwpRotation::typeId) )
	{
		MlScalar quat[4];
		//MlRotation *rot = (MlRotation *) ((char *)this + member->getOffset());
		MlRotation *rot;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&rot);
		
		int i,j,k;
		// First, find largest diagonal in matrix:
		if (t[0][0] > t[1][1])
		{
			if (t[0][0] > t[2][2])
				i = 0;
			else
				i = 2;
		}
		else
		{
			if (t[1][1] > t[2][2])
				i = 1;
			else
				i = 2;
		}

		if (t[0][0]+t[1][1]+t[2][2] > t[i][i])
		{
			// Compute w first:
			quat[3] = mlDiv(
			    mlSqrt(
				t[0][0]+t[1][1]+t[2][2]+ML_SCALAR_ONE),
			    ML_SCALAR_TWO);

			// And compute other values:
			quat[0] = mlDiv((t[1][2]-t[2][1]),(4*quat[3]));
			quat[1] = mlDiv((t[2][0]-t[0][2]),(4*quat[3]));
			quat[2] = mlDiv((t[0][1]-t[1][0]),(4*quat[3]));
		}
		else {
			// Compute x, y, or z first:
			j = (i+1)%3; k = (i+2)%3;

			// Compute first value:
			quat[i] = mlDiv(
			    mlSqrt(
				t[i][i]-t[j][j]-t[k][k]+ML_SCALAR_ONE),
			    ML_SCALAR_TWO);

			// And the others:
			quat[j] = mlDiv((t[i][j]+t[j][i]),(4*quat[i]));
			quat[k] = mlDiv((t[i][k]+t[k][i]),(4*quat[i]));
			quat[3] = mlDiv((t[j][k]-t[k][j]),(4*quat[i]));
		}

		rot->setValue(quat);
	}

	member = m_actorClass->findMember("rotation");
	if ( member && member->getType()->isa(MleDwpVector3::typeId) )
	{
		//MlScalar *rot = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *rot;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&rot);

        MlVector3 rotation;
        t.getRotation(rotation);
        rot[0]=rotation[0];
        rot[1]=rotation[1];
        rot[2]=rotation[2];
	}	

	return rval;
}

void
MleActor::getTransform(MlTransform& t)
{
	// Set the transform to identity.
	t.makeIdentity();

	// Make sure the actor class is present.
	if ( getClass() == NULL )
		return;
	
	// Look for some well-known representations.
	const MleActorMember *member;

	member = m_actorClass->findMember("transform");
	if ( member && member->getType()->isa(MleDwpTransform::typeId) )
	{
		//MlTransform *p = (MlTransform *) ((char *)this + member->getOffset());
		MlTransform *p;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&p);

		t = *p;
	}

	member = m_actorClass->findMember("position");
	if (member)
	{
		if(member->getType()->isa(MleDwpVector3::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			t[3][0] = p[0];
			t[3][1] = p[1];
			t[3][2] = p[2];
		}
		else if (member->getType()->isa(MleDwpVector2::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			t[3][0] = p[0];
			t[3][1] = p[1];
		}
	}

	member = m_actorClass->findMember("translation");
	if(member) {
		if(member->getType()->isa(MleDwpVector3::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			t[3][0] = p[0];
			t[3][1] = p[1];
			t[3][2] = p[2];
		}
		else if(member->getType()->isa(MleDwpVector2::typeId))
		{
			//MlScalar *p = (MlScalar *) ((char *) this + member->getOffset());
			MlScalar *p;
			MlePropertyEntry *entry = member->getEntry();
			// TBD: the following needs to be tested!!!
			entry->getProperty(this, entry->name, (unsigned char **)&p);

			t[3][0] = p[0];
			t[3][1] = p[1];
		}
    }

	member = m_actorClass->findMember("orientation");
	if ( member && member->getType()->isa(MleDwpRotation::typeId) )
	{
		//MlRotation *rot = (MlRotation *) ((char *)this + member->getOffset());
		MlRotation *rot;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&rot);

		// Get the matrix for this rotation.
		MlTransform r;
		rot->getValue(r);

		// Assign the 3x3.
		t[0][0] = r[0][0]; t[0][1] = r[0][1]; t[0][2] = r[0][2];
		t[1][0] = r[1][0]; t[1][1] = r[1][1]; t[1][2] = r[1][2];
		t[2][0] = r[2][0]; t[2][1] = r[2][1]; t[2][2] = r[2][2];
	}


	member = m_actorClass->findMember("rotation");
	if ( member && member->getType()->isa(MleDwpVector3::typeId) )
	{
		//MlScalar *rot = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *rot;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&rot);

		// Get the matrix for this rotation.
        MlVector3 rotation;
        rotation[0]=rot[0];
        rotation[1]=rot[1];
        rotation[2]=rot[2];
		t.setRotationOnly(rotation);
	}


	member = m_actorClass->findMember("scale");
	if ( member && member->getType()->isa(MleDwpVector3::typeId) )
	{
		//MlScalar *s = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *s;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&s);

		t[0][0] = mlMul(t[0][0],s[0]);
		t[0][1] = mlMul(t[0][1],s[0]);
		t[0][2] = mlMul(t[0][2],s[0]);
		t[1][0] = mlMul(t[1][0],s[1]);
		t[1][1] = mlMul(t[1][1],s[1]);
		t[1][2] = mlMul(t[1][2],s[1]);
		t[2][0] = mlMul(t[2][0],s[2]);
		t[2][1] = mlMul(t[2][1],s[2]);
		t[2][2] = mlMul(t[2][2],s[2]);
	}

	member = m_actorClass->findMember("scale");
	if ( member && member->getType()->isa(MleDwpScalar::typeId) )
	{
		//MlScalar *s = (MlScalar *)((char *)this + member->getOffset());
		MlScalar *s;
		MlePropertyEntry *entry = member->getEntry();
		// TBD: the following needs to be tested!!!
		entry->getProperty(this, entry->name, (unsigned char **)&s);

		t[0][0] = mlMul(t[0][0],*s);
		t[0][1] = mlMul(t[0][1],*s);
		t[0][2] = mlMul(t[0][2],*s);
		t[1][0] = mlMul(t[1][0],*s);
		t[1][1] = mlMul(t[1][1],*s);
		t[1][2] = mlMul(t[1][2],*s);
		t[2][0] = mlMul(t[2][0],*s);
		t[2][1] = mlMul(t[2][1],*s);
		t[2][2] = mlMul(t[2][2],*s);
	}
}

#endif /* MLE_DIGITAL_WORKPRINT transformations */
