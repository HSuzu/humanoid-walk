#include "jointPair.hpp"

JointPair::JointPair(int clientID,
                     const char *rightJointName,
                     const char *leftJointName) : _gp("gnuplot") {
    _rightJoint = new Joint(clientID, rightJointName, false, true);
    _leftJoint = new Joint(clientID, leftJointName, false, true);

    _gp << "set terminal qt 0 title '" << rightJointName << " and " << leftJointName << "' noraise\n";
    _gp << "set datafile separator whitespace\n";
    _gp << "while (1) {\n";
    _gp << "set multiplot layout 2,1\n";
    _gp << "plot '" << _rightJoint->tmpDataFileName().toStdString()
        << "' using 1:2 with lines title 'Real " << rightJointName
        << "', '' using 1:3 with lines title 'Desired " << rightJointName << "'\n";
    _gp << "plot '" << _leftJoint->tmpDataFileName().toStdString()
        << "' using 1:2 with lines title 'Real " << leftJointName
        << "', '' using 1:3 with lines title 'Desired " << leftJointName << "'\n";
    _gp << "unset multiplot\n";
    _gp << "pause 0.5\n}\n";
    _gp.flush();

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
    _rightJoint->setJointStats(posAmp, negAmp, neutralAngle, phase + PI, T);
}
