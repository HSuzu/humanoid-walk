#ifndef VREPCLASS_H
#define  VREPCLASS_H

#include <iostream>
extern "C" {
  #include "extApi.h"
}

#include <QString>

class VRepClass {
protected:
  int _clientID;
  QString _name;
  simxInt _handle;
public:
  VRepClass(int clientID, const char* name);
  VRepClass(int clientID, simxInt handle);
  void reset();
};


#endif
