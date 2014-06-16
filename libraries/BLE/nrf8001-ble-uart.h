#ifndef NRF8001_BLE_UART_H__
#define NRF8001_BLE_UART_H__

#include <application.h>
#include "uart_aci.h"

/**
* Main user interface for the Nordic Semiconductor nRF8001 BLE UART library for the Spark Core
* All user functions are defined here, no further involvement with the library should be 
* necessary.
*
* 
* 
* The pin mappings for the SPI connection to the nRF8001 is as follows:
* 
*
* nrf8001		- Spark Core	-	Explanation
*
* MOSI 			- A5					- SPI Master Out, Slave In
* MISO			- A4					- SPI Master In, Slave Out
* CLK				- A3 					- SPI Bus clock
* REQN			- D4					- SPI Master request line
* RDYN			- D3					- SPI Slave ready line
* RESET			- D2					- nRF8001 reset signal (triggered in the library setup)
*/



/**
* Setup routine for all BLE UART related functionality
* Should be executed in the setup() function in the project sketch
*/
void nRF8001BleUartSetup(void);

/**
* Loop function, taking care of all BLE UART communication
* All incoming BLE events and UART messages are processed in this function.
* Should be executed in the loop() function in the project sketch
*/
void nRF8001BleUartLoop(void);

/**
* Public interface for sending data through the BLE UART
*/
bool nRF8001BleUartTx(uint8_t *buffer, uint8_t buffer_len);

/**
* Callback function for incoming UART messages
* Called with the contents and length of the message 
* 
* Implemented by user
*/
void nRF8001BleUartRx(uint8_t *buffer, uint8_t len);


#endif /*NRF8001_BLE_UART_H__*/