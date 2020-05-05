// Tal Bachar
// HW 5
// CSCI 33500

#ifndef STUDENT_GRAPH
#define STUDENT_GRAPH

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "binary_heap.h"
using namespace std;

class Graph {
   public:

   // Constructor
   Graph(const int size = 10) :
      graph_vertices_(size), number_of_vertices_{size} {
         for(int i = 0; i < size; i++) {
            graph_vertices_[i] = Vertex(i+1);
         }
   }

   // add an edge between vetrices
   void new_edge(int src, int dest, float weight) {


      // Add edge properties to origin vertex - target and weight.
      graph_vertices_[src - 1].adjacent_vertices_.push_back(&(graph_vertices_[dest-1]));
      graph_vertices_[src - 1].edge_weights_.push_back(weight);

   }


   //check if two vertices are connected
   //return weight if yes, return -1 if no.
   float isConnected(const int src, const int dest) {
      float weight = -1.0;


      auto first = graph_vertices_[src - 1].adjacent_vertices_.begin();
      auto second = graph_vertices_[src - 1].edge_weights_.begin();

      while(first != graph_vertices_[src - 1].adjacent_vertices_.end()) {
         if((*first)->identity_ == dest) {
            weight = *second;
            break;
         }

         first++;
         second++;
      }


      return weight;
   }


   //public dijkstra algorithm. given a vertex, diksta will return cost
   //if not possible, return not_possible.
   void dijkstra(const int src) {

      // Check valid input
      if(!isValidVertex(src)){
         cout << "Error! no such vertex!";
         return;
      }

      // Execute Dijkstra's algorithm.
      dijkstra_algo(src);

      // Output results
      for(int i = 0; i < number_of_vertices_; i++) {
         if (graph_vertices_[i].distance_ == 2147483648.0) {
            cout << graph_vertices_[i].identity_ << ": ";
            cout << "not_possible" << endl;
         }
         else {
            cout << graph_vertices_[i].identity_ << ": ";
            printPath(graph_vertices_[i]);
               cout << " cost: " << fixed << setprecision(1) << graph_vertices_[i].distance_ << endl;
         }
      }
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
   void dijkstra_algo(const int src) {

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
            cout << " ";
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
