#include "VRepClass.hpp"

VRepClass::VRepClass(int clientID, const char* name) :
  _clientID(clientID),
  _name(name) {
    simxGetObjectHandle(_clientID, _name.toUtf8(), &_handle,
      simx_opmode_blocking);
}

VRepClass::VRepClass(int clientID, simxInt handle) :
  _clientID(clientID),
  _handle(handle) {

  };
