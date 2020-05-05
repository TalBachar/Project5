// Tal Bachar
// HW 5
// CSCI 33500

#include <iostream>
#include <fstream>
#include "graph.h"
#include <string>
#include <sstream>
using namespace std;


// function to open file. A friend recommended doing this in a different
// function instead of inside the code. Must say it looks better and cleaner.
ifstream openFile(const string filename) {
   ifstream graph_file;

   graph_file.open(filename);

   if(graph_file.fail()) {
      cout << "Error: Unable to open file!";
      exit(1);
   }
   return graph_file;
}

// a function that reads data from file and creates graph
Graph create_graph(const string graph_filename) {
   ifstream input_graph = openFile(graph_filename);

   int size = 0;
   int vertex_source = 0;
   int vertex_destination = 0;
   float edgeWeight = 0.0;

   string line;
   stringstream ss;

   getline(input_graph, line);
   ss.str(line);
   ss >> size;
   Graph create_graph(size);


   while(getline(input_graph, line)) {
      ss.clear();
      ss.str(line);

      ss >> vertex_source;

      while(ss >> vertex_destination) {
         ss >> edgeWeight;

         create_graph.addEdge(vertex_source, vertex_destination, edgeWeight);
      }
   }

   input_graph.close();

   return create_graph;
}

//
void queryGraph(const string query_filename, Graph & testGraph) {

   ifstream queryInput = openFile(query_filename);

   int vertex_source = 0;
   int vertex_destination = 0;
   float edgeWeight = 0.0;
   string line;
   stringstream ss;

   // For each edge query, check if edge exists between the origin and "target"
   // vertices.
   while(getline(queryInput, line)) {
      ss.clear();
      ss.str(line);

      ss >> vertex_source >> vertex_destination;

      edgeWeight = testGraph.isConnected(vertex_source, vertex_destination);

      if (edgeWeight != -1.0) {  // if weight isn't sentinel, print connected and weight
         cout << vertex_source << " " << vertex_destination << ": connected " << edgeWeight;
      }
      else {   //otherwise, print not_connected
         cout << vertex_source << " " << vertex_destination << ": not_connected";
      }
      cout << endl;
   }

   //Close file
   queryInput.close();
}

int graphTestDriver(int argc, char **argv) {

   const string graph_filename(argv[1]);
   const string query_filename(argv[2]);

   Graph testGraph = create_graph(graph_filename); //create the Graph
   queryGraph(query_filename, testGraph); // query the graph


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
