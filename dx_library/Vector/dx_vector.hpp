
#ifndef DX_VECTOR
#define DX_VECTOR

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "dx_allocators.hpp"



template <typename Type, template <typename> class Allocator = Allocator_New>
class dx_vector
{
    Type* data = NULL;
    size_t dx_size = 0;
    size_t dx_capacity = 0;

public:
    dx_vector (void) noexcept {}
    explicit dx_vector (size_t dx_size) noexcept;
    dx_vector (size_t dx_size, const Type& example) noexcept;
    dx_vector (const dx_vector <Type, Allocator>& example) noexcept;
    dx_vector (dx_vector <Type, Allocator>&& example) noexcept;

    Type& operator= (const dx_vector <Type, Allocator>& example) noexcept;
    Type& operator= (dx_vector <Type, Allocator>&& example) noexcept;

    Type& operator [] (size_t index) noexcept;
    const Type& operator [] (size_t index) const noexcept;
    
    void* operator new (size_t size, void* ptr);
    void* operator new (size_t size);

    size_t size (void) const noexcept;
    size_t capacity (void) const noexcept;
    int push_back (const Type& example) noexcept;
    void dump (void) const noexcept;

    template <typename Type_, template <typename> class Allocator_>
    friend std::ostream& operator<< (std::ostream &out, const dx_vector <Type_, Allocator_>& vector);
    
    template <typename Type_, template <typename> class Allocator_>
    friend std::ostream& operator>> (std::ostream &out, const dx_vector <Type_, Allocator_>& vector);

    template <typename... Args>
    void emplace_back (Args&&... args);

    ~dx_vector();
};

template <typename Type, template <typename> class Allocator>
dx_vector<Type, Allocator>::dx_vector (size_t new_size) noexcept
{
    Allocator<Type> alloc;
    data = reinterpret_cast <Type*> (alloc.give_memory (new_size * sizeof (Type)));
    Type* temp = data;

    for (size_t i = 0; i < new_size; ++i)
        (*(temp++)) = std::move (Type());

    dx_size = new_size;
    dx_capacity = new_size;
}


template <typename Type, template <typename> class Allocator>
dx_vector<Type, Allocator>::dx_vector (size_t new_size, const Type& example) noexcept
{
    Allocator<Type> alloc;
    data = reinterpret_cast <Type*> (alloc.give_memory (new_size * sizeof (Type)));
    Type* temp = data;

    for (size_t i = 0; i < new_size; ++i)
        (*(temp++)) = example;

    dx_size = new_size;
    dx_capacity = new_size;
}


template <typename Type, template <typename> class Allocator>
dx_vector<Type, Allocator>::dx_vector (const dx_vector <Type, Allocator>& example) noexcept : 
dx_size (example.size()), dx_capacity (example.capacity())
{
    Allocator<Type> alloc;
    data = reinterpret_cast <Type*> (alloc.give_memory (dx_size * sizeof (Type)));
    memcpy (data, example.data, example.dx_size);
}


template <typename Type, template <typename> class Allocator>
dx_vector<Type, Allocator>::dx_vector (dx_vector <Type, Allocator>&& example) noexcept : 
dx_size (example.dx_size()), dx_capacity (example.dx_capacity()), data (example.data)
{
    example.data = NULL;
}

template <typename Type, template <typename> class Allocator>
Type& dx_vector <Type, Allocator>::operator= (const dx_vector <Type, Allocator>& example) noexcept
{
    if (example.size < this->size)
    {
        for (int i = 0; i < example.dx_size; ++i)
            this->data [i] = example.data [i];
    }
    else
    {
        delete[] data;

        Allocator <Type> alloc;
        data = reinterpret_cast <Type*> (alloc.give_memory (example.dx_size * sizeof (Type)));
        memcpy (data, example.data, example.dx_size);
        
        this->dx_capacity = example.dx_size;
    }
    
    this->dx_size = example.dx_size;
}

template <typename Type, template <typename> class Allocator>
Type& dx_vector <Type, Allocator>::operator= (dx_vector <Type, Allocator>&& example) noexcept
{
    this->dx_size = example.dx_size;
    this->dx_capacity = example.dx_capacity;
    this->data = example.data;

    example.data = nullptr;
}

template <typename Type, template <typename> class Allocator>
Type& dx_vector <Type, Allocator>::operator [] (size_t index) noexcept
{
    return data [index];
}

template <typename Type, template <typename> class Allocator>
const Type& dx_vector <Type, Allocator>::operator [] (size_t index) const noexcept
{
    return data [index];
}


template <typename Type, template <typename> class Allocator>
size_t dx_vector <Type, Allocator>::size (void) const noexcept
{
    return this->dx_size;
}

template <typename Type, template <typename> class Allocator>
size_t dx_vector <Type, Allocator>::capacity (void) const noexcept
{
    return this->dx_capacity;
}


template <typename Type, template <typename> class Allocator>
void dx_vector <Type, Allocator>::dump (void) const noexcept
{
    printf ("\n\n-----------------------------------------------------\n");
    printf ("Info about the vector:\n\n");
    printf ("Capacity: %lu;\nSize: %lu\n", this->dx_capacity, this->dx_size);
    printf ("Data pointer: %p\n", this->data);

    for (size_t i = 0; i < this->dx_size; ++i)
    {
        std::cout << "data [" << i << "]: " << this->data[i] << ";\n";
    }

    printf ("-----------------------------------------------------\n\n");
}

template <typename Type, template <typename> class Allocator>
int dx_vector<Type, Allocator>::push_back (const Type& example) noexcept
{
    if (dx_capacity <= dx_size)
    {
        if (dx_capacity == 0)
            dx_capacity = 1;
        else
            dx_capacity *= 2;

        data = (Type*) realloc (data, dx_capacity * sizeof (Type));
    }

    Type& new_elem = *(data + dx_size);
    new_elem = example;

    ++dx_size;
    
    return 0;
}


template <typename Type, template <typename> class Allocator>
template <typename... Args>
void dx_vector <Type, Allocator>::emplace_back (Args&&... args)
{
    if (dx_size >= dx_capacity)
    {
        if (dx_capacity == 0)
            dx_capacity = 1;
        else
            dx_capacity *= 2;

        data = (Type*) realloc (data, dx_capacity * sizeof (Type));
    }

    new (data + dx_size) Type (std::forward <Args...>(args...));
    ++dx_size;

}

template <typename Type, template <typename> class Allocator>
std::ostream& operator<< (std::ostream& out, const dx_vector<Type, Allocator>& vector)
{
    for (size_t i = 0; i < vector.size(); ++i)
        out << vector[i] << " ";

    return out;
}

template <typename Type, template <typename> class Allocator>
std::ostream& operator>> (std::ostream& out, const dx_vector<Type, Allocator>& vector)
{
    for (int i = 0; i < vector.size(); ++i)
        out << vector[i] << " ";

    return out;
}


template <typename Type, template <typename> class Allocator>
void* dx_vector<Type, Allocator>::operator new (size_t size, void* ptr)
{
    printf ("Local new (ptr)\n");
    return ptr;
}

template <typename Type, template <typename> class Allocator>
void* dx_vector<Type, Allocator>::operator new (size_t size)
{
    Allocator <Type> alloc;
    printf ("Local new\n");
    return alloc.give_memory(size);
}

template <typename Type, template <typename> class Allocator>
dx_vector <Type, Allocator>::~dx_vector()
{
    Allocator<Type> alloc;
    alloc.release_memory (this->data);
}

#endif                  // #define DX_VECTOR