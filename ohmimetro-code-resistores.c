#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "inc/display_ssd1306.h"
#include "inc/config_btn.h"
#include "inc/matriz_led.h"

#define ADC_PIN 28 // GPIO para o voltímetro

int R_conhecido = 9753; //10000 // 10k ohms
float R_x = 0.0;           // Resistor desconhecido
// float ADC_VREF = 3.31;     // Tensão de referência do ADC
int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)
float media = 0.0;

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
    media = soma / 500.0;

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

    display_init();   // Inicializa as configurações do display OLED SSD1306
    npInit(LED_PIN); // Inicializa matriz de LEDs NeoPixel.

    // Inicializa ADC
    adc_init();
    adc_gpio_init(ADC_PIN);

    bool cor = true;
    char str_adc[5]; // Buffer para armazenar a string
    while (true) {
        adc_select_input(2);  // Faz leitura média do ADC
        R_x = calcular_res_x(); // Calcula resistência medida

        // Verifica se está fora da faixa aceitável
        if (R_x < 100 || R_x > 150000) {
            // Atualiza o conteúdo do display com animações
            ssd1306_fill(&ssd, !cor);                                   // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);              // Desenha um retângulo
            ssd1306_line(&ssd, 3, 37, 123, 37, cor);                  // Desenha uma linha
            ssd1306_draw_string(&ssd, "Sem resistor", 15, 15);
            ssd1306_draw_string(&ssd, "ADC", 13, 41);             // Desenha uma string
            ssd1306_draw_string(&ssd, "Resisten.", 50, 41);      // Desenha uma string
            ssd1306_line(&ssd, 44, 37, 44, 60, cor);            // Desenha uma linha vertical
            ssd1306_send_data(&ssd); 
            continue; // Volta para o início do while
        }

        // Acha o valor comercial E24 mais próximo
        float valor_e24 = encontrar_e24(R_x);

        // Calcula as cores
        int digito1 = 0, digito2 = 0, multiplicador = 0;
        calcular_faixas(valor_e24, &digito1, &digito2, &multiplicador);

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.0f Ohms", valor_e24);
        sprintf(str_adc, "%1.0f", media); // Converte a média do adc inteiro em string

        // Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor);                                   // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);              // Desenha um retângulo
        ssd1306_line(&ssd, 3, 37, 123, 37, cor);                  // Desenha uma linha
        ssd1306_draw_string(&ssd, cores[digito1], 8, 6);         // Desenha uma string
        ssd1306_draw_string(&ssd, cores[digito2], 8, 16);       // Desenha uma string
        ssd1306_draw_string(&ssd, cores[multiplicador], 8, 26);// Desenha uma string
        ssd1306_draw_string(&ssd, "ADC", 13, 41);             // Desenha uma string
        ssd1306_draw_string(&ssd, "Resisten.", 50, 41);      // Desenha uma string
        ssd1306_line(&ssd, 44, 37, 44, 60, cor);            // Desenha uma linha vertical
        ssd1306_draw_string(&ssd, str_adc, 8, 52);         // Desenha uma string
        ssd1306_draw_string(&ssd, buffer, 48, 52);        // Desenha uma string
        ssd1306_send_data(&ssd);                         // Atualiza o display
    }
}
