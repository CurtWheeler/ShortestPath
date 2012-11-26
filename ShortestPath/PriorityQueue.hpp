#ifndef _PRIORITY_QUEUE_HPP
#define _PRIORITY_QUEUE_HPP

/**
 * Complete this Node class
 */
class Node
{
public:
	int vertex;
	int weight;
	Node(void);
	~Node(void);
};

/**
 * Complete this PriorityQueue class
 */
class PriorityQueue
{
private:
	Node *data;
	int arraySize;
	int heap_size;

	int leftChild(int);
	int rightChild(int);
	int parent(int);

	int getMin();
	
	void perk(int);
	void deperk(int);

public:
	PriorityQueue(void);
	PriorityQueue(int);
	~PriorityQueue(void);
	bool isEmpty();
	int count(){return heap_size;};
	void enqueue(int, int);
	void dequeue();

	void print_array();
};

#endif _PRIORITY_QUEUE_HPP