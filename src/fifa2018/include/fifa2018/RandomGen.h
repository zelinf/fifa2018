#pragma once

#include <random>
#include <cstdint>

namespace fifa2018 {

class RandomGen {
public:
    static RandomGen &getDefaultRandomGen();

private:
    static RandomGen defaultGen;

    RandomGen() = default;

public:
    RandomGen(const RandomGen &) = delete;

    RandomGen(RandomGen &&) = delete;

    ~RandomGen() = default;
 
    /**
     * Get a random integer in [from, to)
     * @param from The lower bound (inclusive)
     * @param to The upper bound (exclusive)
     * @return The generated random number
     * @throws std::invalid_argument if 'to' is not greater than 'from'
     */
    int32_t next(int32_t from, int32_t to);

private:
    std::default_random_engine engine;
    std::uniform_int_distribution<int32_t> dist;
};

}
