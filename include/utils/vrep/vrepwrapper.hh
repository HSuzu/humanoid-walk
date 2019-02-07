#ifndef VREPWRAPPER_HH
#define VREPWRAPPER_HH

extern "C" {
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "extApi.h"
}

#include <QList>

#define VREP_DEFAULT_CMD "vrep"
#define VREP_STARTING_PORT 20000
#define VREP_REMOTEAPI_FORMAT_ARG "-gREMOTEAPISERVERSERVICE_%d_%s_%s"
#define VREP_REMOTEAPI_DEBUG "FALSE"
#define VREP_REMOTEAPI_PREENABLESYNC "TRUE"
#define VREP_SCENE "../NAO.ttt"

class VRepWrapper {
private:
    static VRepWrapper *_vrep;
    VRepWrapper();

public:
    static VRepWrapper *vrep();

public:
    ~VRepWrapper();

private:
    void killVRep(pid_t pid);

private:
    QList<pid_t> _pids;
    QList<int> _availableInstances;
    QList<int> _readyInstances;

    int _cntr = 0;
    int getId();

public:
    int createInstance(int n = 1, int waitRespTime = 30000, bool enableGUI = true);
    int numOfAvailableInstaces();
    int numOfReadyInstances();

    int getInstance();
    int returnInstance(int clientID);
};

#endif // VREPWRAPPER_HH
