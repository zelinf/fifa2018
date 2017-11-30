#include <fifa2018/Tournament.h>
#include <json/json.hpp>
#include <fifa2018/Team.h>
#include <fifa2018/Player.h>
#include <ctime>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <fmt/format.h>
#include <fmt/time.h>
#include <chrono>
#include <vector>
#include <fifa2018/Stage.h>

namespace fifa2018 {

void Tournament::readConfig() {
    // 这个方法执行完之后，teamScores应当包含所有参赛队伍

    // remainingMatches 应当依次包含要执行的比赛，并设置好各场比赛的时间地点。
    // remainingMatches 前面与小组赛有关的比赛的参赛队伍也要设置好

    // playerGoals 和 topThree 保持为空

    try {
        in >> config;

        readTeams(config);
        readTimeAddress(config); // Sets up match queue with date and time;
    } catch (std::exception &) {
        throw InvalidConfigException();
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

std::vector<std::vector<std::shared_ptr<Team>>> Tournament::readGroups(const nlohmann::json &config) {
    // TODO

}

void Tournament::runTournament() {
    readConfig();

    std::vector<std::shared_ptr<Match>> scheduleOfGroupMatch = popMatches(MATCHES_IN_EACH_GROUP * GROUPS);
    configGroupMatch(config, scheduleOfGroupMatch);
    std::shared_ptr<Stage> groupStage = std::make_shared<GroupMatchStage>(
            "group",
            scheduleOfGroupMatch,
            groups);
    (*groupStage)();
    sumStatistics(groupStage->getResult());

    auto scheduleOf16 = popMatches(8);
    groupStage->scheduleOfNextStage(scheduleOf16);
    std::shared_ptr<Stage> stage16 = std::make_shared<Stage>("8", scheduleOf16);
    (*stage16)();
    sumStatistics(stage16->getResult());

    auto scheduleOf8 = popMatches(4);
    stage16->scheduleOfNextStage(scheduleOf8);
    std::shared_ptr<Stage> stage8 = std::make_shared<Stage>("4", scheduleOf8);
    (*stage8)();
    sumStatistics(stage8->getResult());

    auto scheduleOf4 = popMatches(2);
    stage8->scheduleOfNextStage(scheduleOf4);
    std::shared_ptr<Stage> stage4 = std::make_shared<Stage>("2", scheduleOf4);
    (*stage4)();
    sumStatistics(stage4->getResult());

    auto scheduleOfFinals = popMatches(2);
    scheduleOfFinals[0]->setTeamA(stage4->getLosers()[0]);
    scheduleOfFinals[0]->setTeamB(stage4->getLosers()[1]);
    scheduleOfFinals[1]->setTeamA(stage4->getWinners()[0]);
    scheduleOfFinals[1]->setTeamB(stage4->getWinners()[1]);
    std::shared_ptr<FinalStage> stageFinal = std::make_shared<FinalStage>("1", scheduleOfFinals);
    (*stageFinal)();
    sumStatistics(stageFinal->getResult());

    // 输出统计信息
    auto top3 = stageFinal->getTop3();
    std::ofstream out("finalStatistics.txt", std::ofstream::out);
    out << "1st: " << top3[0]->getName() << '\n';
    out << "2nd: " << top3[1]->getName() << '\n';
    out << "3rd: " << top3[2]->getName() << '\n';
    out << '\n';
    stageFinal->printAllStatistics(teamStatistics, out);

}

Player::Position strToPosition(const std::string &str) {
    Player::Position position;
    if (str == "FW") {
        position = Player::Position::FW;
    } else if (str == "MF") {
        position = Player::Position::MF;
    } else if (str == "DF") {
        position = Player::Position::DF;
    } else if (str == "GK") { // position_str == "GK"
        position = Player::Position::GK;
    } else {
        throw std::invalid_argument("Can not convert \""
                                    + str
                                    + "\" to Player::Position");
    }
    return position;
}

Player::Role strToRole(const std::string &str) {
    Player::Role result;
    if (str == "Captain") {
        result = Player::Role::Captain;
    } else if (str == "ViceCaptain") {
        result = Player::Role::ViceCaptain;
    } else if (str == "Member") {
        result = Player::Role::Member;
    } else {
        throw std::invalid_argument("\"" + str
                                    + "\" can not be converted to Player::Role");
    }
    return result;
}

void Tournament::readTeams(const nlohmann::json &config) {
    for (const nlohmann::json &team_cfg : config["teams"]) {
        auto team = std::make_shared<Team>(team_cfg["name"], team_cfg["continent"]);
        for (const nlohmann::json &player_cfg : team_cfg["players"]) {
            // NOTE this actually compiles.
            auto player = std::make_shared<Player>(
                    player_cfg["name"],
                    player_cfg["number"],
                    strToPosition(player_cfg["position"]),
                    strToRole(player_cfg["role"])
            );
            team->addPlayer(player);
        }
        teamStatistics[team] = Statistics();
    }
}

void Tournament::readTimeAddress(const nlohmann::json &config) {
    const int expectedNumOfMatch = 64;
    if (config["time_address"].size() != expectedNumOfMatch) {
        throw InvalidConfigException();
    }
    for (const nlohmann::json &time_addr_cfg : config["time_address"]) {
        std::shared_ptr<Match> match = std::make_shared<Match>();
        const std::string &time_str = time_addr_cfg["time"];
        int year, month, day, hour, minute;
        if (5 != sscanf(time_str.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute)) {
            throw InvalidConfigException();
        }
        struct tm time_struct{};
        time_struct.tm_year = year - 1900;
        time_struct.tm_mon = month - 1;
        time_struct.tm_mday = day;
        time_struct.tm_hour = hour;
        time_struct.tm_min = minute;
        match->setTime(std::chrono::system_clock::from_time_t(mktime(&time_struct)));

        match->setAddress(time_addr_cfg["address"]);

        allMatches.push_back(match);
    }
}

// 8 groups, each groups has 4 teams.
void Tournament::configGroupMatch(const nlohmann::json &config,
                                  std::vector<std::shared_ptr<Match>> &groupMatches) {
    if (config["groups"].size() != 8)
        throw InvalidConfigException();

    int32_t index = 0;
    for (const nlohmann::json &group_json : config["groups"]) {
        if (group_json.size() != 4)
            throw InvalidConfigException();

        std::shared_ptr<Team> team_1 = findTeam(group_json[0]);
        std::shared_ptr<Team> team_2 = findTeam(group_json[1]);
        std::shared_ptr<Team> team_3 = findTeam(group_json[2]);
        std::shared_ptr<Team> team_4 = findTeam(group_json[3]);
        std::vector<std::shared_ptr<Team>> group = {team_1, team_2, team_3, team_4};
        groups.emplace_back(group);

        groupMatches[index]->setTeamA(team_1);
        groupMatches[index]->setTeamB(team_2);

        groupMatches[index + 1]->setTeamA(team_3);
        groupMatches[index + 1]->setTeamB(team_4);

        groupMatches[index + 2]->setTeamA(team_1);
        groupMatches[index + 2]->setTeamB(team_3);

        groupMatches[index + 3]->setTeamA(team_4);
        groupMatches[index + 3]->setTeamB(team_2);

        groupMatches[index + 4]->setTeamA(team_4);
        groupMatches[index + 4]->setTeamB(team_1);

        groupMatches[index + 5]->setTeamA(team_2);
        groupMatches[index + 5]->setTeamB(team_3);

        index += 6;
    }

    if (groupMatches.size() != 48) {
        throw InvalidConfigException();
    }
}

std::shared_ptr<Team> Tournament::findTeam(const std::string &teamName) const {

    auto team_it = std::find_if(teamStatistics.cbegin(), teamStatistics.cend(),
                                [&teamName](const auto &pair) {
                                    return pair.first->getName() == teamName;
                                }
    );
    if (team_it == teamStatistics.cend()) {
        throw InvalidConfigException();
    }
    return team_it->first;
}

void Tournament::sumStatistics(const std::map<std::shared_ptr<Team>, fifa2018::Statistics> &increment) {
    for (const auto &pair : increment) {
        teamStatistics[pair.first] += pair.second;
    }
}

std::vector<std::shared_ptr<Match>> Tournament::popMatches(int32_t count) {
    std::vector<std::shared_ptr<Match>> result;
    for (int32_t i = 0; i < count; ++i) {
        result.push_back(allMatches.front());
        allMatches.pop_front();
    }
    return result;
}

}
