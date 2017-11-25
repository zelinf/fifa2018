#include <fifa2018/Player.h>
#include <utility>

namespace fifa2018 {

Player::Player(std::string name,
               int number,
               Player::Position position,
               Player::Role role,
               std::shared_ptr<Team> team)
        : name(std::move(name)),
          number(number),
          position(position),
          role(role),
          team(team) {}

}