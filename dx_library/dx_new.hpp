#ifndef DX_NEW
#define DX_NEW

#include <stdlib.h>
#include "dx_vector.hpp"

#define NEW_DX new (__FILE__, __LINE__, __PRETTY_FUNCTION__)



struct dx_new_info
{
    size_t* mem_ptr = NULL;
    static int call_num;
    size_t array_size = 0;
    
    char* file_name = NULL;
    char* func_name = NULL;
    int line_num = -1;

    void set_default (size_t size)
    {
        array_size = size;
        mem_ptr = reinterpret_cast <size_t*> (this);
        
        ++call_num;
    }

    void set_full (size_t size, const char* file, const char* func, int line)
    {
        array_size = size;
        file_name = const_cast <char*> (file);
        func_name = const_cast <char*> (func);
        line_num = line;
        mem_ptr = reinterpret_cast <size_t*> (this);
        
        ++call_num;
    }
};

int dx_new_info::call_num = 0;

struct dx_log
{
private:
    dx_vector <dx_new_info, Allocator_Calloc <dx_new_info>> log_file;

public:
    void push_back (dx_new_info new_call)
    {
        log_file.push_back (new_call);
    }

    void print (void)
    {
        printf ("Sorry! Not implemented yet!\n");
    }

} LOG;





void* operator new (size_t size, const char* file_name, int line, const char* func_name)
{
    printf ("Hallo! Ich bin in new in file \"%s\", in function \"%s\", in line %d. Size %lu\n", file_name, func_name, line, size);

    void* mem_ptr = calloc (1, size);
    dx_new_info* new_call = (dx_new_info*) calloc (1, sizeof (dx_new_info));
    new_call->set_full (size, file_name, func_name, line);
    LOG.push_back (*new_call);
    
    return mem_ptr;
}

// void* operator new (size_t size, void* new_ptr)
// {
//     printf ("I have ovrloaded placement new!\n");
//     return new_ptr;
// }

void* operator new (size_t size)
{
    printf ("Hey! I am in new. Size %lu\n", size);
        
    void* mem_ptr = malloc (size);
    dx_new_info* new_call = (dx_new_info*)calloc (1, sizeof (dx_new_info));
    new_call->set_default (size);
    LOG.push_back (*new_call);
    
    return mem_ptr;
}

void* operator new [](size_t size)
{
    printf ("Hey! I am in new - ARRAY. Size %lu\n", size);
    
    void* mem_ptr = malloc (size);
    dx_new_info* new_call = (dx_new_info*)calloc (1, sizeof (dx_new_info));
    new_call->set_default (size);

                        /* ALARM !!! */
    new_call->array_size = *(size_t*)((char*)mem_ptr - sizeof (size_t));
    
    LOG.push_back (*new_call);

    return mem_ptr;
}

void my_new_handler (void)
{
    printf ("Hello. I am new_handler. I'll try to fix your problem.\n");
}


void operator delete (void* ptr)
{
    printf ("I am in delete\n");
    free (ptr);
}


void operator delete[] (void* ptr)
{
    printf ("I am in delete - ARRAY\n");
    free (ptr);
}


#endif              // #define DX_NEW