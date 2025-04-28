#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H

#include "hardware/i2c.h"
#include "ssd1306/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

ssd1306_t ssd;    // Inicializa a estrutura do display
bool cor = true;
char str_adc[5];         // Buffer para armaznar a string  
char str_margem[10];

void display_init(){
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); //Inicializa o display
    ssd1306_config(&ssd); //Configura o display
    ssd1306_send_data(&ssd); //Envia os dados para o display

    //O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void mostrar_sem_resistor_display(){
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor);                             // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);        // Desenha um retângulo
    ssd1306_line(&ssd, 3, 37, 123, 37, cor);            // Desenha uma linha

    if (margem_ohmimetro == 0)      ssd1306_draw_string(&ssd, "500-10k Ohms", 15, 8);
    else if (margem_ohmimetro == 1) ssd1306_draw_string(&ssd, "10k-47k Ohms", 10, 8);
    else if (margem_ohmimetro == 2) ssd1306_draw_string(&ssd, "47k-100k Ohms", 10, 8);
    else if (margem_ohmimetro == 3) ssd1306_draw_string(&ssd, "100k-147k Ohms", 10, 8);

    ssd1306_draw_string(&ssd, "Sem resistor", 20, 25);// Mostra sem resistor quando estiver sem o desconhecido
    ssd1306_draw_string(&ssd, "ADC", 13, 41);        // Desenha uma string
    ssd1306_draw_string(&ssd, "Resisten.", 50, 41); // Desenha uma string
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);       // Desenha uma linha vertical
    ssd1306_send_data(&ssd); 
}

void mostrar_cores_resistores(int digito1, int digito2, int multiplicador, float media, float valor_e24){
    // Converte para string para ser mostrado no display OLED
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.0f Ohms", valor_e24);
    sprintf(str_adc, "%1.0f", media);

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

// Função que atualiza o resistor conhecido baseado na margem
void atualizar_resistor_adc() {
    if (margem_ohmimetro == 0)      R_conhecido = 9753;
    else if (margem_ohmimetro == 1) R_conhecido = 47000;
    else if (margem_ohmimetro == 2) R_conhecido = 100000;
    else if (margem_ohmimetro == 3) R_conhecido = 147000;
}

// Função que mostra em qual margem está ao apertar o botão A
void mostrar_margem_display() {
    // Limpa o display
    ssd1306_fill(&ssd, false);

    ssd1306_draw_string(&ssd, "Margem:", 0, 0);

    // Aparece no display conforme a margem selecionada
    if (margem_ohmimetro == 0) {
        ssd1306_draw_string(&ssd, "500-10k Ohms", 10, 16);
        ssd1306_draw_string(&ssd, "Res. Ref: 10k", 0, 32);
    } else if (margem_ohmimetro == 1) {
        ssd1306_draw_string(&ssd, "10k-47k Ohms", 10, 16);
        ssd1306_draw_string(&ssd, "Res. Ref: 47k", 0, 32);
    } else if (margem_ohmimetro == 2) {
        ssd1306_draw_string(&ssd, "47k-100k Ohms", 10, 16);
        ssd1306_draw_string(&ssd, "Res. Ref: 100k", 0, 32);
    } else if (margem_ohmimetro == 3) {
        ssd1306_draw_string(&ssd, "100k-147k Ohms", 10, 16);
        ssd1306_draw_string(&ssd, "Res. Ref: 100k", 0, 32);
    }

    ssd1306_draw_string(&ssd, "Conecte Ref!", 10, 48); // Aviso

    ssd1306_send_data(&ssd);  // Atualiza o display

    sleep_ms(5000); // Mostra a mensagem por 5 segundos

    // Depois de 5 segundos, limpa de novo o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

#endif