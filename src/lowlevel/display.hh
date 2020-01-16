#ifndef __DISPLAY_HH
#define __DISPLAY_HH

#include <array>
#include <cstdint>
#include <iterator>
#include <list>

namespace lowlevel {
constexpr uint8_t displayWidth = 122;
constexpr uint8_t logicalWidth =
    (displayWidth % 8 == 0) ? (displayWidth / 8) : (displayWidth / 8 + 1);
constexpr uint8_t displayHeight = 250;

class DisplayMatrix {
  public:
    class Iterator
        : public std::iterator<std::input_iterator_tag, uint8_t, uint32_t> {
      public:
        Iterator& operator++() {
            index++;
            auto row = index / logicalWidth;
            auto column = index % logicalWidth;
            value = displayMatrix.matrix[row][column];

            return *this;
        }

        Iterator& operator++(int) {
            auto row = index / logicalWidth;
            auto column = index % logicalWidth;
            value = displayMatrix.matrix[row][column];

            index++;

            return *this;
        }

        bool operator==(const Iterator& o) { return index == o.index; }

        bool operator!=(const Iterator& o) { return !(*this == o); }

        value_type operator*() const { return value; }

      private:
        Iterator(const DisplayMatrix& displayMatrix)
            : Iterator{displayMatrix, 0, displayMatrix.matrix[0][0]} {}

        Iterator(const DisplayMatrix& displayMatrix, uint32_t index,
                 value_type value)
            : displayMatrix{displayMatrix}, index{index}, value{value} {}
        const DisplayMatrix& displayMatrix;
        uint32_t index;
        value_type value;

        friend DisplayMatrix;
    };
    DisplayMatrix();
    DisplayMatrix(DisplayMatrix&& o);
    DisplayMatrix& operator=(DisplayMatrix&& o);

    DisplayMatrix::Iterator begin() const;
    DisplayMatrix::Iterator end() const;

    void set(uint8_t x, uint8_t y);
    void clear(uint8_t x, uint8_t y);
    void setAll();
    void clearAll();

#ifndef NDEBUG
    void _dumpMatrix() const;
#endif

  private:
    std::array<std::array<uint8_t, logicalWidth>, displayHeight> matrix;

    uint8_t getByte(uint8_t x, uint8_t y) const;
    void setByte(uint8_t x, uint8_t y, uint8_t value);

    friend Iterator;
};

} // namespace lowlevel

#endif
