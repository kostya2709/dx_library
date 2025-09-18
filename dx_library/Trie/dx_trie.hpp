
#ifndef DX_TRIE
#define DX_TRIE

#include <unordered_map>


class machine_drawer;



struct Node
{
    int value = 0;
    int terminal_num = 0;
    Node* parent = NULL;
    Node* suff_link = NULL;
    std::unordered_map <int, Node*> container;

    Node (int value = 0, Node* parent = NULL) : value (value), parent (parent) {}
};

namespace std
{
    template <>
    struct hash <Node>
    {
        size_t operator () (const Node& node)
        {
            return node.value;
        }
    };
}


void empty_function (Node*);

class dx_trie
{
    Node* root;
    int size = 1;

public:

    dx_trie();
    ~dx_trie();

    void insert   (const std::string& new_string);
    bool remove   (const std::string& expired_string);
    bool contains (const std::string& string);
    void DFS      (void (*function_in)(Node*) = empty_function, void (*function_out)(Node*) = empty_function);
    void write_strings (void);

    void dump (void);
    void draw (void);
private:

    void dump_cycle (Node* cur_node, int depth);
    void DFS_cycle (Node* node, void (*function_in)(Node*), void (*function_out)(Node*));
    void write_strings_cycle (Node* node, char* buffer_read, char* buffer_write);

    friend class machine_drawer;

    machine_drawer* drawer = NULL;

};


#endif                                                                              // #define  DX_TRIE