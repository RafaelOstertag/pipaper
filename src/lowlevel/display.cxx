#include "display.hh"

#include <sstream>
#include <stdexcept>
#ifndef NDEBUG
#include <iostream>
#endif

using namespace lowlevel;

namespace {
constexpr auto bitsPerByte = 8;

inline void validateX(uint8_t x) {
    if (x > displayWidth) {
        std::stringstream message;
        message << "x is greater than " << displayWidth - 1;
        throw std::invalid_argument(message.str());
    }
}

inline void validateY(uint8_t y) {
    if (y > displayHeight) {
        std::stringstream message;
        message << "y is greater than " << displayHeight - 1;
        throw std::invalid_argument(message.str());
    }
}
} // namespace

DisplayMatrix::DisplayMatrix() : matrix{} {
    for (int i = 0; i < displayHeight; i++)
        matrix[i].fill(0xff);
}

DisplayMatrix::DisplayMatrix(DisplayMatrix&& o) : matrix{std::move(o.matrix)} {}

DisplayMatrix& DisplayMatrix::operator=(DisplayMatrix&& o) {
    matrix = std::move(o.matrix);
    return *this;
}

DisplayMatrix::Iterator DisplayMatrix::begin() const { return Iterator{*this}; }

DisplayMatrix::Iterator DisplayMatrix::end() const {
    return Iterator{*this, logicalWidth * displayHeight, 0xff};
}

void DisplayMatrix::set(uint8_t x, uint8_t y) {
    validateX(x);
    validateY(y);

    auto byteHoldingX = getByte(x, y);

    auto bitIndex = x % bitsPerByte;
    uint8_t bitMask = 1 << (bitsPerByte - 1 - bitIndex);
    byteHoldingX &= ~bitMask;

    setByte(x, y, byteHoldingX);
}

void DisplayMatrix::clear(uint8_t x, uint8_t y) {
    validateX(x);
    validateY(y);

    auto byteHoldingX = getByte(x, y);

    auto bitIndex = x % bitsPerByte;
    uint8_t bitMask = 1 << (bitsPerByte - 1 - bitIndex);
    byteHoldingX |= bitMask;

    setByte(x, y, byteHoldingX);
}

void DisplayMatrix::setAll() {
    for (int i = 0; i < displayHeight; i++) {
        matrix[i].fill(0x0);
    }
}

void DisplayMatrix::clearAll() {
    for (int i = 0; i < displayHeight; i++) {
        matrix[i].fill(0xff);
    }
}

#ifndef NDEBUG
void DisplayMatrix::_dumpMatrix() const {
    for (auto y = displayHeight - 1; y >= 0; y--) {
        for (auto x = 0; x < logicalWidth; x++) {
            auto byte = matrix[y][x];
            uint8_t bitmask = 0x80;
            for (auto bit = 0; bit < 8; bit++) {
                if (byte & bitmask) {
                    std::cerr << '.';
                } else {
                    std::cerr << '0';
                }
                bitmask >>= 1;
            }
        }
        std::cerr << std::endl;
    }
}
#endif

/**
 * Get the byte out of the matrix holding the bit for a given X coordinate
 */
uint8_t DisplayMatrix::getByte(uint8_t x, uint8_t y) const {
    auto byteIndex = x / bitsPerByte;
    return matrix[y][byteIndex];
}

/**
 * Set the byte in the matrix holding the bit for a given X coordinate
 */
void DisplayMatrix::setByte(uint8_t x, uint8_t y, uint8_t value) {
    auto byteIndex = x / bitsPerByte;
    matrix[y][byteIndex] = value;
}
