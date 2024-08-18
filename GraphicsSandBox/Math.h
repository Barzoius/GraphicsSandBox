#pragma once
#include <math.h>

constexpr float FPI = 3.14159265f;
constexpr double DPI = 3.1415926535897932;


enum QUADRANT {
    QUADRANT_I = 1,
    QUADRANT_II,
    QUADRANT_III,
    QUADRANT_IV,
    POSITIVE_X_AXIS = 0,
    POSITIVE_Y_AXIS = 90,
    NEGATIVE_X_AXIS = 180,
    NEGATIVE_Y_AXIS = 270,
    POSITIVE_X_AXIS_AFTER_FULL_TURN = 360
};

template <typename T>
constexpr auto sq(const T& x)
{
    return x * x;
}

template <typename T>
constexpr T interpolate(const T& start, const T& finish, float ALPHA)
{
    return start + (finish - start) * ALPHA;
}

template <typename T>
constexpr T degrees_to_radians(T deg)
{
    return deg * FPI / (T)180.0;
}

template <typename T>
constexpr T radians_to_degrees(T rad)
{
    return rad * (T)180.0 / FPI;
}


template <typename T>
constexpr T degrees_to_quadrants(T deg)
{
    return deg / (T)90.0;
}

template <typename T>
constexpr T quadrants_to_degrees(T quad)
{
    return quad * (T)90.0;
}


template <typename T>
constexpr QUADRANT get_quadrant(T angle)
{
    T normalizedAngle;

    if (angle > (T)360.0)
    {
        normalizedAngle = angle - (T)360.0
    }
    else if (angle < (T)0)
    {
        normalizedAngle = angle + (T)360.0
    }
    else
    {
        normalizedAngle = angle;
    }


    if (normalizedAngle == 0 {
        return POSITIVE_X_AXIS;
    }
    else if (normalizedAngle == 360) {
        return POSITIVE_X_AXIS_AFTER_FULL_TURN;
    }
    else if (normalizedAngle == 90) {
        return POSITIVE_Y_AXIS;
    }
    else if (normalizedAngle == 180) {
        return NEGATIVE_X_AXIS;
    }
    else if (normalizedAngle == 270) {
        return NEGATIVE_Y_AXIS;
    }
    else if (normalizedAngle > 0 && normalizedAngle < 90) {
        return QUADRANT_I;
    }
    else if (normalizedAngle > 90 && normalizedAngle < 180) {
        return QUADRANT_II;
    }
    else if (normalizedAngle > 180 && normalizedAngle < 270) {
        return QUADRANT_III;
    }
    else {
        return QUADRANT_IV;
    }

}

template<typename T>
T wrap_angle(T theta)
{
    const T modded = fmod(theta, (T)2.0 * (T)DPI);
    return (modded > (T)DPI) ?
        (modded - (T)2.0 * (T)DPI) :
        modded;
}