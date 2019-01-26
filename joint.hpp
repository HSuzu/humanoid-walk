#ifndef JOINT_H
#define JOINT_H

#include <QtCore/QString>
#include <utils/gnuplot-iostream.h>
#include <vector>

#include "VRepClass.hpp"

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
    Gnuplot gp;
    std::vector<std::pair<double,double> > _requestedJointAngle;

private:

    float _posAmp;
    float _negAmp;
    float _neutralAngle;
    float _t;
    float _tDelta;
    float _period;
    float _initPhase;
    float _amplFactor = 0;
    float _velFactor = 0;
    simxFloat _initAngle;
public:
    void setJointTargetPosition(float targetAngle);
    Joint(int clientID, const char *jointName);
    void setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T_ms);
    void update();
    void reset();
    float _currentAngle;
    float _neutralAngleDelta;
};

#endif
