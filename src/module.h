
#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <iostream>
#include <cmath>
#include <functional>
#include <utility>
#include <fstream>
#include "omp.h"

#if DEBUG == ON
    #include <ctime> 
#endif

#include "../include/json.hpp"
using json = nlohmann::json;

#include "options.h"

#include "utils/amath.h"
#include "utils/converter.h"

namespace upgmc{
    #include "algorithms/upgmc_clasterization.h"
}


#if IS_LIBRARY == OFF 
    #include "../include/argparse.hpp" // Use only outside library(e.g main.cpp) 
#endif
