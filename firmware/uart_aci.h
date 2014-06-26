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

#ifndef _UART_ACI_H__
#define _UART_ACI_H__

#include <application.h>
#include "lib_aci.h"
#include "aci_setup.h"
#include "uart_over_ble.h"
#include "services.h"

// aci_struct that will contain
// total initial credits
// current credit
// current state of the aci (setup/standby/active/sleep)
// open remote pipe pending
// close remote pipe pending
// Current pipe available bitmap
// Current pipe closed bitmap
// Current connection interval, slave latency and link supervision timeout
// Current State of the the GATT client (Service Discovery)
// Status of the bond (R) Peer address
extern struct aci_state_t aci_state;

/**
* Wrapper for the Serial.print() function, added in order to reduce memory usage
* for comments.
*/
//#define NRF_DO_PRINT (1)

#ifdef NRF_DO_PRINT
#define PRINT(x) Serial.print(x);
#define PRINT_BASE(x, y) Serial.print(x, y);
#else
#define PRINT(x) 
#define PRINT_BASE(x, y) 
#endif


/**
* BLE UART handling of incoming UART configuration data 
*/
bool uart_process_control_point_rx(uint8_t *byte, uint8_t length);


/**
* Top level nRF8001 event handling. Checks and handles all incoming ACI events from the aci_queue.
* This function does application specific event handling, and was the main application layer loop in the 
* Hello_BLE example for the Arduino library. 
*
* Directs any incoming UART messages to the nRF8001BleUartRx callback function defined in nrf8001-ble-uart.h,
* which should be implemented by the user
*/
void aci_loop(void);

#endif
