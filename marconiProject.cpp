#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "nrf24l01.h"
#include "nrf24l01_driver.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SPI_PORT    spi0
#define BUF_LEN 1

#define ANGLE_MASK 0xFF
#define SERVO_MASK 0x01
#define MODE_MASK 0x02

//EMISOR
// // Modo operación (automático, control remoto)
// const int opModePin = 10;
// bool opMode = true;

// //Para controlar el servo2
// bool rightButton = false;
// bool leftButton = false;
// const int rightPin = 12;
// const int leftPin = 15;
// float grados2 = 90;

// //Para control de servo1
// const int timonPin = 28;
// uint16_t potenciometro;
// float grados1;

// //Factor de converion del adc
// const float conversion_factor = 3.3f / (1 << 12);

// char buffer[BUF_LEN];
// uint8_t val2send = 0x0000;
// uint8_t servo2ctrl = 254;
// void printBuff(){
//     for (uint8_t i = 0; i < BUF_LEN; i++)
//     {
//         printf("%u", buffer[i]);
//     }
//     printf("\n");
// }

// void setPeripherals();
// void readVelaDegree();

// int main()
// {
//     stdio_init_all();//Inicializar input-output
//     setPeripherals();//Inicializar perifericos
//     sleep_ms(5000);
//     nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
//     printf("Configurando modulo...\n");
//     NRF.default_config();
//     NRF.goTo_tx();
//     sleep_ms(5000);

//     while(1){
        
//         // //Enviar modo 
//         // if(gpio_get(opModePin)){
//         //     printf("Enviando modo manual\n");
//         //     val2send = 254;
//         //     sprintf(buffer, "%c", val2send);
//         //     NRF.send(buffer);
//         //     sleep_ms(5);

//         // }
//         // else{
//         //     printf("Enviando modo automatico\n");
//         //     val2send = 255;
//         //     sprintf(buffer, "%c", val2send);
//         //     NRF.send(buffer);
//         //     sleep_ms(5);
//         // }

//         //Enviar datos servo 1
//         readVelaDegree();
//         //val2send = grados1/5 + 50; //offset de servo 1 50+grados
//         val2send = grados1;
//         sprintf(buffer, "%c", (char*)val2send);
//         NRF.send(buffer);
//         //printf("Enviado angulo %u a servo1 \n", val2send);
//         //sleep_ms(5);
//         //sleep_ms(3000);
//         printf("Se envió: \n");
//         printBuff();
//         sleep_ms(50);
//     }
// }

// void setPeripherals(){
//     //SYS_INIT
//     adc_init();
//     stdio_init_all();

//     //Modo de operación
//     gpio_init(opModePin);
//     gpio_set_dir(opModePin, GPIO_IN);
    
//     //Potenciometro
//     adc_gpio_init(timonPin);
//     adc_select_input(2);

//     //Pulsadores
//     gpio_init(rightPin);
//     gpio_init(leftPin);
//     gpio_set_dir(rightPin, GPIO_IN);
//     gpio_set_dir(leftPin, GPIO_IN); 
// }

// void readVelaDegree(){
//     potenciometro = adc_read();
//     //printf("Raw value: 0x%03x, voltage: %f V\n", potenciometro, potenciometro * conversion_factor);
//     grados1 = (potenciometro * conversion_factor) * (64);
//     //printf("Los grados medidos son: %f\n", grados1);
// }

// void setTimon(){
//     rightButton = gpio_get(rightPin);
//     leftButton = gpio_get(leftPin);
//     if(rightButton & (grados2<=175)){
//         // printf("Derecha \n");
//         grados2 += 5;
//     }
//     else if(leftButton & (grados2>=5)){
//         // printf("Izquierda \n");
//         grados2 -= 5;
//     }
//     else if(grados2>=95){
//         grados2 -= 5;
//     }
//     else if(grados2<=85){
//         grados2 += 5;
//     }
// }


//RECEPTOR
char buffer[BUF_LEN];

void setDegree(int servoPin, float degree); //Asignar angulo a servo
void setServo(int servoPin, float startDegree); //Inicializar servo
void setPeripherals(); //Inicialializar sistema y perifericos
void setVela(); //Controlar vela
void setTimon(); //Controlar timon

//Variables Funciones del Servo
const float clockDiv = 64;
float wrap = 39062;
const int degree = 0;

//Modo operación (automático, control remoto)
const int opModePin = 10;
bool opMode = true;

