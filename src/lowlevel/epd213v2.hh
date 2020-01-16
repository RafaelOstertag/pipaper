#ifndef __EPD213V2_HH
#define __EPD213V2_HH

#include "display.hh"
#include "spi.hh"

#include <cstdint>
#include <memory>

namespace lowlevel {
enum EpdInitMode { FULL, PARTIAL };

class Epd213V2 {

  public:
    Epd213V2(SpiPtr& spi);
    Epd213V2(const Epd213V2&) = delete;
    Epd213V2& operator=(const Epd213V2&) = delete;
    Epd213V2(Epd213V2&&) = delete;
    Epd213V2& operator=(Epd213V2&&) = delete;

    void initialize(EpdInitMode mode);
    void clear();
    void display(const DisplayMatrix& displayMatrix);

    void displayPartialBaseImage(const DisplayMatrix& displayMatrix);
    void displayPartial(const DisplayMatrix& displayMatrix);

    void wait(uint32_t ms);

    void sleep();

  private:
    SpiPtr spi;

    void waitReady();
    void reset();
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    void turnOnDisplay();
    void turnOnDisplayPartial();

    void initFull();
    void initPartial();
};

using Epd213V2Ptr = std::unique_ptr<Epd213V2>;

} // namespace lowlevel

#endif
