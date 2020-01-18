#include "canvas.hh"

Canvas::Canvas(lowlevel::Epd213V2Ptr& epaper)
    : epaper{std::move(epaper)}, display{} {
    this->epaper->initialize(lowlevel::FULL);
    this->epaper->clear();

    lowlevel::DisplayMatrix cleanMatrix;
    cleanMatrix.clearAll();
    this->epaper->displayPartialBaseImage(cleanMatrix);

    this->epaper->initialize(lowlevel::PARTIAL);

    discard();
}

Canvas::Canvas(Canvas&& o)
    : epaper{std::move(o.epaper)}, display{std::move(o.display)} {}

Canvas& Canvas::operator=(Canvas&& o) {
    epaper = std::move(o.epaper);
    display = std::move(o.display);
    return *this;
}

void Canvas::discard() { display.clearAll(); }

void Canvas::show() { epaper->displayPartial(display); }

void Canvas::setPixel(uint8_t x, uint8_t y, bool enabled) {
    if (enabled)
        display.set(x, y);
    else
        display.clear(x, y);
}

CanvasPtr createCanvas() {
    auto spi = lowlevel::SpiPtr{new lowlevel::Spi{}};
    auto epaper = lowlevel::Epd213V2Ptr{new lowlevel::Epd213V2{spi}};
    return CanvasPtr{new Canvas{epaper}};
}
