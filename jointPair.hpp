#ifndef JOINTPAIR
#define JOINTPAIR

#include "joint.hpp"

class JointPair : public JointInterface {
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
