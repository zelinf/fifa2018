#pragma once

#include <string>

namespace fifa2018 {

class Team {
public:
    Team(const std::string &name, const std::string &continent)
            : name(name), continent(continent) {}

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
};

}
