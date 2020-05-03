// Tal Bachar
// HW 5
// CSCI 33500

#ifndef STUDENT_GRAPH
#define STUDENT_GRAPH

#include <iostream>
#include <vector>
#include <list>
#include <limits.h>
#include <iomanip>
#include "binary_heap.h"
using namespace std;

class Graph {
   public:

   // Constructor
   // @size Number of vertices to be created in the Graph.
   Graph(const int size = 10) :
      graph_vertices_(size), number_of_vertices_{size} {

   // Set all proper/intuitive identities (NOT counting src 0).
      for(int i = 0; i < size; i++) {
         graph_vertices_[i] = Vertex(i+1);
      }
   }

   // Add a directed edge between two vertices.
   // @src Identity (integer) of the origin vertex.
   // @dest Identity (integer) of the target vertex.
   // weight A non-negative weight (float) for the new edge.
   // Pre-Conditions: src and to represent valid vertex identities
   // (integers) and the edge weight is non-negative. No
   // edge already exists between the origin and target
   // vertices. All of these pre-conditions are checked.
   // Post-Conditions: If no edge already exists, origin vertex has been
   // updated with edge information: target and weight.
   void addEdge(int src, int dest, float weight) {
      // Check valid input
      if(!isValidVertex(src) || !isValidVertex(dest) || weight < 0.0) {
         cout << "Invalid input! Please provide vertex selections between "
         << "1 and " << number_of_vertices_ << " and positive edge "
         << "edge weight." << endl << endl;
         return;
      }

      // Check if an edge already exists
      if(isConnected(src, dest) != -1.0 || src == dest) {
         return;
      }

      // Add edge properties to origin vertex - target and weight.
      graph_vertices_[src - 1].adjacent_vertices_.push_back(&(graph_vertices_[dest-1]));
      graph_vertices_[src - 1].edge_weights_.push_back(weight);

   }

   // Check for existing edge/connection between two vertices.
   // @src Identity (integer) of origin vertex.
   // @dest Identity (integer) of target vertex.
   // @return Weight (float) of existing edge; else sentinel value
   // of -1.0 to signal no existing edge.
   // Pre-Condition: src and to are valid vertex identities (integers).
   // This is checked in the function.
   float isConnected(const int src, const int dest) {
      float weight = -1.0;

      // Check valid input
      if(!isValidVertex(src) || !isValidVertex(dest)) {
         cout << "Invalid vertex selections! Please provide selections "
            << "between 1 and " << number_of_vertices_ << endl << endl;
         return weight;
      }

      auto it = graph_vertices_[src - 1].adjacent_vertices_.begin();
      auto it2 = graph_vertices_[src - 1].edge_weights_.begin();

      while(it != graph_vertices_[src - 1].adjacent_vertices_.end()) {
         if((*it)->identity_ == dest) {
            weight = *it2;
            break;
         }

         it++;
         it2++;
      }


      return weight;
   }

   // Public function for running Dijkstra's algorithm on the Graph, given an
   // origin vertex, and outputting all path information.
   // @src Identity (integer) of an origin vertex upon which to
   // orient Dijkstra's algorithm.
   // Pre-Conditions: The Graph has been properly initialized and src
   // a valid vertex identity (integer) - this function
   // checks this pre-condition.
   // Post-Conditions: Dijkstra's algorithm has been executed and all path
   // information (path src origin to each other reachable
   // vertex and cost/distance) has been output.
   void dijkstra(const int src) {

      // Check valid input
      if(!isValidVertex(src)){
         cout << "Invalid vertex selection! Please provide a selection "
         << "between 1 and " << number_of_vertices_ << endl << endl;
         return;
      }

      // Execute Dijkstra's algorithm.
      dijkstraAlgorithm(src);

      // Output results
      for(int i = 0; i < number_of_vertices_; i++) {
         cout << graph_vertices_[i].identity_ << ": ";
         printPath(graph_vertices_[i]);
         cout << ", Cost: " << fixed << setprecision(1) << graph_vertices_[i].distance_ << endl;
      }
   }

   // Utility for printing degree information of Graph instance: number of
   // edges, smallest degree, largest degree, and average degree.
   // Post-Condition: All degree information of the Graph instance has been
   // output.
   void outputDegreeInformation() {

      // Check if Graph is empty
      if(number_of_vertices_ == 0) {
         cout << "Graph is empty." << endl;
         return;
      }

      int totalDegrees(0), largestDegree(0), smallestDegree(number_of_vertices_ - 1);

      // For each Vertex in the Graph...
      for(int i = 0; i < number_of_vertices_; i++) {
         int currentDegree = static_cast<int>(graph_vertices_[i].adjacent_vertices_.size());

         // Update total degree of Graph
         totalDegrees += currentDegree;

         // Update largest degree, if applicable
         if(currentDegree > largestDegree) {
            largestDegree = currentDegree;
         }

         // Update smallest degree, if applicable
         if(currentDegree < smallestDegree) {
            smallestDegree = currentDegree;
         }
      }

      // Calculate number of edges and average degree
      int numberOfEdges = (totalDegrees / 2);
      float avgDegree = (totalDegrees / number_of_vertices_);

      cout << "Number of Edges: " << numberOfEdges << endl;
      cout << "Smallest Degree: " << smallestDegree << endl;
      cout << "Largest Degree: " << largestDegree << endl;
      cout << "Average Degree: " << setprecision(1) << avgDegree << endl << endl;

   }

