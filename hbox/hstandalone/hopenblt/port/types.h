/************************************************************************************//**
* \file         Source/_template/types.h
* \brief        Bootloader types header file.
* \ingroup      Target_hbox_types
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2019  by Feaser    http://www.feaser.com    All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
* This file is part of OpenBLT. OpenBLT is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* OpenBLT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE. See the GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License along with OpenBLT. It
* should be located in ".\Doc\license.html". If not, contact Feaser to obtain a copy.
*
* \endinternal
****************************************************************************************/
#ifndef TYPES_H
#define TYPES_H

/************************************************************************************//**
* \defgroup   Target_hbox_types Type definitions of a port
* \brief      This module implements the variable type definitions of a microcontroller
*             port.
* \ingroup    Target_hbox
****************************************************************************************/
#include "hdefaults.h"
/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Boolean true value. */
#define BLT_TRUE       (true)
/** \brief Boolean false value. */
#define BLT_FALSE      (false)
/** \brief NULL pointer value. */
#define BLT_NULL       (NULL)


/****************************************************************************************
* Type definitions
****************************************************************************************/
/* TODO ##Port Adjust the type definitions such that they match the microcontroller
 * architecture for which this port is being developed. These type definitions are quite
 * generic and can typically be left as is.
 */
typedef bool            blt_bool;                     /**<  boolean type               */
typedef char            blt_char;                     /**<  character type             */
typedef uintptr_t       blt_addr;                     /**<  memory address type        */
typedef uint8_t         blt_int8u;                    /**<  8-bit unsigned integer     */
typedef int8_t          blt_int8s;                    /**<  8-bit   signed integer     */
typedef uint16_t        blt_int16u;                   /**< 16-bit unsigned integer     */
typedef int16_t         blt_int16s;                   /**< 16-bit   signed integer     */
typedef uint32_t        blt_int32u;                   /**< 32-bit unsigned integer     */
typedef int32_t         blt_int32s;                   /**< 32-bit   signed integer     */


#endif /* TYPES_H */
/*********************************** end of types.h ************************************/
