#pragma once

#include <vector>
#include <memory>
#include <ostream>

using std::shared_ptr;

namespace fifa2018 {

class Match;

class Stage {
public:
    explicit Stage(std::vector<shared_ptr<Match>> schedule);

    virtual ~Stage() = default;

    void operator()();

protected:
    virtual void showCandidateTeams(std::ostream &out);

    virtual void showSchedule(std::ostream &out);
private:
    std::vector<shared_ptr<Match>> schedule;
};

}