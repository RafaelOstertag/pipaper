#ifndef __SPI_HH
#define __SPI_HH

#include "bcm2835modes.hh"
#include "pins.hh"

#include <memory>
#include <cstdint>

namespace lowlevel {
class Spi {
  public:
    Spi();
    Spi(const Spi&) = delete;
    Spi& operator=(const Spi&) = delete;
    Spi(Spi&&) = delete;
    Spi& operator=(Spi&&) = delete;

    ~Spi();

    void wait(uint32_t ms);

    void gpioWrite(Pin pin, uint8_t value);
    uint8_t gpioRead(Pin pin);

    void write(uint8_t value);
    void write(uint8_t* data, uint32_t len);

  private:
    static bool initialized;

    void initGpio();
    void stopGpio();

    void initSpi();
    void stopSpi();

    void setGpioMode(Pin pin, Bcm2835Mode mode);
};

using SpiPtr = std::unique_ptr<Spi>;
} // namespace lowlevel

#endif
