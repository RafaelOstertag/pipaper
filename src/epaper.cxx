#include "canvas.hh"
#include "sevensegment.hh"

#include <unistd.h>

int main() {
    auto epaper = new lowlevel::Epd213V2{};
    auto canvas = new Canvas{epaper};

    SevenSegment sevenSegment{*canvas, SevenSegment::LANDSCAPE};

    sevenSegment.show(50, 250, "01234");
    sevenSegment.show(0, 250, "56789");

    canvas->paint();

    sleep(1);

    delete canvas;
    delete epaper;
}
