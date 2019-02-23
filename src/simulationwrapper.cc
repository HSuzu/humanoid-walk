#include "simulationwrapper.hh"

#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

QThread *SimulationWrapper::_vrepThread = nullptr;
VRepWrapper *SimulationWrapper::_vrepWrapper = nullptr;

SimulationWrapper::SimulationWrapper(int nInstances, QObject *parent) : QObject(parent) {
    _seed = time(NULL);

    _vrepThread = new QThread();
    _vrepWrapper = VRepWrapper::wrapper();
    _vrepWrapper->moveToThread(_vrepThread);
    QObject::connect(_vrepThread, &QThread::finished, _vrepThread, &QThread::deleteLater);

    for(int i = 0; i < nInstances; i++) {
        _vrepWrapper->createInstance();
    }

    _vrepThread->start();
}

void SimulationWrapper::populationEvaluator(GAPopulation &p) {
    QList<VRep *> v;

    int idx = 0;
    while(idx < p.size()) {
        VRep *vrep = _vrepWrapper->getInstance();

        if(vrep != nullptr) {
            v.push_back(vrep);
            GAGenome &g = p.individual(idx);
            vrep->processGenome(g);

            idx++;
        } else {
            bool ret = false;
            QMutableListIterator<VRep *> it(v);
            while(it.hasNext()) {
                VRep *inst = it.next();

                if(inst->isGenomeProcessed()) {
                    _vrepWrapper->returnInstance(inst);

                    it.remove();
                    ret = true;
                }
            }

            if(ret == false) {
                sleep(0.1);
            }
        }
    }

    QMutableListIterator<VRep *> it(v);
    while(it.hasNext()) {
        VRep *inst = it.next();

        if(inst->isGenomeProcessed()) {
            _vrepWrapper->returnInstance(inst);

            it.remove();
        }
    }

}

GAPopulation SimulationWrapper::run() {
    GARealAlleleSetArray alleles;
    for (auto &pair : Robot::getAleles()) {
        alleles.add(pair.first, pair.second);
    }

    GARealGenome genome(alleles);
    genome.crossover(GARealBlendCrossover);

    GAPopulation population(genome);
    population.size(150);
    population.evaluator(SimulationWrapper::populationEvaluator);

    GASteadyStateGA ga(population);
    ga.nGenerations(200);
    ga.pReplacement(0.8);
    ga.pMutation(0.1);
    ga.evolve(_seed);

    return ga.population();
}
