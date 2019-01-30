#ifndef JOINTPAIR
#define JOINTPAIR

#include "joint.hpp"
#include "helpers.hpp"

#include <utils/gnuplot/gnuplot-iostream.h>

class JointPair : public JointInterface {
private:
    Gnuplot _gp;
public:
    Joint* _leftJoint;
    Joint* _rightJoint;
    JointPair(int clientID,
              const char *rightJointName,
              const char *leftJointName);
    void setJointStats(float posAmp, float negAmp, float zeroAngle, float phase, float T);
    void update();
    void reset();
};

#endif
