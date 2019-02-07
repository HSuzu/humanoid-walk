#include "shape.hpp"
#include "helpers.hpp"

#include <printcolors.hpp>

int Shape::_n_untitle_shapes = 0;

Shape::Shape(int clientID, const char* name, simxFloat (&position)[3],
simxFloat (&orientation)[3]) :
    VRepClass(clientID, name)
{
    for (int i = 0; i < 3; i++) {
        _initialOrientation[i] = orientation[i];
        _initialPosition[i] = position[i];
    }
    this->reset();
}

Shape::Shape(int clientID, const char* name, simxInt handle) :
    VRepClass(clientID, handle)
{
    _name = QString::fromUtf8(name);

    simxGetObjectPosition(clientID, handle, sim_handle_parent, _initialPosition, simx_opmode_blocking);
    simxGetObjectOrientation(clientID, handle, sim_handle_parent, _initialOrientation, simx_opmode_blocking);
}

Shape::Shape(int clientID, simxInt handle) :
    VRepClass(clientID, handle)
{
    _name = QString::fromUtf8("Untitle Shape ");
    _name.push_back(QString::number(_n_untitle_shapes));
    _n_untitle_shapes++;

    simxGetObjectPosition(clientID, handle, sim_handle_parent, _initialPosition, simx_opmode_blocking);
    simxGetObjectOrientation(clientID, handle, sim_handle_parent, _initialOrientation, simx_opmode_blocking);
}

void Shape::reset() {
    int ret = simxSetObjectPosition(_clientID, _handle, sim_handle_parent,
                                    _initialPosition, simx_opmode_oneshot);

    if (ret != simx_return_ok && ret != simx_return_novalue_flag) {
        std::cerr << PrintColors::BOLDRED << "[ ERROR ] " << PrintColors::RESET << "Setting position for shape " << _name.toStdString() << ". ";
        log_response(ret);
    }
    ret = simxSetObjectOrientation(_clientID, _handle, sim_handle_parent,
                                   _initialOrientation, simx_opmode_oneshot);

    if (ret != simx_return_ok && ret != simx_return_novalue_flag) {
        std::cerr << PrintColors::BOLDRED << "[ ERROR ] " << PrintColors::RESET << "Setting orientation for shape " << _handle << " (" << _name.toStdString() << "). ";
        log_response(ret);
    }
}

std::ostream& operator<< (std::ostream& stream, const Shape& shape) {
    stream << "Shape name: " << shape._name.toStdString() << " Handle: " << shape._handle << std::endl;
    stream << "Initial position: " << "[" << shape._initialPosition[0] << ", " <<
              shape._initialPosition[1] << ", " << shape._initialPosition[2] << "]" << std::endl;
    return stream;
}

void Shape::positionBlocking(simxFloat position[3]) {
    simxGetObjectPosition(_clientID, _handle, sim_handle_parent, position, simx_opmode_blocking);
}
