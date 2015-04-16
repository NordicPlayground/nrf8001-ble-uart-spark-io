#nrf8001-ble-uart-spark-io

Drivers and example files for a Spark Core for Bluetooth low energy as UART or as a node in a BLE-mesh
(uses the nRF8001 from [Nordic Semiconductor](https://www.nordicsemi.com/ "Go to Nordic Semiconductor's homepage"))

##Contents
The firmware folder contains the library, with the entire user interface defined in nrf8001-ble-uart-spark-io.h.
Examples of usage can be found in /firmware/examples, structured as simple Arduino sketches.

##Background
This library is a port from Nordic Semiconductor's [Bluetooth Low Energy for Arduino SDK](https://github.com/NordicSemiconductor/ble-sdk-arduino "Go to BLE for Arduino GitHub repository").
Tested with the RedBearLab Bluetooth Low Energy Shield v2.0 (uart application) and the nRF51-DK(PCA10028) (mesh application)

##Connecting the nRF8001 to the Spark Core
The nRF8001 and nRF51-DK communicate with the Spark Core via SPI, acting like a slave.
Connect the Spark Core to the RedBearLab Bluetooth Low Energy Shield using this pin configuration:

| BLE Shield		| Spark Core	|	Explanation |
| --------- | ----------- | ----------- |
| MOSI 			| A5					| SPI Master Out, Slave In |
| MISO			| A4					| SPI Master In, Slave Out |
| CLK				| A3 					| SPI Bus clock |
| REQN			| D4					| SPI Master request line |
| RDYN			| D3					| SPI Slave ready line |
| RESET			| D2					| nRF8001 reset signal (triggered in the library setup) |
| VIN				| 3V3					| Main voltage supply for the nRF8001 |
| GND				| GND					| Voltage supply for the nRF8001 radio |

You may initialize the library with other pin configurations, but some configurations may not work.
The nRF51-DK will need other pins.
If you have [Bluetooth Low Energy broadcast mesh](https://github.com/NordicSemiconductor/nRF51-ble-bcast-mesh "Go to BLE broadcast mesh repository")
the connections will look like this:

| nRF51-DK		| Spark Core	|	Explanation |
| --------- | ----------- | ----------- |
| P0.25 			| A5					| SPI Master Out, Slave In |
| P0.28			| A4					| SPI Master In, Slave Out |
| P0.29				| A3 					| SPI Bus clock |
| P0.24			| D4					| SPI Master request line |
| P0.20			| D3					| SPI Slave ready line |
| RESET			| D2					| nRF8001 reset signal (triggered in the library setup) |
| VDD				| 3V3					| Main voltage supply for the nRF8001 |
| GND				| GND					| Voltage supply for the nRF8001 radio |

##Running the example
The library contains an example Sketch called "nrf8001-ble-uart.ino". This example displays the most basic use case for the library, and lets you communicate with a terminal application on your computer through the Spark Core over BLE.

This example also shows how you can change the on-air name of the device with the nRF8001BleUartNameSet() function. Note that the name change may not appear in the Android UART application before you disconnect and connect again.

The library also contains a Sketch file called "ble-mesh-cloud.ino" which displays the usage of  registering functions and variables to be accessible via http-calls.
[See this documentation about it.](https://devzone.nordicsemi.com/blogs/672/accessing-the-ble-mesh-via-the-sparkio-cloud "Go to devzone blog")

###Terminal
You will need a two way terminal application on your computer in order to communicate with the uart application on your Spark Core (You can use [Termite](http://www.compuphase.com/software_termite.htm "Go to the Termite project home page") from Compu Phase, it is free)

You have to open the COM-port to the Spark Core before you either flash or reset your device.

Set up your terminal application with the following parameters:

| Field | Value |
|-------|-------|
| Port: | The COM-port your Spark Core is on. If you have the Spark CLI, this can be found with the command line command "spark serial list" |
| Baud rate: | 115200 |
| Data bits: | 8 |
| Stop bits: | 1 |
| Parity:    | None |
| Flow control: | None |

You should also append LF ('\n') to your messages (available as an option in most terminal applications).

###Flashing your Code

In the Spark.io Build IDE, Press "Libraries" in the bottom left corner, find the nrf8001-ble-uart-spark-io library, and press "INCLUDE IN APP".
Copy the code in "nrf8001-ble-uart.ino" over to your project sketch, and flash it onto your Spark Core with the lightning button on the left
or select the .ino file and press "USE THIS EXAMPLE".

Your Spark Core should start flashing purple for a while, and start breathing cyan. This means it's active, and if you've done everything right, it should be on air!

The example can be used with the "nRF UART 2.0" app for Android, available in the Google Play Store, or the "nRF UART" app for iOS, available in the iTunes App Store. Note that this app requires a BLE-compliant device. 

###Using the uart example with the App

Open the nRF UART app for Android or iOS, and press the "Connect" button near the top of the screen. For the Android application, you'll see a list of available BLE devices in your area. Press your "Spark" device in the list to connect to it. For the iOS application, you may have to press the button until it says "Scanning...", the application will then automatically connect to the nearest Spark. The app should show the message "Connected to: Spark" or similar. You can now start communicating with your Spark Core.

##The next steps
You can use the interface functions in the nrf8001-ble-uart-spark-io.h file to communicate with the nrf8001.

The way the framework uses BLE is determined by the services.h file in the firmware folder. This file is autogenerated from nRFgo Studio, available at [Nordic Semiconductor's homepage](http://nordicsemi.com). The xml file used to generate the services.h-file provided in the framework is stored in the docs/ folder in the GitHub repo ("services_setup.xml"). The framework implements all functionality of its Arduino-based counterpart, meaning that one could implement any BLE application with it, but due to limitations in the Spark.io toolchain, this repo only implements the UART application. 

##Forum
[Nordic Developer Zone](http://devzone.nordicsemi.com/ "Go to Nordic developer zone")

##Resources
[Spark.io](http://spark.io "Go to Spark.io homepage")  

[nRF8001](https://www.nordicsemi.com/eng/Products/Bluetooth-R-low-energy/nRF8001 "Go to product") (Data sheet for the nRF8001) 

[RedBearLab BLE Shield from MakerShed](http://www.makershed.com/Bluetooth_Low_Energy_BLE_Shield_for_Arduino_p/mkrbl1.htm "Go to product")  

[RedBearLab BLE Shield from SeeedStudio](http://www.seeedstudio.com/depot/bluetooth-40-low-energy-ble-shield-v20-p-1631.html "Go to product")  
