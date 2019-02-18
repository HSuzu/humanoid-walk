#include "VRepClass.hpp"

VRepClass::VRepClass(int clientID, const char* name, QObject *parent) :
    _clientID(clientID),
    _name(name),
    QObject(parent) {
    simxGetObjectHandle(_clientID, _name.toUtf8(), &_handle,
                        simx_opmode_blocking);
}

VRepClass::VRepClass(int clientID, simxInt handle, QObject *parent) :
    _clientID(clientID),
    _handle(handle),
    QObject(parent) {

};
