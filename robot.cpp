#include "robot.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cmath>
#include <string>

#include <printcolors.hpp>

#include "helpers.hpp"
#include "NAO.hpp"


Robot::Robot(int clientID, const char* name) :
    VRepClass(clientID, name) {
    int handles_count = 0;
    int names_count = 0;
    simxInt *shapeHandles_tmp;
    simxChar *shapeNames_tmp;
    //  simxInt *shapeHandles;
    //  simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking);
    // simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking)

    simxGetObjectGroupData(clientID, sim_object_shape_type, 0, &handles_count, &shapeHandles_tmp, NULL, NULL, NULL, NULL, &names_count, &shapeNames_tmp, simx_opmode_blocking);

    simxInt *shapeHandles = new simxInt[handles_count];
    for(int i = 0; i < handles_count; i++) {
        shapeHandles[i] = shapeHandles_tmp[i];
    }

    QList<QString> shapeNames;
    if(names_count > 0) {
        QString name = QString(shapeNames_tmp);
        shapeNames.push_back(name);

        for(int i = 0; names_count > 1 ; i++) {
            if(shapeNames_tmp[i] == '\0') {
                names_count--;

                name = QString(&(shapeNames_tmp[i+1]));
                shapeNames.push_back(name);
            }
        }
    }

    for (int i = 0; i < handles_count; i++) {
        _shapes.push_back( Shape( clientID, shapeNames.at(i).toStdString().data(), shapeHandles[i]) );
        _shapeNameHandler.insert(shapeNames.at(i), QPair<int, int>(shapeHandles[i], i));
        std::cerr << PrintColors::BOLDGREEN << "[ OK ] " << PrintColors::RESET << "Loaded shape " << shapeNames.at(i).toStdString() << "..\n";
    }

    _nao = new NAO(clientID);
    _nao->getNAOJoints(_joints);
    _numJoints = _joints.size();

    simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_streaming);
    while(simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_buffer) != simx_return_ok);

    simxGetObjectOrientation(_clientID, _handle, -1, _initialOrientation, simx_opmode_blocking);

    simxSynchronous(clientID, true);

    getLHipPos(_lHipPos, simx_opmode_streaming);
    getRLShoulderPos(_RLShoulderPos, simx_opmode_streaming);

    while(getLHipPos(_lHipPos, simx_opmode_buffer) != simx_return_ok);
    while(getRLShoulderPos(_RLShoulderPos, simx_opmode_buffer) != simx_return_ok);

    std::cout << "BodyLHip: " << _lHipPos[0] << " " << _lHipPos[1] << " " << _lHipPos[2] << "\n";
    std::cout << "RLShoulder: " << _RLShoulderPos[0] << " " << _RLShoulderPos[1] << " " << _RLShoulderPos[2] << "\n";
}

Robot::~Robot() {
    getLHipPos(_lHipPos, simx_opmode_discontinue);
    getRLShoulderPos(_RLShoulderPos, simx_opmode_discontinue);
    simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_discontinue);
}

simxInt Robot::getLHipPos(float *position, simxInt opMode) {
    return simxGetObjectPosition(_clientID,
                                 _shapeNameHandler.value("l_hip_roll_link_respondable3").first,
                                 -1,
                                 position,
                                 opMode);
}
simxInt Robot::getRLShoulderPos(float *position, simxInt opMode) {
    return simxGetObjectPosition(_clientID,
                                 _shapeNameHandler.value("l_shoulder_pitch_respondable3").first,
                                 _shapeNameHandler.value("r_shoulder_pitch_respondable3").first,
                                 position,
                                 opMode);
}


