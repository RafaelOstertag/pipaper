#include "bmp.hh"
#include "lowlevel/display.hh"

#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace {
struct BitmapInfoHeader {
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colors;
    uint32_t importantColors;
};

struct BmpHeader {
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    BitmapInfoHeader dibHeader;
};

constexpr int identifierSize{2};
constexpr int headerSize{sizeof(BmpHeader)};
constexpr int pixelWordSize{sizeof(uint32_t)};

inline bool isBmpFile(const char* identifier) {
    return identifier[0] == 'B' && identifier[1] == 'M';
}
} // namespace

BitmapFile::BitmapFile(const std::string& filename) : filename{filename} {}

void BitmapFile::show(Canvas& canvas) {
    auto bmpFile =
        std::fstream{filename, std::ios_base::in | std::ios_base::binary};

    char identifier[identifierSize];
    bmpFile.read(identifier, identifierSize);
    if (bmpFile.eof() || bmpFile.fail() || !isBmpFile(identifier)) {
        throw std::runtime_error(filename + " is not a BMP file");
    }

    BmpHeader bmpHeader;
    bmpFile.read(reinterpret_cast<char*>(&bmpHeader), headerSize);
    if (bmpFile.eof()) {
        throw std::runtime_error(filename + " is not a valid BMP file");
    }

    if (bmpHeader.dibHeader.bitsPerPixel != 1) {
        throw std::runtime_error(filename +
                                 " is not a monochrome file (bitsPerPixel!=1)");
    }

    if (bmpHeader.dibHeader.compression != 0) {
        throw std::runtime_error(filename +
                                 " compressed BMP files not supported");
    }

    bmpFile.seekg(bmpHeader.offset);

    auto rowWidthInBytes =
        ((bmpHeader.dibHeader.width * bmpHeader.dibHeader.bitsPerPixel + 31) /
         32) *
        4;

    auto buffer = std::make_unique<char[]>(rowWidthInBytes);
    auto maxY = std::min(static_cast<uint32_t>(lowlevel::displayHeight),
                         bmpHeader.dibHeader.height);
    auto maxX = std::min(static_cast<uint32_t>(lowlevel::displayWidth),
                         bmpHeader.dibHeader.width);

    uint32_t y = 0;
    for (uint32_t sizeCount = 0; y < maxY; sizeCount += rowWidthInBytes) {
        bmpFile.read(buffer.get(), rowWidthInBytes);

        uint32_t x = 0x00;
        for (auto pixelByteIndex = 0u; pixelByteIndex < rowWidthInBytes;
             pixelByteIndex++) {
            uint8_t* pixelPtr =
                reinterpret_cast<uint8_t*>(buffer.get() + pixelByteIndex);
            auto value = (*pixelPtr);

            uint8_t bitmask = 0x80;
            for (auto bitIndex = 0; bitIndex < 8 && x < maxX; bitIndex++) {
                if (value & bitmask) {
                    canvas.setPixel(x, y, false);
                } else {
                    canvas.setPixel(x, y, true);
                }
                x++;
                bitmask >>= 1;
            }
        }
        y++;
    }
}