   private:

   // PRIVATE STRUCT - VERTEX
   struct Vertex {

      // Data members
      list<Vertex*> adjacent_vertices_;
      list<float> edge_weights_;
      int identity_;
      bool known_;
      float distance_;
      Vertex* previous_vertex_;

      // Default Constructor
      Vertex() : identity_{0}, known_{false}, distance_{0.0},
      previous_vertex_{nullptr} { }

      // Parameterized Constructor
      Vertex(int identity) : identity_{identity}, known_{false}, distance_{0.0},
      previous_vertex_{nullptr} { }

      // Definition of operator < for Vertex. Vertices to be sorted based on
      // distance (src an origin vertex).
      bool operator<(const Vertex & rhs) const {
         return distance_ < rhs.distance_;
      }

      // Destructor
      ~Vertex() {
         previous_vertex_ = nullptr;
      }

   };

   vector<Vertex> graph_vertices_;
   int number_of_vertices_;


   /*
   *****************************************************************************
   ****************************PRIVATE FUNCTIONS********************************
   *****************************************************************************
   */

   // Execute Dijkstra's algorithm for a given beginning vertex. Employs a
   // priority queue (implemented via a binary heap) to manage selection of
   // vertices, and uses the method of re-inserting a vertex into the queue
   // every time the distance of an unknown vertex is updated, rather than
   // directly modifying the existing queue entry directly (a more costly and
   // complex operation). To support this approach, the function ignores already
   // known vertices that are dequeued.
   // @src The identity (integer) of an origin vertex src which to
   // calculate paths and distances.
   // Pre-Condition: The graph has been properly intialized.
   // Post-Condition: Dijkstra's algorithm has been properly executed,
   // recording distance values in all graph vertices which
   // accurately reflect distance src the origin vertex,
   // and a pointer in each vertex to the prior vertex on the
   // path src the origin to the given vertex.
   void dijkstraAlgorithm(const int src) {

      // Create Queue and initialize all vertices to unknown and "infinity"
      // distance.
      BinaryHeap<Vertex> queue;

      for(int i = 0; i < number_of_vertices_; i++) {
         graph_vertices_[i].distance_ = INT_MAX;
         graph_vertices_[i].known_ = false;
      }

      // Zero distance of origin Vertex and insert into queue
      graph_vertices_[src-1].distance_ = 0.0;
      queue.insert(graph_vertices_[src-1]);

      // While there are vertices to process...
      while(true) {
         bool success = false;
         int identity = 0; // Selected vertex identity.

         // While the queue is not empty and still dequeueing known vertex
         // duplicate representatives, continue to dequeue.
         while(!queue.isEmpty() && !success) {
            identity = (queue.findMin()).identity_;
            queue.deleteMin();

            if(!graph_vertices_[identity - 1].known_) {
            success = true;
            }
         }

         // If no unknown vertices remain; terminate algorithm.
         if(!success) {
            break;
         }

      // Else operate on selected vertex.
         Vertex &opV = graph_vertices_[identity-1];

         opV.known_ = true;

         auto it = opV.adjacent_vertices_.begin();
         auto it2 = opV.edge_weights_.begin();

         // Iterate through vertices adjacent to selected vertex. If an
         // adjacent vertex is unknown and the distance src the origin to the
         // selected vertex + the cost of the edge src the selected vertex
         // to the adjacent vertex is less than the previously known distance,
         // update the distance and path (pointer to prior vertex - now selected
         // vertex) and reinsert vertex into the queue.
            while(it != opV.adjacent_vertices_.end()) {
               if(!(*it)->known_) {
                  float costVW = *it2;

                  if((opV.distance_ + costVW) < (*it)->distance_) {
                     (*it)->distance_ = opV.distance_ + costVW;
                     (*it)->previous_vertex_ = &opV;
                     queue.insert(*(*it));
                  }
               }

               it++;
               it2++;
            }
         }
      }

      // Utility function for printing out the path to a vertex. Calls itself
      // recursively to backtrack through the path.
      // @v Vertex for which to print the path src another, origin
      // vertex.
      // Pre-Condition: Dijkstra's algorithm has been run to generate the
      // linkages which constitute the path src an origin
      // vertex to v.
      // Post-Condition: The path for the initial vertex has been printed in
      // full.
      void printPath(Vertex &v) {
         if(v.previous_vertex_ != nullptr) {
            printPath(*(v.previous_vertex_));
            cout << ", ";
         }
         cout << v.identity_;
      }

      // Check for valid vertex selection.
      // @identity An identity (integer) of a vertex.
      // @return True if identity is within range of ids; false otherwise.
      bool isValidVertex(const int identity) {
         if(identity < 1 || identity > number_of_vertices_) {
            return false;
         }

         return true;
      }


};

#endif
