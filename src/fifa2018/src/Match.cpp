#include <fifa2018/Match.h>
#include <fifa2018/RandomGen.h>
#include <fifa2018/Player.h>
#include <fifa2018/Team.h>
#include <utility>

namespace fifa2018 {

void Match::randomPlayerGoal(std::shared_ptr<Team> team) {
    RandomGen &randomGen = RandomGen::getDefaultRandomGen();

    int32_t playerIndex = randomGen
            .next(0, static_cast<int32_t>(team->getPlayers().size()));
    ++playerGoals[team->getPlayers()[playerIndex]];
    onNewGoal(team->getPlayers()[playerIndex]);
}

void Match::randomGoal(bool must) {
    int32_t teamIndex = RandomGen::getDefaultRandomGen().next(0, must ? 2 : 3);
    switch (teamIndex) {
        case 0:
            ++teamGoals[teamA];
            randomPlayerGoal(teamA);
            break;
        case 1:
            ++teamGoals[teamB];
            randomPlayerGoal(teamB);
            break;
        default:
            break;
    }
}

void Match::runMatch(std::function<void(std::shared_ptr<Player>)> onNewGoal) {
    hasRun = true;
    this->onNewGoal = std::move(onNewGoal);

    RandomGen &randomGen = RandomGen::getDefaultRandomGen();
    for (int32_t i = 0; i != 4; ++i) {
        randomGoal(false);
    }
    // 如果不允许平局，应该继续比赛，直到某方再得一分
    if (!allowDraw && teamGoals[teamA] == teamGoals[teamB]) {
        randomGoal(true);
    }
}

int32_t Match::goalOfTeamA() const {
    requireMatchRun();
    return teamGoals.find(teamA)->second;
}

int32_t Match::goalOfTeamB() const {
    requireMatchRun();
    return teamGoals.find(teamB)->second;
}

const std::map<std::shared_ptr<Player>, int32_t> &Match::goalOfPlayers() const {
    requireMatchRun();
    return playerGoals;
}

Match::Match(std::shared_ptr<Team> teamA,
             std::shared_ptr<Team> teamB,
             std::string address,
             const time_type &time,
             bool allowDraw)
        : teamA(std::move(teamA)),
          teamB(std::move(teamB)),
          address(std::move(address)),
          time(time),
          allowDraw(allowDraw) {
    teamGoals.insert({teamA, 0});
    teamGoals.insert({teamB, 0});
}

}
