// Author: Nicholas Graca
// 10/19/17 Homework 5 simple_graph.c

#include <stdlib.h>
#include "simple_graph.h"
#include <string.h>

// Creates a new node with a certain name and
// gives it a blank list of neighbors
GraphNode * grh_create_node( char * name){
	size_t size = strlen(name)+1;
	GraphNode * node = malloc(200);
	strncpy(node->name, name, size);
	ObjectList * neighbors = ol_create();
	node->neighbors = neighbors;
	return node;
} 

//Deletes adn frees the neighbors by using the ol_destroy function
void grh_delete_node(GraphNode * node){
	ol_destroy(node->neighbors);
}

//Deletes and frees the graph by calling delete node on its neighbors
void grh_delete_graph( ObjectList * graph ){
	Iter * i = ol_iterator(graph);
	GraphNode * node;
	while( ol_has_next(i)){
		node = ol_next(i);
		grh_delete_node(node);
		free(node);
	}
	free(graph);
	free(i);
}

// Finds if a name of a city is in the graph and if it is then it
// returns the pointer to the node
GraphNode * grh_find_node_by_name( ObjectList * graph, char * name){
	Iter * i = ol_iterator(graph);
	while(ol_has_next(i)){
		GraphNode * node = ol_next(i);
		if(!strcmp(node->name, name)){
			free(i);
			return node;
		}
	}
	free(i);
	return NULL;
}

//Prints the graph the way it was specified in the write up
void grh_print_graph(ObjectList * graph){
	Iter * i = ol_iterator(graph);
	if(ol_get_size(graph) != 0){
		printf("Graph: {name: neighbor names list}\n");
		while(ol_has_next(i)){
			GraphNode * node = ol_next(i);
			printf("%s: ", node->name);
			Iter * secIter = ol_iterator(node->neighbors);
			while(ol_has_next(secIter)){
				node = ol_next(secIter);
				if(!ol_has_next(secIter))
					printf("%s", node->name);
				else
					printf("%s, ", node->name);
			}
		
			printf("\n");
			free(secIter);
		}
	}
	free(i);
}

// Reads the file and adds all the info to the graph
// Also checks to make sure it doesnt add a city more than once
void grh_load_file( ObjectList * graph, FILE * input){
	char limit[] = "\n,";
	char * name;
	char * token;
	char line[2000];
	GraphNode * node;
	while(fgets(line, 2000, input)){
		if(strcmp(line, "\n")!=0){	
			name = strtok(line, limit);
			if(name == NULL)
				return;
			if(grh_find_node_by_name(graph, name) == NULL){
				node = grh_create_node(name);
				ol_insert(graph, node);
			}
			else
				node = grh_find_node_by_name(graph, name);
			token = strtok(NULL, limit);
			while(token != NULL){
				GraphNode * neighbor;
				if(grh_find_node_by_name(graph, token) == NULL){
					neighbor = grh_create_node(token);
					ol_insert(graph, neighbor);
				}
				else
					neighbor = grh_find_node_by_name(graph, token);
				if(grh_find_node_by_name(node->neighbors, neighbor->name) == NULL){
					ol_insert(node->neighbors, neighbor);
				}
				if(grh_find_node_by_name(neighbor->neighbors, node->name) == NULL)
					ol_insert(neighbor->neighbors, node);
				token = strtok(NULL, limit);
			}
		}
	}
}
