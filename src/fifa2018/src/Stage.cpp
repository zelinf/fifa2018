#include <fifa2018/Stage.h>
#include <fifa2018/Team.h>
#include <fifa2018/Player.h>
#include <fifa2018/Match.h>
#include <fmt/format.h>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fmt/time.h>
#include <iostream>
#include <thread>

namespace fifa2018 {

Stage::Stage(const std::string &label,
             std::vector<std::shared_ptr<Match>> &schedule)
        : schedule(schedule), label(label) {}

void Stage::operator()() {
    printSchedule(fmt::format("schedule_{}.txt", label));
    runMatches("simulationLog.txt");
    printStatistics(fmt::format("result_{}.txt", label));
    printWinners(fmt::format("team_{}.txt", label));
}

void Stage::printSchedule(const std::string &fileName) {
    std::ofstream out(fileName, std::ofstream::out);
    out << "schedule for " << stageFullName() << '\n';

    afterTitle(out);
    std::sort(schedule.begin(), schedule.end(), [](std::shared_ptr<Match> lhs, std::shared_ptr<Match> rhs) {
        return lhs->getTime() < rhs->getTime();
    });

    bool isFirstMatch = true;
    std::time_t currentDate;
    for (std::shared_ptr<Match> match : schedule) {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(
                match->getTime());
        int yday_1;
        int yday_2;
        if (!isFirstMatch) {
            yday_1 = std::gmtime(&currentTime)->tm_yday;
            yday_2 = std::gmtime(&currentDate)->tm_yday;
        }
        if (isFirstMatch
            || yday_1 != yday_2 ) {
            isFirstMatch = false;
            currentDate = currentTime;
            out << fmt::format("{:%B %e}\n", *std::gmtime(&currentTime));
        }

        out << fmt::format("{} vs {}, {}, {:%H:%M}\n",
                           match->getTeamA()->getName(),
                           match->getTeamB()->getName(),
                           match->getAddress(),
                           *std::gmtime(&currentTime));
    }
}

std::string Stage::stageFullName() const {
    return fmt::format("round of {}", label);
}

void Stage::afterTitle(std::ofstream &out) {
    // No operation.
}

std::string roleToString(Player::Role role) {
    switch (role) {
        case Player::Role::Captain:
            return "Captain";
        case Player::Role::ViceCaptain:
            return "Vice-Captain";
        case Player::Role::Member:
            return "Member";
    }
}

class StreamCompose {
public:
    StreamCompose(std::ostream &os1, std::ostream &os2)
            : os1(os1), os2(os2) {}

