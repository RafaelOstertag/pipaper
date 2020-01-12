#include "sevensegment.hh"

#include <array>
#include <bitset>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
constexpr int height = 16;
constexpr int width = 5;
constexpr int padding = 2;
// clang-format off
std::array<std::bitset<width>, height> leftBitmap {
    std::bitset<width>{"10000"},
    std::bitset<width>{"11000"}, 
    std::bitset<width>{"11100"},
    std::bitset<width>{"11110"}, 
    std::bitset<width>{"11111"},
    std::bitset<width>{"11111"},
    std::bitset<width>{"11111"},
    std::bitset<width>{"11111"}, 
    std::bitset<width>{"11111"},
    std::bitset<width>{"11111"}, 
    std::bitset<width>{"11111"}, 
    std::bitset<width>{"11110"},
    std::bitset<width>{"11100"}, 
    std::bitset<width>{"11000"},
    std::bitset<width>{"10000"}
};

std::array<std::bitset<height>, width> topBitmap {
    std::bitset<height>{"1111111111111111"}, 
    std::bitset<height>{"0111111111111110"}, 
    std::bitset<height>{"0011111111111100"},
    std::bitset<height>{"0001111111111000"},
    std::bitset<height>{"0000111111110000"}
};

std::array<std::bitset<height>, width> middleBitmap {
    std::bitset<height>{"0011111111111100"}, 
    std::bitset<height>{"0111111111111110"}, 
    std::bitset<height>{"1111111111111111"}, 
    std::bitset<height>{"0111111111111110"},
    std::bitset<height>{"0011111111111100"}
};

// clang-format on
} // namespace

SevenSegment::SevenSegment(Canvas& canvas, Orientation rotation = PORTRAIT)
    : canvas{canvas}, orientation{rotation} {}

void SevenSegment::show(uint8_t x, uint8_t y, const std::string& digits) {
    auto currentX = x;
    auto currentY = y;
    for (auto it = digits.cbegin(); it != digits.cend(); ++it) {
        show(currentX, currentY, std::stoi(std::string{*it}));
        switch (orientation) {
        case PORTRAIT:
            currentX += 5 * padding + height;
            break;
        case LANDSCAPE:
            currentY -= 5 * padding + height;
            break;
        }
    }
}

