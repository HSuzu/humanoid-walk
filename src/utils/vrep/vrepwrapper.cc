#include "vrepwrapper.hh"

VRepWrapper *VRepWrapper::_wrapper = nullptr;
int VRepWrapper::_uniquePort = VREP_STARTING_PORT;
int VRepWrapper::_uniqueId = 0;

VRepWrapper::VRepWrapper() : QObject() {
}

VRepWrapper *VRepWrapper::wrapper() {
    if(_wrapper == nullptr) {
        _wrapper = new VRepWrapper();
    }

    return _wrapper;
}

VRepWrapper::~VRepWrapper() {
    QList<VRep *> vrep = _vrepInstances.values();
    for(int i = 0; i < vrep.size(); i++) {
        if(vrep.at(i))
            delete vrep.at(i);
    }

    QList<QThread *> thread = _threads.values();
    for(int i = 0; i < thread.size(); i++) {
        if(thread.at(i))
            delete thread.at(i);
    }
}

int VRepWrapper::getPort() {
    return _uniquePort++;
}

int VRepWrapper::getId() {
    return _uniqueId++;
}

int VRepWrapper::createInstance() {
    int id = getId();

    QThread *thread = new QThread();
    VRep *vrep = new VRep(id, getPort());
    _defaultScene = QString::fromLatin1(VREP_SCENE);
    vrep->setScene(_defaultScene);
    vrep->moveToThread(thread);

    QObject::connect(thread, &QThread::started, vrep, &VRep::start);
    QObject::connect(vrep, &VRep::finished, thread, &QThread::quit);
    QObject::connect(vrep, &VRep::finished, vrep, &VRep::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    QObject::connect(vrep, &VRep::started, this, &VRepWrapper::startedVRep);

    QObject::connect(vrep, &VRep::error, this, &VRepWrapper::errorVRep);

    _vrepInstances.insert(id, vrep);
    _threads.insert(id, thread);

    thread->start();

    return 0;
}

void VRepWrapper::errorVRep(int id, QString err) {
    std::cout << err.toStdString() << "\n";

    VRep *vrep = _vrepInstances.value(id);
    QThread *thread = _threads.value(id);

    _vrepInstances.remove(id);

    if(vrep != 0 && thread != 0) {
        QObject::disconnect(thread, &QThread::started, vrep, &VRep::start);
        QObject::disconnect(vrep, &VRep::finished, thread, &QThread::quit);
        QObject::disconnect(vrep, &VRep::finished, vrep, &VRep::deleteLater);
        QObject::disconnect(thread, &QThread::finished, thread, &QThread::deleteLater);

        QObject::disconnect(vrep, &VRep::error, this, &VRepWrapper::errorVRep);
        QObject::disconnect(vrep, &VRep::started, this, &VRepWrapper::startedVRep);
    }

    if(vrep != 0) delete vrep;
    if(thread != 0) _availableThreads.insert(id, thread);
}

void VRepWrapper::startedVRep(int id) {
    VRep *vrep = _vrepInstances.value(id);
    if(vrep != 0) _availableInstaces.push_back(vrep);
}

void VRepWrapper::clearUnusedThreads() {
    QList<int> ids = _availableThreads.keys();
    for(int i = 0; i < ids.size(); i++) {
        _threads.remove(ids.at(i));

        delete _availableThreads.value(ids.at(i));
    }
}

VRep *VRepWrapper::getInstance() {
    if(_availableInstaces.size() == 0) {
        return nullptr;
    }

    VRep *vrep = _availableInstaces.first();
    _availableInstaces.removeFirst();

    return vrep;
}

int VRepWrapper::returnInstance(VRep *vrep) {
    int id = vrep->id();

    if(_vrepInstances.find(id) != _vrepInstances.end()) {
        _availableInstaces.push_back(vrep);
        return 0;
    }

    return -1;
}

int VRepWrapper::numOfAvailableInstances() {
    return _availableInstaces.size();
}
