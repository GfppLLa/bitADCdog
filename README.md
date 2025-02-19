# bitADCdog

Este projeto foi desenvolvido como parte de uma tarefa para a **Embarcatech**, Unidade 6, Capítulo 8, do professor **Wilton Lacerda Silva**. O projeto utiliza a placa **BitDogLab** com o microcontrolador **RP2040**. O código foi escrito em **C** e faz uso das bibliotecas **ssd1306.h** e **fonts.h**, implementando funções como **ADC**, **PWM**, **SSD1306** e **IRQ** para controlar diferentes componentes da placa.

## Funcionalidades Implementadas

1. **Leitura do Joystick via ADC:**
   - A posição do joystick é lida utilizando o **ADC** da placa RP2040, permitindo que o quadrado na tela OLED se mova de acordo com a posição do joystick.

2. **Controle de LED via PWM:**
   - O **PWM** é utilizado para controlar o brilho de um LED na placa **BitDogLab**, ajustando sua intensidade com base na posição do joystick.

3. **Exibição de Quadrado na Tela SSD1306:**
   - Um quadrado é desenhado na tela OLED e se move de acordo com a posição do joystick.

4. **Controle da Borda com IRQ:**
   - Usando interrupções de hardware (**IRQ**), o projeto permite que a borda da tela seja alterada sempre que o botão **GPIO 22** ou o botão do joystick for pressionado. A borda é desenhada de forma intercalada, com linhas alternadas entre preenchidas e vazias.

5. **Interação com a Tela OLED:**
   - A tela OLED exibe o quadrado e as alterações da borda. O quadrado é limitado pela borda da tela, não se sobrepondo a ela.

## Como Funciona

1. **Leitura do Joystick (ADC):**
   - O valor do ADC é lido e mapeado para determinar a posição do quadrado na tela OLED. O movimento do joystick altera as coordenadas do quadrado, permitindo que ele se mova horizontal e verticalmente pela tela.

2. **Controle de Brilho do LED (PWM):**
   - O **PWM** modula a intensidade do LED. A modulação é ajustada conforme a posição do joystick, com o brilho aumentando ou diminuindo com base nos movimentos do joystick.

3. **Exibição do Quadrado na Tela OLED:**
   - O quadrado é desenhado na tela, e sua posição é atualizada com base nos valores de ADC lidos do joystick. A tela é constantemente atualizada para refletir as mudanças de posição do quadrado, garantindo uma movimentação suave.

4. **Alteração da Borda com IRQ:**
   - O botão **GPIO 22** ou o botão do joystick são usados para ativar a alteração da borda da tela. A borda cresce intercalando linhas preenchidas e vazias, criando um efeito visual interessante. A borda pode crescer até 20 linhas, com 10 linhas preenchidas e 10 linhas vazias. O quadrado é restrito à área interna da tela, sem se sobrepor à borda.

## Estilo da Borda

- Quando o **GPIO 22** ou o botão do joystick é pressionado, a borda da tela muda.
- A borda é composta por linhas alternadas entre preenchidas e vazias, criando um padrão visual dinâmico.
- A borda pode crescer até 20 linhas, sendo 10 linhas preenchidas e 10 linhas vazias. O quadrado sempre será mantido dentro da área visível da tela e nunca se sobreporá à borda.

## Dependências

Para compilar e executar o projeto, são necessárias as seguintes bibliotecas:

- **ssd1306.h**: Biblioteca para controle do display OLED.
- **fonts.h**: Biblioteca para fontes e renderização de texto no display.
- **Bibliotecas de hardware do RP2040**: Para controlar o ADC, PWM, GPIO e IRQ.
## Como Clonar e Compilar o Projeto

### Clonando o Repositório

1. Abra o terminal no seu computador.
2. Clone o repositório para o seu diretório local utilizando o comando:
   ```bash
   git clone https://github.com/GfppLLa/bitADCdog
   ```
3. Aessaro o diretorio:
    ```bash
    cd bitADCdog
    ```
4.Criar diretorio de compilação:

    ```bash
    mkdir build
    ```

    ```bash
    cd build
    ```
5. Executar o CMake para configurar ambiente:

    ```bash
    cmake ..
    ```
6. Compile o projeto:
    ```bash
    make
    ```
7. Copie o arquivo `bitADCdog.uf2` para a pasta raiz do pico_w


## Licença

Este projeto está licenciado sob a licença **GNU GPL**.

## Vídeo de Demonstração


[Assista ao vídeo de demonstração](https://youtube.com/live/lmGFb7LOQek)

## Execução
**GLEISON FONSECA DE PAULA**


## Considerações Finais

<div align="center">
    GNUs not Unix
<pre>
    _-`````-,           ,- '- .
  .'   .- - |          | - -.  `.
 /.'  /                     `.   \
:/   :      _...   ..._      ``   :
::   :     /._ .`:'_.._\.    ||   :
::    `._ ./  ,`  :    \ . _.''   .
`:.      /   |  -.  \-. \\_      /
  \:._ _/  .'   .@)  \@) ` `\ ,.'
     _/,--'       .- .\,-.`--`.
       ,'/''     (( \ `  )    
        /'/'  \    `-'  (      
         '/''  `._,-----'
          ''/'    .,---'
           ''/'      ;:
             ''/''  ''/
               ''/''/''
                 '/'/'
                  `;      
<pre>
</div>