void Robot::update() {
    simxPauseCommunication(_clientID, 1);
    for (auto &joint : _joints) {
        joint->update();
    }

//    float leftAnkleAngle = -1*(_nao->_legHip->_leftJoint->_currentAngle +
//                               _nao->_knee->_leftJoint->_currentAngle);
//    float rightAnkleAngle = -1*(_nao->_legHip->_rightJoint->_currentAngle +
//                                _nao->_knee->_rightJoint->_currentAngle);
//    float leftAnkleAngle = -1*(_nao->_legHip->_leftJoint->jointRealPosition() +
//                               _nao->_knee->_leftJoint->jointRealPosition());
//    float rightAnkleAngle = -1*(_nao->_legHip->_rightJoint->jointRealPosition() +
//                                _nao->_knee->_rightJoint->jointRealPosition());

//    _nao->_ankle->_leftJoint->setJointTargetPosition(leftAnkleAngle);
//    _nao->_ankle->_rightJoint->setJointTargetPosition(rightAnkleAngle);

//    float leftAnkleRollAngle = -1*_nao->_legHipRoll->_leftJoint->_currentAngle;
//    float rightAnkleRollAngle = -1*_nao->_legHipRoll->_rightJoint->_currentAngle;
//    float leftAnkleRollAngle = -1*_nao->_legHipRoll->_leftJoint->jointRealPosition();
//    float rightAnkleRollAngle = -1*_nao->_legHipRoll->_rightJoint->jointRealPosition();

//    _nao->_ankleRoll->_leftJoint->setJointTargetPosition(leftAnkleRollAngle);
//    _nao->_ankleRoll->_rightJoint->setJointTargetPosition(rightAnkleRollAngle);

    simxPauseCommunication(_clientID, 0);

    simxSynchronousTrigger(_clientID);
}

void Robot::reset() {
    simxPauseCommunication(_clientID, 1);
    for ( auto &j : _joints ) {
        j->reset();
    }
    _nao->_ankle->reset();
    _nao->_ankleRoll->reset();

    simxPauseCommunication(_clientID, 0);

    for (int i = 0; i < 30; i++) {
        simxSynchronousTrigger(_clientID);
    }

    simxPauseCommunication(_clientID, 1);
    int posRet = simxSetObjectPosition(_clientID, _handle, -1,
                                       _initialPosition, simx_opmode_oneshot);
    int oriRet = simxSetObjectOrientation(_clientID, _handle, -1,
                                          _initialOrientation, simx_opmode_oneshot );

    if(posRet != simx_return_ok && posRet != simx_return_novalue_flag) {
        std::cerr << PrintColors::BOLDRED << "[ ERROR ] " << PrintColors::RESET << "Setting position for robot. ";
        log_response(posRet);
    }

    if(oriRet != simx_return_ok && oriRet != simx_return_novalue_flag) {
        std::cerr << PrintColors::BOLDRED << "[ ERROR ] " << PrintColors::RESET << "Setting orientation for robot. ";
        log_response(oriRet);
    }

    for ( auto &s : _shapes ) {
        s.reset();
    }

    simxPauseCommunication(_clientID, 0);

    for (int i = 0; i < 30; i++) {
        simxSynchronousTrigger(_clientID);
    }

}

