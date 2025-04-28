#ifndef CAL_OHMIMETRO_H
#define CAL_OHMIMETRO_H

#define ADC_PIN 28

// Variáveis globais
int R_conhecido = 9753;
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
    float melhores_fatores[] = {1, 10, 100, 1000, 10000}; // Fatores para escalar os valores da série E24 (cobrindo faixas de 10Ω a 910kΩ)
    float melhor = 0;         // Variável que armazenará o melhor candidato encontrado
    float menor_erro = 1e9;  // Variável que armazenará o menor erro encontrado (inicializado com valor muito alto)

    for (int i = 0; i < 5; i++) {       // Percorre cada fator de escala
        for (int j = 0; j < 24; j++) { // Calcula o valor do resistor candidato (valor da série E24 vezes o fator)

            float candidato = e24_series[j] * melhores_fatores[i]; // Calcula o erro absoluto entre a resistência medida e o candidato
            float erro = fabs(resistencia - candidato);           // Calcula o erro absoluto entre a resistência medida e o candidato

            // Se o erro for menor que o menor erro encontrado até agora
            if (erro < menor_erro) {
                menor_erro = erro;   // Atualiza o menor erro
                melhor = candidato; // Atualiza o melhor valor candidato
            }
        }
    }
    return melhor; // Retorna o valor da série E24 mais próximo da resistência medida
}

// Função que calcula os dígitos e o multiplicador para representar a resistência em faixas de cor
void calcular_faixas(float valor, int* d1, int* d2, int* mult) {
    while (valor >= 100) {
        valor /= 10;
        (*mult)++; // Conta quantas vezes o valor foi dividido por 10 (cada divisão representa um zero a mais no multiplicador)
    }
    // - Se mult = 0 → multiplicador x1 (ex.: 47Ω)
    // - Se mult = 1 → multiplicador x10 (ex.: 470Ω)
    
    int inteiro = (int)(valor + 0.5);// Arredonda o valor para o inteiro mais próximo
    *d1 = inteiro / 10;             // d1 recebe o primeiro dígito (dezena)
    *d2 = inteiro % 10;            // d2 recebe o segundo dígito (unidade)
}

#endif