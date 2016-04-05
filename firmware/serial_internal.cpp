#include "serial_internal.h"
#include <stdint.h>



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
        //NVIC_EnableIRQ(ADC_IRQn);

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
        //NVIC_DisableIRQ(ADC_IRQn);
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
