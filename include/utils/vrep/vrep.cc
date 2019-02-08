#include "vrep.hh"

VRep::VRep(int id, int port, QObject *parent) : QObject (parent) {
    _id = id;

    _pid = -1;
    _readFd = -1;
    _writeFd = -1;

    _clientID = -1;
    _robot = nullptr;

    _remoteAPIPort = port;
    _remoteAPIDebug = false;
    _remoteAPIPreEnableSync = true;
}

VRep::~VRep() {
    killVRep();

    if(_robot != nullptr) {
        delete _robot;
    }

    close(_readFd);
    close(_writeFd);
}

void VRep::killVRep() {
    if(_pid > 0) {
        kill(_pid, SIGINT);

        int status;
        waitpid(_pid, &status, 0);
    }

    _pid = -1;
}

int VRep::id() {
    return _id;
}

int VRep::clientID() {
    return _clientID;
}

Robot *VRep::robot() {
    return _robot;
}

QString VRep::scene() {
    return _scene;
}

void VRep::setScene(QString scene) {
    _scene = scene;
}

int VRep::remoteAPIPort() {
    return _remoteAPIPort;
}

void VRep::setRemoteAPIPort(int port) {
    _remoteAPIPort = port;
}

bool VRep::remoteAPIDebug() {
    return _remoteAPIDebug;
}

void VRep::setRemoteAPIDebug(bool debug) {
    _remoteAPIDebug = debug;
}

bool VRep::remoteAPIPreEnableSync() {
    return _remoteAPIPreEnableSync;
}

void VRep::setRemoteAPIPreEnableSync(bool enableSync) {
    _remoteAPIPreEnableSync = enableSync;
}

int VRep::readFd() {
    return _readFd;
}

int VRep::writeFd() {
    return _writeFd;
}

void VRep::start() {
    int readPipeFd[2];
    int writePipeFd[2];

    if(pipe(readPipeFd) < 0) {
        emit error(_id, QString::fromUtf8("[ERROR/VRep] Fail to open read pipe. %s").arg(strerror(errno)));

        emit finished();
        return;
    }

    if(pipe(writePipeFd) < 0) {
        emit error(_id, QString::fromUtf8("[ERROR/VRep] Fail to open write pipe. %s").arg(strerror(errno)));

        emit finished();
        return;
    }

    _pid = fork();

    if(_pid == 0) { // child process
        close(readPipeFd[0]); // close read
        close(writePipeFd[1]); // close write

        _writeFd = readPipeFd[1];
        _readFd = writePipeFd[0];

        close(0); // close previous stdin
        dup2(_readFd, 0); // replace stdin with pipe

        close(1); // close previous stdout
        dup2(_writeFd, 1); // replace stdout with pipe

        printf("Ok.\n");

        char remoteArg[50];
        char remoteDebug[6];
        char remoteSync[6];

        if(remoteAPIDebug()) {
            strncpy(remoteDebug, "TRUE", sizeof(remoteDebug));
        } else {
            strncpy(remoteDebug, "FALSE", sizeof(remoteDebug));
        }

        if(remoteAPIPreEnableSync()) {
            strncpy(remoteSync, "TRUE", sizeof(remoteSync));
        } else {
            strncpy(remoteSync, "FALSE", sizeof(remoteSync));
        }

        sprintf(remoteArg, VREP_REMOTEAPI_FORMAT_ARG, remoteAPIPort(), remoteDebug, remoteSync);

        char *argv[] = {VREP_DEFAULT_CMD, remoteArg, _scene.toUtf8().data(), NULL};
        if(execv(VREP_DEFAULT_CMD, argv) < 0) {
            printf("Fail.\n");
            exit(-1);
        }
    } else if(_pid > 0) { // parent process
        close(readPipeFd[1]); // close write
        close(writePipeFd[0]); // close read;

        _readFd = readPipeFd[0];
        _writeFd = writePipeFd[1];

        FILE *readFile = fdopen(_readFd, "r");

        size_t n;
        char *status = NULL;
        getline(&status, &n, readFile);

        if(!strncmp(status, "Ok.", n)) {
            free(status);
            emit error(_id, QString::fromUtf8("[ERROR/VRep] Something went wrong inside the child..."));

            emit finished();
            return;
        } else {
            free(status);
        }

        getline(&status, &n, readFile);

        if(strncmp(status, "Fail.", n)) {
            free(status);
            emit error(_id, QString::fromUtf8("[ERROR/VRep] Fail to exec VREP."));

            emit finished();
            return;

        } else {
            free(status);
        }

        _clientID = simxStart("127.0.0.1", remoteAPIPort(), true, true, 5000, 5);
        if(_clientID == -1) {
            emit error(_id, QString::fromLatin1("[Error/VRep] Fail to connect."));
            killVRep();

            emit finished();
        }

        simxStartSimulation(_clientID, simx_opmode_blocking);

        _robot = new Robot(_clientID, "NAO");

        emit started(_id);
    } else {
        emit error(_id, QString::fromUtf8("[Error/VRep] Fail to fork. %s").arg(strerror(errno)));

        emit finished();
        return;
    }
}

