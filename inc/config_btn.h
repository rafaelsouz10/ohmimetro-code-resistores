#ifndef CONFIG_BTN_H
#define CONFIG_BTN_H

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"

#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events){
    reset_usb_boot(0, 0);
}

// Configuração inicial ds botões
void setup_gpio_BTN() {
  gpio_init(botaoB);
  gpio_set_dir(botaoB, GPIO_IN);
  gpio_pull_up(botaoB);
}

#endif