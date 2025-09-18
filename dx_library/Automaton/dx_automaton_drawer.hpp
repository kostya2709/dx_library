/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/



#ifndef AUTOMATON_DRAWER
#define AUTOMATON_DRAWER

#include <unordered_map>
#include <unordered_set>


#include <unistd.h>                                     ///< To execute
#include <wait.h>

enum 
{
    SIZE = 1024,
    MAX_SIZE = 1000001
};


/**
    * \brief This class is used as a template class for nodes, which can be used in dx_machine_drawer.
    * \version 1.1
    * \date 28, November, 2020
    * \warning The node class which you use in your automaton should be inherited from this class.
    * \example class suffix_node : public dx_abstract_node <suffix_node>
    * \note Main fields of a node class are already declared in dx_abstract_node. Check and use them.
**/
template <typename node_type>
struct dx_abstract_node
{
protected:

    int terminal_num = 0;
    node_type* suf_link = NULL;
    std::unordered_map <char, node_type*> transition;
 
public:
    dx_abstract_node (node_type* suf_link = NULL) : suf_link (suf_link) {}

    template <typename automaton_type, typename auto_node_type>
    friend class dx_machine_drawer;
};




 

/**
    * \brief This class allows you to draw your automaton.
    * \version 1.0
    * \date 28, November, 2020
    * \warning Nodes class and automaton class should have some properties (read below).
    * \example      suffix_automaton suff (str);                                        \
                    dx_machine_drawer <suffix_automaton, suffix_node> drawer (&suff);   \
                    drawer.draw();
    * 
    * This class receives your automaton class and node class, which is used in the automaton class.
    * This class (dx_machine_drawer) should be a friend of your class to have an access to some fields.
    * The node class you use should be inherited from the dx_abstract_node class (read above).
**/
template <typename automaton_type, typename node_type>
class dx_machine_drawer
{
private:
 
    automaton_type* automaton;                                                  ///< The pointer to the automaton to draw.
    static int picture_number;                                                  ///< Static counter of drawn pictures.
    std::unordered_set <dx_abstract_node <node_type>*> vertex_set;              ///< Set of visited vertexes.
 
    /**
     * \brief Recursive function which draws one node.
     * \param [in] FILE* of the file to print graphviz-style code.
     * \param [in] dx_abstract_node <node_type>* The pointer to the node to draw.
     **/
    void recursive_draw (FILE* f, dx_abstract_node <node_type>* node);
   
    /**
     * \brief Recursive function which adjusts suffix links to the nodes.
     * \param [in] FILE* of the file to print graphviz-style code.
     * \param [in] dx_abstract_node <node_type>* The pointer to the node to draw.
     **/
    void recursive_sufflinks (FILE* f, dx_abstract_node <node_type>* node);

    void execute (const char* gv_file_name, const char* file_name);
    
    static char str_path [SIZE];                                                ///< The static field to remember lines we came through.
    static char* write_to;                                                      ///< The pointer where to write sybols to.
 
public:

    /**
     * \brief Class constructor.
     * \param [in] automaton_type* pointer to the automaton to draw.
     **/
    explicit dx_machine_drawer (automaton_type* automaton) : automaton (automaton) {}
 
    /**
     * \brief The main function which draws the whole automaton.
     * \param [in] const char* pointer to the name to call the picture. By default, it will contain a serial number of the picture.
     **/
    void draw (const char* graph_name = NULL);
};
 
template <typename automaton_type, typename node_type>
char dx_machine_drawer <automaton_type, node_type>::str_path [SIZE];
 
template <typename automaton_type, typename node_type>
char* dx_machine_drawer <automaton_type, node_type>::write_to = str_path;
 
template <typename automaton_type, typename node_type>
int dx_machine_drawer <automaton_type, node_type>::picture_number = 0;
 
 
 