//LED Interno
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

//Servomotor 1
const int servoPin1 = 0;
const int timonPin = 28;
uint16_t potenciometro;
float grados1;

//Servomotor 2
const int servoPin2 = 1;
const int rightPin = 12;
const int leftPin = 15;
float grados2 = 90;
bool rightButton = false;
bool leftButton = false;

uint8_t valReceive = 0;

//Modos de funcionamiento
bool automaticMode = false; //Iniciar en modo manual

//Factor de converion del adc
const float conversion_factor = 3.3f / (1 << 12);

void printBuff(){
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        printf("%u", buffer[i]);
    }
    printf("\n");
}

int main()
{
    setPeripherals();
    sleep_ms(5000);
    nrf24l01_driver NRF(SPI_PORT, 5, 6);
    
    printf("Configurando modulo...\n");
    NRF.default_config();
    NRF.goTo_rx();
    sleep_ms(5000);
    printf("Esperando datos....\n");

    while(1){

        if(!(NRF.data_inc())){
            //printf("Datos recibidos...\n");
            NRF.receive(buffer); //Recibir datos
            //printf("datos recibidos\n");
            printBuff();//Imprimir lo que se recibe
            valReceive = buffer[0]; //Pasar char a uint8_t por si las moscas (No funcionó)
            printf("Moviendo servo1 %u \n", valReceive); //Verificar el angulo en que se mueve el servo
            // setServo(servoPin1, valReceive); //Mover el servo en el angulo que llega
            setDegree(servoPin1, valReceive);
            sleep_ms(50);
            // if(buffer[0] == 255){//Cambiar entre modo automatico
            //     printf("Control automatico establecido \n");
            //     gpio_put(PICO_DEFAULT_LED_PIN, 1);
            // }
            // else if(buffer[0] == 254){//Cambiar a modo manual
            //     printf("Control manual establecido \n");
            //     gpio_put(PICO_DEFAULT_LED_PIN, 0);
            // }
            

            //printBuff();
            //setDegree(servoPin1, buffer[0]);
            //sleep_ms(1000);
        }
    }
}


//INICIALIZAR
void setPeripherals(){
    //SYS_INIT
    adc_init();
    stdio_init_all();

    //Modo de operación
    gpio_init(opModePin);
    gpio_set_dir(opModePin, GPIO_IN);

    //LED Interno
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    //Potenciometro
    adc_gpio_init(timonPin);
    adc_select_input(2);
    setServo(servoPin1, degree);
    setServo(servoPin2, degree);

    //Pulsadores
    gpio_init(rightPin);
    gpio_init(leftPin);
    gpio_set_dir(rightPin, GPIO_IN);
    gpio_set_dir(leftPin, GPIO_IN); 
}

//CONTROLAR MOTORES
void setVela(){
    potenciometro = adc_read();
    // printf("Raw value: 0x%03x, voltage: %f V\n", potenciometro, potenciometro * conversion_factor);
    grados1 = (potenciometro * conversion_factor) * (55);
    // printf("Grados: %f °\n", grados1);
    setDegree(servoPin1, grados1);
}
void setTimon(){
    rightButton = gpio_get(rightPin);
    leftButton = gpio_get(leftPin);
    if(rightButton & (grados2<=175)){
        // printf("Derecha \n");
        grados2 += 5;
    }
    else if(leftButton & (grados2>=5)){
        // printf("Izquierda \n");
        grados2 -= 5;
    }
    else if(grados2>=95){
        grados2 -= 5;
    }
    else if(grados2<=85){
        grados2 += 5;
    }
    setDegree(servoPin2, grados2);
}

//FUNCIONES DEL SERVO
void setDegree(int servoPin, float degree)
{
    float millis;
    millis = ((100/9)*degree) + 400;
    pwm_set_gpio_level(servoPin, (millis/20000.f)*wrap);
}

void setServo(int servoPin, float startDegree)
{
    gpio_set_function(servoPin, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(servoPin);

    pwm_config config = pwm_get_default_config();
    
    uint64_t clockspeed = clock_get_hz(clk_sys); //Get the current frequency of the specified clock

    wrap = clockspeed/clockDiv/50;

    pwm_config_set_clkdiv(&config, clockDiv);
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);

    float startMillis;
    startMillis = ((100/9)*startDegree) + 400;
    setDegree(servoPin, startMillis);
}