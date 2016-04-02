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

#define NRF_ERROR_BASE_NUM      (0x0)       ///< Global error base
#define NRF_ERROR_SDM_BASE_NUM  (0x1000)    ///< SDM error base
#define NRF_ERROR_SOC_BASE_NUM  (0x2000)    ///< SoC error base
#define NRF_ERROR_STK_BASE_NUM  (0x3000)    ///< STK error base

#define HANDLE_CACHE_ENTRY_INVALID      (RBC_MESH_HANDLE_CACHE_ENTRIES)
#define DATA_CACHE_ENTRY_INVALID        (RBC_MESH_DATA_CACHE_ENTRIES)

typedef uint16_t rbc_mesh_value_handle_t;

static handle_entry_t   m_handle_cache[RBC_MESH_HANDLE_CACHE_ENTRIES];
static data_entry_t     m_data_cache[RBC_MESH_DATA_CACHE_ENTRIES];
static uint32_t         m_handle_cache_head;
static uint32_t         m_handle_cache_tail;
static bool             m_is_initialized = false;
static fifo_t           m_task_fifo;
static cache_task_t     m_task_fifo_buffer[CACHE_TASK_FIFO_SIZE];
static bool             m_handle_task_scheduled;

uint32_t vh_value_persistence_get(rbc_mesh_value_handle_t handle, bool* p_persistent);
uint32_t vh_tx_event_flag_get(rbc_mesh_value_handle_t handle, bool* is_doing_tx_event);
void event_handler_critical_section_end(void);
void event_handler_critical_section_begin(void);
static uint16_t handle_entry_get(rbc_mesh_value_handle_t handle);


/* specialized function pointer for copying memory between two instances */
typedef void (*fifo_memcpy)(void* dest, const void* src);

typedef struct
{
  void* elem_array;
  uint32_t elem_size;
  uint32_t array_len;
  uint32_t head;
  uint32_t tail;
  fifo_memcpy memcpy_fptr; /* must be a valid function or NULL */
} fifo_t;

typedef struct
{
    cache_task_type_t type;
    rbc_mesh_value_handle_t handle;
    union
    {
        struct
        {
            mesh_packet_t* p_packet;
        } enable;
        struct
        {
            mesh_packet_t* p_packet;
        } local_update;
    } params;
} cache_task_t;

typedef struct
{
    rbc_mesh_value_handle_t handle;             /** data handle */
    uint16_t                version;            /** last received handle version */
    uint16_t                index_next : 15;    /** linked list index next */
    uint16_t                tx_event   : 1;     /** TX event flag */
    uint16_t                index_prev : 15;    /** linked list index prev */
    uint16_t                persistent : 1;     /** Persistent flag */
    uint16_t                data_entry;         /** index of the associated data entry */
} handle_entry_t;

typedef struct
{
    trickle_t trickle;
    mesh_packet_t* p_packet;
} data_entry_t;

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

void event_handler_critical_section_end(void)
{
    uint32_t was_masked;
    _DISABLE_IRQS(was_masked);
    if (!--g_critical)
    {
        NVIC_EnableIRQ(QDEC_IRQn);
    }
    _ENABLE_IRQS(was_masked);
}

void event_handler_critical_section_begin(void)
{
    uint32_t was_masked;
    _DISABLE_IRQS(was_masked);
    if (!g_critical++)
    {
        NVIC_DisableIRQ(QDEC_IRQn);
    }
    _ENABLE_IRQS(was_masked);
}

static uint16_t handle_entry_get(rbc_mesh_value_handle_t handle)
{
    event_handler_critical_section_begin();
    uint16_t i = m_handle_cache_head;

    while (m_handle_cache[i].handle != handle)
    {
        if (m_handle_cache_tail == i)
        {
            event_handler_critical_section_end();
            return HANDLE_CACHE_ENTRY_INVALID; /* checked all entries */
        }
        HANDLE_CACHE_ITERATE(i);
    }
    
    event_handler_critical_section_end();
    return i;
}



#endif /* _SERIAL_INTERNAL_H__ */

