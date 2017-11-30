#pragma once

#include <utility>
#include <functional>
#include <vector>
#include <memory>
#include <ostream>
#include <fifa2018/Statistics.h>
#include <map>

using std::shared_ptr;

namespace fifa2018 {

class Match;

class Team;

class Player;

class Stage {
public:
    explicit Stage(const std::string &label,
                   std::vector<std::shared_ptr<Match>> &schedule);

    virtual ~Stage() = default;

    // 执行这个阶段的比赛。返回下一阶段的比赛计划
    void operator()();

    // 为下一阶段比赛配置日程。传入的schedule应当已经配置好日程，
    // 这个方法负责配置对战队伍
    virtual void scheduleOfNextStage(std::vector<std::shared_ptr<Match>> &schedule);

    // 返回当前阶段的统计数据，即每支队伍的胜利、得分情况
    std::map<std::shared_ptr<Team>, Statistics> getResult() const { return result; };

    std::vector<std::shared_ptr<Team>> getWinners();

    std::vector<std::shared_ptr<Team>> getLosers();

private:
    std::map<std::shared_ptr<Team>, Statistics> result;

protected:
    std::vector<std::shared_ptr<Team>> winners;

    std::vector<std::shared_ptr<Team>> losers;

protected:
    // 下面这些虚函数，子类根据需要重写。
    virtual std::string stageFullName() const;

    // 阶段1：输出日程安排（schedule_x.txt)
    virtual void printSchedule(const std::string &fileName);

    virtual void afterTitle(std::ofstream &out); // 输出完日程标题执行这个方法

    // 阶段2：各场比赛过程（在标准输出显示直播）
    virtual void runMatches(const std::string &fileName);

    // 阶段3：输出统计结果（到文件）
    virtual void printStatistics(const std::string &fileName);

    virtual std::string showStatisticsTableHeading();

    virtual std::string showStatisticsOfTeam(std::shared_ptr<Team>, const Statistics &statistics);

    // 阶段4：输出晋级球队（到文件）
    virtual void printWinners(const std::string &fileName);

private:
    std::string label;
    std::vector<std::shared_ptr<Match>> schedule;

protected:
    std::string &labelRef() { return label; }

    std::vector<std::shared_ptr<Match>> &scheduleRef() { return schedule; };

    std::function<void(std::shared_ptr<Player>)> addGoalForPlayer;

public:
    void setAddGoalForPlayer(std::function<void(std::shared_ptr<Player>)> newFunc) {
        addGoalForPlayer = std::move(newFunc);
    }
};

class GroupMatchStage : public Stage {
private:
    std::vector<std::vector<std::shared_ptr<Team>>> groups;
public:
    GroupMatchStage(const std::string &label,
                    std::vector<std::shared_ptr<Match>> &schedule,
                    std::vector<std::vector<std::shared_ptr<Team>>> groups);

protected:
    std::string stageFullName() const override;

    void afterTitle(std::ofstream &out) override;

    void printStatistics(const std::string &fileName) override;

    void scheduleOfNextStage(std::vector<std::shared_ptr<Match>> &schedule) override;
};

class FinalStage : public Stage {
public:
    FinalStage(const std::string &label, std::vector<std::shared_ptr<Match>> &schedule)
            : Stage(label, schedule) {}

protected:
    std::string stageFullName() const override {
        return "final";
    }

    void afterTitle(std::ofstream &out) override;

    void printWinners(const std::string &fileName) override;

    void printStatistics(const std::string &fileName) override;

public:
    void printAllStatistics(const std::map<std::shared_ptr<Team>, Statistics> &stat,
                            std::ofstream &out);

    std::vector<std::shared_ptr<Team>> getTop3();

private:
    std::vector<std::shared_ptr<Team>> top3;
};

}