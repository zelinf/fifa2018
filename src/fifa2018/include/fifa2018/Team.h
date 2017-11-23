#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace fifa2018 {

class Player;

class Team {
public:
    Team(std::string name, std::string continent)
            : name(std::move(name)), continent(std::move(continent)) {}

    const std::string &getName() const { return name; }

    void setName(const std::string &newName) {
        name = newName;
    }

    const std::string &getContinent() const { return continent; }

    void setContinent(const std::string &newContinent) {
        continent = newContinent;
    }

private:
    std::string name;
    std::string continent;

    std::vector<std::shared_ptr<Player>> players;
};

}
