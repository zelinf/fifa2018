#include <fifa2018/Team.h>
#include <fifa2018/Player.h>
#include <algorithm>

namespace fifa2018 {

void Team::addPlayer(std::shared_ptr<Player> player) {
    players.push_back(player);
    player->setTeam(shared_from_this());
    sorted = false;
}

const std::vector<std::shared_ptr<Player>> &Team::getPlayers() const {
    sortById();
    return players;
}

void Team::forEachPlayer(
        std::function<void(std::shared_ptr<const Player>)> f) const {
    sortById();
    for (auto p : players) {
        f(p);
    }
    sorted = false;
}

void Team::sortById() const {
    if (!sorted) {
        std::sort(players.begin(), players.end(),
                  [](std::shared_ptr<Player> lhs, std::shared_ptr<Player> rhs) {
                      return lhs->getNumber() < rhs->getNumber();
                  });
        sorted = true;
    }
}
}

