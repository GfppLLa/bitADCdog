#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "include/ssd1306.h"
#include "include/font.h"

//leds
#define LED_VERDE 11
#define LED_AZUL 12
#define LED_VERMELHO 13
//botoes
#define BOTAO_A 5
#define BOTAO_J 22
#define BOTAO_C 6
volatile _Atomic uint32_t ultimo_acionamento //para debouncing
static volatile uint32_t ultimo_pressionamento = 0;
volatile uint32_t hora_presente;
//pwm
#define WRAP 6000

//joystick
#define EIXO_X 27 //para virar adc1
#define EIXO_Y 28 //para virar adc0
volatile uint16_t posicao_x;
volatile uint16_t posicao_y;


// display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADRRS 0x3C
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define SIDE_BOX 8
#define UP_BOX8 8
#define CENTRO_X 2131
#define CENTRO_Y 1990
#define ZONA_OFF 45

//variaveis
#define DEBOUNCING_TIME_US 220
ssd1306_t ssd;
volatile _Atomic uint azuIsLigado=0;
volatile _Atomic uint ledsAreLigados=1; 
volatile _Atomic uint sobe_um=0;
volatile _Atomic uint desce_um=0;
volatile _Atomic bool estado_verde =false;
volatile _Atomic uint teste_um=0;


void atualiza_leds(uint led, uint16_t valor_adc, uint16_t centro)
{
    int desvio, maximo;
    (if ledsAreLigados==0)
    {
        pwm_set_gpio_level(led, 0);
        return;
    }
    else {
        desvio=valor_adc-centro;
        if(desvio>-ZONA_OFF&&desvio<ZONA_OFF)//se tiver no centro
        {
            pwm_set_gpio_level(led, 0);
            return;
        }
        else{
            if(desvio>0){maximo=4095-centro;}
            else{maximo=centro;}
            pwm_set_gpio_level(led, (((abs(desvio)-ZONA_OFF)*WRAP)/(maximo-ZONA_OFF)));
            return;
        }
    }
}
void tratar_botoes(uint gpio, uint32_t events)
{
    //debounce();
    
      hora_presente = to_us_since_boot(get_absolute_time());
    if(gpio==BOTAO_A)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_A);
            if(botao_pressionado){
                    sobe_um=1;//desligas os leds
            }
        }

    } else if( gpio ==BOTAO_J)
    {
         if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_J);
            if(botao_pressionado){
                    desce_um=1;//mudar estado led verde e mudar as bordas do lcd
            }
        }
    } else if(gpio==BOTAO_C)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_C);
            if(botao_pressionado)
            {saida_teste=1;}
        }

    }
    
            ultimo_pressionamento=hora_presente;
    

}

uint16_t ler_adc(uint canaladc)
{
    adc_select_read(canaladc);
    uint16_t leitura =adc_read();
    return leitura;
}

