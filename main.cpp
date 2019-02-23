#include <QCoreApplication>
#include <iostream>

#include <simulationwrapper.hh>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    SimulationWrapper sw(4, &a);
    sw.run();

    return a.exec();
}
