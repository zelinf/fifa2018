#include <fifa2018/Stage.h>
#include <fifa2018/Match.h>

#include <utility>

namespace fifa2018 {

Stage::Stage(std::vector<shared_ptr<Match>> schedule)
        : schedule(std::move(schedule)) {

}

void Stage::operator()() {

}

}
