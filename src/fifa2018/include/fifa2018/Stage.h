#pragma once

#include <vector>
#include <memory>
#include <ostream>

using std::shared_ptr;

namespace fifa2018 {

class Match;

class Stage {
public:
    explicit Stage(const std::string &label,
        std::vector<std::shared_ptr<Match>> &schedule);

    virtual ~Stage() = default;

    // 执行这个阶段的比赛。返回下一阶段的比赛计划
    std::vector<std::shared_ptr<Match>> operator()();

protected:
	// 下面这些虚函数，子类根据需要重写。

    // 阶段1：输出日程安排（schedule_x.txt)
    virtual void printSchedule(const std::string &fileName);

    virtual std::string showSingleMatchPlan();

    // 阶段2：各场比赛过程（在标准输出显示直播）
    virtual void runMatches(const std::string &fileName);

    // 阶段3：输出统计结果（到文件）
    virtual void printStatistics(const std::string &fileName);

    // 阶段4：输出晋级球队（到文件）
    virtual void printWinners(const std::string &fileName);

private:
    std::string label;
    std::vector<std::shared_ptr<Match>> schedule;
};

class GroupMatchStage : public Stage {

};

}