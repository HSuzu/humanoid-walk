#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>
#include <iomanip>


Joint::Joint(int clientID, const char *jointName) :
    VRepClass(clientID, jointName),
    _currentAngle(0.0),
    _neutralAngle(0.0)
{
    simxGetJointPosition(_clientID, _handle, &_initAngle, simx_opmode_streaming);
    while(simxGetJointPosition(_clientID, _handle, &_initAngle,simx_opmode_buffer)!=simx_return_ok);

    std::cout << "[ " << jointName << " ] Initial joint angle: " << _initAngle << std::endl;
}

Joint::~Joint() {
    simxGetJointPosition(_clientID, _handle, &_initAngle, simx_opmode_discontinue);
}

void Joint::setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T_ms) {
    _posAmp = posAmp;
    _negAmp = negAmp;
    _initPhase = phase;
    _neutralAngle = neutralAngle;
    _period = T_ms;

#ifdef DEBUG
    std::cout << "[ " << _name.toStdString() << " ] " <<std::endl;
    std::cout << "PosAmp: " << std::setprecision(2) << _posAmp << ", ";
    std::cout << "NegAmp: " << std::setprecision(2) <<  _negAmp << ", ";
    std::cout << "NeutralAngle: " << std::setprecision(2) << _neutralAngle << ", ";
    std::cout << "Initial Phase: " << std::setprecision(2) << _initPhase << std::endl;
#endif
}

void Joint::update() {
    if (!_enabled) return;
    // Relative position of the moviment if we were in a senoidal function of period 1s
    float senPos = _t/_period + _initPhase/2*PI;
    senPos = senPos - (int)senPos;

    float amplitude = senPos < 0.5f ? _posAmp : _negAmp;
    float w = 2 * PI/_period;
    float newAngle = _neutralAngle + amplitude * std::sin(_velFactor*w* _t + _initPhase);
    _t += step_ms;

    if (_velFactor < 0.99f) {
        _velFactor = 1-std::exp(-(_t/_dampingFactor));

        if(_velFactor >= 0.99f) {
            _velFactor = 1.0f;
        }
    }

    if(newAngle == newAngle) {
        this->setJointTargetPosition(newAngle);
    }
}

void Joint::reset() {
    _t = 0.0f;
    _velFactor = 0.0f;
    this->setJointTargetPosition(_neutralAngle);
}

void Joint::setJointTargetPosition(float targetAngle) {
    int ret = simxSetJointTargetPosition(_clientID, _handle, targetAngle, simx_opmode_oneshot);
    _currentAngle = targetAngle;
#ifdef DEBUG
    // std::cout << "[ " << _name << " ] Current angle: " << std::setprecision(2) << _currentAngle <<std::endl;
    if (ret) {
        std::cout << "Error in joint " << _name << " for angle " << targetAngle
                  << "." << std::endl;
        log_response(ret);
    }
#endif
}


float Joint::jointRealPosition() {
    simxGetJointPosition(_clientID, _handle, &_realJointPosition, simx_opmode_buffer);

    return _realJointPosition;
}
