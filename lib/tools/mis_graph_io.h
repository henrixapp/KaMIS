#pragma once
#include "definitions.h"
#include "data_structure/graph_access.h"
class mis_graph_io {
    public:
    static void writeIndependentSet(graph_access & G, std::string filename);
};