#include "canvas.hh"
#include "sevensegment.hh"

#include <chrono>
#include <thread>

int main() {
    auto epaper = new lowlevel::Epd213V2{};
    auto canvas = new Canvas{epaper};

    SevenSegment sevenSegment{*canvas, SevenSegment::LANDSCAPE};

    sevenSegment.show(50, 0, "12:00");
    canvas->paint();
    std::this_thread::sleep_for(std::chrono::seconds{1});

    canvas->discard();
    sevenSegment.show(50, 0, "12 00");
    canvas->paint();
    std::this_thread::sleep_for(std::chrono::seconds{1});

    delete canvas;
    delete epaper;
}
