#ifdef TEST_DEPLOY

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
    const char *address = ((argc >= 2) ? argv[1] : "127.0.0.1");
    const int portNumber = ((argc >= 3) ? atoi(argv[2]) : 19997);

    int clientID = simxStart(address, portNumber, true, true, 2000, 5);
    //  simxStartSimulation(clientID, simx_opmode_blocking);

    if (clientID == -1) {
        std::cout << PrintColors::BOLDRED << "[ ERROR ]" << PrintColors::RESET << " Failed to Connect" << std::endl;
        return -1;
    }

    r = new Robot(clientID, "NAO");

    std::vector<float> params = {200, 1.09377, 0.935061, -0.370039, 3.85175, 2.22327, 2.00699, 4.93061, 0, 0.833838, 0.936261, -0.953097, 4.95489, -0.151992, -0.276104, 0.767171, 2.15373, -0.470789, 0.14251, -1.12816, 3.13303, -1.05358, -0.034, -0.721709, 0.0137414, 0.332784, 1.25865, -2.50433, 0.98861, -1.00088, 2.61992, -0.657374, -0.980126, -1.13465, 1.83196};

    for(int i = 0; i < 5; i++) {
        result res = r->runExperiment(params, 15.0, "Test");
        std::cout << PrintColors::GREEN << "Score: " << std::setprecision(2) <<  res.score << PrintColors::RESET << std::endl;
    }

    delete r;
    return 0;
}

#endif
