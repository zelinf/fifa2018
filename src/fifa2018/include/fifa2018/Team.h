#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <functional>

namespace fifa2018 {

class Player;

class Team : public std::enable_shared_from_this<Team> {
public:
    Team(std::string name, std::string continent)
            : name(std::move(name)),
              continent(std::move(continent)),
              sorted(true) {}

    const std::string &getName() const { return name; }

    void setName(const std::string &newName) {
        name = newName;
    }

    const std::string &getContinent() const { return continent; }

    void setContinent(const std::string &newContinent) {
        continent = newContinent;
    }

    /**
     * Add a player to this team. Automatically set
     * the 'team' field of the player.
     * @param player The new player.
     */
    void addPlayer(std::shared_ptr<Player> player);

    const std::vector<std::shared_ptr<Player>> &getPlayers() const;

    void forEachPlayer(std::function<void(std::shared_ptr<const Player>)> f) const;

private:
    std::string name;
    std::string continent;

    mutable std::vector<std::shared_ptr<Player>> players;
    mutable bool sorted;

    void sortById() const;
};

}
