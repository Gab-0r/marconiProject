#include "nrf24l01.h"
#include "nrf24l01_driver.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <string.h>

#define LOW 0
#define HIGH 1

nrf24l01_driver::nrf24l01_driver(spi_inst_t *spi, uint16_t cs, uint16_t ce)
{
    spi_ = spi;
    cs_ = cs;
    ce_ = ce;

    gpio_init(cs_);
    gpio_init(ce_);

    gpio_set_dir(cs_, 1);
    gpio_set_dir(ce_, 1);

    gpio_put(ce_, LOW);
    gpio_put(cs_, HIGH);

    spiConfig();

}

nrf24l01_driver::~nrf24l01_driver()
{
}

void nrf24l01_driver::spiConfig(){
    spi_init(spi_, 1000000 );
    gpio_set_function(2, GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);
    gpio_set_function(4, GPIO_FUNC_SPI);
}

uint8_t nrf24l01_driver::readReg(uint8_t reg){
    uint8_t result = 0;
    reg = ( 0b00011111 & reg);
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &reg,1);
    spi_read_blocking(spi_, 0xff,&result, 1);
    gpio_put(cs_, 1);

    return result;
}

void nrf24l01_driver::writeReg(uint8_t reg, uint8_t data){
    writeReg(reg, &data, 1);
}
void nrf24l01_driver::writeReg(uint8_t reg, uint8_t *data, uint8_t size){
    reg = 0b00100000 | ( 0b00011111 & reg);
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &reg, 1);
    spi_write_blocking(spi_, (uint8_t*)data, size);
    gpio_put(cs_, 1);
}

void nrf24l01_driver::config(){
    gpio_put(cs_, 1);
    gpio_put(ce_, 0);
    sleep_ms(11);

    writeReg(0, 0b00001010); // config.
    sleep_us(1500);

    writeReg(1,0); // no ack.

    writeReg(5, 120); // channel.

    writeReg(0x0a, (uint8_t*)"gyroc",5);
    writeReg(0x10, (uint8_t*)"gyroc",5);

    writeReg(0x11, 32);
}

void nrf24l01_driver::modeTX(){
    uint8_t reg = readReg(0);
    reg &= ~(1<<0);
    writeReg(0, reg);
    
    gpio_put(ce_, 0);
    
    sleep_us(130);
}
void nrf24l01_driver::modeRX(){
    uint8_t reg = readReg(0);
    reg |= (1<<0);
    writeReg(0, reg);
    
    gpio_put(ce_, 1);
    
    sleep_us(130);

}

void nrf24l01_driver::sendMessage(char *msg){
    uint8_t reg = 0b10100000;
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &reg,1);
    spi_write_blocking(spi_, (uint8_t*)msg, 32);
    gpio_put(cs_, 1);

    gpio_put(ce_, 1);
    sleep_us(10);
    gpio_put(ce_, 0);
}
void nrf24l01_driver::receiveMessage(char *msg){
    uint8_t cmd = 0b01100001;   
    gpio_put(cs_, 0);
    spi_write_blocking(spi_, &cmd, 1);

    spi_read_blocking(spi_,0xff, (uint8_t*)msg,32);
    gpio_put(cs_, 1);;

}

uint8_t nrf24l01_driver::newMessage(){
    uint8_t fifo_status = readReg(0x17);

    return !(0x00000001 & fifo_status);
}

void nrf24l01_driver::setChannel(uint8_t ch){
    writeReg(5, ch); // channel.
}

void nrf24l01_driver::setRXName(char *name){
    if( strlen(name) != 5) return;
    writeReg(0x0a, (uint8_t*)name,5);
}

void nrf24l01_driver::setTXName(char *name){
    if( strlen(name) != 5) return;
    writeReg(0x10, (uint8_t*)name,5);
}
