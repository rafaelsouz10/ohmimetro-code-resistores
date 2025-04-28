#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "inc/cal_ohmimetro.h"
#include "inc/config_btn.h"
#include "inc/display_ssd1306.h"
#include "inc/animacao_resistor.h"

int main(){
    // Inicializa o botão BOOTSEL
    setup_gpio_BTN();
    gpio_set_irq_interrupt_btn(); // Configuração da interrupção com callback para botões A e B

    display_init();             // Inicializa as configurações do display OLED SSD1306
    npInit(LED_PIN);           // Inicializa matriz de LEDs NeoPixel.

    // Inicializa ADC
    adc_init();
    adc_gpio_init(ADC_PIN);

    int margem_antiga = -1; // Controle de mudança de margem

    while (true) {
        if (margem_antiga != margem_ohmimetro) {
            margem_antiga = margem_ohmimetro;
            atualizar_resistor_adc();   // Atualiza resistor correto
            mostrar_sem_resistor();    // Limpa matriz RGB
            mostrar_margem_display(); // Mostra no OLED
        }

        adc_select_input(2);     // Faz leitura média do ADC
        R_x = calcular_res_x(); // Calcula resistência medida

        // Verifica se está fora da faixa aceitável
        if (R_x < 100 || R_x > 190000) {
            mostrar_sem_resistor_display(); // Mostra no display que não tem resistor conectado
            mostrar_sem_resistor();        // Limpa matriz RGB   
        } else {    
            float valor_e24 = encontrar_e24(R_x); // Acha o valor comercial E24 mais próximo

            // Calcula as cores
            int digito1 = 0, digito2 = 0, multiplicador = 0;
            calcular_faixas(valor_e24, &digito1, &digito2, &multiplicador);

            // Mostra as cores no display OLED
            mostrar_cores_resistores(digito1, digito2, multiplicador, media, valor_e24);
            
            // Mostra cores na matriz RGB
            mostrar_resistor_com_faixas(digito1, digito2, multiplicador);
        }
    }
}
