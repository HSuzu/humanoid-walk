#ifndef VREPCLASS_H
#define  VREPCLASS_H

#include <iostream>
extern "C" {
  #include "extApi.h"
}

#include <QObject>
#include <QString>

class VRepClass : public QObject {
    Q_OBJECT
protected:
  int _clientID;
  QString _name;
  simxInt _handle;
public:
  VRepClass(int clientID, const char* name, QObject *parent = nullptr);
  VRepClass(int clientID, simxInt handle, QObject *parent = nullptr);
  void reset();
};


#endif
