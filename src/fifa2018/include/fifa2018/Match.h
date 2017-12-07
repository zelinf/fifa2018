#pragma once

#include <memory>
#include <cstdint>
#include <utility>
#include <map>
#include <stdexcept>
#include <chrono>
#include <functional>

namespace fifa2018 {

class Team;

class Player;

class Match {
private:
    std::shared_ptr<Team> teamA;
    std::shared_ptr<Team> teamB;
    bool allowDraw;
    std::string address;
public:
    using time_type = std::chrono::system_clock::time_point;
private:
    time_type time;
public:
    /**
     * 创建一个比赛，但模拟不会立即执行
     * @param teamA 第一支队伍
     * @param teamB 第二支队伍
     * @param timeAddress 时间地点
     * @param allowDraw 是否允许平局
     */
    explicit Match(std::shared_ptr<Team> teamA = nullptr,
                   std::shared_ptr<Team> teamB = nullptr,
                   std::string address = "",
                   const time_type &time = std::chrono::system_clock::now(),
                   bool allowDraw = false);

    /**
     * 模拟这场比赛，执行完这个方法后才可以调用返回比赛结果的方法
     */
    void runMatch(std::function<void(std::shared_ptr<Player>, std::shared_ptr<Team>)> onNewGoal =
    std::function<void(std::shared_ptr<Player>, std::shared_ptr<Team>)>());

private:
    bool hasRun = false;

    void requireMatchRun() const {
        if (!hasRun)
            throw std::logic_error(
                    "Match must run before "
                            "the execution of this method.");
    }

    std::function<void(std::shared_ptr<Player>, std::shared_ptr<Team>)> onNewGoal;

public:

    std::shared_ptr<Team> getTeamA() const { return teamA; }

    std::shared_ptr<Team> getTeamB() const { return teamB; }

    void setTeamA(std::shared_ptr<Team> teamA) {
        hasRun = false;
        this->teamA = std::move(teamA);
    }

    void setTeamB(std::shared_ptr<Team> teamB) {
        hasRun = false;
        this->teamB = std::move(teamB);
    }

    void setTime(const time_type &time) { this->time = time; }

    void setAddress(std::string address) { this->address = std::move(address); }

    void setAllowDraw() { allowDraw = true; }

    /**
     * 返回第一支队伍的进球数
     * @throw std::logic_error 如果runMatch方法还未被调用
     */
    int32_t goalOfTeamA();

    /**
     * 返回第二支队伍的进球数
     * @throw std::logic_error 如果runMatch方法还未被调用
     */
    int32_t goalOfTeamB();

    /**
     * 返回各个球员的进球数。未进球的球员不会被包含在这里
     */
    const std::map<std::shared_ptr<Player>, int32_t> &goalOfPlayers() const;

    /**
     * 返回比赛进行的时间
     * @return 时间
     */
    time_type getTime() const { return time; }

    /**
     * 返回比赛进行的地点
     * @return 地点
     */
    const std::string &getAddress() const { return address; }

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
