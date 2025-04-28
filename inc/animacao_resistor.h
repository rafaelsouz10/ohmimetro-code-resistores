#ifndef ANIMACAO_RESISTOR_H
#define ANIMACAO_RESISTOR_H

#include <stdint.h>
#include "matriz_led.h"

// Definição da estrutura que agrupa três componentes de cor (R, G, B)
typedef struct {
    uint8_t r, g, b;
} CorRGB;

// Lista de cores padrão para resistores
static const CorRGB cores_rgb[] = {
    {0, 0, 0},       // Preto
    {90, 20, 0},    // Marrom
    {255, 0, 0},   // Vermelho
    {255, 80, 0},     // Laranja
    {255, 225, 0},   // Amarelo
    {0, 255, 0},    // Verde
    {0, 0, 255},   // Azul
    {128, 0, 128},    // Violeta
    {128, 128, 128}, // Cinza
    {255, 255, 255} // Branco
};

// Função para mostrar faixas na matriz RGB
void mostrar_resistor_com_faixas(int digito1, int digito2, int multiplicador) {
    int matriz[5][5][3] = {0}; // Começa tudo apagado

    // Linha 1 - Primeira faixa
    for (int coluna = 0; coluna < 5; coluna++) {
        matriz[coluna][1][0] = cores_rgb[digito1].r;
        matriz[coluna][1][1] = cores_rgb[digito1].g;
        matriz[coluna][1][2] = cores_rgb[digito1].b;
    }

    // Linha 2 - Segunda faixa
    for (int coluna = 0; coluna < 5; coluna++) {
        matriz[coluna][2][0] = cores_rgb[digito2].r;
        matriz[coluna][2][1] = cores_rgb[digito2].g;
        matriz[coluna][2][2] = cores_rgb[digito2].b;
    }

    // Linha 3 - Multiplicador
    for (int coluna = 0; coluna < 5; coluna++) {
        matriz[coluna][3][0] = cores_rgb[multiplicador].r;
        matriz[coluna][3][1] = cores_rgb[multiplicador].g;
        matriz[coluna][3][2] = cores_rgb[multiplicador].b;
    }

    print_sprite(matriz);
    npWrite();
}

// Função para mostrar que não há resistor conectado
void mostrar_sem_resistor() {
    npClear();
    npWrite();
}

#endif // ANIMACAO_RESISTOR_H
