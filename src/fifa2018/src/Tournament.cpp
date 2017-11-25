#include <fifa2018/Tournament.h>

namespace fifa2018 {

void Tournament::readConfig() {
    // TODO read initial data from input stream.

    // 这个方法执行完之后，teamScores应当包含所有参赛队伍

    // remainingMatches 应当依次包含要执行的比赛，并设置好各场比赛的时间地点。
    // remainingMatches 前面与小组赛有关的比赛的参赛队伍也要设置好

    // playerGoals 和 topThree 保持为空
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

}
