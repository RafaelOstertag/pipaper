#include "spi.hh"
#include "../debug.hh"

#ifndef NDEBUG
#include <iostream>
#endif
#include <bcm2835.h>
#include <stdexcept>

using namespace lowlevel;

bool Spi::initialized{false};

Spi::Spi() {
    if (!bcm2835_init()) {
        throw std::runtime_error("Cannot initialize bcm2835");
    }

    initGpio();
    initSpi();

    DEBUG_OUT("Initialized");
    Spi::initialized = true;
}

Spi::~Spi() {
    if (!Spi::initialized) {
        return;
    }

    stopGpio();
    stopSpi();
}

void Spi::wait(uint32_t ms) { bcm2835_delay(ms); }

void Spi::gpioWrite(Pin pin, uint8_t value) {
    bcm2835_gpio_write(static_cast<uint8_t>(pin), value);
}

uint8_t Spi::gpioRead(Pin pin) {
    return bcm2835_gpio_lev(static_cast<uint8_t>(pin));
}

void Spi::write(uint8_t value) { bcm2835_spi_transfer(value); }

void Spi::write(uint8_t* data, uint32_t len) {
    char* readBuffer = new char[len];
    bcm2835_spi_transfernb(reinterpret_cast<char*>(data), readBuffer, len);
    delete[] readBuffer;
}

void Spi::initGpio() {
    DEBUG_OUT("Initialize GPIO");
    setGpioMode(RST, OUTPUT);
    setGpioMode(DC, OUTPUT);
    setGpioMode(CS, OUTPUT);
    setGpioMode(BUSY, INPUT);

    gpioWrite(CS, 1);
}

void Spi::stopGpio() {
    DEBUG_OUT("Stopping GPIO");
    gpioWrite(CS, 0);
    gpioWrite(DC, 0);
    gpioWrite(RST, 0);
}

void Spi::initSpi() {
    DEBUG_OUT("Initialize SPI");
    bcm2835_spi_begin(); // Start spi interface, set spi pin for the reuse
                         // function
    bcm2835_spi_setBitOrder(
        BCM2835_SPI_BIT_ORDER_MSBFIRST);        // High first transmission
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); // spi mode 0
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); // Frequency
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                    // set CE0
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);    // enable cs0
}

void Spi::stopSpi() {
    DEBUG_OUT("Stopping SPI");
    bcm2835_spi_end();
    bcm2835_close();
}

inline void Spi::setGpioMode(Pin pin, Bcm2835Mode mode) {
    bcm2835_gpio_fsel(static_cast<uint8_t>(pin), static_cast<uint8_t>(mode));
}
