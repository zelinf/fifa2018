#pragma once

#include <memory>
#include <cstdint>
#include <utility>
#include <map>

namespace fifa2018 {

class Team;

class Player;

class Match {
private:
    std::shared_ptr<Team> teamA;
    std::shared_ptr<Team> teamB;
    bool allowDraw;
public:
    Match(std::shared_ptr<Team> teamA, std::shared_ptr<Team> teamB, bool allowDraw);

    void runMatch();

    int32_t goalOfTeamA() const;

    int32_t goalOfTeamB() const;

    bool isDraw() const { return goalOfTeamA() == goalOfTeamB(); }

    const std::map<std::shared_ptr<Player>, int32_t> &goalOfPlayers() const;

private:
    std::map<std::shared_ptr<Player>, int32_t> playerGoals;
    std::map<std::shared_ptr<Team>, int32_t> teamGoals;

    // 给定进球队伍，随机选择一个队员作为进球者
    void randomPlayerGoal(std::shared_ptr<Team> team);

    // 随机模拟比赛的一段时间，可能有1方进球，也可能没有进球。
    // 如果must为true，必然会有一方进球，否则有可能无进球。
    // 如果有一支队伍进球，这个方法会正确设置进球的球员
    void randomGoal(bool must);
};

}
