#include <fifa2018/RandomGen.h>
#include <stdexcept>

namespace fifa2018 {

RandomGen RandomGen::defaultGen;

RandomGen &RandomGen::getDefaultRandomGen() {
    return defaultGen;
}

int32_t RandomGen::next(int32_t from, int32_t to) {
    if (from >= to)
        throw std::invalid_argument("'to' must be greater than 'from'");

    if (dist.min() != from || dist.max() != to - 1) {
        std::uniform_int_distribution<int32_t>::param_type new_param(from, to - 1);
        dist.param(new_param);
    }
    return dist(engine);
}

}