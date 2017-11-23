#pragma once

#include <memory>
#include <vector>

namespace fifa2018 {

class Team;

class Player {
public:
    enum class Position {
        GK, // Goalkeeper
        DF, // Defender
        MF, // midfielder
        FW  // forward
    };

    enum class Role {
        Captain, ViceCaptain, Member
    };

private:
    std::string name;
    Position position;
    Role role;

    std::shared_ptr<Team> team;

public:
    std::shared_ptr<Team> getTeam() const;
};

}
