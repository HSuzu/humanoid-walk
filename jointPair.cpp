#include "jointPair.hpp"

JointPair::JointPair(int clientID,
  const char *rightJointName,
  const char *leftJointName) {
    _rightJoint = new Joint(clientID, rightJointName);
    _leftJoint = new Joint(clientID, leftJointName);
  }


void JointPair::reset() {
  _leftJoint->reset();
  _rightJoint->reset();
}

void JointPair::update() {
  _leftJoint->update();
  _rightJoint->update();
}

void JointPair::setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T) {
  _leftJoint->setJointStats(posAmp, negAmp, neutralAngle, phase, T);
  _rightJoint->setJointStats(posAmp, negAmp, neutralAngle, phase + 0.5, T);
}
