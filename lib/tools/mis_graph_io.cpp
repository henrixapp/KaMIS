#include "mis_graph_io.h"
#include <fstream>
void mis_graph_io::writeIndependentSet(graph_access & G, std::string filename) {
        std::ofstream f(filename.c_str());
        std::cout << "writing independent set to " << filename << " ... " << std::endl;

        forall_nodes(G, node) {
                f << G.getPartitionIndex(node) <<  std::endl;
        } endfor

        f.close();
}