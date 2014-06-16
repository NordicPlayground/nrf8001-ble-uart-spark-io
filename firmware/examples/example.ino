/**
* Example usage of the nrf8001-ble-uart library from Nordic Semiconductor
* Note that all user functions are defined in the nrf8001-ble-uart.h, no further
* interaction with the library should be necessary.
*
* The nRF8001BleUartRx function is not implemented in the library itself, and 
* should be handled as a callback function for any incoming messages over the BLE UART
* 
* 
*/
#include "nrf8001-ble-uart.h"

/**
* The data to be sent via the BLE UART is first stored in a 
* a buffer
*/

#define SERIAL_BUFFER_LENGTH 32

static uint8_t serial_buffer[SERIAL_BUFFER_LENGTH];


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
	delay(1000);
	
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
* message with a newline ("\n")
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
			nRF8001BleUartTx(serial_buffer, msg_length - 1);
			
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
