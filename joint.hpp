#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"
#include <cmath>

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

    simxFloat _initAngle;
public:
    void setJointTargetPosition(float targetAngle);
    Joint(int clientID, const char *jointName);
    void setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T_ms);
    void update();
    void reset();

    float jointRealPosition();

    float _currentAngle;
};

#endif
