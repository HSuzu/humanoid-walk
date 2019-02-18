#ifndef UTILS_HH
#define UTILS_HH

#include <cmath>

class Utils {
private:
    Utils();
public:
    static float distanceXY(const float *p1, const float *p2);
    static float distanceYZ(const float *p1, const float *p2);
    static float distanceXZ(const float *p1, const float *p2);
    static float distance(const float *p1, const float *p2);

    static float vsizeXY(const float *v);
    static float vsizeYZ(const float *v);
    static float vsizeXZ(const float *v);
    static float vsize(const float *v);
};

#endif // UTILS_HH
