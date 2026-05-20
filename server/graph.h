#include<queue>
#include<vector>
#include<list>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#ifndef graph_h
#define graph_h


class Graph {
    public:
        Graph(string file_name);
        ~Graph();
        void make_graph_from_file(string filename);
        vector <int> BFS(int from , int to);
    private:
        vector <int> *vertex;
        vector< list<int>* > *adj;
        bool is_vertex(int data);
        int find_index(int data);
        bool is_neighbor(int source_index, int neighbor);
        void add_vertex(int source , int target);
};

#endif 