template <typename automaton_type, typename node_type>
void dx_machine_drawer <automaton_type, node_type>::recursive_sufflinks (FILE* f, dx_abstract_node <node_type>* node) 
{ 
    if (vertex_set.count (node))
        return;
    else 
        vertex_set.insert (node);
 
 
    if (node->suf_link)
        fprintf (f, "\"node_%p\"->\"node_%p\" [style = dashed, color = \"gray\"];\n", node, node->suf_link);
    
    for (auto iter = node->transition.begin(); iter != node->transition.end(); ++iter)
        recursive_sufflinks (f, (*iter).second);
}

 
 
template <typename automaton_type, typename node_type>
void dx_machine_drawer <automaton_type, node_type>::recursive_draw (FILE* f, dx_abstract_node <node_type>* node) 
{
    if (vertex_set.count (node))
        return;
    else 
        vertex_set.insert (node);
 
 
    dx_abstract_node <node_type>* father_index = node;
    for (auto iter = node->transition.begin(); iter != node->transition.end(); ++iter)
    {
        *write_to = (*iter).first;
        ++write_to;
 
        if ((*iter).second->terminal_num)
            fprintf (f, "\"node_%p\" [ shape = \"doublecircle\", fillcolor = \"gray\" label=\"%s\"];\n", (*iter).second, str_path);
        else
            fprintf (f, "\"node_%p\" [ fillcolor = \"lightblue\" label = \"\"];\n", (*iter).second);
 
        fprintf (f, "\"node_%p\"->\"node_%p\" [ label = \"%c\"];\n", father_index, (*iter).second, (*iter).first);
        
 
        recursive_draw (f, (*iter).second);
        *write_to = 0;
        --write_to;
    }
    
}
 
 
template <typename automaton_type, typename node_type>
void dx_machine_drawer <automaton_type, node_type>::draw (const char* graph_name) 
{
    char file_name [SIZE] = {};
    if (NULL == graph_name)
        sprintf (file_name, "graph_%d", dx_machine_drawer <automaton_type, node_type>::picture_number);
    else
        sprintf (file_name, "%s", graph_name);
    
    char gv_file_name [SIZE] = {};
    sprintf (gv_file_name, "%s.gv", file_name);
 
    FILE* f = fopen (gv_file_name, "w");
 
    fprintf (f, "digraph {");
    fprintf (f, "rankdir = \"LR\";");
    fprintf (f, "node [shape=\"circle\", fixedsize=true, style=\"filled\", fillcolor=\"green\", width=0.3, fontsize=5];\n\n");
    fprintf (f, "dpi=\"300\";\n\n");
 
    fprintf (f, "\"node_%p\" [ fillcolor = \"lightgray\", label = \"\"];\n", automaton->root);
    
    
    recursive_draw (f, automaton->root);
    vertex_set.clear();
    
    recursive_sufflinks (f, automaton->root);
    vertex_set.clear();
 
    fprintf (f, "}");
    fclose (f);
 
    execute (gv_file_name, file_name);
 
    ++dx_machine_drawer <automaton_type, node_type>::picture_number;
}


template <typename automaton_type, typename node_type>
void dx_machine_drawer <automaton_type, node_type>::execute (const char* gv_file_name, const char* file_name) 
{ 
    char full_name [MAX_SIZE] = {};
    sprintf (full_name, "%s.pdf", file_name);

    int status = 0;
    pid_t pid = fork();                     // compilation and execution
    if (-1 == pid)
        throw std::runtime_error ("Error! Can't create a new process!");

    if (0 == pid)
    {
        execlp ("dot", "dot", "-Tpdf", gv_file_name, "-o", full_name, NULL);
        throw std::runtime_error ("Error! Can't execute the program!");
    }
    else 
        waitpid(pid, &status, 0);


    pid = fork();
    if (-1 == pid)
        throw std::runtime_error ("Error! Can't create a new process!");

    if (0 == pid)
    {
        execlp ("rm", "rm", gv_file_name, NULL);
        throw std::runtime_error ("Error! Can't delete the file!");
    }
    else 
        waitpid(pid, &status, 0);
}

#endif                  // #define AUTOMATON_DRAWER