#include "bmp.hh"
#include "canvas.hh"
#include "os/signal.hh"
#include "sevensegment.hh"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

class ExitHandler : public os::SignalHandler {
  public:
    ExitHandler(volatile bool* keepRunning) : keepRunning{keepRunning} {}

    virtual void handle(int signal, siginfo_t* info, void* userContext) {
        *keepRunning = false;
    }

  private:
    volatile bool* keepRunning;
};

int main() {
    auto canvas = createCanvas();

    SevenSegment sevenSegment{*canvas, SevenSegment::LANDSCAPE};

    volatile bool keepRunning = true;
    os::SignalHandlerPtr exitHandler{new ExitHandler{&keepRunning}};

    os::Signal::action(SIGTERM, exitHandler);
    os::Signal::action(SIGINT, exitHandler);
    os::Signal::action(SIGQUIT, exitHandler);

    while (keepRunning) {
        auto seconds = std::time(nullptr);
        auto localTime = std::localtime(&seconds);

        canvas->discard();

        std::stringstream ascTime;
        if (localTime->tm_sec % 15 == 0) {
            std::this_thread::sleep_for(std::chrono::seconds{1});

            BitmapFile bitmapFile{"/tmp/test.bmp"};
            bitmapFile.paint(*canvas);
            canvas->show();

            std::this_thread::sleep_for(std::chrono::seconds{5});
        } else {
            ascTime << std::setfill('0') << std::setw(2) << localTime->tm_hour
                    << ':' << std::setfill('0') << std::setw(2)
                    << localTime->tm_min << ':' << std::setfill('0')
                    << std::setw(2) << localTime->tm_sec;

            sevenSegment.paint(50, 249, ascTime.str());

            canvas->show();
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }
    }
}
