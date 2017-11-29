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

    // ִ������׶εı�����������һ�׶εı����ƻ�
    std::vector<std::shared_ptr<Match>> operator()();

protected:
	// ������Щ�麯�������������Ҫ��д��

    // �׶�1������ճ̰��ţ�schedule_x.txt)
    virtual void printSchedule(const std::string &fileName);

    virtual std::string showSingleMatchPlan();

    // �׶�2�������������̣��ڱ�׼�����ʾֱ����
    virtual void runMatches(const std::string &fileName);

    // �׶�3�����ͳ�ƽ�������ļ���
    virtual void printStatistics(const std::string &fileName);

    // �׶�4�����������ӣ����ļ���
    virtual void printWinners(const std::string &fileName);

private:
    std::string label;
    std::vector<std::shared_ptr<Match>> schedule;
};

class GroupMatchStage : public Stage {

};

}