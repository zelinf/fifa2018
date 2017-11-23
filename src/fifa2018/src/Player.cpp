#include <fifa2018/Player.h>

namespace fifa2018 {

std::shared_ptr<Team> Player::getTeam() const {
    return team;
}
}