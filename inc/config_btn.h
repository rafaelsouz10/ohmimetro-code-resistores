#ifndef CONFIG_BTN_H
#define CONFIG_BTN_H

#include "hardware/timer.h"
#include "pico/bootrom.h" // Trecho para modo BOOTSEL com botão B

//definição de GPIO botões
#define BTN_A 5
#define BTN_B 6

// Variáveis globais 
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
int margem_ohmimetro = 0;

// Função de interrupção com debouncing do botão
void gpio_irq_handler(uint gpio, uint32_t events){
  // Obtém o tempo atual em microssegundos
  uint32_t current_time = to_us_since_boot(get_absolute_time());

  // Verifica se passou tempo suficiente desde o último evento
  if (current_time - last_time > 300000) { // 300 ms de debouncing
    last_time = current_time; // Atualiza o tempo do último evento

    if (gpio == BTN_A) {
      if (margem_ohmimetro < 3) margem_ohmimetro++;
      else margem_ohmimetro = 0;

    } else if (gpio == BTN_B) reset_usb_boot(0, 0);
  }
}

// Configuração inicial ds botões
void setup_gpio_BTN() {
  //Botão A
  gpio_init(BTN_A);
  gpio_set_dir(BTN_A, GPIO_IN);
  gpio_pull_up(BTN_A);

  //Botão B
  gpio_init(BTN_B);
  gpio_set_dir(BTN_B, GPIO_IN);
  gpio_pull_up(BTN_B);
}

void gpio_set_irq_interrupt_btn(){
  // Configuração da interrupção com callback para botão A
  gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  // Configuração da interrupção com callback para botão B
  gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

#endif