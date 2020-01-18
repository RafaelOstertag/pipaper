#ifndef __BMP_HH
#define __BMP_HH

#include "canvas.hh"
#include "lowlevel/display.hh"

#include <memory>
#include <string>
#include <vector>

class BitmapFile {
  public:
    BitmapFile(const std::string& filename);

    void paint(Canvas& canvas);
    void paint(Canvas& canvas, uint8_t upToY);
    uint8_t height() { return bitmap->size(); }
    uint8_t width() { return (*bitmap)[0].size(); }

  private:
    std::string filename;
    std::unique_ptr<std::vector<std::vector<bool>>> bitmap;
};

#endif
