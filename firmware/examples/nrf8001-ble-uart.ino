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

/**
* Example usage of the nrf8001-ble-uart library from Nordic Semiconductor
* Note that all user functions are defined in the nrf8001-ble-uart.h, no further
* interaction with the library should be necessary.
*
* The nRF8001BleUartRx function is not implemented in the library itself, and 
* should be handled as a callback function for any incoming messages over the BLE UART
* 
* The example demonstrates how you can use the library to send and receive messages over UART.
* It also demonstrates the nRF8001BleUartNameSet() function, that dynamically can change the on-air 
* device name.
* 
*/

#include "nrf8001-ble-uart-spark-io/nrf8001-ble-uart-spark-io.h"

/**
* The data to be sent via the BLE UART is first stored in a 
* a buffer
*/

#define SERIAL_BUFFER_LENGTH 32

static uint8_t serial_buffer[SERIAL_BUFFER_LENGTH];

/**
* A macro that checks for a name=xxxx command from the terminal.
*/
#define IS_NAME_SETTING(buf) (memcmp((void*) buf, (void*) "name=", 5) == 0)





/**
* Small helper function to erase the contents of the serial buffer
*/
static void clear_serial_buffer()
{
	for (uint8_t i = 0; i < SERIAL_BUFFER_LENGTH; ++i)
	{
		serial_buffer[i] = ' ';
	}
}

/**
* Main Arduino compliant setup function
*/
void setup(void)
{
	// Initiate Serial communication
	Serial.begin(115200);
	
	// The USB serial on the Spark Core may take  
	// a second to set up:
	delay(3000);
	// Do the BLE UART setup:
	nRF8001BleUartSetup();
	
	
	// Clear the serial buffer:
	clear_serial_buffer();	
}

/**
* Implementation of RX callback function
* Handles any incoming data from the BLE UART
* 
* Simply sends all messages directly to terminal
*/
void nRF8001BleUartRx(uint8_t *buffer, uint8_t len)
{
	Serial.print("Received data: ");
	
	for (uint8_t i = 0; i < len; ++i)
	{
		Serial.print((char) buffer[i]);
	}
	
	Serial.print("\n");
}

/**
* Function for polling USB serial. Make sure the terminal ends 
* message with a newline ('\n')
*/
void handle_serial_input()
{
	static uint8_t msg_length = 0;
	
	// Poll Serial input:
	while (Serial.available())
	{
		serial_buffer[msg_length++] = (uint8_t) Serial.read();

		// If the end of the message is reached or the buffer full,
		// send the message
		if ('\n' == serial_buffer[msg_length - 1] || 
							SERIAL_BUFFER_LENGTH == msg_length)
		{
			// check if the input is the name=xxxx command. 
			if (IS_NAME_SETTING(serial_buffer))
			{
			
				//copy name into new, small buffer.
				char name_buf[msg_length - 6];
				memcpy(name_buf, &serial_buffer[5], msg_length - 6);
			
				//set name
				nRF8001BleUartNameSet(name_buf, msg_length - 6);
			}
			else
			{
				//input was not a name setting, send it over BLE UART.
				nRF8001BleUartTx(serial_buffer, msg_length - 1);
			}
			
			// Prepare the buffer for the next message:
			clear_serial_buffer();
			msg_length = 0;
		}
	}			
}

void loop(void)
{
	// Run the BLE UART loop once in every loop, 
	// to let it handle any BLE events
	nRF8001BleUartLoop();
	
	// handle all serial reads in one separate function:
	handle_serial_input();
}
