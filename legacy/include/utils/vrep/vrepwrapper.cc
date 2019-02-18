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
    QList<VRep *> vrep;
    for(int i = 0; i < vrep.size(); i++) {
        delete vrep.at(i);
    }

    QList<QThread *> thread;
    for(int i = 0; i < thread.size(); i++) {
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

    VRep *vrep = new VRep(id, getPort());
    QThread *thread = new QThread();
    vrep->moveToThread(thread);

    QObject::connect(thread, &QThread::started, vrep, &VRep::start);
    QObject::connect(vrep, &VRep::finished, thread, &QThread::quit);
    QObject::connect(vrep, &VRep::finished, vrep, &VRep::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    QObject::connect(vrep, &VRep::started, this, &VRepWrapper::startedVRep);

    _vrepInstances.insert(id, vrep);
    _threads.insert(id, thread);

    thread->start();
}

void VRepWrapper::errorVRep(int id, QString err) {
    std::cout << err.toStdString() << "\n";

    VRep *vrep = _vrepInstances.value(id);
    QThread *thread = _threads.value(id);

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
