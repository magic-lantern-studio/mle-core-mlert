/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleOpcode.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


#ifndef __MLE_OPCODE_H_
#define __MLE_OPCODE_H_

/**
 * @brief An enumeration for the Digital Playprint operational codes.
 */
enum _MleDppOpcodes {
    beginActorSetOpcode = 0x80,   /**< Begin an Actor Set. */
    createActorOpcode,            /**< Create an Actor. */
    setSetOpcode,                 /**< Set the Set as current. */
    bindRoleOpcode,               /**< Bind a Role to an Actor. */
    parentRoleOpcode,             /**< Bind a Role to a parent Role. */
    childRoleOpcode,              /**< Role is a child if another Role. */
    setPropertyOffsetOpcode,      /**< Set Property offset. */
    setPropertyLengthOpcode,      /**< Set Property length. */
    copyPropertyOpcode,           /**< Copy Property. */
    copyMediaRefOpcode,           /**< Copy Media Reference. */
    endActorGroupOpcode,          /**< Mark the end of an actor group or cast. */
    copyDynamicPropertyOpcode,    /**< Copy an Dynamic Property. */
    copyArrayPropertyOpcode,      /**< Copy an Array of Properties. */
    createSetOpcode,              /**< Create a Set. */
    endSetOpcode,                 /**< Mark the end of a Set. */
} MleDppOpcodes;

#define valueBits 7
#define valueMask ((1 << (valueBits)) - 1) 


#endif /* __MLE_OPCODE_H */
