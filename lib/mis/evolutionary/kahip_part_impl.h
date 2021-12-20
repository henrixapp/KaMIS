#pragma once
#include <fstream>
#include "lib/partition/uncoarsening/separator/vertex_separator_algorithm.h"
#include "app/balance_configuration.h"
#include "app/configuration.h"
#include "lib/partition/graph_partitioner.h"
void internal_build_graph( PartitionConfig & partition_config, 
                           int* n, 
                           int* vwgt, 
                           int* xadj, 
                           int* adjcwgt, 
                           int* adjncy,
                           graph_access & G) {
        G.build_from_metis(*n, xadj, adjncy); 
        G.set_partition_count(partition_config.k); 
 
        srand(partition_config.seed);
        random_functions::setSeed(partition_config.seed);
       
        if(vwgt != NULL) {
                forall_nodes(G, node) {
                        G.setNodeWeight(node, vwgt[node]);
                } endfor
        }

        if(adjcwgt != NULL) {
                forall_edges(G, e) {
                        G.setEdgeWeight(e, adjcwgt[e]);
                } endfor 
        }

        balance_configuration bc;
        bc.configurate_balance( partition_config, G);
}
void internal_nodeseparator_call(PartitionConfig & partition_config, 
                          bool suppress_output, 
                          int* n, 
                          int* vwgt, 
                          int* xadj, 
                          int* adjcwgt, 
                          int* adjncy, 
                          int* nparts, 
                          double* imbalance, 
                          int* num_nodeseparator_vertices, 
                          int** separator,
                          int* part) {

        using namespace std;
        //first perform std partitioning using KaFFPa
        streambuf* backup = cout.rdbuf();
        ofstream ofs;
        ofs.open("/dev/null");
        if(suppress_output) {
               cout.rdbuf(ofs.rdbuf()); 
        }

        partition_config.k         = *nparts;
        partition_config.imbalance = 100*(*imbalance);
        graph_access G;     
        internal_build_graph( partition_config, n, vwgt, xadj, adjcwgt, adjncy, G);

        
        graph_partitioner partitioner;
        partitioner.perform_partitioning(partition_config, G);

        forall_nodes(G, node) {
                part[node] = G.getPartitionIndex(node);
        } endfor

        // now compute a node separator from the partition of the graph
        complete_boundary boundary(&G);
        boundary.build();

        vertex_separator_algorithm vsa;
        std::vector<NodeID> internal_separator;
        vsa.compute_vertex_separator(partition_config, G, boundary, internal_separator);

        // copy to output variables
        *num_nodeseparator_vertices =  internal_separator.size();
        *separator = new int[*num_nodeseparator_vertices];
        for( unsigned int i = 0; i < internal_separator.size(); i++) {
                (*separator)[i] = internal_separator[i];
        }

        ofs.close();
        cout.rdbuf(backup);
}


void node_separator(int* n, 
                          int* vwgt, 
                          int* xadj, 
                          int* adjcwgt, 
                          int* adjncy, 
                          int* nparts, 
                          double* imbalance, 
                          bool suppress_output, 
                          int seed,
                          int mode,
                          int* num_separator_vertices, 
                          int** separator,
                          int* part) {
        configuration cfg;
        PartitionConfig partition_config;
        partition_config.k = *nparts;

        switch( mode ) {
                case FAST: 
                        cfg.fast(partition_config);
                        break;
                case ECO: 
                        cfg.eco(partition_config);
                        break;
                case STRONG: 
                        cfg.strong(partition_config);
                        break;
                case FASTSOCIAL: 
                        cfg.fastsocial(partition_config);
                        break;
                case ECOSOCIAL: 
                        cfg.ecosocial(partition_config);
                        break;
                case STRONGSOCIAL: 
                        cfg.strongsocial(partition_config);
                        break;
                default: 
                        cfg.eco(partition_config);
                        break;
        }
        partition_config.seed = seed;

        internal_nodeseparator_call(partition_config, suppress_output, n, vwgt, xadj, adjcwgt, adjncy, nparts, imbalance, num_separator_vertices, separator, part);
}