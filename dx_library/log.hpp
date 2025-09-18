#ifndef LIBS
#define LIBS

#include <GL/glew.h>
 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <utility>
#include <GL/glut.h>


#ifndef MY_TYPE
#include "my_type.hpp"
#define MY_TYPE 1
#endif

void displayMe  (void);
void create_window (void);


struct dx_point
{
    float x = 0;
    float y = 0;
    float z = 0;

    dx_point (float x, float y, float z = 0) : x (x), y (y), z (z) {}
};

struct dx_colour
{
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    uint8_t alpha = 0;

    dx_colour (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : 
    red (red), green (green), blue (blue), alpha (alpha) {}
};




template <typename Type>
struct point_array
{
    size_t size = 0;
    Type* array_size = nullptr;
    Type* comparisons = nullptr;
    Type* assignments = nullptr; 

    point_array (size_t size);
    ~point_array();
};

template <typename Type>
point_array<Type>::point_array (size_t size) : size (size)
{
    array_size  = new Type [size];
    comparisons = new Type [size];
    assignments = new Type [size];
}

template <typename Type>
point_array<Type>::~point_array()
{
    delete[] array_size;
    delete[] comparisons;
    delete[] assignments;
}


/*!
@brief This function sorts quickly elements of an array.
@param[in] left The left bound of the part of the array needs sorting.
@param[in] right The right bound of the part of the array needs sorting.
@param[in] *pointer[NLINES] The array which elements need sorting.
*/

template <typename Type>
void Qsort (int left, int right, Type* arr, int (*compare)(const Type&, const Type&))
{
    assert (arr);

    if (left >= right)
        return;

    int mid = (left + right) / 2;
    std::swap (arr [left], arr [mid]);

    int last = left;
    int i = 0;

    for (i = left; i <= right; i++)
        if (compare (arr [left], arr [i]) > 0)
            std::swap (arr [i], arr [++last]);

    std::swap (arr [left], arr [last]);

    Qsort (left, last -  1, arr, compare);
    Qsort (last + 1, right, arr, compare);

}

namespace
{
    static int compare_num = 0;
}

template <typename Type>
int comparator (const Type& first, const Type& second)
{
    ++compare_num;
    return first > second;
}

template <typename Type>
void reset_counter (void)
{
    compare_num = 0;
}

template <typename Type>
int get_counter (void)
{
    return compare_num;
}


template <typename Type>
void calculations (point_array<int>& points)
{
    for (int cycle = 0; cycle < points.size; ++cycle)
    {
        super_type <int> value [points.array_size [cycle]];
        for (int i = 0; i < points.array_size [cycle]; ++i)
        {
            value [i] = rand() % 100;
        }

        printf ("comparisons: %d\n", get_counter<int>());
        
        Qsort (0, 4, value, comparator);

        points.assignments [cycle] = super_type<int>::get_assignment();
        points.comparisons [cycle] = super_type<int>::get_comparison();

        printf ("\nassignments: %d\n", super_type<int>::get_assignment());
        printf ("comparisons: %d\n", super_type<int>::get_comparison());
        printf ("comparisons in comparator: %d\n", get_counter<int>());

        reset_counter<int>();
        super_type<int>::reset_counter();
    }
}



#endif