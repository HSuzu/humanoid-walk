#ifndef SIMULATIONWRAPPER_HH
#define SIMULATIONWRAPPER_HH

#include <ga/ga.h>

#include <QObject>
#include <QList>
#include <ctime>

#include <utils/vrep/vrepwrapper.hh>
#include <utils/vrep/vrep.hh>

typedef struct _genomeExecData {
    VRep *vrep;
} GenomeExecutionData;

class SimulationWrapper : public QObject {
    Q_OBJECT
private:
    int _seed;

    static QThread *_vrepThread;
    static VRepWrapper *_vrepWrapper;
public:
    SimulationWrapper(int nInstances, QObject *parent = nullptr);

    GAPopulation run();

public:
    static void populationEvaluator(GAPopulation &p);
};

#endif // SIMULATIONWRAPPER_HH
