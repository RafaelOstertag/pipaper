#include "bmp.hh"
#include "canvas.hh"
#include "dirlist.hh"
#include "os/signal.hh"
#include "sevensegment.hh"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
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
            DirectoryList directoryList("/var/lib/epaper/bitmaps");
            auto files = directoryList.list();
            if (!files.empty()) {

                std::random_shuffle(files.begin(), files.end());
                auto filename = files[0];

                std::cout << "displaying file " << filename << '\n';

                BitmapFile bitmapFile{filename};
                bitmapFile.paint(*canvas);

                canvas->show();
            }

            std::this_thread::sleep_for(std::chrono::seconds{5});
        } else {
            ascTime << std::setfill('0') << std::setw(2) << localTime->tm_hour
                    << ':' << std::setfill('0') << std::setw(2)
                    << localTime->tm_min << ':' << std::setfill('0')
                    << std::setw(2) << localTime->tm_sec;

            sevenSegment.paint(50, 249, ascTime.str());

            canvas->show();
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
        }
    }
}
