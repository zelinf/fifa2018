#include <fifa2018/Statistics.h>

namespace fifa2018 {

Statistics &Statistics::operator+=(const Statistics &rhs) {
    win += rhs.win;
    draw += rhs.draw;
    lose += rhs.lose;
    goalsFor += rhs.goalsFor;
    goalsAgainst += rhs.goalsAgainst;
    return *this;
}

}
