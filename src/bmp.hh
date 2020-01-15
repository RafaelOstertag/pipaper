#ifndef __BMP_HH
#define __BMP_HH

#include "canvas.hh"
#include "lowlevel/display.hh"

#include <string>

class BitmapFile {
  public:
    BitmapFile(const std::string& filename);

    void show(Canvas& canvas);

  private:
    std::string filename;
};

#endif
