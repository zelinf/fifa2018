#include <fifa2018/Tournament.h>
#include <json/json.hpp>
#include <fifa2018/Team.h>
#include <fifa2018/Player.h>
#include <ctime>

namespace fifa2018 {

void Tournament::readConfig() {
    // TODO read initial data from input stream.

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

void Tournament::runGroupMatch() {
    // TODO runGroupMatch
    // 执行小组赛
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

void Tournament::configGroupMatch(const nlohmann::json &config) {
    // TODO read configuration of groups and set up matches.
}

}
