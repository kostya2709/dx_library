#ifndef DX_POINT
#define DX_POINT

#include <math.h>

double calculate_angle (double x, double y);

struct dx_point
{
    double x = 0;
    double y = 0;
    double z = 0;

    dx_point (void) = default;
    dx_point (double x, double y, double z = 0) : x (x), y (y), z (z) {}

    dx_point operator-(const dx_point& another_point) const noexcept
    {
        dx_point result;
        result.x = this->x - another_point.x;
        result.y = this->y - another_point.y;
        result.z = this->z - another_point.z;

        return result;
    }

    dx_point operator+(const dx_point& another_point) const noexcept
    {
        dx_point result;
        result.x = this->x + another_point.x;
        result.y = this->y + another_point.y;
        result.z = this->z + another_point.z;
        
        return result;
    }

    template <typename T>
    dx_point operator+(T delta) const noexcept
    {        
        return dx_point (x + delta, y + delta);
    }

    template <typename T>
    dx_point operator-(T delta) const noexcept
    {        
        return dx_point (x - delta, y - delta);
    }

    template <typename T>
    dx_point operator*(T times) const noexcept
    {
        return dx_point (x * times, y * times);
    }

    template <typename T>
    dx_point operator/(T times) const noexcept
    {
        if (times == 0)
            return *this;

        return dx_point (x / times, y / times);
    }

    double length (void) const noexcept
    {
        return hypot (x, y);
    }

    double operator*(const dx_point& vec) const noexcept          /// scalar multiplication
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    dx_point proection (const dx_point& vec) const noexcept
    {
        return vec * ((*this * vec) / (vec.length() * vec.length()));
    }

    double angle (void) const noexcept                                      /// return angle in radian
    {      
        if (x > 0 && y > 0)
            return atan (y / x);
        if (x > 0 && y < 0)
            return atan (y / x) + 2 * M_PI;
        else
            return atan (y / x) + M_PI;
    }

    operator bool ()
    {
        double delta = 0.001;
        if (abs(x) < delta && abs (y) < delta)
            return 0;
        else return 1;
    }
};

#endif                  // #define DX_POINT