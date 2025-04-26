#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "inc/display_ssd1306.h"
#include "inc/config_btn.h"
#include <math.h>

#define ADC_PIN 28 // GPIO para o voltímetro

int R_conhecido = 9753; //10000 // 10k ohms
float R_x = 0.0;           // Resistor desconhecido
float ADC_VREF = 3.31;     // Tensão de referência do ADC
int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)

// Valores da série E24
float e24_series[] = {10,11,12,13,15,16,18,20,22,24,27,30,33,36,39,43,47,51,56,62,68,75,82,91};
const char* cores[] = {"Preto", "Marrom", "Vermelho", "Laranja", "Amarelo", "Verde", "Azul", "Violeta", "Cinza", "Branco"};



// Lê o valor do resistor desconhecido a medir
float calcular_res_x(){
    float soma = 0.0;
    for (int i = 0; i < 500; i++) {
        soma += adc_read();
        sleep_ms(1);
    }
    float media = soma / 500.0;

    return (R_conhecido * media) / (ADC_RESOLUTION - media); // Calcula resistência medida
}

// Encontra o valor mais próximo da série E24
float encontrar_e24(float resistencia) {
    float melhores_fatores[] = {1, 10, 100, 1000, 10000};
    float melhor = 0;
    float menor_erro = 1e9;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 24; j++) {
            float candidato = e24_series[j] * melhores_fatores[i];
            float erro = fabs(resistencia - candidato);
            if (erro < menor_erro) {
                menor_erro = erro;
                melhor = candidato;
            }
        }
    }
    return melhor;
}

// Calcula as faixas de cor do resistor
void calcular_faixas(float valor, int* d1, int* d2, int* mult) {
    while (valor >= 100) {
        valor /= 10;
        (*mult)++;
    }
    int inteiro = (int)(valor + 0.5);
    *d1 = inteiro / 10;
    *d2 = inteiro % 10;
}

int main(){
    // Inicializa o botão BOOTSEL
    setup_gpio_BTN();
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa I2C e OLED
    display_init();   // Inicializa as configurações do display OLED SSD1306

    // Inicializa ADC
    adc_init();
    adc_gpio_init(ADC_PIN);

    while (true) {
        adc_select_input(2);  // Faz leitura média do ADC
        R_x = calcular_res_x(); // Calcula resistência medida

        // Acha o valor comercial E24 mais próximo
        float valor_e24 = encontrar_e24(R_x);

        // Calcula as cores
        int digito1 = 0, digito2 = 0, multiplicador = 0;
        calcular_faixas(valor_e24, &digito1, &digito2, &multiplicador);

        // Atualiza display
        ssd1306_fill(&ssd, false);

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.0f Ohms", valor_e24);
        ssd1306_draw_string(&ssd, buffer, 0, 0);

        ssd1306_draw_string(&ssd, "Cores:", 0, 20);
        ssd1306_draw_string(&ssd, cores[digito1], 5, 30);
        ssd1306_draw_string(&ssd, cores[digito2], 5, 40);
        ssd1306_draw_string(&ssd, cores[multiplicador], 5, 50);

        ssd1306_send_data(&ssd);
        sleep_ms(700);
    }
}
