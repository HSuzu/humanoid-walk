#ifndef VREPWRAPPER_HH
#define VREPWRAPPER_HH

#include <QObject>
#include <QThread>
#include <QHash>
#include <QString>

#include <iostream>

#include "vrep.hh"

#define VREP_STARTING_PORT 20000
#define VREP_SCENE "../NAO.ttt"

typedef int vrep_id;

class VRepWrapper : public QObject {
    Q_OBJECT
private:
    static VRepWrapper *_wrapper;
    VRepWrapper();

public:
   static VRepWrapper *wrapper();

private:
    static vrep_id _uniqueId;
    static int _uniquePort;

    QString _defaultScene;

    QHash<int, VRep *> _vrepInstances;
    QHash<int, QThread *> _threads;

    QList<VRep *> _availableInstaces;
    QHash<int, QThread *> _availableThreads;
private:
    static int getPort();
    static int getId();
public:
    ~VRepWrapper();

    int createInstance();
    void clearUnusedThreads();

    int numOfAvailableInstances();

    VRep *getInstance();
    int returnInstance(VRep *vrep);
public slots:
    void errorVRep(int id, QString err);
    void startedVRep(int id);
};

#endif // VREPWRAPPER_HH
