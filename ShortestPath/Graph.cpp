#include "Graph.hpp"

//#define infin 2071690107
#define infin 999

// The ctor should read, parse, and build the graph from the given fileName
Graph::Graph(std::string fileName)
{
	// complete me
	G = new int[SIZE][SIZE];
	P = new int[SIZE][COL];

	fillG();
	

	file_to_graph(fileName);

	//printG();
	//printV();

	//MyshortestPath(1);
	//printP();


}

// Clean up your memory
Graph::~Graph(void)
{
	// complete me
	delete G;
	delete P;
}

// Return the shortest path for the given index.
// There are 5 paths to compute at the end of each file.
// When index == 1, return the first, index == 2, return the second, etc.
int Graph::shortestPath(int index)
{
	fillP();

	//get start & ends
	int start = startV[index - 1];
	int end = endV[index - 1];

	P[start][1] = 0;
	P[start][2] = 1;

	return build_path(start, end);;
}

void Graph::file_to_graph(string fileName)
{
	string l;
	ifstream f (fileName, ios::in);
	int v1, v2, w;
	bool isGraph = true;
	if (f.is_open())
	{
		while (f.good())
		{
			while (isGraph)
			{
				getline (f,l);
				stringstream ss(l);
				ss >> v1 >> v2 >> w;
				cout << "v1=" << v1 << " v2=" << v2 << " w=" << w << endl;
				G[(int)v1][(int)v2] = w;
				if(G[(int)v2][(int)v1] == infin)
				{
					G[(int)v2][(int)v1] = w;
				}
				if(l.empty())
				{
					isGraph = false;
				}
			}
			while(!isGraph && f.good())
			{
				getline (f,l);
				stringstream ss(l);
				ss >> v1 >> v2;
				startV.push_back(v1);
				endV.push_back(v2);
			}
			startV.pop_back();
			endV.pop_back();
		}
		f.close();
	}
	else cout << "Unable to open file"; 
};

void Graph::fillG()
{
	for(int y = 0; y < SIZE;y++)
	{
		for(int x = 0; x < SIZE;x++)
		{
			G[y][x] = infin;
		}
		cout << endl;
	}
}

void Graph::fillP()
{
	for(int y = 0; y < SIZE; y++)
	{
		for(int x = 0; x < COL; x++)
		{
			P[y][x] = infin;
		}
		cout << endl;
	}
}

//int Graph::MyshortestPath(int index)
//{
//	//get start & ends
//	int start = startV[index - 1];
//	int end = endV[index - 1];
//
//	P[start][1] = 0;
//	P[start][2] = 1;
//
//	//cout << "THE SHORTEST PATH " << build_path(start, end) << endl;
//	return build_path(start, end);
//}


int Graph::build_path(int start, int end)
{
	int curr_vert = start;
	int dist = 0;
	int temp_dist;

	while (P[end][2] == infin)
	{
		for(int x = 0; x < SIZE;x++)
		{
			if(G[curr_vert][x] != infin)
			{
				//cout << "v=" << x << " w=" << G[curr_vert][x] << "\t";

				temp_dist = dist;
				temp_dist += G[curr_vert][x];

				if(P[x][0] > temp_dist && P[x][2] == infin)
				{
					P[x][0] = temp_dist;
				}
			}
		}

		cout << endl;

		int shortestDistIndex = 0;

		for(int y = 1; y < SIZE; y++)
		{
			if(P[y][0] <= P[shortestDistIndex][0] && P[y][2] == infin)
			{
				shortestDistIndex = y;
			}
		}

		//set permanent
		P[shortestDistIndex][1] = P[shortestDistIndex][0];
		P[shortestDistIndex][2] = 0;
		
		//printP();

		curr_vert = shortestDistIndex;
		dist = temp_dist;
	}

	return P[end][1];
};


void Graph::printG()
{
	for(int y = 0; y < SIZE;y++)
	{
		for(int x = 0; x < SIZE;x++)
		{
			cout << G[y][x] << "\t";
		}
		cout << endl;
	}
}

void Graph::printV()
{
	for(int i = 0; i < startV.size();i++)
	{
		cout << startV[i] << "->" << endV[i] << endl;
	}
}

void Graph::printP()
{
	for(int y = 0; y < SIZE; y++)
	{
		cout << "v" << y << " -- ";
		for(int x = 0; x < COL; x++)
		{
			cout << P[y][x] << " ";
		}
		cout << endl;
	}
}