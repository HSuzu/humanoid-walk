#include "vrepwrapper.hh"

VRepWrapper *VRepWrapper::_vrep = nullptr;

VRepWrapper::VRepWrapper() {}

VRepWrapper::~VRepWrapper() {
    for(int i = 0; i < _pids.size(); i++) {
        killVRep(_pids.at(i));
    }
}

void VRepWrapper::killVRep(pid_t pid) {
    for(int j = 0; j < 2; j++) {
        kill(pid, SIGINT);
    }
}

VRepWrapper *VRepWrapper::vrep() {
    if(_vrep == nullptr) {
        _vrep = new VRepWrapper();
    }

    return _vrep;
}

int VRepWrapper::createInstance(int n, int waitRespTime, bool enableGUI) {
    int cnt = 0;

    for(int i = 0; i < n; i++) {
        pid_t pid = fork();

        int id = getId();
        if(pid == 0) {
            char remoteArg[50];
            sprintf(remoteArg, VREP_REMOTEAPI_FORMAT_ARG, VREP_STARTING_PORT + id, VREP_REMOTEAPI_DEBUG, VREP_REMOTEAPI_PREENABLESYNC);

            if(enableGUI) {
                char *argv[] = {VREP_DEFAULT_CMD, remoteArg, VREP_SCENE, NULL};
                execv(VREP_DEFAULT_CMD, argv);
            } else {
                char *argv[] = {VREP_DEFAULT_CMD, remoteArg, "-h", VREP_SCENE, NULL};
                execv(VREP_DEFAULT_CMD, argv);
            }
        } else if (pid > 0) {
            _pids.push_back(pid);

            int clientID = simxStart("127.0.0.1", VREP_STARTING_PORT + id, true, true, waitRespTime, 5);
            if(clientID >= 0) {
                cnt++;

                _availableInstances.push_back(clientID);

                simxStartSimulation(clientID, simx_opmode_blocking);
                _readyInstances.push_back(clientID);
            } else {
                killVRep(_pids.last());
                _pids.removeLast();
            }
        } else { // An error occured
            return cnt;
        }
    }

    return cnt;
}

int VRepWrapper::numOfAvailableInstaces() {
    return _availableInstances.size();
}

int VRepWrapper::numOfReadyInstances() {
    return _readyInstances.size();
}

int VRepWrapper::getId() {
    return  _cntr++;
}

int VRepWrapper::getInstance() {
    if(_readyInstances.size() > 0) {
        int id = _readyInstances.first();
        _readyInstances.pop_front();

        return id;
    }

    return -1;
}

int VRepWrapper::returnInstance(int clientID) {
    _readyInstances.push_back(clientID);

    return 1;
}
