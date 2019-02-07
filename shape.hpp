#ifndef SHAPE_H
#define  SHAPE_H

#include "VRepClass.hpp"

class Shape : public VRepClass {
private:
  static int _n_untitle_shapes;
  simxFloat _initialPosition[3];
  simxFloat _initialOrientation[3];
public:
  Shape(int clientID, const char* name, simxFloat (&position)[3], simxFloat (&orientation)[3]);
  Shape(int clientID, const char* name, simxInt handle);
  Shape(int clientID, simxInt handle);
  friend std::ostream& operator<< (std::ostream& stream, const Shape& shape);

  void positionBlocking(simxFloat position[3]);
  void reset();
};

#endif
