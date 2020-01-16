#ifndef __CANVAS_HH
#define __CANVAS_HH

#include "lowlevel/epd213v2.hh"

class Canvas {
  public:
    Canvas(lowlevel::Epd213V2Ptr& epaper);
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;
    Canvas(Canvas&&) = delete;
    Canvas& operator=(Canvas&&) = delete;

    ~Canvas();

    void discard();
    void show();

    void setPixel(uint8_t x, uint8_t y, bool enabled = true);

  private:
    lowlevel::Epd213V2Ptr& epaper;
    lowlevel::DisplayMatrix display;
};

using CanvasPtr = std::unique_ptr<Canvas>;

#endif
