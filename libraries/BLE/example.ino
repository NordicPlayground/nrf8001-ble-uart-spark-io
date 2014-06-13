#include "nrf8001-ble-uart.h"

/**
* The data to be sent via the BLE UART is first stored in a 
* a buffer
*/

#define SERIAL_BUFFER_LENGTH 64

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

void setup(void)
{
	// Initiate Serial communication
	Serial.begin(115200);
	
	// The USB serial on the Spark Core may take  
	// a second to set up:
	delay(1000);
	
	// Do the BLE UART setup:
	ble_uart_setup();
	
	// Clear the serial buffer:
	clear_serial_buffer();	
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
			ble_uart_tx(serial_buffer, msg_length - 1);
			
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
	ble_uart_loop();
	
	// handle all serial reads in one separate function:
	handle_serial_input();
}
