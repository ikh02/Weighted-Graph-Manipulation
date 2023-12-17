#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
using namespace std;

class Graph
{
	private:
		// adjacency list to store the graph
		unordered_map<string, vector<pair<string, int> >> adjList;

	// flag to determine if the graph is directed or not
	bool isDirected;

	public:
		// constructor to initialize the graph
		Graph(bool isDirected)
		{
			this->isDirected = isDirected;
		}

	// function to add a new node to the graph
	void addNode(string node)
	{
		adjList[node] = vector<pair<string, int>> ();
	}

	// function to remove a node from the graph
	void removeNode(string node)
	{
		// remove all edges to this node
		for (auto &adjNode: adjList)
		{
			vector<pair<string, int>> &edges = adjNode.second;

			for (auto it = edges.begin(); it != edges.end();)
			{
				if (it->first == node)
				{
					it = edges.erase(it);
				}
				else
				{ 		++it;
				}
			}
		}

		// remove the node from the adjacency list
		adjList.erase(node);
	}

	// function to add an edge to the graph
	void addEdge(string source, string destination, int weight)
	{
		adjList[source].push_back(make_pair(destination, weight));

		// if the graph is not directed, add an edge from destination to source
		if (!isDirected)
		{
			adjList[destination].push_back(make_pair(source, weight));
		}
	}

	// function to remove an edge from the graph
	void removeEdge(string source, string destination)
	{
		vector<pair<string, int>> &edges = adjList[source];

		for (auto it = edges.begin(); it != edges.end();)
		{
			if (it->first == destination)
			{
				it = edges.erase(it);
			}
			else
			{ 	++it;
			}
		}

		// if the graph is not directed, remove the edge from destination to source as well
		if (!isDirected)
		{
			edges = adjList[destination];

			for (auto it = edges.begin(); it != edges.end();)
			{
				if (it->first == source)
				{
					it = edges.erase(it);
				}
				else
				{ 		++it;
				}
			}
		}
	}

	// function to adjust the weight of an edge
	void adjustEdgeWeight(string source, string destination, int newWeight)
	{
		vector<pair<string, int>> &edges = adjList[source];

		for (auto &edge: edges)
		{
			if (edge.first == destination)
			{
				edge.second = newWeight;
			}
		}

		// if the graph is not directed, adjust the weight of the edge from destination to source as well
		if (!isDirected)
		{
			edges = adjList[destination];

			for (auto &edge: edges)
			{
				if (edge.first == source)
				{
					edge.second = newWeight;
				}
			}
		}
	}

	// function to perform DFS on the graph
	void DFS(string startNode)
	{
		// create a map to store the visited nodes
		unordered_map<string, bool> visited;

		// create a stack to store the nodes in the correct order
		stack<string> nodes;

		// perform DFS on the graph
		DFSextra(startNode, visited, nodes);

		// print the nodes in the correct order
		while (!nodes.empty())
		{
			cout << nodes.top() << " ";
			nodes.pop();
		}

		cout << endl;
	}

	// recursive helper function for DFS
	void DFSextra(string node, unordered_map<string, bool> &visited,
		stack<string> &nodes)
	{
		// mark the current node as visited
		visited[node] = true;

		// visit all the neighbors of the current node
		for (auto &adjNode: adjList[node])
		{
			if (!visited[adjNode.first])
			{
				DFSextra(adjNode.first, visited, nodes);
			}
		}

		// push the current node onto the stack
		nodes.push(node);
	}

	// function to check if the graph has a cycle
	bool hasCycle()
	{
		// create a map to store the visited nodes
		unordered_map<string, bool> visited;

		// create a map to store the recursion stack
		unordered_map<string, bool> recStack;

		// check if the graph has a cycle
		for (auto &adjNode: adjList)
		{
			if (hasCycleextra(adjNode.first, visited, recStack))
			{
				return true;
			}
		}

		return false;
	}

	// recursive helper function for checking cycles
	bool hasCycleextra(string node, unordered_map<string, bool> &visited,
		unordered_map<string, bool> &recStack)
	{
		// mark the current node as visited and on the recursion stack
		visited[node] = true;

		recStack[node] = true;

		// visit all the neighbors of the current node
		for (auto &adjNode: adjList[node])
		{
			if (!visited[adjNode.first] &&
				hasCycleextra(adjNode.first, visited, recStack))
			{
				return true;
			}
			else if (recStack[adjNode.first])
			{
				return true;
			}
		}

		// remove the current node from the recursion stack
		recStack[node] = false;

		return false;
	}

	// function to perform topological sort on the graph
	void topologicalSort()
	{
		// create a map to store the visited nodes
		unordered_map<string, bool> visited;

		// create a stack to store the nodes in the correct order
		stack<string> nodes;

		// perform topological sort on the graph
		for (auto &adjNode: adjList)
		{
			if (!visited[adjNode.first])
			{
				topologicalSortextra(adjNode.first, visited, nodes);
			}
		}

		// print the nodes in the correct order
		while (!nodes.empty())
		{
			cout << nodes.top() << " ";
			nodes.pop();
		}

		cout << endl;
	}

