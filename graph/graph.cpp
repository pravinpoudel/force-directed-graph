#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

//this initialization will not throw error 
struct NodeLayout {
	int index = 0;
	struct Position {
		float x = rand()%1000/100;
		float y = rand()%1000/100;
	}position;

	struct Displacement {
		float x = 0.0;
		float y = 0.0;
	}displacement;
};

struct EdgeLayout {
	NodeLayout& node1;
	NodeLayout& node2;
	float weight;
};

//size_t is used as data type for unsigned type or non negative number
//size_t could be used for unsigned char, unsigned short, unsigned int, unsigned long or
//unsigned long long

struct GraphLayout {

	vector<NodeLayout> nodeList;
	vector<EdgeLayout> edgeList;

	void addNode(size_t node_count);
	void addEdge(size_t v0, size_t v1, float weight);
	void start(size_t iteration);

};

void GraphLayout::addNode(size_t node_count) {
	for (int i = 0; i < node_count; i++) {
		NodeLayout node;
		node.index = i;
		printf("x %f and y %f\n", node.position.x, node.position.y);
		nodeList.push_back(node);
	}
}

void GraphLayout::addEdge(size_t v0, size_t v1, float weight) {
	if (v0 == v1 || weight == 0.0f|| (nodeList.size()<max(v0, v1))) {
		return;
	}
	//please check why other initialization method not working; learn struct more in c++
	//EdgeLayout NewEdge;

	struct EdgeLayout NewEdge = { nodeList[v0], nodeList[v1], weight };
	edgeList.push_back(NewEdge);
}

void GraphLayout::start(size_t max_iteration_count) {
	size_t nodeCount = nodeList.size();

	int WIDTH = 600;
	int HEIGHT = 400;
	float area = WIDTH * HEIGHT;
	float temperature = WIDTH / 10.0f;
	//optimal edge/link length 
	float k = sqrt(area / nodeCount);
	k = 10.0;
	float kSquare = area / nodeCount;
	kSquare = 100.0;
	//i am not checking epsilon now
	//but will put once i know that is the value range
	//it's not good to only check the iteration count

	int iterationCount = 0;
	while(iterationCount<max_iteration_count)
		{
		temperature *= (1.0 -((iterationCount*1.0) / max_iteration_count));
		iterationCount++;
		//repulsive force effect
		for (auto iterator = nodeList.begin(); iterator != nodeList.end(); iterator++) {
			iterator->displacement = { 0.0f, 0.0f };
			for (auto iterator2 = nodeList.begin(); iterator2 != nodeList.end(); iterator2++) {
				if (iterator != iterator2) {
					float dx = iterator2->position.x - iterator->position.x;
					float dy = iterator2->position.y - iterator->position.y;
					float d = dx * dx + dy * dy;
					float fr = (kSquare / sqrt(d));
					cout << fr << endl;
					float cofficientx = dx / sqrt(d);
					float cofficienty = dy / sqrt(d);
					iterator2->displacement.x += fr * cofficientx;
					iterator2->displacement.y += fr * cofficienty;
				}
			}
		}

		//calculate the attractive force
		for (auto iterator = edgeList.begin(); iterator != edgeList.end(); iterator++) {
			float dx = iterator->node1.position.x - iterator->node2.position.x;
			float dy = iterator->node1.position.y - iterator->node2.position.y;
			float dSquare = dx * dx + dy * dy;
			float d = sqrt(dSquare);
			float fa = (dSquare / k);
			iterator->node1.displacement.x -= (dx / d) * fa;
			iterator->node1.displacement.y -= (dy / d) * fa;
			iterator->node2.displacement.x += (dx / d) * fa;
			iterator->node2.displacement.y += (dy / d) * fa;
		}
		
		//limit displacement to the temperature
		for (auto iterator = nodeList.begin(); iterator != nodeList.end(); iterator++) {
			float d = sqrt(iterator->displacement.x * iterator->displacement.x + iterator->displacement.y * iterator->displacement.y);
			iterator->position.x += ((iterator->displacement.x) / d) * min(d, temperature);
			iterator->position.y += (iterator->displacement.y / d) * min(d, temperature);
		}

	}
	printf("%d\n",iterationCount);
}

int main( int argc, char ** argv) {

	GraphLayout graph;
	graph.addNode(5);
	graph.addEdge(0, 1, 1.0);
	graph.addEdge(0, 2, 1.0);
	graph.addEdge(1, 3, 1.0);
	graph.addEdge(2, 3, 1.0);
	graph.addEdge(3, 4, 1.0);
	graph.addEdge(1, 4, 1.0);
	graph.start(10000);
	for (auto iterator = graph.nodeList.begin(); iterator != graph.nodeList.end(); iterator++) {
		printf(" node %d  coordinate is ( %f, %f)\n", iterator->index, iterator->position.x, iterator->position.y);
	}
}
