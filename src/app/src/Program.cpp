#include <iostream>
#include <fstream>
#include <string>
#include <fifa2018/Tournament.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Exactly 1 argument is expected: path to config file\n";
        return 1;
    }

    std::string configFilePath(argv[1]);

    //std::string configFilePath("resources/config.json");

    std::ifstream config(configFilePath, std::ios_base::in);

    if (!config) {
        std::cout << "Configuration file doesn't exist, or can't be loaded.\n";
        return 1;
    }

    fifa2018::Tournament tournament(config);
    tournament.runTournament();

    return 0;
}
