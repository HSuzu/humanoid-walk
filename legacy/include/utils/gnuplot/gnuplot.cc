#include "gnuplot.hh"

GnuplotIO *GnuplotIO::_gpio = nullptr;
int GnuplotIO::_idx = 0;

GnuplotIO::GnuplotIO() : _mutex(), _gp("gnuplot -persist") {
}

GnuplotIO *GnuplotIO::gp(int *idx) {
    if(idx != nullptr) {
        *idx = _idx++;
    }

    if(_gpio == nullptr) {
        _gpio = new GnuplotIO();
    }

    return _gpio;
}

void GnuplotIO::getControl(int idx) {
    _mutex.lock();
    _gp << "set terminal qt " << idx << " noraise\n";
}

void GnuplotIO::releaseControl() {
    _gp << "\n";
    flush();
    _mutex.unlock();
}

void GnuplotIO::setWindowName(int idx, const char *name) {
    getControl(idx);
    _gp << "set terminal qt " << idx << " title '" << name << "'\n";
    releaseControl();
}

void GnuplotIO::flush() {
    _gp.flush();
}
