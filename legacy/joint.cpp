#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>
#include <iomanip>

#define JOINT_FLUSH_COUNT 6

Joint::Joint(int clientID, const char *jointName, bool displayGraphs, bool saveTmp) :
    VRepClass(clientID, jointName),
    _currentAngle(0.0),
    _neutralAngle(0.0)
{
    simxGetJointPosition(_clientID, _handle, &_initAngle, simx_opmode_streaming);
    while(simxGetJointPosition(_clientID, _handle, &_initAngle,simx_opmode_buffer)!=simx_return_ok);

    std::cout << "[ " << jointName << " ] Initial joint angle: " << _initAngle << std::endl;

    _saveTmpData = saveTmp || displayGraphs;

    if(_saveTmpData) {
        _flush_count = JOINT_FLUSH_COUNT;

        _fileName = QString::fromUtf8(".") + _name;

        _fd.open(_fileName.toStdString(), std::ios::out);
        _fd << "0 " << _initAngle << " 0\n";
        _fd << "0.01 " << _initAngle << " 0\n";
        _fd.flush();
    }

    if(displayGraphs) {
        _gp = new Gnuplot("gnuplot -persist");

        *_gp << "set terminal qt 0 title '" << _name.toStdString() << "'\n";
    //    _gp << "set xrange[0:15000]\n";
    //    _gp << "set yrange[-3:3]\n";
        *_gp << "set datafile separator whitespace\n";
        *_gp << "while (1) {plot '" << _fileName.toStdString()
            << "' using 1:2 with lines title 'Real " << _name.toStdString()
            << "', '' using 1:3 with lines title 'Desired " << _name.toStdString()
            << "'; pause 0.5;}\n";
        _gp->flush();
    } else {
        _gp = nullptr;
    }
}

Joint::~Joint() {
    simxGetJointPosition(_clientID, _handle, &_initAngle, simx_opmode_discontinue);

    if(_fd.is_open()) {
        _fd.close();
    }

    if(_gp != nullptr) {
        delete _gp;
    }
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
    float senPos = _velFactor*_t/_period + _initPhase/(2*PI);
    senPos = senPos - (int)senPos;
    if(senPos < 0) {
        senPos = 1 + senPos;
    }

    float amplitude = senPos < 0.5f ? _posAmp : _negAmp;
    float w = 2 * PI/_period;
    float newAngle = _neutralAngle + amplitude * std::sin(_velFactor*w* _t + _initPhase);
    _t += step_ms;

    if(_saveTmpData) {
        _fd << _t << " " << jointRealPosition() << " " << newAngle << "\n";

        if(--_flush_count == 0) {
            _fd.flush();
            _flush_count = JOINT_FLUSH_COUNT;
        }
    }

    if (_velFactor < 0.99f) {
        _velFactor = 1-std::exp(-(_t/_dampingFactor));

        if(_velFactor >= 0.99f) {
            _velFactor = 1.0f;
        }
    }

    if(newAngle == newAngle) {
        this->setJointTargetPosition(newAngle);
    } else {
        std::cout << "[" << _name.toStdString() << "] Invalid new angle: " << newAngle << "\n";
    }
}

void Joint::reset() {
    if(_saveTmpData) {
        _fd.close();

        _fd.open(_fileName.toStdString(), std::ios::out);
        _fd << "0 " << _initAngle << " 0\n";
        _fd << "0.01 " << _initAngle << " 0\n";
        _fd.flush();
    }

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

std::ofstream &Joint::tmpDataFile() {
    return _fd;
}

QString Joint::tmpDataFileName() {
    return _fileName;
}
