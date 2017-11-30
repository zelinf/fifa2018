#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <deque>
#include <fifa2018/Match.h>
#include <json/json.hpp>
#include <fifa2018/Statistics.h>

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

private:
    std::istream &in;

private:
    nlohmann::json config;

    std::deque<std::shared_ptr<Match>> allMatches;

    // 各个队伍获得的积分
    std::map<std::shared_ptr<Team>, Statistics> teamStatistics;
    // 各个球员的进球数
    std::map<std::shared_ptr<Player>, int32_t> playerGoals;

    void readTeams(const nlohmann::json &config);

    void readTimeAddress(const nlohmann::json &config);

    void configGroupMatch(const nlohmann::json &config, std::vector<std::shared_ptr<Match>> &groupMatches);

    std::shared_ptr<Team> findTeam(const std::string &teamName) const;

    static const int MATCHES_IN_EACH_GROUP = 6;
    static const int GROUPS = 8;

    std::vector<std::vector<std::shared_ptr<Team>>> groups;

    void sumStatistics(const std::map<std::shared_ptr<Team>, fifa2018::Statistics> &increment);

    std::vector<std::vector<std::shared_ptr<Team>>> readGroups(const nlohmann::json &config);

    std::vector<std::shared_ptr<Match>> popMatches(int32_t count);
};

}