    void print(const std::string &content) {
        os1 << content;
        os2 << content;
    }

private:
    std::ostream &os1;
    std::ostream &os2;
};

void printTeamInfo(StreamCompose &writer, std::shared_ptr<Team> team) {
    writer.print(team->getName() + "\n");

    for (auto player : team->getPlayers()) {
        std::string formatString = player->getRole() == Player::Role::Member ?
                                   "#{}, {}, {}" :
                                   "#{}, {} ({}), {}";
        int32_t playerNumber = player->getNumber();
        std::string playerName = player->getName();
        std::string position = positionToString(player->getPosition());
        if (player->getRole() == Player::Role::Member) {
            writer.print(fmt::format("#{}, {}, {}\n", playerNumber, playerName,
                                     position));
        } else {
            writer.print(fmt::format("#{}, {} ({}), {}\n", playerNumber,
                                     playerName, roleToString(player->getRole()), position));
        }
    }
}

void Stage::runMatches(const std::string &fileName) {
    std::ofstream fileStream(fileName, std::ofstream::app);
    StreamCompose writer(fileStream, std::cout);

    writer.print(fmt::format("Current stage: {}\n", stageFullName()));
    for (std::shared_ptr<Match> match : schedule) {
        std::shared_ptr<Team> teamA = match->getTeamA();
        std::shared_ptr<Team> teamB = match->getTeamB();
        writer.print(fmt::format("{} vs {}\n", teamA->getName(), teamB->getName()));

        printTeamInfo(writer, teamA);
        printTeamInfo(writer, teamB);

        writer.print("Playing...\n");

        match->runMatch([&writer, this, &match](std::shared_ptr<Player> goalMaker,
                                                std::shared_ptr<Team> theOtherTeam) {
            goalMaker->getTeam()->getName();
            addGoalForPlayer(goalMaker);
            writer.print(fmt::format("{} did a goal, and it was #{}, {} did the goal.\n",
                                     goalMaker->getTeam()->getName(),
                                     goalMaker->getNumber(),
                                     goalMaker->getName()));
            writer.print(fmt::format("Now it's {}:{}\n",
                                     match->goalOfTeamA(), match->goalOfTeamB()));

            result[goalMaker->getTeam()].addGoal();
            result[theOtherTeam].loseGoal();
            using namespace std::chrono_literals;
//            TODO change this value
//            std::this_thread::sleep_for(200ms);
        });
        writer.print(fmt::format("Result: {}:{}\n",
                                 match->goalOfTeamA(), match->goalOfTeamB()));
        if (match->goalOfTeamA() > match->goalOfTeamB()) {
            result[teamA].addWin();
            result[teamB].addLose();
            winners.push_back(teamA);
            losers.push_back(teamB);
        } else if (match->goalOfTeamA() < match->goalOfTeamB()) {
            result[teamA].addLose();
            result[teamB].addWin();
            winners.push_back(teamB);
            losers.push_back(teamA);
        } else {
            result[teamA].addDraw();
            result[teamB].addDraw();
        }
    }
}

void Stage::printStatistics(const std::string &fileName) {
    std::ofstream fileStream(fileName, std::ofstream::out);

    fileStream << showStatisticsTableHeading() << '\n';

    for (const auto &pair : result) {
        fileStream << showStatisticsOfTeam(pair.first, pair.second);
        fileStream << '\n';
    }
}

std::string Stage::showStatisticsTableHeading() {
    return fmt::format("{:^18} {:^3} {:^3} {:^3} {:^4} {:^4} {:^4} {:^5}",
                       "Team", "W", "D", "L", "GF", "GA", "GD", "Pts");
}

std::string Stage::showStatisticsOfTeam(std::shared_ptr<Team> team, const Statistics &statistics) {
    return fmt::format("{:^18} {:^3d} {:^3d} {:^3d} {:^4d} {:^4d} {:^4d} {:^5d}",
                       team->getName(),
                       statistics.getWin(),
                       statistics.getDraw(),
                       statistics.getLose(),
                       statistics.getGoalsFor(),
                       statistics.getGoalsAgainst(),
                       statistics.getGoalsDiff(),
                       statistics.getPts());
}

void Stage::printWinners(const std::string &fileName) {
    std::ofstream fileStream(fileName, std::ofstream::out);

    fileStream << "Qualified for next stage:\n";
    for (std::shared_ptr<Team> winner : winners) {
        fileStream << winner->getName() << '\n';
    }
}

void Stage::scheduleOfNextStage(std::vector<std::shared_ptr<Match>> &schedule) {
    if (winners.size() % 4 == 0 && winners.size() >= 4 && winners.size() == 2 * schedule.size()) {
        int j = 0;
        for (int i = 0; i < winners.size(); i += 4) {
            schedule[j]->setTeamA(winners[i]);
            schedule[j]->setTeamB(winners[i + 2]);
            schedule[j + 1]->setTeamA(winners[i + 1]);
            schedule[j + 1]->setTeamB(winners[i + 3]);
            j += 2;
        }
    } else {
        throw std::invalid_argument("Illegal schedule.");
    }
}

std::vector<std::shared_ptr<Team>> Stage::getWinners() {
    return winners;
}

std::vector<std::shared_ptr<Team>> Stage::getLosers() {
    return losers;
}

std::string GroupMatchStage::stageFullName() const {
    return "group";
}

void GroupMatchStage::afterTitle(std::ofstream &out) {
    if (scheduleRef().size() != 8 * 6) {
        throw std::invalid_argument("Group stage should have 48 matches.");
    }
    out << "Matches by squads\n";
    for (int i = 0; i < 8; ++i) { // 8 groups
        out << "Group " << static_cast<char>(i + 'A') << '\n';
        for (int j = 0; j < 6; ++j) { // each group has 6 matches
            int index = i * 6 + j;
            time_t time = std::chrono::system_clock::to_time_t(scheduleRef()[index]->getTime());
            out << fmt::format(" {} vs {}, {}, {:%B %e %H:%M}\n",
                               scheduleRef()[index]->getTeamA()->getName(),
                               scheduleRef()[index]->getTeamB()->getName(),
                               scheduleRef()[index]->getAddress(),
                               *std::gmtime(&time));
        }
    }

    out << "Matches by date\n";
}

void GroupMatchStage::printStatistics(const std::string &fileName) {
    std::ofstream out(fileName, std::ofstream::out);

    char groupLabel = 'A';
    for (const std::vector<std::shared_ptr<Team>> &group : groups) {
        out << fmt::format("Final result for group {}\n", groupLabel);
        out << showStatisticsTableHeading() << '\n';
        for (std::shared_ptr<Team> team : group) {
            out << showStatisticsOfTeam(team, getResult()[team]) << '\n';
        }
        groupLabel += 1;
    }

    // 更正winners. 小组赛的胜者是根据积分决定的，而不是默认的淘汰制
    winners.clear();
    losers.clear();
    for (const std::vector<std::shared_ptr<Team>> &group : groups) {
        std::vector<std::pair<std::shared_ptr<Team>, Statistics>> groupStat;
        for (auto team : group) {
            groupStat.push_back({team, getResult()[team]});
        }
        std::sort(groupStat.begin(), groupStat.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.second < rhs.second;
        });
        winners.push_back(groupStat[0].first);
        winners.push_back(groupStat[1].first);

        for (int32_t i = 2; i < groupStat.size(); ++i) {
            losers.push_back(groupStat[i].first);
        }
    }
}

