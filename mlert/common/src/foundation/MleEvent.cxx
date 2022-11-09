/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundatoin Library API */

/**
 * @file MleEvent.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Wizzer Works
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleEvent.h"

int MleEvent::makeId(short group, short id)
{
    int newId = ((group << 16) | id);
    return newId;
}

short MleEvent::getGroupId(int cid)
{
    short groupId = (short)(cid >> 16);
    return groupId;
}

short MleEvent::getEventId(int cid)
{
    short eventId = (short)(cid & 0x00FF);
    return eventId;
}

MleEvent::MleEvent()
  : m_id(MLE_EVENT_INVALID_ID),
	m_type(MLE_EVENT_DELAYED),
	m_calldata(nullptr)
{ /* Do nothing extra. */ }

MleEvent::MleEvent(int id)
  : m_type(MLE_EVENT_DELAYED),
	m_calldata(nullptr)
{
    m_id = id;
}

MleEvent::MleEvent(int id, short type)
  : m_calldata(nullptr)
{
    m_id = id;
    m_type = type;
}

MleEvent::MleEvent(int id, void *calldata)
  : m_type(MLE_EVENT_DELAYED)
{
    m_id = id;
    m_calldata = calldata;
}

MleEvent::MleEvent(int id, short type, void *calldata)
{
    m_id = id;
    m_type = type;
    m_calldata = calldata;
}

bool MleEvent::operator==(const MleEvent &rhs)
{
	bool isEqual = false;
	isEqual = (this->m_id == rhs.m_id) && (this->m_type == rhs.m_type);
	// ToDo: Probably only need to test m_id here.
    return isEqual;
}

std::string MleEvent::toString()
{
	std::string type;

	if (m_type == MleEvent::MLE_EVENT_IMMEDIATE)
	    type = std::string("MLE_EVENT_IMMEDIATE");
	else if (m_type == MleEvent::MLE_EVENT_DELAYED)
	    type = std::string("MLE_EVENT_DELAYED");
	else
	    type = std::string("UNKNOWN");

	std::string str = "MleEvent: id=" + std::to_string(m_id) + " : type=" + type;

	return str;
}
