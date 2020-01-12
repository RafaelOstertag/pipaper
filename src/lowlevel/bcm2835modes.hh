#ifndef __BCM2835MODES_HH
#define __BCM2835MODES_HH

#include <bcm2835.h>

namespace lowlevel {
enum Bcm2835Mode {
    INPUT = BCM2835_GPIO_FSEL_INPT,
    OUTPUT = BCM2835_GPIO_FSEL_OUTP
};
}

#endif
