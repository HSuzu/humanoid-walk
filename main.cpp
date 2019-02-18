#include <QCoreApplication>

#include <simulationwrapper.hh>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    SimulationWrapper sw(2);
    sw.run();

    return 0;
//    return a.exec();
}
