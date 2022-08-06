#include "hardware/spi.h"
#include "hardware/gpio.h"

class nrf24l01_driver
{
private:

public: // Vars.
    spi_inst_t *spi_;
    uint16_t cs_;
    uint16_t ce_;

    nrf24l01_driver(spi_inst_t *port, uint16_t csn, uint16_t ce);
    ~nrf24l01_driver();

    uint8_t readReg(uint8_t reg);

    void writeReg( uint8_t reg, uint8_t data);
    void writeReg( uint8_t reg, uint8_t *data, uint8_t size);

    void config();

    void spiConfig();

    void modeTX();
    void modeRX();

    void sendMessage(char *msg);
    void receiveMessage(char *msg);

    uint8_t newMessage();

    void setChannel(uint8_t ch);
    void setRXName(char *name);
    void setTXName(char *name);


};