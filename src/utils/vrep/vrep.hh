#ifndef VREP_HH
#define VREP_HH

extern "C" {
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include "extApi.h"
}

#include <ga/ga.h>


#include <QObject>
#include <QString>
#include <QMutex>

#include <robot/robot.hpp>

#define VREP_DEFAULT_CMD "/usr/bin/vrep"
#define VREP_REMOTEAPI_FORMAT_ARG "-gREMOTEAPISERVERSERVICE_%d_%s_%s"

class VRep : public QObject {
    Q_OBJECT

private:
    int _id;

    pid_t _pid;
    int _readFd;
    int _writeFd;

    int _clientID;
    Robot *_robot;

    int _remoteAPIPort;
    bool _remoteAPIDebug;
    bool _remoteAPIPreEnableSync;

    QString _scene;

    QMutex _genomeProcess;
    GAGenome *_genome;
public:
    VRep(int id, int port, QObject *parent = nullptr);
    ~VRep();

public:
    int id();

    int clientID();
    Robot *robot();

    QString scene();
    void setScene(QString scene);

    int remoteAPIPort();
    void setRemoteAPIPort(int port);

    bool remoteAPIDebug();
    void setRemoteAPIDebug(bool debug);

    bool remoteAPIPreEnableSync();
    void setRemoteAPIPreEnableSync(bool enableSync);

    void killVRep();

    int readFd();
    int writeFd();

    void processGenome(GAGenome &g);
    void waitGenome();
    bool isGenomeProcessed();

public slots:
    void start();
    void processEndExperiment();
    void _processGenomeSlot_();

signals:
    void finished();
    void error(int id, QString err);
    void started(int id);

    void _processGenomeSignal_();
};

#endif // VREP_HH
