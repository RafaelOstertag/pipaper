set(sources
    bmp.cxx
    canvas.cxx
    dirlist.cxx
    epaper.cxx
    lowlevel/display.cxx
    lowlevel/epd213v2.cxx
    lowlevel/spi.cxx
    os/signal.cxx
    sevensegment.cxx
)

add_executable(epaper ${sources})
target_link_libraries(epaper bcm2835)
