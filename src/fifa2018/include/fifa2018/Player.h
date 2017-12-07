#pragma once

#include <memory>
#include <utility>
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

    Player(std::string name,
           int number,
           Position position,
           Role role,
           std::shared_ptr<Team> team = nullptr);

private:
    std::string name;

    int number;
    Position position;
    Role role;

    std::weak_ptr<Team> team;
public:
    /**
     * Set the team of this player. The player will not
     * be added to that team.
     * @param newTeam The new team for this player.
     */
    void setTeam(const std::shared_ptr<Team> &newTeam) { team = newTeam; }

    std::shared_ptr<Team> getTeam() const { return team.lock(); }

    const std::string &getName() const { return name; }

    int getNumber() const { return number; }

    Position getPosition() const { return position; }

    Role getRole() const { return role; }
};

std::string positionToString(Player::Position pos);

}
