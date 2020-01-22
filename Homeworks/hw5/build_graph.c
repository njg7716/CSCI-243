// Author: Nicholas Graca (njg7716)
// Homework 5, 10/17/17

#include "simple_graph.h"

int main(){
	ObjectList * graph = ol_create();
	printf("Reading graph data...\n");
	grh_load_file(graph, stdin);
	printf("Finished reading graph data.\n");
	grh_print_graph(graph);
	grh_delete_graph(graph);
	return 0;
}
