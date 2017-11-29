#include <fifa2018/Stage.h>
#include <fifa2018/Match.h>

#include <utility>
#include <fmt/format.h>

namespace fifa2018 {

Stage::Stage(const std::string &label,
    std::vector<std::shared_ptr<Match>> &schedule)
    : schedule(schedule), label(label) {}

std::vector<std::shared_ptr<Match>> Stage::operator()() {
    printSchedule(fmt::format("schedule_{}.txt", label));
    runMatches("simulationLog.txt");
    printStatistics(fmt::format("result_{}.txt", label));
    printWinners(fmt::format("team_{}.txt", label));

}

}
