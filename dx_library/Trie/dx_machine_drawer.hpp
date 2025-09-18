
#ifndef MACHINE_DRAWER
#define MACHINE_DRAWER

#include "dx_trie.hpp"



class machine_drawer
{
private:

    dx_trie* trie;
    static int picture_number;
    void recursive_draw (FILE* f, Node* node, char* str_start, char* str_end);
    
    char* string;
    char* write_to;

public:
    explicit machine_drawer (dx_trie* trie) : trie (trie) {}

    void draw (const char* graph_name = NULL);

};

#endif                                                                              // #define MACHINE_DRAWER