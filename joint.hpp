#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"
#include <cmath>

#include <utils/gnuplot/gnuplot-iostream.h>
#include <fstream>

class JointInterface
{
public:
    JointInterface(){}
    virtual ~JointInterface(){}
    virtual void setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T) = 0;
    virtual void update() = 0;
    virtual void reset() = 0;
    bool _enabled = true;
};

class Joint : public VRepClass, public JointInterface {
private:
    float _posAmp; // Amplitude for the positive semi-cycle of the senoidal function
    float _negAmp; // Amplitude for the negative semi-cycle of the senoidal function
    float _neutralAngle; // The angle that the joint move around
    float _t; // Current time of simulation
    float _period; // Period of the movement (ms)
    const float _dampingFactor = 300; // This control the time for the function to reach its peak (time approx DP * 3.5)
    float _velFactor = 0;
    float _initPhase;

    float _realJointPosition;

    simxFloat _initAngle;

    std::ofstream _fd;
    QString _fileName;
    bool _saveTmpData;

    int _gpidx;
    Gnuplot *_gp;
    int _flush_count;
public:
    void setJointTargetPosition(float targetAngle);
    Joint(int clientID, const char *jointName, bool displayGraphs = true, bool saveTmp = true);
    ~Joint();
    void setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T_ms);
    void update();
    void reset();

    float jointRealPosition();

    std::ofstream &tmpDataFile();
    QString tmpDataFileName();

    float _currentAngle;
};

#endif
