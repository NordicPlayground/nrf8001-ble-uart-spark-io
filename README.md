#nrf8001-ble-uart-spark-io

Bluetooth low energy as UART for Spark Core (uses the nRF8001 from [Nordic Semiconductor](https://www.nordicsemi.com/ "Go to Nordic Semiconductor's homepage")). 

##Contents
The firmware folder contains the library, with the entire user interface defined in nrf8001-ble-uart.h.
An example of usage can be found in /firmware/examples, structured as a simple Arduino sketch.

##Background
This library is a port from Nordic Semiconductor's [Bluetooth Low Energy for Arduino SDK](https://github.com/NordicSemiconductor/ble-sdk-arduino "Go to BLE for Arduino GitHub repository").

####Connecting the nRF8001 to the Spark Core
The nRF8001 communicates with the Spark Core via SPI, using this pin configuration:

|nrf8001		| Spark Core	|	Explanation |
| --------- | ----------- | ----------- |
| MOSI 			| A5					| SPI Master Out, Slave In |
| MISO			| A4					| SPI Master In, Slave Out |
| CLK				| A3 					| SPI Bus clock |
| REQN			| D4					| SPI Master request line |
| RDYN			| D3					| SPI Slave ready line |
| RESET			| D2					| nRF8001 reset signal (triggered in the library setup) |

##Forum
[Nordic Developer Zone](http://devzone.nordicsemi.com/ "Go to Nordic developer zone")

##Resources
[Spark.io](http://spark.io "Go to Spark.io homepage")  
[nRF8001](https://www.nordicsemi.com/eng/Products/Bluetooth-R-low-energy/nRF8001 "Go to product")  (Data sheet for the nRF8001)
[RedBearLab BLE Shield from MakerShed](http://www.makershed.com/Bluetooth_Low_Energy_BLE_Shield_for_Arduino_p/mkrbl1.htm "Go to product")  
[RedBearLab BLE Shield from SeeedStudio](http://www.seeedstudio.com/depot/bluetooth-40-low-energy-ble-shield-v20-p-1631.html "Go to product")  
