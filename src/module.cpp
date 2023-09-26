#include "module.h"

#include "utils/amath.cpp"

namespace upgmc{
    #include "algorithms/upgmc_clasterization.cpp"
}

#if IS_LIBRARY == OFF 
    #include "main.cpp"
#endif