void SevenSegment::show(uint8_t x, uint8_t y, uint8_t number) {
    switch (number) {
    case 0:
        turnOn0(x, y);
        turnOn1(x, y);
        turnOn2(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        break;
    case 1:
        turnOn5(x, y);
        turnOn4(x, y);
        break;
    case 2:
        turnOn0(x, y);
        turnOn5(x, y);
        turnOn6(x, y);
        turnOn2(x, y);
        turnOn3(x, y);
        break;
    case 3:
        turnOn0(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        turnOn6(x, y);
        break;
    case 4:
        turnOn1(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        turnOn6(x, y);
        break;
    case 5:
        turnOn0(x, y);
        turnOn1(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn6(x, y);
        break;
    case 6:
        turnOn0(x, y);
        turnOn1(x, y);
        turnOn2(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn6(x, y);
        break;
    case 7:
        turnOn0(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        break;
    case 8:
        turnOn0(x, y);
        turnOn1(x, y);
        turnOn2(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        turnOn6(x, y);
        break;
    case 9:
        turnOn0(x, y);
        turnOn1(x, y);
        turnOn3(x, y);
        turnOn4(x, y);
        turnOn5(x, y);
        turnOn6(x, y);
        break;
    default:
        std::stringstream message;
        message << "Number " << number
                << " cannot be display. Choose a value between 0 and 9";
        throw std::invalid_argument(message.str());
    }
}

void SevenSegment::drawPixel(uint8_t startX, uint8_t startY, uint8_t offsetX,
                             uint8_t offsetY) {
    uint8_t pixelX;
    uint8_t pixelY;
    switch (orientation) {
    case PORTRAIT:
        pixelX = startX + offsetX;
        pixelY = startY + offsetY;
        break;
    case LANDSCAPE:
        pixelX = startX + offsetY;
        pixelY = startY - offsetX;
        break;
    }

    canvas.setPixel(pixelX, pixelY);
}

void SevenSegment::drawTop(uint8_t x, uint8_t y) {
    for (int indexX = 0; indexX < height; indexX++) {
        for (int indexY = 0; indexY < width; indexY++) {
            if (topBitmap[width - 1 - indexY].test(indexX))
                drawPixel(x, y, indexX, indexY);
        }
    }
}

void SevenSegment::drawLeft(uint8_t x, uint8_t y) {
    for (int indexX = 0; indexX < width; indexX++) {
        for (int indexY = 0; indexY < height; indexY++) {
            if (leftBitmap[indexY].test(width - indexX - 1))
                drawPixel(x, y, indexX, indexY);
        }
    }
}

void SevenSegment::drawBottom(uint8_t x, uint8_t y) {
    for (int indexX = 0; indexX < height; indexX++) {
        for (int indexY = 0; indexY < width; indexY++) {
            if (topBitmap[indexY].test(indexX))
                drawPixel(x, y, indexX, indexY);
        }
    }
}
void SevenSegment::drawRight(uint8_t x, uint8_t y) {
    for (int indexX = 0; indexX < width; indexX++) {
        for (int indexY = 0; indexY < height; indexY++) {
            if (leftBitmap[indexY].test(indexX))
                drawPixel(x, y, indexX, indexY);
        }
    }
}

void SevenSegment::drawMiddle(uint8_t x, uint8_t y) {
    for (int indexX = 0; indexX < height; indexX++) {
        for (int indexY = 0; indexY < width; indexY++) {
            if (middleBitmap[indexY].test(indexX))
                drawPixel(x, y, indexX, indexY);
        }
    }
}

/*
 *             0
 *          +-----+
 *          |     |
 *        1 |     | 5
 *          |  6  |
 *          +-----+
 *          |     |
 *        2 |     | 4
 *          |  3  |
 *          +-----+
 *       (x,y)
 *
 *    ^
 *    |
 * y  |
 *    |
 *    +--->
 * (0,0)  x
 */

inline void SevenSegment::turnOn0(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawTop(x + padding, y + 2 * height + 3 * padding - width);
        break;
    case LANDSCAPE:
        drawTop(x + 2 * height + 3 * padding - width, y - padding);
        break;
    }
}

inline void SevenSegment::turnOn1(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawLeft(x, y + height + 2 * padding);
        break;
    case LANDSCAPE:
        drawLeft(x + height + 2 * padding, y);
        break;
    }
}

inline void SevenSegment::turnOn2(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawLeft(x, y + padding);
        break;
    case LANDSCAPE:
        drawLeft(x + padding, y);
        break;
    }
}

inline void SevenSegment::turnOn3(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawBottom(x + padding, y);
        break;
    case LANDSCAPE:
        drawBottom(x, y - padding);
        break;
    }
}

inline void SevenSegment::turnOn4(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawRight(x + height + 2 * padding - width, y + padding);
        break;
    case LANDSCAPE:
        drawRight(x + padding, y - (height + 2 * padding - width));
        break;
    }
}

inline void SevenSegment::turnOn5(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawRight(x + height + 2 * padding - width, y + height + 2 * padding);
        break;
    case LANDSCAPE:
        drawRight(x + height + 2 * padding, y - (height + 2 * padding - width));
        break;
    }
}

inline void SevenSegment::turnOn6(uint8_t x, uint8_t y) {
    switch (orientation) {
    case PORTRAIT:
        drawMiddle(x + padding, y + height - width + 2 * padding + 1);
        break;
    case LANDSCAPE:
        drawMiddle(x + height - width + 2 * padding + 1, y - padding);
        break;
    }
}
