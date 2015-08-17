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
#include "nrf8001-ble-uart-spark-io/serial_evt.h"

#define ACCESS_ADDR     (0xA541A68F)

// defining the state of the system for communication
enum state_t{
    init,           // not yet initialized
    ready,          // no external command in work
    waitingForEvent // external command in work, waiting for answer from SPI slave
};

int state = init;

// buffer for the last value send by the SPI slave
int lastResponse;

int initState = 0;

// callback function for http command to set a handle value
int set_val(String args){

    if(state != ready){
        return -1;
    }
    state = waitingForEvent;

    uint8_t handle = args[0] - '0';
    uint8_t value  = args[1] - '0';
    
    return rbc_mesh_value_set(handle, &value, (uint8_t) 1);
}

// callback function for http command to get a handle value
int get_val_req(String args){

    if(state != ready){
        return -1;
    }
    state = waitingForEvent;

    uint8_t handle = args[0] - '0';
    
    return rbc_mesh_value_get(handle);
}

aci_pins_t pins;

// arduino conform init function
void setup(void)
{
  Serial.begin(9600);
  
  pins.board_name = BOARD_DEFAULT; //See board.h for details REDBEARLAB_SHIELD_V1_1 or BOARD_DEFAULT
  pins.reqn_pin   = A2;
  pins.rdyn_pin   = D5;
  pins.mosi_pin   = A5;
  pins.miso_pin   = A4;
  pins.sck_pin    = A3;

  pins.spi_clock_divider      = SPI_CLOCK_DIV64; //SPI_CLOCK_DIV8  = 8MHz SPI speed
                                                 //SPI_CLOCK_DIV16 = 4MHz SPI speed
                                                 //SPI_CLOCK_DIV64 = 1MHz SPI speed

  pins.reset_pin              = D2;
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

// sending intialization commands to SPI slave
// alternating sending of commands and waiting for response
void initConnectionSlowly(){
    switch(initState) {
        case 0:
            rbc_mesh_init(ACCESS_ADDR, (uint8_t) 38, (uint8_t) 2, (uint32_t) 100);
            initState++;
            Serial.println("Sent init command");
            break;
        case 1:
            rbc_mesh_value_enable((uint8_t) 1);
            initState++;
            Serial.println("Enabled value 1");
            break;
        case 2:
            rbc_mesh_value_enable((uint8_t) 2);
            initState++;
            Serial.println("Enabled value 2");
            break;
        case 3:
            state = ready;
            Serial.println("init done");
    }
}

// arduino conform main loop
void loop() {
    static bool newMessage = false;
    // send next initialization command to SPI slave until we leave init state
    if (state == init && newMessage) {
        initConnectionSlowly();
    }
    
    //Process any ACI commands or events
    serial_evt_t evnt;
    newMessage = rbc_mesh_evt_get(&evnt);

    if(newMessage && evnt.opcode == SERIAL_EVT_OPCODE_CMD_RSP){
        if (evnt.params.cmd_rsp.status != ACI_STATUS_SUCCESS)
        {
            Serial.print("Error response on cmd ");
            Serial.print(evnt.params.cmd_rsp.command_opcode, HEX);
            Serial.print(": ");
            Serial.print(evnt.params.cmd_rsp.status, HEX);
            Serial.println();
        }

        if (state == waitingForEvent) {
            state = ready;
            // save the response value of SPI slave into buffer
            // can be read via callback function
            lastResponse = evnt.params.cmd_rsp.response.val_get.data[0];
        }
    }
}
