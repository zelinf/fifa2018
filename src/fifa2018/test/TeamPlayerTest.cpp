#include <catch/catch.hpp>
#include <fifa2018/Team.h>
#include <fifa2018/Player.h>
#include <memory>

using namespace fifa2018;

TEST_CASE("Team and Player") {
    auto team = std::make_shared<Team>("Russia", "Europe");
    team->addPlayer(std::make_shared<Player>("Makarov", 2, Player::Position::FW,
                                             Player::Role::Captain));
    std::shared_ptr<Player> makarov = team->getPlayers().back();
    REQUIRE("Makarov" == makarov->getName());
    REQUIRE(team == makarov->getTeam());
    REQUIRE(2 == makarov->getNumber());

    SECTION("getPlayers() is sorted by id") {
        team->addPlayer(std::make_shared<Player>("Price", 1, Player::Position::DF,
                                                 Player::Role::Member));
        REQUIRE("Price" == team->getPlayers().front()->getName());
    }
}