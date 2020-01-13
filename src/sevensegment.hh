#ifndef __SEVENSEGMENT_HH
#define __SEVENSEGMENT_HH

#include "canvas.hh"
#include <cstdint>
#include <string>

class SevenSegment {
  public:
    enum Orientation { LANDSCAPE, PORTRAIT };
    SevenSegment(Canvas& canvas, Orientation orientation);

    void show(uint8_t x, uint8_t y, const std::string& digits);

    void show(uint8_t x, uint8_t y, uint8_t number);

  private:
    Canvas& canvas;
    Orientation orientation;
    void drawPixel(uint8_t startX, uint8_t startY, uint8_t offsetX,
                   uint8_t offsetY);
    void drawTop(uint8_t x, uint8_t y);
    void drawLeft(uint8_t x, uint8_t y);
    void drawBottom(uint8_t x, uint8_t y);
    void drawRight(uint8_t x, uint8_t y);
    void drawMiddle(uint8_t x, uint8_t y);
    void drawColon(uint8_t x, uint8_t y);

    void turnOn0(uint8_t x, uint8_t y);
    void turnOn1(uint8_t x, uint8_t y);
    void turnOn2(uint8_t x, uint8_t y);
    void turnOn3(uint8_t x, uint8_t y);
    void turnOn4(uint8_t x, uint8_t y);
    void turnOn5(uint8_t x, uint8_t y);
    void turnOn6(uint8_t x, uint8_t y);
};

#endif
