#include "gpio.h"
#include "button.h"

static vm_t *vm_pointer;
static volatile msg_t pub_msg;
static volatile int pub = LUOS_PROTOCOL_NB;

void rx_btn_cb(vm_t *vm, msg_t *msg) {
    if (msg->header.cmd == ASK_PUB_CMD) {
        // fill the message infos
        pub_msg.header.cmd = IO_STATE;
        pub_msg.header.target_mode = ID;
        pub_msg.header.target = msg->header.source;
        pub_msg.header.size = sizeof(char);
        pub_msg.data[0] = HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin);
        pub = ASK_PUB_CMD;
        vm_pointer = vm;
        return;
    }
}

void button_init(void) {
    luos_module_create(rx_btn_cb, STATE_MOD, "button_mod");
}

void button_loop(void) {
    if (pub != LUOS_PROTOCOL_NB) {
        luos_send(vm_pointer, &pub_msg);
        pub = LUOS_PROTOCOL_NB;
    }
}
