/**
 * Implement the Node and PriorityQueue classes
 */

#include "PriorityQueue.hpp"
#include <iostream>
using namespace std;

Node::Node()
{
}

Node::~Node()
{
}


PriorityQueue::PriorityQueue(void)
{
	int size = 100;
	data = new Node[size];
	heap_size = 0;
	arraySize = size;
}

PriorityQueue::PriorityQueue(int size)
{
	data = new Node[size];
	heap_size = 0;
	arraySize = size;
}

PriorityQueue::~PriorityQueue(void)
{
	delete data;
}

int PriorityQueue::leftChild(int i)
{
	return (2 * i + 1);
};

int PriorityQueue::rightChild(int i)
{
	return (2 * i + 2);
};

int PriorityQueue::parent(int i)
{
	return ((i - 1) / 2);
};



int PriorityQueue::getMin()
{
	return data[0].weight;
}


bool PriorityQueue::isEmpty()
{
	return heap_size ? 0 : 1;
}

void PriorityQueue::perk(int i)
{
	int p_i, tmpV, tmpW;
	if(i != 0)
	{
		p_i = parent(i);
		if(data[p_i].weight > data[i].weight)
		{
			tmpV = data[p_i].vertex;
			tmpW = data[p_i].weight;
			data[p_i].vertex = data[i].vertex;
			data[p_i].weight = data[i].weight;
			data[i].vertex = tmpV;
			data[i].weight = tmpW;
			perk(p_i);
		}
	}
}

void PriorityQueue::deperk(int i)
{
	int left = leftChild(i);
	int right = rightChild(i);
	int min = getMin();
	int tmpV;
	int tmpW;

	if(right >= heap_size)
	{
		if(left >= heap_size)
		{
			return;
		}
		else
		{
			min = left;
		}
	}
	else
	{
		if(data[left].weight <= data[right].weight)
		{
			min = left;
		}
		else
		{
			min = right;
		}
	}
	if(data[i].weight > data[min].weight)
	{
		tmpV = data[min].vertex;
		tmpW = data[min].weight;
		data[min].vertex = data[i].vertex;
		data[min].weight = data[i].weight;
		data[i].vertex = tmpV;
		data[i].weight = tmpW;
		deperk(min);
	}
}

void PriorityQueue::enqueue(int v, int w)
{
	if(heap_size == arraySize)
	{
		cout << "We've reached maximum capacity!@!";
	}
	else
	{
		data[heap_size].vertex = v;
		data[heap_size].weight = w;
		perk(heap_size);
		heap_size++;
	}
}

void PriorityQueue::dequeue()
{
	data[0].vertex = data[heap_size - 1].vertex;
	data[0].weight = data[heap_size - 1].weight;
	heap_size--;
	if(heap_size > 0)
	{
		deperk(0);
	}
}