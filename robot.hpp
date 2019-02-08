#ifndef ROBOT_H
#define  ROBOT_H

#include "VRepClass.hpp"
#include "joint.hpp"
#include "shape.hpp"
#include "NAO.hpp"

#include <vector>

#include <QString>
#include <QList>
#include <QHash>
#include <QPair>

#include <QMutex>

#include <utils/utils.hh>

struct result{
    float score, dx, dy, time;
};

class Robot : public VRepClass {
    Q_OBJECT
private:
    NAO *_nao;

    std::vector<JointInterface *> _joints;
    int _numJoints;

    std::vector<Shape *> _shapes;
    QHash<QString, QPair<simxInt, int> > _shapeNameHandler;

    simxFloat _initialPosition[3];
    simxFloat _initialOrientation[3];

    float _lHipPos[3];
    float _RLShoulderPos[3];

    result _lastResult;

    QMutex _isRunning;
public:
    Robot(int clientID, const char* name);
    ~Robot();

    void reset();
    void update();
    void printPosition(simxFloat* position);

    std::vector< std::pair<float, float> > getAleles();

    void setGenome( const std::vector<float> &genome );
    result runExperiment( const std::vector<float> &genome, const float time_s, const std::string label);

    void run(const float time_s);

    void wait();
    void results(result *r);
signals:
    void _runSignal_(const float time_s);

private slots:
    void _runSlot_(const float time_s);

private:
    simxInt getLHipPos(float *position, simxInt opMode);
    simxInt getRLShoulderPos(float *position, simxInt opMode);
};

#endif
