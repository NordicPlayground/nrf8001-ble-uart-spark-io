#ifndef NRF8001_BLE_UART_H__
#define NRF8001_BLE_UART_H__

#include <application.h>

/**
* Setup routine for all BLE UART related functionality
* Should be executed in the setup() function in the project sketch
*/
void ble_uart_setup(void);

/**
* Loop function, taking care of all BLE UART communication
* All incoming BLE events and UART messages are processed in this function.
* Should be executed in the loop() function in the project sketch
*/
void ble_uart_loop(void);

/**
* Public interface for sending data through the BLE UART
*/
void ble_uart_tx(uint8_t *buffer, uint8_t buffer_len);


#endif /*NRF8001_BLE_UART_H__*/