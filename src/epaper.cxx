#include "bmp.hh"
#include "canvas.hh"
#include "sevensegment.hh"

#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

int main() {
    auto canvas = createCanvas();

    SevenSegment sevenSegment{*canvas, SevenSegment::LANDSCAPE};

    while (false) {
        auto seconds = std::time(nullptr);
        auto localTime = std::localtime(&seconds);

        canvas->discard();

        std::stringstream ascTime;
        ascTime << localTime->tm_hour << ':' << localTime->tm_min;

        sevenSegment.paint(50, 200, ascTime.str());

        canvas->show();
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }

    /* canvas->discard();
    sevenSegment.show(50, 0, "12 00");
    canvas->paint();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    BitmapFile bitmapFile{"/tmp/test.bmp"};
    bitmapFile.show(*canvas);

    canvas->paint();
    std::this_thread::sleep_for(std::chrono::seconds{1}); */
}