void GroupMatchStage::scheduleOfNextStage(std::vector<std::shared_ptr<Match>> &schedule) {
    if (schedule.size() != 8) {
        throw std::invalid_argument("Next schedule should have 8 matches.");
    }
    schedule[0]->setTeamA(winners[0]);
    schedule[0]->setTeamB(winners[3]);
    schedule[1]->setTeamA(winners[2]);
    schedule[1]->setTeamB(winners[1]);

    schedule[2]->setTeamA(winners[4]);
    schedule[2]->setTeamB(winners[7]);
    schedule[3]->setTeamA(winners[6]);
    schedule[3]->setTeamB(winners[5]);

    schedule[4]->setTeamA(winners[8]);
    schedule[4]->setTeamB(winners[11]);
    schedule[5]->setTeamA(winners[10]);
    schedule[5]->setTeamB(winners[9]);

    schedule[6]->setTeamA(winners[12]);
    schedule[6]->setTeamB(winners[15]);
    schedule[7]->setTeamA(winners[14]);
    schedule[7]->setTeamB(winners[13]);
}

GroupMatchStage::GroupMatchStage(const std::string &label, std::vector<std::shared_ptr<Match>> &schedule,
                                 std::vector<std::vector<std::shared_ptr<Team>>> groups)
        : Stage(label, schedule), groups(std::move(groups)) {
    for (std::shared_ptr<Match> match : schedule) {
        match->setAllowDraw();
    }
}

void FinalStage::afterTitle(std::ofstream &out) {
    out << "The second one is the final.\n";
}

void FinalStage::printWinners(const std::string &fileName) {
    std::ofstream out(fileName, std::ofstream::out);
    out << "The winner is:\n";
    if (scheduleRef()[1]->goalOfTeamA() > scheduleRef()[1]->goalOfTeamB()) {
        out << scheduleRef()[1]->getTeamA()->getName();
    } else {
        out << scheduleRef()[1]->getTeamB()->getName();
    }
    out << '\n';
}

void FinalStage::printStatistics(const std::string &fileName) {
    std::ofstream out(fileName, std::ofstream::out);
    out << "1st vs 2nd\n";
    out << showStatisticsTableHeading() << '\n';
    {
        std::shared_ptr<Team> teamA = scheduleRef()[1]->getTeamA();
        out << showStatisticsOfTeam(teamA, getResult()[teamA]) << '\n';
        std::shared_ptr<Team> teamB = scheduleRef()[1]->getTeamB();
        out << showStatisticsOfTeam(teamB, getResult()[teamB]) << '\n';
    }
    out << "\n";

    out << "3rd vs 4th\n";
    out << showStatisticsTableHeading() << '\n';
    {
        std::shared_ptr<Team> teamA = scheduleRef()[0]->getTeamA();
        out << showStatisticsOfTeam(teamA, getResult()[teamA]) << '\n';
        std::shared_ptr<Team> teamB = scheduleRef()[0]->getTeamB();
        out << showStatisticsOfTeam(teamB, getResult()[teamB]) << '\n';
    }
    out << "\n";
}

void FinalStage::printAllStatistics(const std::map<std::shared_ptr<Team>, Statistics> &stat, std::ofstream &out) {
    std::vector<std::pair<std::shared_ptr<Team>, Statistics>> statList(stat.cbegin(), stat.cend());
    std::sort(statList.begin(), statList.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.second < rhs.second;
    });

    out << showStatisticsTableHeading() << '\n';
    for (int32_t i = 0; i < 10; ++i) {
        out << showStatisticsOfTeam(statList[i].first, statList[i].second) << '\n';
    }
}

std::vector<std::shared_ptr<Team>> FinalStage::getTop3() {
    if (scheduleRef()[1]->goalOfTeamA() > scheduleRef()[1]->goalOfTeamB()) {
        top3.push_back(scheduleRef()[1]->getTeamA());
        top3.push_back(scheduleRef()[1]->getTeamB());
    } else {
        top3.push_back(scheduleRef()[1]->getTeamB());
        top3.push_back(scheduleRef()[1]->getTeamA());
    }

    if (scheduleRef()[0]->goalOfTeamA() > scheduleRef()[0]->goalOfTeamB()) {
        top3.push_back(scheduleRef()[0]->getTeamA());
    } else {
        top3.push_back(scheduleRef()[0]->getTeamB());
    }
    return top3;
}

}
