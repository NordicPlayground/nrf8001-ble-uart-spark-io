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

// send commands with
// curl https://api.spark.io/v1/devices/[ID]/set_val -d access_token=[token] -d "args=11"

#include "nrf8001-ble-uart-spark-io/lib_aci.h"
#include "nrf8001-ble-uart-spark-io/boards.h"

#include "nrf8001-ble-uart-spark-io/rbc_mesh_interface.h"


enum state_t{
    init,
    ready,
    waitingForEvent
};

int state = init;

int lastResponse;

int set_val(String args){

    if(state != ready){
        return -1;
    }
    state = waitingForEvent;

    uint8_t handle = args[0] - '0';
    uint8_t value  = args[1] - '0';
    
    return rbc_mesh_value_set(handle, &value, (uint8_t) 1);
}

int get_val_req(String args){

    if(state != ready){
        return -1;
    }
    state = waitingForEvent;

    uint8_t handle = args[0] - '0';
    
    return rbc_mesh_value_get(handle);
}

aci_pins_t pins;

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
                                                 //if you are having trouble with the connection this might be the reason
                                                 //you might want SPI_CLOCK_DIV64 = 256 KHz

  pins.reset_pin              = D2; //4 for Nordic board, UNUSED for REDBEARLAB_SHIELD_V1_1
  pins.active_pin             = NRF_UNUSED;
  pins.optional_chip_sel_pin  = NRF_UNUSED;

  pins.interface_is_interrupt = false; //Interrupts still not available in Chipkit
  pins.interrupt_number       = 1;

  rbc_mesh_hw_init(&pins);

  Spark.function("set_val", set_val);
  Spark.function("get_val_req", get_val_req);
  Spark.variable("state", &state, INT);
  Spark.variable("lastResponse", &lastResponse, INT);

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
            if(newMessage){
                rbc_mesh_value_enable((uint8_t) 1);
                initState++;
            }
            break;
        case 2:
            if(newMessage){
                rbc_mesh_value_enable((uint8_t) 2);
                initState++;
            }
            break;
        case 3:
            if(newMessage){
                state = ready;
                Serial.println("init done");
            }
    }
}

void loop() {
    if(state == init){
        initConnectionSlowly();
    }
    //Process any ACI commands or events
    hal_aci_data_t evnt;
    newMessage = rbc_mesh_evt_get(&evnt);

    if(state == waitingForEvent && newMessage){
        state = ready;
        
        lastResponse = evnt.buffer[evnt.buffer[0]];
    }
}