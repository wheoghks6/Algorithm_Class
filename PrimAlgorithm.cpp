#include<iostream>
#include<cstring>
#include<vector>
using namespace std;

#define Unseen 0
#define in_Tree 1
#define Fringe 2

vector<int> result;
bool inTree[10001];
int result_count = 0;
int weight_sum = 0;

class Node {
public:
	int value;
	int weight;
	int state;

	Node()
	{
		value = 0;
		weight = 0;
		state = Unseen;
	}
};


class Graph {
public:
	vector<Node*>* graph_list;
	int size;
	Graph(int size)
	{
		this->size = size + 1;
		graph_list = new vector<Node*>[this->size];
	}

	void insert(int v1, int v2, int weight)
	{
		Node* node1 = new Node();
		Node* node2 = new Node();

		node1->value = v2;
		node1->weight = weight;

		node2->value = v1;
		node2->weight = weight;


		graph_list[v1].push_back(node1);
		graph_list[v2].push_back(node2);

	}
};

class priority_queue {
public:
	Node* queue[1000000];
	int root_idx;
	int heap_idx;
	priority_queue()
	{
		this->root_idx = 1;
		this->heap_idx = 0;
	}

	void push(Node* node1)
	{

		queue[++heap_idx] = node1;
		upheap(heap_idx);
	}

	void upheap(int idx)
	{
		if (idx == root_idx)
			return;

		int par = idx / 2;

		if (queue[par]->weight > queue[idx]->weight)
		{
			swap(par, idx);
			upheap(par);
		}
		else if (queue[par]->weight == queue[idx]->weight)
		{
			if (queue[par]->value > queue[idx]->value)
			{
				swap(par, idx);
				upheap(par);
			}
		}
		
	}

	Node * pop()
	{
		Node* Min = queue[root_idx];
		while (inTree[Min->value] == in_Tree)
		{
			queue[root_idx] = queue[heap_idx];
			heap_idx--;
			downheap(root_idx);
			Min = queue[root_idx];
			if (heap_idx <= 0)
			{
				return nullptr;
			}
		}
		queue[root_idx] = queue[heap_idx];
		heap_idx--;
		downheap(root_idx);
		return Min;
	}

	void downheap(int idx)
	{
		int left = idx * 2;
		int right = idx * 2 + 1;

		if (right <= heap_idx) //오른쪽 자식까지 존재할때
		{
			if (queue[left]->weight < queue[right]->weight) //왼쪽 자식이 오른쪽 자식보다 weight이 낮을 때
			{
				if (queue[left]->weight < queue[idx]->weight) //왼쪽 자식이 부모보다 weight이 낮을 때
				{
					swap(left, idx);
					downheap(left);
				}
				else if (queue[left]->weight == queue[idx]->weight) //왼쪽 자식과 부모의 weight이 같을 때
				{
					if (queue[idx]->value > queue[left]->value) //value가 자식이 더 작을 때
					{
						swap(left, idx);
						downheap(left);
					}
					else
						return;
				}
				else
					return;
			}
			else if (queue[left]->weight == queue[right]->weight) //왼쪽 자식과 오른쪽 자식의 weight이 같을 때
			{
				if (queue[left]->value < queue[right]->value) //왼쪽의 value가 더 낮을 때
				{
					if (queue[left]->weight < queue[idx]->weight) //부모보다 weight이 낮은 경우
					{
						swap(left, idx); 
						downheap(left);
					}
					else if (queue[left]->weight == queue[idx]->weight) //왼쪽과 부모의 weight이 같을 때
					{
						if (queue[idx]->value > queue[left]->value) //자식이 value가 더작으면 swap
						{
							swap(left, idx);
							downheap(left);
						}
						else
							return;
					}
					else
						return;
				}
				else //오른쪽 value가 더 낮을 때
				{
					if (queue[right]->weight < queue[idx]->weight)
					{
						swap(right, idx);
						downheap(right);
					}
					else if (queue[right]->weight == queue[idx]->weight)
					{
						if (queue[right]->value < queue[idx]->value)
						{
							swap(right, idx);
							downheap(right);
						}
					}
					else
						return;
				}
			}
			else //right가 left보다 weight이 작을때
			{
				if (queue[right]->weight < queue[idx]->weight) //오른쪽 자식이 부모보다 weight이 낮을 때
				{
					swap(right, idx);
					downheap(right);
				}
				else if (queue[right]->weight == queue[idx]->weight) //부모와 오른쪽 자식의 weightㅇ이 같을 때
				{
					if (queue[right]->value < queue[idx]->value) //오른쪽 자식의 value가 더 낮을 때
					{
						swap(right, idx);
						downheap(right);
					}
					else
						return;
				}
				else
					return;
			}

		}
		else if (left <= heap_idx) //왼쪽 자식까지만 존재할 경우
		{
			if (queue[left]->weight < queue[idx]->weight) //왼쪽 자식이 부모보다 weight이 작은 경우
			{
				swap(left, idx);
				downheap(left);
			}
			else if (queue[left]->weight == queue[idx]->weight) //왼쪽과 부모의 weight이 같을 때
			{
				if (queue[idx]->value > queue[left]->value) //왼쪽 value가 부모보다 낮을 때
				{
					swap(left, idx);
					downheap(left);
				}
				else
					return;
			}
			else
				return;
		}
		else
			return;
	}

	void swap(int idx1, int idx2)
	{
		Node* temp = queue[idx1];
		queue[idx1] = queue[idx2];
		queue[idx2] = temp;
	}
};

class Tree {

public:
	int root;

	Tree(int size)
	{
		root = 0;
	}

	void Prims(int startNum, Graph* g)
	{
		memset(inTree, Unseen, sizeof(inTree));
		root = startNum;
		inTree[root] = in_Tree;
		priority_queue* q = new priority_queue();
		result.push_back(startNum);
		vector<Node*> cur;
		Node* next = nullptr;
		weight_sum = 0;

		for (int i = 0; i < g->graph_list[startNum].size(); i++)
		{
			q->push(g->graph_list[startNum][i]);
		}
		

		while (1)
		{
			Node* find = q->pop();
			if (find != nullptr)
			{
				int find_value = find->value;
				
				result.push_back(find_value);
				weight_sum += find->weight;
				
				inTree[find_value] = in_Tree;

				for (int i = 0; i < g->graph_list[find_value].size(); i++)
				{
					q->push(g->graph_list[find_value][i]);
				}

			}
			else
				break;
		}

		cout << weight_sum << ' ';
		for (int i = 0; i < result.size(); i++)
		{
			cout << result[i] << ' ';
		}
		cout << '\n';
		result.clear();

		delete q;
	}



};

int main()
{
	int n, m, q;
	int v1, v2, weight;
	char p;
	int start;
	cin >> n >> m >> q;


	Graph* g = new Graph(n);
	Tree* root = new Tree(m);

	while (m--)
	{
		cin >> v1 >> v2 >> weight;
		g->insert(v1, v2, weight);

	}
	while (q--)
	{
		cin >> p >> start;
		if (p == 'P')
		{
			root->Prims(start, g);
			
		}
	}
}