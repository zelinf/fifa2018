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

namespace fifa2018 {

void Tournament::readConfig() {
    // 这个方法执行完之后，teamScores应当包含所有参赛队伍

    // remainingMatches 应当依次包含要执行的比赛，并设置好各场比赛的时间地点。
    // remainingMatches 前面与小组赛有关的比赛的参赛队伍也要设置好

    // playerGoals 和 topThree 保持为空

    try {
        nlohmann::json config;
        in >> config;

        readTeams(config);
        readTimeAddress(config); // Sets up match queue with date and time;
        configGroupMatch(config);
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


void Tournament::runGroupMatch() {
    // TODO runGroupMatch
    // 执行小组赛
    // The first 48 matches are group match. Each 6 matches belongs to a group.
    {
        std::ofstream schedule("schedule16.txt", std::ofstream::out);
        showGroupMatchByGroup(schedule);
        showGroupMatchByDate(schedule);
    }

    {
        std::ofstream procedure("procedure16.txt", std::ofstream::out);
        procedure << "Group stage:\n";

    }
}

void Tournament::runRemainingMatches() {
    // TODO runRemainingMatches
    // 模拟小组赛之后的所有比赛，按要求输出
    // 设置好相关统计字段，即playerGoals和topThree
}

void Tournament::showStatistics() const {
    // TODO showStatistics
    // 显示冠军、亚军、季军，积分前10的队伍，射手榜
}

void Tournament::runTournament() {
    readConfig();
    runGroupMatch();
    runRemainingMatches();
    showStatistics();
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
        teamScores[team] = 0;
    }
}

void Tournament::readTimeAddress(const nlohmann::json &config) {
    const int expectedNumOfMatch = 2; // TODO set total number of matches.
    if (config["time_address"].size() != expectedNumOfMatch) {
        throw InvalidConfigException();
    }
    for (const nlohmann::json &time_addr_cfg : config["time_address"]) {
        Match match;
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
        match.setTime(std::chrono::system_clock::from_time_t(mktime(&time_struct)));

        match.setAddress(time_addr_cfg["address"]);

        remainingMatches.push_back(match);
    }
}

// 8 groups, each groups has 4 teams.
void Tournament::configGroupMatch(const nlohmann::json &config) {
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

        remainingMatches[index].setTeamA(team_1);
        remainingMatches[index].setTeamB(team_2);

        remainingMatches[index + 1].setTeamA(team_3);
        remainingMatches[index + 1].setTeamB(team_4);

        remainingMatches[index + 2].setTeamA(team_1);
        remainingMatches[index + 2].setTeamB(team_3);

        remainingMatches[index + 3].setTeamA(team_4);
        remainingMatches[index + 3].setTeamB(team_2);

        remainingMatches[index + 4].setTeamA(team_4);
        remainingMatches[index + 4].setTeamB(team_1);

        remainingMatches[index + 5].setTeamA(team_2);
        remainingMatches[index + 5].setTeamB(team_3);

        index += 6;
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

void Tournament::showGroupMatchByGroup(std::ostream &out) const {
    out << "Matches by groups\n";

    int matchIndex = 0;
    for (int currentGroup = 1; currentGroup <= GROUPS; ++currentGroup) {
        char groupLabel = static_cast<char>('A' + (currentGroup - 1));
        out << fmt::format("Group {}\n", groupLabel);

        for (int groupMatch = 0; groupMatch < GROUP_MATCHES; ++groupMatch) {
            const std::string &teamAName = remainingMatches[matchIndex].getTeamA()->getName();
            const std::string &teamBName = remainingMatches[matchIndex].getTeamB()->getName();
            const std::string &address = remainingMatches[matchIndex].getAddress();
            time_t time = std::chrono::system_clock::to_time_t(remainingMatches[matchIndex].getTime());
            out << fmt::format(" {} vs {}, {}, {:%B %e %H:%M}\n", teamAName, teamBName, address,
                               *std::gmtime(&time));
            ++matchIndex;
        }
        out << "\n";
    }
    out << "\n";
}

void Tournament::showGroupMatchByDate(std::ostream &out) const {
    out << "Matches by date\n";
    std::vector<const Match *> matches;
    for (int i = 0; i < GROUPS * GROUP_MATCHES; ++i) {
        const Match *match = &remainingMatches[i];
        matches.push_back(match);
    }
    std::sort(matches.begin(), matches.end(),
              [](const Match *lhs, const Match *rhs) {
                  return lhs->getTime() < rhs->getTime();
              });
    bool isFirstMatch = true;
    std::time_t currentDate;
    for (const Match *match : matches) {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(match->getTime());
        if (isFirstMatch
            || std::gmtime(&currentTime)->tm_yday != std::gmtime(&currentDate)->tm_yday) {
            isFirstMatch = false;
            currentDate = currentTime;
            out << fmt::format("{:%B %e}\n", currentDate);
        }

        out << fmt::format("{} vs {}, {}, {:%H:%M}\n",
                           match->getTeamA()->getName(),
                           match->getTeamB()->getName(),
                           match->getAddress(),
                           currentTime);
    }
}

}
