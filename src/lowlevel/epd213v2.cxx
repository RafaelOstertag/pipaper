#include "epd213v2.hh"
#include "../debug.hh"
#include "display.hh"
#include "spi.hh"

#include <stdexcept>

using namespace lowlevel;

namespace {
constexpr uint32_t POLLWAIT = 100;

const unsigned char lutFullUpdate[] = {
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, // LUT0: BB:     VS 0 ~7
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, // LUT1: BW:     VS 0 ~7
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, // LUT2: WB:     VS 0 ~7
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, // LUT3: WW:     VS 0 ~7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT4: VCOM:   VS 0 ~7

    0x03, 0x03, 0x00, 0x00, 0x02, // TP0 A~D RP0
    0x09, 0x09, 0x00, 0x00, 0x02, // TP1 A~D RP1
    0x03, 0x03, 0x00, 0x00, 0x02, // TP2 A~D RP2
    0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
    0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
    0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
    0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6

    0x15, 0x41, 0xA8, 0x32, 0x30, 0x0A,
};

const unsigned char lutPartialUpdate[] = {
    // 20 bytes
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT0: BB:     VS 0 ~7
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT1: BW:     VS 0 ~7
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT2: WB:     VS 0 ~7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT3: WW:     VS 0 ~7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // LUT4: VCOM:   VS 0 ~7

    0x0A, 0x00, 0x00, 0x00, 0x00, // TP0 A~D RP0
    0x00, 0x00, 0x00, 0x00, 0x00, // TP1 A~D RP1
    0x00, 0x00, 0x00, 0x00, 0x00, // TP2 A~D RP2
    0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
    0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
    0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
    0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6

    0x15, 0x41, 0xA8, 0x32, 0x30, 0x0A,
};

} // namespace

Epd213V2::Epd213V2(SpiPtr& spi) : spi{std::move(spi)} {}

Epd213V2::Epd213V2(Epd213V2&& o) : spi{std::move(o.spi)} {}

Epd213V2& Epd213V2::operator=(Epd213V2&& o) {
    spi = std::move(o.spi);
    return *this;
}

void Epd213V2::initialize(EpdInitMode epdInitMode) {
    switch (epdInitMode) {
    case FULL:
        initFull();
        break;
    case PARTIAL:
        initPartial();
        break;

    default:
        throw std::invalid_argument("Init mode is unknown");
    }
}

void Epd213V2::display(const DisplayMatrix& displayMatrix) {
    DEBUG_OUT("Display image");
#ifndef NDEBUG
    displayMatrix._dumpMatrix();
#endif
    sendCommand(0x24);
    for (auto value : displayMatrix) {
        sendData(value);
    }
    turnOnDisplay();
}

void Epd213V2::displayPartialBaseImage(const DisplayMatrix& displayMatrix) {
    DEBUG_OUT("Display Partial Base Image");
#ifndef NDEBUG
    displayMatrix._dumpMatrix();
#endif
    sendCommand(0x24);
    for (auto value : displayMatrix) {
        sendData(value);
    }

    sendCommand(0x26);
    for (auto value : displayMatrix) {
        sendData(value);
    }

    turnOnDisplay();
}

void Epd213V2::displayPartial(const DisplayMatrix& displayMatrix) {
    DEBUG_OUT("Display image partial");
#ifndef NDEBUG
    displayMatrix._dumpMatrix();
#endif
    sendCommand(0x24);
    for (auto value : displayMatrix) {
        sendData(value);
    }
    turnOnDisplayPartial();
}

void Epd213V2::wait(uint32_t ms) { spi->wait(ms); }

void Epd213V2::sleep() {
    DEBUG_OUT("Enter deep sleep");
    sendCommand(0x10);
    sendData(0x01);
    spi->wait(100);
}

void Epd213V2::clear() {
    DEBUG_OUT("Clear display");
    sendCommand(0x24);
    for (uint8_t j = 0; j < displayHeight; j++) {
        for (uint8_t i = 0; i < logicalWidth; i++) {
            sendData(0xff);
        }
    }

    turnOnDisplay();
}

void Epd213V2::waitReady() {
    DEBUG_OUT("Waiting for display to get ready");
    while (spi->gpioRead(BUSY) == 1)
        spi->wait(POLLWAIT);

    DEBUG_OUT("Display ready");
}

void Epd213V2::reset() {
    spi->gpioWrite(RST, 1);
    spi->wait(200);
    spi->gpioWrite(RST, 0);
    spi->wait(10);
    spi->gpioWrite(RST, 1);
    spi->wait(200);
}

void Epd213V2::sendCommand(uint8_t command) {
    spi->gpioWrite(DC, 0);
    spi->gpioWrite(CS, 0);
    spi->write(command);
    spi->gpioWrite(CS, 1);
}

void Epd213V2::sendData(uint8_t data) {
    spi->gpioWrite(DC, 1);
    spi->gpioWrite(CS, 0);
    spi->write(data);
    spi->gpioWrite(CS, 1);
}

void Epd213V2::turnOnDisplay() {
    sendCommand(0x22);
    sendData(0xC7);
    sendCommand(0x20);
    waitReady();
}

void Epd213V2::turnOnDisplayPartial() {
    sendCommand(0x22);
    sendData(0x0C);
    sendCommand(0x20);
    waitReady();
}

void Epd213V2::initFull() {
    DEBUG_OUT("Initialize full");
    reset();

    waitReady();
    sendCommand(0x12); // soft reset
    waitReady();

    sendCommand(0x74); // set analog block control
    sendData(0x54);
    sendCommand(0x7E); // set digital block control
    sendData(0x3B);

    sendCommand(0x01); // Driver output control
    sendData(0xF9);
    sendData(0x00);
    sendData(0x00);

    sendCommand(0x11); // data entry mode
    sendData(0x01);

    sendCommand(0x44); // set Ram-X address start/end position
    sendData(0x00);
    sendData(0x0F); // 0x0C-->(15+1)*8=128

    sendCommand(0x45); // set Ram-Y address start/end position
    sendData(0xF9);    // 0xF9-->(249+1)=250
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);

    sendCommand(0x3C); // BorderWavefrom
    sendData(0x03);

    sendCommand(0x2C); // VCOM Voltage
    sendData(0x55);    //

    sendCommand(0x03);
    sendData(lutFullUpdate[70]);

    sendCommand(0x04); //
    sendData(lutFullUpdate[71]);
    sendData(lutFullUpdate[72]);
    sendData(lutFullUpdate[73]);

    sendCommand(0x3A); // Dummy Line
    sendData(lutFullUpdate[74]);
    sendCommand(0x3B); // Gate time
    sendData(lutFullUpdate[75]);

    sendCommand(0x32);
    for (int count = 0; count < 70; count++) {
        sendData(lutFullUpdate[count]);
    }

    sendCommand(0x4E); // set RAM x address count to 0;
    sendData(0x00);
    sendCommand(0x4F); // set RAM y address count to 0X127;
    sendData(0xF9);
    sendData(0x00);
    waitReady();
}

void Epd213V2::initPartial() {
    DEBUG_OUT("Initialize partial");
    reset();

    sendCommand(0x2C); // VCOM Voltage
    sendData(0x26);

    waitReady();

    sendCommand(0x32);
    for (int count = 0; count < 70; count++) {
        sendData(lutPartialUpdate[count]);
    }

    sendCommand(0x37);
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);
    sendData(0x40);
    sendData(0x00);
    sendData(0x00);

    sendCommand(0x22);
    sendData(0xC0);

    sendCommand(0x20);
    waitReady();

    sendCommand(0x3C); // BorderWavefrom
    sendData(0x01);
}
