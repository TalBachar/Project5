#include <iostream>
#include <fstream>
#include "graph.h"
#include <string>
#include <sstream>
using namespace std;

ifstream openFile(const string filename) {
   ifstream input;

   // Open input file
   input.open(filename);

   // Check for failure to open input file
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
   int originVertex = 0;
   int targetVertex = 0;
   float edgeWeight = 0.0;

   string line;
   stringstream sLine;

   // Read in size and initialize Graph
   getline(graphInput, line);
   sLine.str(line);
   sLine >> size;
   Graph newGraph(size);

   // Read in directed edges and construct Graph
   while(getline(graphInput, line)) {
      sLine.clear();
      sLine.str(line);

      sLine >> originVertex;

      while(sLine >> targetVertex) {
         sLine >> edgeWeight;

         newGraph.addEdge(originVertex, targetVertex, edgeWeight);
      }
   }

   //Close file
   graphInput.close();

   return newGraph;
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
