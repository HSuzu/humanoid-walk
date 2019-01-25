#include "NAO.hpp"

NAO::NAO(int clientID) :
  _clientID(clientID) {
    _legHip = new JointPair(_clientID, "RHipPitch3", "LHipPitch3");
    _legHipRoll = new JointPair(_clientID, "RHipRoll3", "LHipRoll3");
    
    _knee =  new JointPair(_clientID, "RKneePitch3", "LKneePitch3");
    
    _ankle = new JointPair(_clientID, "RAnklePitch3", "LAnklePitch3");
    _ankleRoll = new JointPair(_clientID, "RAnkleRoll3", "LAnkleRoll3");

    _shoulder = new JointPair(_clientID, "RShoulderPitch3", "LShoulderPitch3");

    _elbow = new JointPair(_clientID, "RElbowRoll3", "LElbowRoll3");
    // _elbow->_enabled=false;
}

void NAO::getNAOJoints(std::vector<JointInterface *> &jointVector) {
//    jointVector.push_back( _hip );

    // LEG
    jointVector.push_back( _knee );
    jointVector.push_back( _ankle );
    jointVector.push_back( _ankleRoll );
    jointVector.push_back( _legHip );
    jointVector.push_back( _legHipRoll );

  // ARM
    jointVector.push_back( _shoulder );
    jointVector.push_back( _elbow );
}
