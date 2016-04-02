#ifndef _SERIAL_INTERNAL_H__
#define _SERIAL_INTERNAL_H__

#include <stdint.h>
#include <stdbool.h>

#define __packed __attribute__((__packed__))
#define __packed_armcc __attribute__((__packed_armcc__))
#define __packed_gcc __attribute__((__packed_gcc__))

#define RBC_MESH_VALUE_MAX_LEN (28)
#define SEGMENT_LENGTH              (16)

#define NRF_SUCCESS                           (NRF_ERROR_BASE_NUM + 0)  ///< Successful command
#define NRF_ERROR_SVC_HANDLER_MISSING         (NRF_ERROR_BASE_NUM + 1)  ///< SVC handler is missing
#define NRF_ERROR_SOFTDEVICE_NOT_ENABLED      (NRF_ERROR_BASE_NUM + 2)  ///< SoftDevice has not been enabled
#define NRF_ERROR_INTERNAL                    (NRF_ERROR_BASE_NUM + 3)  ///< Internal Error
#define NRF_ERROR_NO_MEM                      (NRF_ERROR_BASE_NUM + 4)  ///< No Memory for operation
#define NRF_ERROR_NOT_FOUND                   (NRF_ERROR_BASE_NUM + 5)  ///< Not found
#define NRF_ERROR_NOT_SUPPORTED               (NRF_ERROR_BASE_NUM + 6)  ///< Not supported
#define NRF_ERROR_INVALID_PARAM               (NRF_ERROR_BASE_NUM + 7)  ///< Invalid Parameter
#define NRF_ERROR_INVALID_STATE               (NRF_ERROR_BASE_NUM + 8)  ///< Invalid state, operation disallowed in this state
#define NRF_ERROR_INVALID_LENGTH              (NRF_ERROR_BASE_NUM + 9)  ///< Invalid Length
#define NRF_ERROR_INVALID_FLAGS               (NRF_ERROR_BASE_NUM + 10) ///< Invalid Flags
#define NRF_ERROR_INVALID_DATA                (NRF_ERROR_BASE_NUM + 11) ///< Invalid Data
#define NRF_ERROR_DATA_SIZE                   (NRF_ERROR_BASE_NUM + 12) ///< Data size exceeds limit
#define NRF_ERROR_TIMEOUT                     (NRF_ERROR_BASE_NUM + 13) ///< Operation timed out
#define NRF_ERROR_NULL                        (NRF_ERROR_BASE_NUM + 14) ///< Null Pointer
#define NRF_ERROR_FORBIDDEN                   (NRF_ERROR_BASE_NUM + 15) ///< Forbidden Operation
#define NRF_ERROR_INVALID_ADDR                (NRF_ERROR_BASE_NUM + 16) ///< Bad Memory Address
#define NRF_ERROR_BUSY   

#define RBC_MESH_ACCESS_ADDRESS_BLE_ADV             (0x8E89BED6) /**< BLE spec defined access address. */
#define RBC_MESH_INTERVAL_MIN_MIN_MS                (5) /**< Lowest min-interval allowed. */
#define RBC_MESH_INTERVAL_MIN_MAX_MS                (60000) /**< Highest min-interval allowed. */
#define RBC_MESH_VALUE_MAX_LEN                      (23) /**< Longest legal payload. */
#define RBC_MESH_INVALID_HANDLE                     (0xFFFF) /**< Designated "invalid" handle, may never be used */
#define RBC_MESH_APP_MAX_HANDLE                     (0xFFEF) /**< Upper limit to application defined handles. The last 16 handles are reserved for mesh-maintenance. */


uint32_t vh_value_persistence_get(rbc_mesh_value_handle_t handle, bool* p_persistent);
uint32_t vh_tx_event_flag_get(rbc_mesh_value_handle_t handle, bool* is_doing_tx_event)


typedef uint16_t rbc_mesh_value_handle_t;

typedef struct __attribute((packed))
{
    uint8_t id;
    uint8_t ver;
} bl_id_t;

typedef struct __attribute((packed))
{
    uint32_t company_id;
    uint16_t app_id;
    uint32_t app_version;
} app_id_t;

typedef struct __attribute((packed))
{
    uint16_t sd;
    bl_id_t bootloader;
    app_id_t app;
} fwid_t;

typedef union __attribute((packed))
{
    app_id_t app;
    bl_id_t bootloader;
    uint16_t sd;
} fwid_union_t;

typedef struct __attribute((packed))
{
    uint16_t packet_type;
    union __attribute((packed))
    {
        struct __attribute((packed))
        {
            uint16_t version;
            uint8_t data[RBC_MESH_VALUE_MAX_LEN];
        } raw;
        fwid_t fwid;
        struct __attribute((packed))
        {
            uint8_t dfu_type    : 4;
            uint8_t _rfu1       : 4;
            uint8_t authority   : 3;
            uint8_t flood       : 1;
            uint8_t relay_node  : 1;
            uint8_t _rfu2       : 3;
            uint32_t transaction_id;
            fwid_union_t fwid;
        } state;
        struct __attribute((packed))
        {
            uint16_t segment;
            uint32_t transaction_id;
            uint32_t start_address;
            uint32_t length; /* in words */
            uint16_t signature_length;
            uint8_t diff        : 1;
            uint8_t single_bank : 1;
            uint8_t first       : 1;
            uint8_t last        : 1;
            uint8_t _rfu        : 4;
        } start;
        struct __attribute((packed))
        {
            uint16_t segment;
            uint32_t transaction_id;
            uint8_t data[SEGMENT_LENGTH];
        } data;
        struct __attribute((packed))
        {
            uint16_t segment;
            uint32_t transaction_id;
        } req_data;
        struct __attribute((packed))
        {
            uint16_t segment;
            uint32_t transaction_id;
            uint8_t data[SEGMENT_LENGTH];
        } rsp_data;
    } payload;
} dfu_packet_t;

uint32_t vh_value_persistence_get(rbc_mesh_value_handle_t handle, bool* p_persistent)
{
    if (!m_is_initialized)
        return NRF_ERROR_INVALID_STATE;

    if (handle == RBC_MESH_INVALID_HANDLE)
    {
        return NRF_ERROR_INVALID_ADDR;
    }
    
    event_handler_critical_section_begin();
    
    uint16_t handle_index = handle_entry_get(handle);
    if (handle_index == HANDLE_CACHE_ENTRY_INVALID)
    {
        event_handler_critical_section_end();
        return NRF_ERROR_NOT_FOUND;
    }

    *p_persistent = m_handle_cache[handle_index].persistent;
    
    event_handler_critical_section_end();
    return NRF_SUCCESS;
}

uint32_t vh_tx_event_flag_get(rbc_mesh_value_handle_t handle, bool* is_doing_tx_event)
{
    if (!m_is_initialized)
        return NRF_ERROR_INVALID_STATE;

    if (handle == RBC_MESH_INVALID_HANDLE)
    {
        return NRF_ERROR_INVALID_ADDR;
    }

    event_handler_critical_section_begin();
    
    uint16_t handle_index = handle_entry_get(handle);
    if (handle_index == HANDLE_CACHE_ENTRY_INVALID)
    {
        event_handler_critical_section_end();
        return NRF_ERROR_NOT_FOUND;
    }

    *is_doing_tx_event = m_handle_cache[handle_index].tx_event;

    event_handler_critical_section_end();
    return NRF_SUCCESS;
}

#endif /* _SERIAL_INTERNAL_H__ */