result Robot::runExperiment( const std::vector<float> &genome, const float time_s, const std::string label) {
    std::ofstream routefile;
    routefile.open("routes/" + label + ".csv");

    if(routefile.fail()) {
        std::cout << "[ ERROR ] Fail to open log file ga.csv (Errno " << std::ios_base::failbit << ") Error: " << strerror(errno) << "\n";
    }

    std::cout << PrintColors::BOLDWHITE << "\n[ New Experiment ] " << label << PrintColors::RESET << "\n";

    this->setGenome(genome);
    this->reset();

    simxFloat position[3];
    simxFloat oldPosition[3];

    QList<simxFloat> maxDispBodyLHip;
    QList<simxFloat> maxDispRLShoulder;
    simxFloat tmpMaxDispBodyLHip[3];
    simxFloat tmpMaxDispRLShoulder[3];

    for(int i = 0; i < 3; i++) {
       position[i] = _initialPosition[i];
       oldPosition[i] = position[i];
    }

    maxDispBodyLHip.push_back(0);
    maxDispRLShoulder.push_back(0);

    simxFloat maxZ =_initialPosition[2];
    std::cout << "maxZ: " << maxZ << "\n";

    float dx, dy, dist = 0, intgr = 0;

    bool hasRobotFallen = false;
    float num_ticks = time_s*1000/step_ms;
    int ncycles;
    for(ncycles = 0; ncycles < num_ticks && !hasRobotFallen; ncycles++) {
        this->update();
        simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_buffer);

        dx = position[0] - oldPosition[0];
        dy = position[1] - oldPosition[1];
        float dist_tmp = sqrt(pow(dx, 2) + pow(dy, 2));

        if(dist_tmp > 0.1) {
            oldPosition[0] = position[0];
            oldPosition[1] = position[1];
            oldPosition[2] = position[2];

            dist += dist_tmp;
        }

        intgr += std::abs(position[1] - _initialPosition[1])*dx;

        routefile << std::setprecision(3) << position[0] << ", " << position[1] << ", " << position[2] << std::endl;

        if (ncycles%300==0){
            std::cout << "NAO displacement:\n";
            std::cout << "\t x: " << position[0] - _initialPosition[0] << std::endl;
            std::cout << "\t y: " << position[1] - _initialPosition[0] << std::endl;
        }

        if (position[2] > maxZ) {
            maxZ = position[2];
        } else if (position[2] < maxZ - 0.05f) {
            std::cout << PrintColors::BOLDYELLOW << "Robot Fell" << PrintColors::RESET << std::endl;
            hasRobotFallen = true;
        }

        getLHipPos(tmpMaxDispBodyLHip, simx_opmode_buffer);
        getRLShoulderPos(tmpMaxDispRLShoulder, simx_opmode_buffer);

        float tmpDispBodyLHip = Utils::distanceXY(tmpMaxDispBodyLHip, _initialPosition);
        float tmpDispRLShoulder = Utils::vsizeXZ(tmpMaxDispRLShoulder);

//        if(tmpDispBodyLHip > maxDispBodyLHip.last()) {
            maxDispBodyLHip.push_back(tmpDispBodyLHip);
            if(maxDispBodyLHip.size() > 30) {
                maxDispBodyLHip.removeFirst();
            }
//        }

//        if(tmpDispRLShoulder > maxDispRLShoulder.last()) {
            maxDispRLShoulder.push_back(tmpDispRLShoulder);
            if(maxDispRLShoulder.size() > 30) {
                maxDispRLShoulder.removeFirst();
            }
//        }
    }

    routefile.close();

    for (int j = 0; j < 5; j++) {
        simxSynchronousTrigger(_clientID);
    }

    if(dist < 0.5f) {
        dist = 0;
    }

    float dispBodyLHip = maxDispBodyLHip.first();
    float dispRLShoulder = maxDispRLShoulder.first();

    if(dispBodyLHip == 0) {
        if(maxDispBodyLHip.size() >= 30) {
            dispBodyLHip = maxDispBodyLHip.at(1);
        } else if(ncycles < 200) {
            dispBodyLHip = 100.0f; // infinity;
        }
    }

    if(dispRLShoulder == 0) {
        if(maxDispRLShoulder.size() >= 30) {
            dispRLShoulder = maxDispRLShoulder.at(1);
        } else if(ncycles < 200) {
            dispRLShoulder = 100.0f; // infinity;
        }
    }

    std::cout << "fdds : " << maxDispRLShoulder.size() << ", " << maxDispBodyLHip.size()  << "\n";
    std::cout << "fdds : " << maxDispRLShoulder.first() << ", " << maxDispBodyLHip.first()  << "\n";
    std::cout << "fdds : " << maxDispRLShoulder.last() << ", " << maxDispBodyLHip.last()  << "\n";

    dx = position[0] - _initialPosition[0];
    dy = position[1] - _initialPosition[1];
    float time = ncycles*step_ms/1000.0f;
    float avgvel = dist/time;

    float fdx = fmax(dx, 0.0f);
    float score = 50*fdx + (1-std::exp(-fdx/3))*(15*time + 50*std::exp(-dispBodyLHip/0.05) + 50*std::exp(-dispRLShoulder/0.05));
    struct result r = {score, dx, dy, time};

    std::cout << "\n[ End Experiment ] " << label << "\n";
    std::cout << "Results:"
              << "\n\tdist: " << dist
              << "\n\tcycles: " << ncycles
              << "\n\tdx:" << dx
              << "\n\tdy:" << dy
              << "\n\ttime:" << time
              << "\n\tavg velocity: " << avgvel
              << "\n\tIntegrative/dx: " << intgr/dx
              << "\n\tDispBodyLHip: " << std::setprecision(10) << dispBodyLHip
              << "\n\tDispRLShoulder: " << dispRLShoulder
              << "\n";

    return r;
}

