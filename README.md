# Ohmímetro Visual com Código de Cores

---

## Descrição do Projeto

Desenvolver um sistema capaz de identificar resistores de valores na faixa de 510Ω até 100kΩ, pertencentes à série E24 (tolerância de 5%), com visualização gráfica das informações em um display OLED e em uma matriz de LEDs WS2812. O sistema identifica o valor do resistor desconhecido, aproxima para o valor comercial mais próximo da série E24 e exibe as cores correspondentes dos anéis do resistor.

---

## Requisitos

- **Microcontrolador**: Raspberry Pi Pico ou Raspberry Pi Pico W (opcional)
- **Placa de Desenvolvimento:** BitDogLab (opcional).
- **Conta Criada no Wokwi Simulator**: [Wokwi](https://wokwi.com/).
- **Editor de Código**: Visual Studio Code (VS Code).
- **SDK do Raspberry Pi Pico** configurado no sistema.
- Ferramentas de build: **CMake** e **Ninja**.

---

## Instruções de Uso

### 1. Clone o Repositório

Clone o repositório para o seu computador:
```bash
git clone https://github.com/rafaelsouz10/ohmimetro-code-resistores.git
cd ohmimetro-code-resistores
code .
```
---

### 2. Instale as Dependências

Certifique-se de que o SDK do Raspberry Pi Pico está configurado corretamente no VS Code. As extensões recomendadas são:

- **C/C++** (Microsoft).
- **CMake Tools**.
- **Wokwi Simulator**.
- **Raspberry Pi Pico**.

---

### 3. Configure o VS Code

Abra o projeto no Visual Studio Code e siga as etapas abaixo:

1. Certifique-se de que as extensões mencionadas anteriormente estão instaladas.
2. No terminal do VS Code, compile o código clicando em "Compile Project" na interface da extensão do Raspberry Pi Pico.
3. O processo gerará o arquivo `.uf2` necessário para a execução no hardware real.

---

### 4. Teste no Simulador Wokwi Integrado ao VS Code

Após ter configurado o VS Code conforme descrito no item 3, siga os passos abaixo para simular o projeto:

1. Abra o arquivo `diagram.json` no Visual Studio Code.
2. Clique no botão "Play" disponível na interface.
3. Divirta-se interagindo com os componentes disponíveis no simulador integrado!

---

### 5. Teste no Hardware Real

#### Utilizando a Placa de Desenvolvimento BitDogLab com Raspberry Pi Pico W:

1. Conecte a placa ao computador no modo BOTSEEL:
   - Pressione o botão **BOOTSEL** (localizado na parte de trás da placa de desenvolvimento) e, em seguida, o botão **RESET** (localizado na frente da placa).
   - Após alguns segundos, solte o botão **RESET** e, logo depois, solte o botão **BOOTSEL**.
   - A placa entrará no modo de programação.

2. Compile o projeto no VS Code utilizando a extensão do [Raspberry Pi Pico W](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico):
   - Clique em **Compile Project**.

3. Execute o projeto clicando em **Run Project USB**, localizado abaixo do botão **Compile Project**.

---

### 🔌 6. Conexões e Esquemático
Abaixo está o mapeamento de conexões entre os componentes e a Raspberry Pi Pico W:

| **Componentes**        | **Pino Conectado (GPIO)** |
|------------------------|---------------------------|
| Display SSD1306 (SDA)  | GPIO 14                   |
| Display SSD1306 (SCL)  | GPIO 15                   |
  Matriz LED RGB         | GPIO 7                    |
| Botão A                | GPIO 5                    |
| Botão B                | GPIO 6                    |


#### 🛠️ Hardware Utilizado
- **Microcontrolador:** Raspberry Pi Pico W
- **Display:** OLED SSD1306 (I2C)
- **Botões A e B**
- **Matriz LED RGB**

---

### 7. Funcionamento do Sistema

#### 📌 Funcionalidades

**O sistema possui duas funcionalidades principais:**

**1. Medição de Resistores e Exibição Visual Automática**
Ao conectar um resistor desconhecido ao sistema, a medição é realizada automaticamente por meio do ADC da Raspberry Pi Pico W, utilizando um divisor de tensão com um resistor de referência. Após a leitura:

    - O sistema aproxima o valor encontrado ao resistor comercial mais próximo da série E24.

    - O valor calculado é exibido no display OLED, juntamente com a representação textual das cores dos anéis do resistor.

    - Simultaneamente, a matriz de LEDs WS2812 é utilizada para representar visualmente as faixas de cor do resistor em três linhas (primeiro dígito, segundo dígito e multiplicador).

    - Se nenhum resistor estiver conectado ou se o valor lido estiver fora da faixa válida, o sistema exibe a mensagem "Sem Resistor" no display OLED e apaga todos os LEDs da matriz.

**2. Alternância entre Margens de Medição (ativada ao pressionar o botão A)**
O botão A permite alternar entre quatro margens de medição distintas, adaptando o valor do resistor conhecido para melhorar a precisão da leitura:
    - Faixa 500Ω a ~10kΩ: Resistor de referência de 10kΩ;
    - Faixa 10kΩ a ~47kΩ: Resistor de referência de 47kΩ;
    - Faixa 47kΩ a ~100kΩ: Resistor de referência de 100kΩ;
    - Faixa 100kΩ a ~147kΩ: Associação em série de resistores 100kΩ + 47kΩ (147kΩ).

Sempre que a margem é alterada, o display OLED atualiza automaticamente para indicar a nova faixa configurada e solicita a conexão do novo resistor de referência.

**Função Especial – Botão B (Modo BOOTSEL)**
O botão B é reservado para reinicializar a placa e colocá-la diretamente em modo BOOTSEL, permitindo a regravação do firmware via conexão USB.
---

### 8. Vídeo Demonstrativo

Click [AQUI](O link do vídeo irá aqui) para acessar o link do Vídeo Ensaio
