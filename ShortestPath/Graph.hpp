#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Graph
{
  public:
    Graph(string fileName);
    ~Graph(void);

    int shortestPath(int index);
	int MyshortestPath(int index);
	void printG();
	void printV();
	void printP();

  private:
	  static int const SIZE = 100;
	  static int const COL = 3;
	  int (*G)[SIZE];
	  int (*P)[COL];
	  vector<int> startV;
	  vector<int> endV;
	  vector<int> posV;
	  vector<int> pathV;
	  vector<int> pathW;

	  void fillG();
	  void fillP();
	  void file_to_graph(string);
	  int build_path(int, int, int, int);
};

#endif // _GRAPH_HPP
