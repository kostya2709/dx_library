

#include "dx_machine_drawer.hpp"

const int MAX_STR_SIZE = 0;

int machine_drawer::picture_number = 0;


void machine_drawer::recursive_draw (FILE* f, Node* node, char* str_start, char* str_end)
{
    Node* father_index = node;
    for (auto iter = node->container.begin(); iter != node->container.end(); ++iter)
    {
        *str_end = (*iter).first;
        ++str_end;

        if ((*iter).second->terminal_num)
            fprintf (f, "\"node_%p\" [ fillcolor = \"gray\" label=\"%s\"];\n", (*iter).second, str_start);
        else
            fprintf (f, "\"node_%p\" [ fillcolor = \"lightblue\" label = \"\"];\n", (*iter).second);

        fprintf (f, "\"node_%p\"->\"node_%p\" [ label = \"%c\"];\n", father_index, (*iter).second, (*iter).first);
        

        recursive_draw (f, (*iter).second, str_start, str_end);
        *str_end = 0;
        --str_start;
    }
    
}


void machine_drawer::draw (const char* graph_name)
{
    char file_name [MAX_STR_SIZE] = {};

    char* string = (char*) calloc (1, MAX_STR_SIZE);
    char* write_to = string;

    if (NULL == graph_name)
        sprintf (file_name, "graph_%d", machine_drawer::picture_number);
    else
        sprintf (file_name, "%s", graph_name);
    
    char gv_file_name [MAX_STR_SIZE] = {};
    sprintf (gv_file_name, "%s.gv", file_name);

    FILE* f = fopen (gv_file_name, "w");

    fprintf (f, "digraph {");
    fprintf (f, "node [shape=\"circle\", fixedMAX_STR_SIZE=true, style=\"filled\", fillcolor=\"green\", width=0.3, fontMAX_STR_SIZE=5];\n\n");
    fprintf (f, "dpi=\"300\";\n\n");

    fprintf (f, "\"node_%p\" [ fillcolor = \"lightgray\", label = \"\"];\n", trie->root);
    recursive_draw (f, trie->root, string, write_to);

    fprintf (f, "}");
    fclose (f);

    char str1[38] = {};
    sprintf (str1, "dot -Tps %s -o %s.ps", gv_file_name, file_name);

    system (str1);

    ++ machine_drawer::picture_number;
}