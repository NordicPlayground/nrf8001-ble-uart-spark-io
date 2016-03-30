/***********************************************************************************
Copyright (c) Nordic Semiconductor ASA
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of other
  contributors to this software may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************************/
#ifndef _SERIAL_EVENT_H__
#define _SERIAL_EVENT_H__

/*#include <stdint.h>
#include <stdbool.h>
#include "toolchain.h" */
//#include "mesh_aci.h"
//#include "dfu_types_mesh.h"

#include "serial_internal.h"
#include "serial_command.h"
#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    SERIAL_EVT_OPCODE_DEVICE_STARTED        = 0x81,
    SERIAL_EVT_OPCODE_ECHO_RSP              = 0x82,
    SERIAL_EVT_OPCODE_CMD_RSP               = 0x84,
    SERIAL_EVT_OPCODE_EVENT_NEW             = 0xB3,
    SERIAL_EVT_OPCODE_EVENT_UPDATE          = 0xB4,
    SERIAL_EVT_OPCODE_EVENT_CONFLICTING     = 0xB5,
    SERIAL_EVT_OPCODE_EVENT_TX              = 0xB6,
    SERIAL_EVT_OPCODE_DFU                   = 0x78
} serial_evt_opcode_t;


typedef enum
{
    OPERATING_MODE_TEST,
    OPERATING_MODE_SETUP,
    OPERATING_MODE_STANDBY
} operating_mode_t;


/****** CMD RSP EVT PARAMS ******/
typedef struct
{
    rbc_mesh_value_handle_t handle;
    uint8_t flag;
    uint8_t value;
} serial_evt_cmd_rsp_params_flag_get_t;

typedef struct
{
    uint8_t major;
    uint8_t minor_1;
    uint8_t minor_2;
} serial_evt_cmd_rsp_params_build_version_t;

typedef struct
{
    uint32_t access_addr;
} serial_evt_cmd_rsp_params_access_addr_t;

typedef struct
{
    uint8_t channel;
} serial_evt_cmd_rsp_params_channel_t;

typedef struct
{
    uint32_t int_min;
} serial_evt_cmd_rsp_params_int_min_t;

typedef struct
{
    rbc_mesh_value_handle_t handle;
    uint8_t data[RBC_MESH_VALUE_MAX_LEN];
} serial_evt_cmd_rsp_params_val_get_t;

typedef struct
{
    uint16_t packet_type;
} serial_evt_cmd_rsp_params_dfu_t;

/****** EVT PARAMS ******/
typedef struct
{
    uint8_t data[29];
} serial_evt_params_echo_t;

typedef struct
{
    uint8_t command_opcode;
    aci_status_code_t status;
    union
    {
        serial_evt_cmd_rsp_params_build_version_t build_version;
        serial_evt_cmd_rsp_params_access_addr_t access_addr;
        serial_evt_cmd_rsp_params_channel_t channel;
        serial_evt_cmd_rsp_params_flag_get_t flag;
        serial_evt_cmd_rsp_params_int_min_t int_min;
        serial_evt_cmd_rsp_params_val_get_t val_get;
        serial_evt_cmd_rsp_params_dfu_t dfu;
    } response;        
} serial_evt_params_cmd_rsp_t;

typedef struct
{
    rbc_mesh_value_handle_t handle;
    uint8_t data[RBC_MESH_VALUE_MAX_LEN];
} serial_evt_params_event_new_t;

typedef struct
{
    rbc_mesh_value_handle_t handle;
    uint8_t data[RBC_MESH_VALUE_MAX_LEN];
} serial_evt_params_event_update_t;

typedef struct 
{
    rbc_mesh_value_handle_t handle;
    uint8_t data[RBC_MESH_VALUE_MAX_LEN];
} serial_evt_params_event_conflicting_t;

typedef struct 
{
    rbc_mesh_value_handle_t handle;
    uint8_t data[RBC_MESH_VALUE_MAX_LEN];
} serial_evt_params_event_tx_t;

typedef struct 
{
    operating_mode_t operating_mode;
    uint8_t hw_error;
    uint8_t data_credit_available;
} serial_evt_params_event_device_started_t;

typedef struct 
{
    dfu_packet_t packet;
} serial_evt_params_dfu_t;

typedef struct 
{
    uint8_t length;
    uint8_t opcode;
    union
    {
        serial_evt_params_echo_t                    echo;
        serial_evt_params_cmd_rsp_t                 cmd_rsp;
        serial_evt_params_event_new_t               event_new;
        serial_evt_params_event_update_t            event_update;
        serial_evt_params_event_conflicting_t       event_conflicting;
        serial_evt_params_event_tx_t                event_tx;
        serial_evt_params_event_device_started_t    device_started;
        serial_evt_params_dfu_t                     dfu;
    } params;
} serial_evt_t;


#endif /* _SERIAL_EVENT_H__ */
