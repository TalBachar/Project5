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

void queryGraph(const string query_filename, Graph & testGraph) {
   // Open query file
   ifstream queryInput = openFile(query_filename);

   int originVertex = 0;
   int targetVertex = 0;
   float edgeWeight = 0.0;
   string line;
   stringstream sLine;

   // For each edge query, check if edge exists between the origin and "target"
   // vertices.
   while(getline(queryInput, line)) {
      sLine.clear();
      sLine.str(line);

      sLine >> originVertex >> targetVertex;

      edgeWeight = testGraph.isConnected(originVertex, targetVertex);

      // if(originVertex == targetVertex) {
      //    cout << originVertex << " " << targetVertex
      //    << ": Same vertex - implicit weight of 0";
      // }
      if (edgeWeight != -1.0) {
         cout << originVertex << " " << targetVertex << ": connected " << edgeWeight;
      }
      else {
         cout << originVertex << " " << targetVertex << ": not_connected";
      }
      cout << endl;
   }

   //Close file
   queryInput.close();
}

int graphTestDriver(int argc, char **argv) {

   const string graph_filename(argv[1]);
   const string query_filename(argv[2]);

   Graph testGraph = createGraph(graph_filename);
   queryGraph(query_filename, testGraph);


    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
		cout << "Usage: " << argv[0] << " <GRAPH_File>" << "<ADJACENCY_QUERYFILE>" << endl;
		return 0;
    }

    graphTestDriver(argc, argv);

    return 0;
}
