
#ifndef DX_ALLOCATORS
#define DX__ALLOCATORS

template <typename Type, template <typename> class Allocator>
class dx_vector;

template <typename Type>
class Allocator_Calloc
{
    void* give_memory (size_t num_bytes)
    {
        return calloc (1, num_bytes);
    }

    void release_memory (Type* ptr)
    {
        free (ptr);
    }

    friend class dx_vector <Type, Allocator_Calloc>;
};

template <typename Type>
class Allocator_New
{
    void* give_memory (size_t num_bytes)
    {
        return new char [num_bytes];
    }

    void release_memory (Type* ptr)
    {
        delete[] ptr;
    }

    friend class dx_vector <Type, Allocator_New>;
};

#endif                  // #define DX_ALLOCATORS