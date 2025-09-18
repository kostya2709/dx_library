

#include "dx_trie.hpp"
#include "dx_machine_drawer.hpp"

const int MAX_STR_SIZE = 4096;


void empty_function (Node*) {}

void DFS_delete (Node* node)
{
    delete node;
}

dx_trie::dx_trie()
{
    root = new Node();
    drawer = new machine_drawer (this);
}

dx_trie::~dx_trie()
{
    DFS (empty_function, DFS_delete);
    delete drawer;
}


void dx_trie::write_strings (void)
{
    char* buffer = (char*) calloc (1, MAX_STR_SIZE);
    write_strings_cycle (root, buffer, buffer);
    free (buffer);
}

void dx_trie::write_strings_cycle (Node* node, char* buffer_read, char* buffer_write)
{
    for (int i = 0; i < node->terminal_num; ++i)
        puts (buffer_read);

    for (auto iter = node->container.begin(); iter != node->container.end(); ++iter)
    {
        *buffer_write = iter->first;
        write_strings_cycle (iter->second, buffer_read, buffer_write + 1);
    }


    *buffer_write = 0;
    --buffer_write;
}

void dx_trie::DFS (void (*function_in)(Node*), void (*function_out)(Node*))
{
    DFS_cycle (root, function_in, function_out);
}

void dx_trie::DFS_cycle (Node* node, void (*function_in)(Node*), void (*function_out)(Node*))
{
    function_in (node);

    for (auto iter = node->container.begin(); iter != node->container.end(); ++iter)
        DFS_cycle (iter->second, function_in, function_out);
        
    function_out (node);

    return;
}

void dx_trie::dump_cycle (Node* cur_node, int depth)
{
    for (auto iter = cur_node->container.begin(); iter != cur_node->container.end(); ++iter)
    {
        printf ("depth %d: %c\n", depth, (*iter).first);
        dump_cycle ((*iter).second, depth + 1);
    }

    if (cur_node->terminal_num)
        printf (" %d end\n\n", cur_node->terminal_num);
    else
        printf ("\n");
}

void dx_trie::dump (void)
{
    dump_cycle (root, 0);
}



void dx_trie::insert (const std::string& new_string)
{
    Node* cur_node = root;

    for (unsigned int i = 0; i < new_string.size(); ++i)
    {
        auto next_node = cur_node->container.find (new_string.at (i));
        
        if (next_node != cur_node->container.end())                                 // if there is an edge with this letter
        {
            cur_node = (*next_node).second;
        }
        else
        {
            Node* new_node = new Node (new_string.at (i), cur_node);
            cur_node->container.insert (std::make_pair (new_string.at (i), new_node));
            ++size;
            cur_node = new_node;
        }

        if (new_string.size() == i + 1)
            ++(cur_node->terminal_num);
    }
}

bool dx_trie::remove (const std::string& new_string)
{
    Node* cur_node = root;

    for (unsigned int i = 0; i < new_string.size(); ++i)
    {
        auto next_node = cur_node->container.find (new_string.at (i));
        
        if (next_node != cur_node->container.end())                                 // if there is an edge with this letter
        {
            cur_node = (*next_node).second;
        }
        else
        {
            return 0;
        }
    }

    --cur_node->terminal_num;

    while ((0 == cur_node->terminal_num) && (0 == cur_node->container.size()))
    {
        cur_node->parent->container.erase (cur_node->value);
        delete cur_node;
        cur_node = cur_node->parent;
    }

    return 1;
}

bool dx_trie::contains (const std::string& new_string)
{
    Node* cur_node = root;

    for (unsigned int i = 0; i < new_string.size(); ++i)
    {
        auto next_node = cur_node->container.find (new_string.at (i));
        
        if (next_node != cur_node->container.end())                                 // if there is an edge with this letter
        {
            cur_node = (*next_node).second;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

void dx_trie::draw (void)
{
    drawer->draw();
}