void iniciaHardware()
{
    //iniciar o pwm
    //iniciar o adc
    //inicia a irq
    //inicia o ssd(funções herdadas do prof)
    
    //leds
    //gpio_init(LED_AZUL);
    //gpio_set_dir(LED_AZUL, GPIO_OUT);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE,0);

    //gpio_init(LED_VERMELHO);
    //gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    //gpio pwm azul
    gpio_set_function(LED_AZUL, GPIO_FUNC_PWM);//define led azul como pẃm
    uint slice=pwm_gpio_to_slice_num(LED_AZUL);//numero do slice do led azul
    pwm_set_clkdiv(slice, 4.0);//configura divisor de clock e reduz freq pwm
    pwm_set_wrap(slice, WRAP);//define a onda pwm maximo
    pwm_set_gpio_level(LED_AZUL, 0);//define nivel inicial pwm
    pwm_set_enabled(slice, true);//habilita o slice pra gerar sinais

    //gpio pwm led vermelho
    gpio_set_function(LED_VERMELHO, GPIO_FUNC_PWM);
    slice=pwm_gpio_to_slice_num(LED_VERMELHO);
    pwm_set_clkdiv(slice, 4.0);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(LED_VERMELHO, 0);
    pwm_set_enabled(slice, true);

    //adc
    adc_init();//inicializa o analog digital converter
    adc_gpio_init(EIXO_X);//inicia gio joy x para adc
    adc_gpio_init(EIXO_Y)//inicia gpio joy y para adc

//display no i2c
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_funcion(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    gpio_pull_up(I2C_SDA);

    ssd1306_init(&ssd, SCREEN_WIDTH, SCREEN_HEIGHT, false, ADRRS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    //limpar display]
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    //botoes
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_J);
    gpio_set_dir(BOTAO_J, GPIO_IN);
    gpio_pull_up(BOTAO_J);
    gpio_init(BOTAO_C);
    gpio_set_dir(BOTAO_C, GPIO_IN);
    gpio_pull_up(BOTAO_C);






}

int main()
{
    stdio_init_all();
    iniciaHardware();
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_J, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_C, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
   boas_vindas(&ssd);
   entrarModoBootloader();
    sleep_ms(2000);

    while(true)
   {
    if(sobe_um==1)
    {
        //desligas os leds azul e vermelho
        ledsAreLigados=0;
        sobe_um=0;
    }
    if(desce_um==1)
    {
        //muda estado led verde
        estado_verde=!estado_verde;
        gpio_put(LED_VERDE, estado_verde);
        desce_um=0;
    }
    if(teste_um==1)
    {
        mensagem_bootloader(&ssd);
        entrarModoBootloaderDois();
        gpio_put(LED_AZUL,0);gpio_put(LED_VERDE,0);gpio_put(LED_VERMELHO,0);
        reset_usb_boot(0, 0);
    }
    posicao_x=ler_adc(1);
    posicao_y=ler_adc(0);
    //função para atualizar os leds já com as posições x e y
    atualiza_leds(LED_AZUL, posicao_x, CENTRO_X);
    atualiza_leds(LED_VERMELHO, posicao_y, CENTRO_Y);

    
   }
}

//funções herdadas
//--------------------------
void boas_vindas(ssd1306_t *ssd)
{
  ssd1306_fill(ssd,false); //Limpa display
  ssd1306_rect(ssd,3,3,122,58,true,false); //Desenha retângulo
  ssd1306_draw_string(ssd,"   gleison",4,25); 
  ssd1306_draw_string(ssd,"   embarcatech",4,45);

  ssd1306_send_data(ssd); //Atualiza o display
}
void mensagem_bootloader(ssd1306_t *ssd)
{
    ssd1306_fill(ssd,false); //Limpa display
    sleep_ms(500);
    ssd1306_rect(ssd,3,3,122,58,true,false); //Desenha retângulo
    ssd1306_draw_string(ssd,"   PICO",4,25); 
    ssd1306_draw_string(ssd,"   BOOTLOADER",4,45);
    ssd1306_send_data(ssd); //Atualiza o display
}
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms) {
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_PWM); // Configura pino do buzzer para PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_B_PIN);

    pwm_set_wrap(slice_num, 125000000 / frequency); // Período do PWM
    pwm_set_gpio_level(BUZZER_B_PIN, (125000000 / frequency) / 2); // Duty cycle 50%
    pwm_set_enabled(slice_num, true); // Ativa o PWM

    sleep_ms(duration_ms); // Toca por tempo especificado

    pwm_set_enabled(slice_num, false); // Desliga o PWM
    gpio_set_function(BUZZER_B_PIN, GPIO_FUNC_SIO);
    gpio_put(BUZZER_B_PIN, 0);
}

void entrarModoBootloader() {

    tocar_tom_buzzer(1200, 250); 
    sleep_ms(100);              
    tocar_tom_buzzer(1600, 250); 
    sleep_ms(100);               
    tocar_tom_buzzer(2200, 350);
    sleep_ms(100);               
    tocar_tom_buzzer(1200, 200);
    sleep_ms(50);                
    tocar_tom_buzzer(1600, 200); 
    sleep_ms(50);                
    tocar_tom_buzzer(2200, 450);
    }
    void entrarModoBootloaderDois() {
    tocar_tom_buzzer(2000, 300);  
    sleep_ms(150);                
    tocar_tom_buzzer(1500, 250);  
    sleep_ms(150);                
    tocar_tom_buzzer(1000, 250);  
    sleep_ms(200);                
    tocar_tom_buzzer(800, 300);   
    sleep_ms(250);                
    tocar_tom_buzzer(600, 400);   
    sleep_ms(300);                
    tocar_tom_buzzer(400, 500);  
    
    }