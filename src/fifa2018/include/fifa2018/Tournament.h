#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <deque>
#include <fifa2018/Match.h>
#include <json/json.hpp>

namespace fifa2018 {

class Team;

class Player;

class Tournament {
public:
    /**
     * 创建一个比赛模拟器
     * @param in 用于读取配置文件的输入流 （英文）
     */
    explicit Tournament(std::istream &in) :
            in(in) {}

    /**
     * 执行比赛。各阶段日志输出到指定文件（英文）
     */
    void runTournament();

    class InvalidConfigException : public std::exception {
    public:
        InvalidConfigException() = default;
    };

private:
    // 从输入流读取初始配置数据，包括队伍、队员信息以及比赛安排
    // 设置好小组赛的比赛队伍
    void readConfig();

    // 模拟小组赛，并设置好1/8决赛的比赛队伍
    void runGroupMatch();

    // 模拟 1/8, 1/4, 1/2, 决赛, 季军争夺赛
    void runRemainingMatches();

    // 输出统计信息
    void showStatistics() const;

private:
    std::istream &in;

private:
    // 剩余要进行的比赛 (back进，front出）
    std::deque<Match> remainingMatches;
    // 各个队伍获得的积分
    std::map<std::shared_ptr<Team>, int32_t> teamScores;
    // 各个球员的进球数
    std::map<std::shared_ptr<Player>, int32_t> playerGoals;
    // 冠军、亚军、季军
    std::vector<std::shared_ptr<Team>> topThree;

    void readTeams(const nlohmann::json &config);

    void readTimeAddress(const nlohmann::json &config);

    void configGroupMatch(const nlohmann::json &config);

    std::shared_ptr<Team> findTeam(const std::string &teamName) const;

    static const int GROUP_MATCHES = 6;
    static const int GROUPS = 8;

    void showGroupMatchByGroup(std::ostream &out) const;

    void showGroupMatchByDate(std::ostream &out) const;
};

}
