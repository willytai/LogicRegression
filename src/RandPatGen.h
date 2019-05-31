#ifndef __PATTERN_GEN_H__
#define __PATTERN_GEN_H__

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>

#define my_srandom srandom
#define my_random  random

typedef size_t Pat;
#define UnitPatSize 64

namespace Pattern
{

class Generator
{
public:
    Generator() { my_srandom(getpid()); }
    Generator(unsigned seed) { my_srandom(seed); }

    int operator () (const int range) const { // generate int from [0 range-1]
        return int(range * (double(my_random()) / INT_MAX));
    }

    void operator () (std::vector<Pat>& patterns) {
        for (int i = 0; i < (int)patterns.size(); ++i) {
            Pat p = 0;
            for (int j = 0; j < 64; ++j) {
                p = p << 1;
                p += (*this)(2);
            }
            patterns[i] = p;
        }
    }
};

}

#endif /* __PATTERN_GEN_H__ */
