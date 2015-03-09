// This #include statement was automatically added by the Spark IDE.
#include "rbc_mesh_interface.h"

// This #include statement was automatically added by the Spark IDE.
//#include "nrf8001-ble-uart-spark-io/nrf8001-ble-uart-spark-io.h"

/***********************************************************************************
Copyright (c) Nordic Semiconductor ASA
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of other
  contributors to this software may be used to endorse or promote products
  derived from this software without specific prior written permission.

  4. This software must only be used in a processor manufactured by Nordic
  Semiconductor ASA, or in a processor manufactured by a third party that
  is used in combination with a processor manufactured by Nordic Semiconductor.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************************/

// send commands with
// curl https://api.spark.io/v1/devices/[ID]/set_val -d access_token=[token] -d "args=11"

//#include <SPI.h>
#include "nrf8001-ble-uart-spark-io/lib_aci.h"
#include "nrf8001-ble-uart-spark-io/boards.h"

#include "rbc_mesh_interface.h"


static aci_pins_t pins;

static uint8_t         uart_buffer[20];
static uint8_t         uart_buffer_len = 0;
static uint8_t         dummychar = 0;

int set_val(String args){


    uint8_t handle = args[0] - '0';
    uint8_t value  = args[1] - '0';
    
   // Serial.print("will send ");
    //7Serial.print(handle);
    //Serial.print(" ");
    //Serial.println(value);
    
    return rbc_mesh_value_set(handle, &value, (uint8_t) 1);
}

int tmp = 4;

void setup(void)
{
  Serial.begin(9600);
  
  pins.board_name = BOARD_DEFAULT; //See board.h for details REDBEARLAB_SHIELD_V1_1 or BOARD_DEFAULT
  pins.reqn_pin   = D4; //SS for Nordic board, 9 for REDBEARLAB_SHIELD_V1_1
  pins.rdyn_pin   = D3; //3 for Nordic board, 8 for REDBEARLAB_SHIELD_V1_1
  pins.mosi_pin   = A5;
  pins.miso_pin   = A4;
  pins.sck_pin    = A3;

  pins.spi_clock_divider      = SPI_CLOCK_DIV64; //SPI_CLOCK_DIV8  = 2MHz SPI speed
                                                 //SPI_CLOCK_DIV16 = 1MHz SPI speed
  
  pins.reset_pin              = D2; //4 for Nordic board, UNUSED for REDBEARLAB_SHIELD_V1_1
  pins.active_pin             = NRF_UNUSED;
  pins.optional_chip_sel_pin  = NRF_UNUSED;

  pins.interface_is_interrupt = false; //Interrupts still not available in Chipkit
  pins.interrupt_number       = 1;

  rbc_mesh_hw_init(&pins);
 
  Spark.function("set_val", set_val);
  Spark.variable("tmp", &tmp, INT);

  return;
}

int initState = 0;
bool newMessage = false;

void initConnectionSlowly(){
    uint8_t accAddr[4] = {0x8f, 0xa6, 0x41,0xa5 };
    switch(initState) {
        case 0:
            rbc_mesh_init(accAddr, (uint8_t) 38, (uint8_t) 2);
            initState++;
            break;
        case 1:
            if(newMessage) initState++;
            break;
        case 2:
            rbc_mesh_value_enable((uint8_t) 1);
            initState++;
            break;
        case 3:
            if(newMessage) initState++;
            break;
        case 4:
            initState++;
            rbc_mesh_value_enable((uint8_t) 2);
            Serial.println("init done");
    }
}


void loop() {
    initConnectionSlowly();

    //Process any ACI commands or events
    hal_aci_data_t evnt;
    newMessage = rbc_mesh_evt_get(&evnt);

}