std::vector< std::pair<float, float> > Robot::getAleles() {
    std::vector< std::pair<float, float> > alleles;

//    alleles.push_back( std::make_pair( 200.0, 600.0) ); // T(ms)
    alleles.push_back( std::make_pair( 200.0, 3000.0) ); // T(ms)
    if (_nao->_legHip->_enabled) {
        alleles.push_back( std::make_pair( -2.02, 2.02 ) ); //A: Pos Amplitude
        alleles.push_back( std::make_pair( -2.02, 2.02 ) ); //B: Neg Amplitude
        alleles.push_back( std::make_pair( -1.53, 0.48 ) ); //Oc : Neutral Angle
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase
    }

    if (_nao->_knee->_enabled) {
        alleles.push_back( std::make_pair( -3.1, 3.1) ); //C: pos amp
        alleles.push_back( std::make_pair( -3.1, 3.1) ); //C: Neg amp
        alleles.push_back( std::make_pair( -0.09, 2.11) ); //Oj: neutral angle
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase
    }

    if (_nao->_shoulder->_enabled) {
        alleles.push_back( std::make_pair( -4, 4) ); //D+: pos amp
        alleles.push_back( std::make_pair( -4, 4) ); //D+: pos amp
        alleles.push_back( std::make_pair( -2.085, 2.085) ); //D+: pos amp
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase
    }

    if (_nao->_legHipRoll->_enabled) {
        alleles.push_back( std::make_pair( -1.2, 1.2) ); // E: Pos amp
        alleles.push_back( std::make_pair( -1.2, 1.2) ); // E: Neg amp
        alleles.push_back( std::make_pair( -0.37, 0.79) ); // E: Neutral ang
        alleles.push_back( std::make_pair( 0, 2*PI) ); // E: Neutral ang

        alleles.push_back( std::make_pair( -1.2, 1.2) ); // E: Pos amp
        alleles.push_back( std::make_pair( -1.2, 1.2) ); // E: Neg amp
        alleles.push_back( std::make_pair( -0.79, 0.37) ); // E: Neutral ang
        alleles.push_back( std::make_pair( 0, 2*PI) ); // E: Neutral ang
    }

    if (_nao->_elbow->_enabled) {
        alleles.push_back( std::make_pair( -1.54, -0.034) );
        alleles.push_back( std::make_pair( -1.54, -0.034) );
    }

    if(_nao->_ankle->_enabled) {
        alleles.push_back( std::make_pair( -2.11, 2.11) ); //D+: pos amp
        alleles.push_back( std::make_pair( -2.11, 2.11) ); //D+: pos amp
        alleles.push_back( std::make_pair( -1.18, 0.92) ); //D+: pos amp
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase
    }

    if(_nao->_ankleRoll->_enabled) {
        alleles.push_back( std::make_pair( -1.15, 1.15) ); //D+: pos amp
        alleles.push_back( std::make_pair( -1.15, 1.15) ); //D+: pos amp
        alleles.push_back( std::make_pair( -0.39, 0.76) ); //D+: pos amp
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase

        alleles.push_back( std::make_pair( -1.15, 1.15) ); //D+: pos amp
        alleles.push_back( std::make_pair( -1.15, 1.15) ); //D+: pos amp
        alleles.push_back( std::make_pair( -0.76, 0.39) ); //D+: pos amp
        alleles.push_back( std::make_pair( 0.0, 2*PI ) ); //t: phase
    }

    return alleles;
}

void Robot::setGenome(const std::vector<float> &genome) {
    int i = 0;
    float T = genome[i++];
//    std::cout << "T(ms): " << std::setprecision(2) << T << std::endl;

    if (_nao->_legHip->_enabled) {
        _nao->_legHip->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }

    if (_nao->_knee->_enabled) {
        _nao->_knee->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }

    if (_nao->_shoulder->_enabled) {
        _nao->_shoulder->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }

    if (_nao->_legHipRoll->_enabled) {
        _nao ->_legHipRoll->_leftJoint->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
        _nao ->_legHipRoll->_rightJoint->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }

    if (_nao->_elbow->_enabled) {
        _nao->_elbow->_leftJoint->setJointStats( genome[i], genome[i+1], 0.0, 0.0, T);
        _nao->_elbow->_rightJoint->setJointStats( -1*genome[i+1], -1*genome[i], 0.0, PI, T);
        i+=2;
    }

    if (_nao->_ankle->_enabled) {
        _nao->_ankle->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }

    if (_nao->_ankleRoll->_enabled) {
        _nao->_ankleRoll->_leftJoint->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
        _nao->_ankleRoll->_rightJoint->setJointStats( genome[i], genome[i+1], genome[i+2], genome[i+3], T);
        i+=4;
    }
}

void Robot::printPosition(simxFloat* position) {
    simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
    std::cout << "NAO Position: [" << position[0] << ", " << position[1] << ", " << position[2] << "]" << std::endl;
}
