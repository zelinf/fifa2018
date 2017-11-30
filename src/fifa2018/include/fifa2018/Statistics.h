#pragma once

#include <cstdint>

namespace fifa2018 {

class Statistics {
public:
    Statistics()
            : win(0), draw(0), lose(0), goalsFor(0), goalsAgainst(0) {}

    // 增加1的进球数
    void addGoal() {
        ++goalsFor;
    }

    // 增加1的失球数
    void loseGoal() {
        ++goalsAgainst;
    }

    // 增加一次胜利
    void addWin() {
        ++win;
    }

    // 增加一次平局
    void addDraw() {
        ++draw;
    }

    // 增加一次失败
    void addLose() {
        ++lose;
    }

    // 返回胜利场数
    int32_t getWin() const { return win; }

    // 返回平局场数
    int32_t getDraw() const { return draw; }

    // 返回失败场数
    int32_t getLose() const { return lose; }

    // 返回进球数
    int32_t getGoalsFor() const { return goalsFor; }

    // 返回失球数
    int32_t getGoalsAgainst() const { return goalsAgainst; }

    // 返回净进球数
    int32_t getGoalsDiff() const { return goalsFor - goalsAgainst; }

    int32_t getPts() const { return win * 3 + draw; }

    // 合并两个统计数据
    Statistics &operator+=(const Statistics &rhs);

    friend Statistics operator+(const Statistics &lhs, const Statistics &rhs) {
        Statistics result = lhs;
        result += rhs;
        return result;
    }

    friend bool operator<(const Statistics &lhs, const Statistics &rhs) {
        return lhs.getPts() > rhs.getPts();
    }

private:
    int32_t win;
    int32_t draw;
    int32_t lose;
    int32_t goalsFor;
    int32_t goalsAgainst;
};

}
