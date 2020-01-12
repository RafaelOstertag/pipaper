#include "canvas.hh"

Canvas::Canvas(lowlevel::Epd213V2* epaper) : epaper{epaper}, display{} {
    epaper->initialize(lowlevel::FULL);
    epaper->clear();

    lowlevel::DisplayMatrix cleanMatrix;
    cleanMatrix.clearAll();
    epaper->displayPartialBaseImage(cleanMatrix);

    epaper->initialize(lowlevel::PARTIAL);

    discard();
}

Canvas::~Canvas() {
    epaper->initialize(lowlevel::FULL);
    epaper->clear();
}

void Canvas::discard() { display.clearAll(); }

void Canvas::paint() { epaper->displayPartial(display); }

void Canvas::setPixel(uint8_t x, uint8_t y, bool enabled) {
    if (enabled)
        display.set(x, y);
    else
        display.clear(x, y);
}
