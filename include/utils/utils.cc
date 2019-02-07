#include "utils.hh"

Utils::Utils() {}

float Utils::distanceXY(const float *p1, const float *p2) {
    float dx = std::pow(p1[0] - p2[0], 2);
    float dy = std::pow(p1[1] - p2[1], 2);

    return std::sqrt(dx + dy);
}

float Utils::distanceYZ(const float *p1, const float *p2) {
    float dy = std::pow(p1[1] - p2[1], 2);
    float dz = std::pow(p1[2] - p2[2], 2);

    return std::sqrt(dy + dz);
}

float Utils::distanceXZ(const float *p1, const float *p2) {
    float dx = std::pow(p1[0] - p2[0], 2);
    float dz = std::pow(p1[2] - p2[2], 2);

    return std::sqrt(dx + dz);
}

float Utils::distance(const float *p1, const float *p2) {
    float dx = std::pow(p1[0] - p2[0], 2);
    float dy = std::pow(p1[1] - p2[1], 2);
    float dz = std::pow(p1[2] - p2[2], 2);

    return std::sqrt(dx + dy + dz);
}

float Utils::vsizeXY(const float *v) {
    float dx = std::pow(v[0], 2);
    float dy = std::pow(v[1], 2);

    return std::sqrt(dx + dy);
}

float Utils::vsizeYZ(const float *v) {
    float dy = std::pow(v[1], 2);
    float dz = std::pow(v[2], 2);

    return std::sqrt(dy + dz);
}

float Utils::vsizeXZ(const float *v) {
    float dx = std::pow(v[0], 2);
    float dz = std::pow(v[2], 2);

    return std::sqrt(dx + dz);
}

float Utils::vsize(const float *v) {
    float dx = std::pow(v[0], 2);
    float dy = std::pow(v[1], 2);
    float dz = std::pow(v[2], 2);

    return std::sqrt(dx + dy + dz);
}
