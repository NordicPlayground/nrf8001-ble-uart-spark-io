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

#define MESH_UUID                           (0xFEE4)
#define MESH_ADV_DATA_TYPE                  (0x16)
#define BLE_ADV_PACKET_PAYLOAD_MAX_LENGTH   (31)

#define CACHE_TASK_FIFO_SIZE            (8)


#if defined(__CC_ARM)

/* ARMCC and GCC have different ordering for packed typedefs, must separate macros */
    #define __packed_gcc
    #define __packed_armcc __packed

    #define _DISABLE_IRQS(_was_masked) _was_masked = __disable_irq()
    #define _ENABLE_IRQS(_was_masked) if (!_was_masked) { __enable_irq(); }

#elif defined(__GNUC__)

    #define __packed_armcc
    #define __packed_gcc __attribute__((packed))

    #define _DISABLE_IRQS(_was_masked) do{ \
        __ASM volatile ("MRS %0, primask" : "=r" (_was_masked) );\
        __ASM volatile ("cpsid i" : : : "memory");\
    } while(0)

    #define _ENABLE_IRQS(_was_masked) if (!_was_masked) { __enable_irq(); }



/** @brief Default value for the number of handle cache entries */
#ifndef RBC_MESH_HANDLE_CACHE_ENTRIES
    #define RBC_MESH_HANDLE_CACHE_ENTRIES           (100)
#endif

/** @brief Default value for the number of data cache entries */
#ifndef RBC_MESH_DATA_CACHE_ENTRIES
    #define RBC_MESH_DATA_CACHE_ENTRIES             (35)
#endif

/** @brief Length of app-event FIFO. Must be power of two. */
#ifndef RBC_MESH_APP_EVENT_QUEUE_LENGTH
    #define RBC_MESH_APP_EVENT_QUEUE_LENGTH         (16)
#endif

/** @brief Length of low level radio event FIFO. Must be power of two. */
#ifndef RBC_MESH_RADIO_QUEUE_LENGTH
    #define RBC_MESH_RADIO_QUEUE_LENGTH             (8)
#endif

#define HANDLE_CACHE_ENTRY_INVALID      (RBC_MESH_HANDLE_CACHE_ENTRIES)
#define DATA_CACHE_ENTRY_INVALID        (RBC_MESH_DATA_CACHE_ENTRIES)

typedef uint16_t rbc_mesh_value_handle_t;

static uint32_t         m_handle_cache_head;
static uint32_t         m_handle_cache_tail;
static bool             m_is_initialized = false;
static bool             m_handle_task_scheduled;
static uint32_t 		g_critical = 0;

#define HANDLE_CACHE_ITERATE(index)     do { index = m_handle_cache[index].index_next; } while (0)
#define HANDLE_CACHE_ITERATE_BACK(index)     do { index = m_handle_cache[index].index_prev; } while (0)

uint32_t vh_value_persistence_get(rbc_mesh_value_handle_t handle, bool* p_persistent);
uint32_t vh_tx_event_flag_get(rbc_mesh_value_handle_t handle, bool* is_doing_tx_event);
void event_handler_critical_section_end(void);
void event_handler_critical_section_begin(void);
static uint16_t handle_entry_get(rbc_mesh_value_handle_t handle);


/* specialized function pointer for copying memory between two instances */
typedef void (*fifo_memcpy)(void* dest, const void* src);

typedef enum {
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                 */
  NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted           */
  HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                 */
  SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                          */
  DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor                                                    */
  PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                              */
  SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                */
/* ----------------------  nrf51 Specific Interrupt Numbers  ---------------------- */
  POWER_CLOCK_IRQn              =   0,              /*!<   0  POWER_CLOCK                                                      */
  RADIO_IRQn                    =   1,              /*!<   1  RADIO                                                            */
  UART0_IRQn                    =   2,              /*!<   2  UART0                                                            */
  SPI0_TWI0_IRQn                =   3,              /*!<   3  SPI0_TWI0                                                        */
  SPI1_TWI1_IRQn                =   4,              /*!<   4  SPI1_TWI1                                                        */
  GPIOTE_IRQn                   =   6,              /*!<   6  GPIOTE                                                           */
  ADC_IRQn                      =   7,              /*!<   7  ADC                                                              */
  TIMER0_IRQn                   =   8,              /*!<   8  TIMER0                                                           */
  TIMER1_IRQn                   =   9,              /*!<   9  TIMER1                                                           */
  TIMER2_IRQn                   =  10,              /*!<  10  TIMER2                                                           */
  RTC0_IRQn                     =  11,              /*!<  11  RTC0                                                             */
  TEMP_IRQn                     =  12,              /*!<  12  TEMP                                                             */
  RNG_IRQn                      =  13,              /*!<  13  RNG                                                              */
  ECB_IRQn                      =  14,              /*!<  14  ECB                                                              */
  CCM_AAR_IRQn                  =  15,              /*!<  15  CCM_AAR                                                          */
  WDT_IRQn                      =  16,              /*!<  16  WDT                                                              */
  RTC1_IRQn                     =  17,              /*!<  17  RTC1                                                             */
  QDEC_IRQn                     =  18,              /*!<  18  QDEC                                                             */
  LPCOMP_IRQn                   =  19,              /*!<  19  LPCOMP                                                           */
  SWI0_IRQn                     =  20,              /*!<  20  SWI0                                                             */
  SWI1_IRQn                     =  21,              /*!<  21  SWI1                                                             */
  SWI2_IRQn                     =  22,              /*!<  22  SWI2                                                             */
  SWI3_IRQn                     =  23,              /*!<  23  SWI3                                                             */
  SWI4_IRQn                     =  24,              /*!<  24  SWI4                                                             */
  SWI5_IRQn                     =  25               /*!<  25  SWI5                                                             */
} IRQn_Type;

typedef __packed_armcc struct
{
    uint8_t type : 4;
    uint8_t _rfu1 : 2;
    uint8_t addr_type : 1;
    uint8_t _rfu2 : 1;
    uint8_t length;
    uint8_t _rfu3;
} __packed_gcc ble_packet_header_t;

typedef __packed_armcc struct
{
    ble_packet_header_t header;
    uint8_t addr[6];
    uint8_t payload[BLE_ADV_PACKET_PAYLOAD_MAX_LENGTH];
} __packed_gcc mesh_packet_t;

typedef __packed_armcc struct
{
    uint64_t        t;              /* Absolute value of t. Equals g_trickle_time (at set time) + t_relative */
    uint64_t        i;              /* Absolute value of i. Equals g_trickle_time (at set time) + i_relative */
    uint32_t        i_relative;     /* Relative value of i. Represents the actual i value in IETF RFC6206 */
    uint8_t         c;              /* Consistent messages counter */
} __packed_gcc trickle_t;

typedef struct
{
  void* elem_array;
  uint32_t elem_size;
  uint32_t array_len;
  uint32_t head;
  uint32_t tail;
  fifo_memcpy memcpy_fptr; /* must be a valid function or NULL */
} fifo_t;

typedef enum
{
    CACHE_TASK_TYPE_ENABLE,
    CACHE_TASK_TYPE_LOCAL_UPDATE
} cache_task_type_t;

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

static handle_entry_t   m_handle_cache[RBC_MESH_HANDLE_CACHE_ENTRIES];
static data_entry_t     m_data_cache[RBC_MESH_DATA_CACHE_ENTRIES];
static fifo_t           m_task_fifo;
static cache_task_t     m_task_fifo_buffer[CACHE_TASK_FIFO_SIZE];


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

