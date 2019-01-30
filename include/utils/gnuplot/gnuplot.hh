#ifndef GNUPLOTIO_HH
#define GNUPLOTIO_HH

#include <utils/gnuplot/gnuplot-iostream.h>
#include <QMutex>

class GnuplotIO {
private:
    static GnuplotIO *_gpio;

    static int _idx;
private:
    QMutex _mutex;
    Gnuplot _gp;
private:
    GnuplotIO();
public:
    static GnuplotIO *gp(int *idx);

    void getControl(int idx);
    void releaseControl();

    void setWindowName(int idx, const char *name);

    void flush();


//    template<typename T, std::size_t N>
//    GnuplotIO &operator<<(const T (&v)[N]);

    template<typename T>
    GnuplotIO &operator<<(const T &info);
};

template<typename T>
GnuplotIO &GnuplotIO::operator<<(const T &info) {
    _gp << info;

    return *this;
}

//template<typename T, std::size_t N>
//GnuplotIO &GnuplotIO::operator<<(const T (&v)[N]) {
//    _gp << v;

//    return *this;
//}


#endif // GNUPLOTIO_HH
