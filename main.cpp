#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>

#include <printcolors.hpp>

extern "C" {
#include "extApi.h"
}

#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

#include "robot.hpp"

Robot* r = 0;

int main(int argc, const char** argv) {
    unsigned seed = 100;

    const char *address = ((argc >= 2) ? argv[1] : "127.0.0.1");
    const int portNumber = ((argc >= 3) ? atoi(argv[2]) : 20000);

    int clientID = simxStart(address, portNumber, true, true, 2000, 5);
    //  simxStartSimulation(clientID, simx_opmode_blocking);

    if (clientID == -1) {
        std::cout << PrintColors::BOLDRED << "[ ERROR ]" << PrintColors::RESET << " Failed to Connect" << std::endl;
        return -1;
    }

    std::vector<float> genome = {2.5e+02, -0.2, -0.28, -0.61, -0.15, -2, 0.9, 0.97, -0.84, -0.071, -0.62, -0.26, -0.24, -0.53, 0.15, 0.38, -1.2};
    r = new Robot(clientID, "NAO");
    std::string label = "Test Genome";
    result res = r->runExperiment(genome, 150.0, label);
    std::cout << PrintColors::BOLDGREEN << "\nAvg Score: " << std::setprecision(2) << res.score << PrintColors::RESET << std::endl;

    delete r;
    return 0;
}
