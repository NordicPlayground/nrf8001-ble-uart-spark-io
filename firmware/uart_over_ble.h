/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef UART_OVER_BLE_H__
#define UART_OVER_BLE_H__

/**
 * @def UART_OVER_BLE_DISCONNECT
 * @brief 
 * Command to queue a ACI Disconnect to the nRF8001
 */
#define UART_OVER_BLE_DISCONNECT      (0x01)


/**
 * @def UART_OVER_BLE_LINK_TIMING_REQ
 * @brief
 * Command to queue a ACI Change Timing to the nRF8001
 */
#define UART_OVER_BLE_LINK_TIMING_REQ (0x02)

/**
 * @def UART_OVER_BLE_TRANSMIT_STOP
 * @brief
 * Command to stop sending UART over BLE packets 
 */
#define UART_OVER_BLE_TRANSMIT_STOP   (0x03)


/**
 * @def UART_OVER_BLE_TRANSMIT_OK
 * @brief
 * Command to allow sending UART over BLE packets 
 */
#define UART_OVER_BLE_TRANSMIT_OK     (0x04)

typedef struct
{
    uint8_t uart_rts_local;  /* State of the local UART RTS  */
    uint8_t uart_rts_remote; /* State of the remote UART RTS */
} uart_over_ble_t;  

/**
 * @}
 */

#endif // UART_OVER_BLE_H__
