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
#include "../dx_library/Graphics/dx_graphics.hpp"
#include "../dx_library/Engine/dx_abstract_engine.hpp"


#ifndef MY_TYPE
#include "my_type.hpp"
#define MY_TYPE 1
#endif

void displayMe  (void);
void create_window (void);






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
void calculations (dx_vector <Type>& assign, dx_vector <Type>& compare)
{
    for (size_t cycle = 0; cycle < assign.size(); ++cycle)
    {
        super_type <int> value [(int) assign[cycle].x];
        for (int i = 0; i < (int) assign[cycle].x; ++i)
        {
            value [i] = rand() % 100;
        }
        
        Qsort (0, 4, value, comparator);

        assign [cycle].y = super_type<int>::get_assignment();
        compare [cycle].y = super_type<int>::get_comparison();

        printf ("\nassignments: %d\n", super_type<int>::get_assignment());
        printf ("comparisons: %d\n", super_type<int>::get_comparison());
        printf ("comparisons in comparator: %d\n", get_counter<int>());

        reset_counter<int>();
        super_type<int>::reset_counter();
    }
}



#endif