	// recursive helper function for topological sort
	void topologicalSortextra(string node, unordered_map< string,
		bool > &visited, stack<string> &nodes)
	{
		// mark the current node as visited
		visited[node] = true;

		// visit all the neighbors of the current node
		for (auto &adjNode: adjList[node])
		{
			if (!visited[adjNode.first])
			{
				topologicalSortextra(adjNode.first, visited, nodes);
			}
		}

		// push the current node onto the stack
		nodes.push(node);
	}

	// function to find the shortest path between two nodes
	void shortestPath(string startNode, string endNode)
	{
		// create a map to store the distances from the start node
		unordered_map<string, int> distances;
		// create a queue for BFS
		queue<string> q;
		// initialize all distances to infinity
		for (auto &adjNode: adjList)
		{
			distances[adjNode.first] = 999999;
		}

		// set the distance of the start node to 0
		distances[startNode] = 0;

		// enqueue the start node
		q.push(startNode);

		// perform BFS on the graph
		while (!q.empty())
		{
			string node = q.front();

			q.pop();

			// visit all the neighbors of the current node
			for (auto &adjNode: adjList[node])
			{
				if (distances[adjNode.first] > distances[node] + adjNode.second)
				{
					distances[adjNode.first] = distances[node] + adjNode.second;
					q.push(adjNode.first);
				}
			}
		}

		// print the distance from the start node to the end node
		cout << "Shortest distance from " << startNode << " to " << endNode <<
			": " << distances[endNode] << endl;
	}
};

int main()
{
	// prompt the user to decide if the graph should be directed or undirected
	cout <<
		"Should the graph be directed or undirected? (Enter 1 for directed or 0 for undirected)" <<
		endl;

	bool isDirected;
	cin >> isDirected;

	// create the graph
	Graph g(isDirected);

	// variables for user input
	int choice;
	string node1, node2;
	int weight;

	// continue looping until the user chooses to exit
	while (true)
	{
		// prompt the user for their choice
		cout << "What would you like to do?" << endl;
		cout << "1) Add a new node" << endl;
		cout << "2) Remove a node" << endl;
		cout << "3) Add an edge" << endl;
		cout << "4) Remove an edge" << endl;
		cout << "5) Adjust the weight of an edge" << endl;
		cout << "6) Perform DFS" << endl;
		cout << "7) Check if the graph has a cycle" << endl;
		cout << "8) Perform topological sort if the graph has no cycle" << endl;
		cout << "9) Find the shortest path between two nodes" << endl;
		cout << "0) Exit" << endl;
		cin >> choice;

		// perform the chosen action
		switch (choice)
		{
			case 1:

				// add a new node
				cout << "Enter the label for the new node:" << endl;
				cin >> node1;
				g.addNode(node1);
				break;

			case 2:

				// remove a node
				cout << "Enter the label for the node to be removed:" << endl;
				cin >> node1;
				g.removeNode(node1);
				break;

			case 3:

				// add an edge
				cout << "Enter the labels for the source and destination nodes:" <<endl;
				cin >> node1 >> node2;
				cout << "Enter the weight of the edge:" << endl;
				cin >> weight;
				g.addEdge(node1, node2, weight);
				break;

			case 4:

				// remove an edge
				cout << "Enter the labels for the source and destination nodes:" <<
					endl;
				cin >> node1 >> node2;
				g.removeEdge(node1, node2);
				break;

			case 5:

				// adjust the weight of an edge
				cout << "Enter the labels for the source and destination nodes:" <<
					endl;
				cin >> node1 >> node2;
				cout << "Enter the new weight of the edge:" << endl;
				cin >> weight;
				g.adjustEdgeWeight(node1, node2, weight);
				break;

			case 6:

				// perform DFS
				cout << "Enter the label for the start node:" << endl;
				cin >> node1;
				cout << "DFS order: ";
				g.DFS(node1);
				break;

			case 7:

				// check if the graph has a cycle
				if (g.hasCycle())
				{
					cout << "The graph has a cycle." << endl;
				}
				else
				{
					cout << "The graph does not have a cycle." << endl;
				}
				break;

			case 8:
				// perform topological sort if the graph has no cycle
				if (g.hasCycle())
				{
					cout <<
						"Cannot perform topological sort on a graph with a cycle." <<
						endl;
				}
				else{
					cout << "Topological sort order: ";
					g.topologicalSort();
				}
				break;

			case 9:
				// find the shortest path between two nodes
				cout << "Enter the labels for the start and end nodes:" << endl;
				cin >> node1 >> node2;
				g.shortestPath(node1, node2);
				break;
			case 0:
				// exit the program
				return 0;
				cout << "Invalid choice. Please try again." << endl;
				break;
		}
	}
	return 0;
}