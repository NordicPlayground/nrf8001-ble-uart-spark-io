#ifndef _SERIAL_INTERNAL_H__
#define _SERIAL_INTERNAL_H__

#include <stdint.h>
#include <stdbool.h>

#define __packed __attribute__((__packed__))
#define __packed_armcc __attribute__((__packed_armcc__))
#define __packed_gcc __attribute__((__packed_gcc__))

#define RBC_MESH_VALUE_MAX_LEN (28)
#define SEGMENT_LENGTH              (16)

typedef uint16_t rbc_mesh_value_handle_t;

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

#endif /* _SERIAL_INTERNAL_H__ */

