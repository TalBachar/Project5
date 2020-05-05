// Tal Bachar
// HW 5
// CSCI 33500

#include <iostream>
#include <fstream>
#include "graph.h"
#include <string>
#include <sstream>
using namespace std;

ifstream openFile(const string filename) {
   ifstream input;

   input.open(filename);

   if(input.fail()) {
      cout << "Error: Unable to open file!";
      exit(1);
   }

   return input;
}

Graph createGraph(const string graph_filename) {
   // Open graph input file
   ifstream graphInput = openFile(graph_filename);

   int size = 0;
   int vertex_source = 0;
   int vertex_destination = 0;
   float edgeWeight = 0.0;

   string line;
   stringstream ss;

   // Read in size and initialize Graph
   getline(graphInput, line);
   ss.str(line);
   ss >> size;
   Graph graph(size);

   // Read in directed edges and construct Graph
   while(getline(graphInput, line)) {
      ss.clear();
      ss.str(line);

      ss >> vertex_source;

      while(ss >> vertex_destination) {
         ss >> edgeWeight;

         graph.new_edge(vertex_source, vertex_destination, edgeWeight);
      }
   }

   //Close file
   graphInput.close();

   return graph;
}

int pathfindDriver(int argc, char **argv) {

   const string graph_filename(argv[1]);
   const string starting_vertex(argv[2]);
   int check_this_vertex = stoi(starting_vertex);

   Graph testGraph = createGraph(graph_filename);
   testGraph.dijkstra(check_this_vertex);

       return 0;
   }

int main(int argc, char **argv) {
    if (argc != 3) {
		cout << "Usage: " << argv[0] << " <GRAPH_FILE>" << "<STARTING_VERTEX>" << endl;
		return 0;
    }

    pathfindDriver(argc, argv);

    return 0;
}
