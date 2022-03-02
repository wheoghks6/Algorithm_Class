#include <iostream>
using namespace std;

#define BLACK  1
#define RED 0

class Node {
public:
	int id;
	string name;
	int size;
	int price;
	int color;
	Node* par;
	Node* left;
	Node* right;

	Node() {
		this->id = 0;
		this->name = '\0';
		this->size = 0;
		this->price = 0;
		this->color = BLACK;
		this->par = nullptr;
		this->left = nullptr;
		this->right = nullptr;
	}
};

class RBTree {
public:
	Node* root;

	RBTree()
	{
		root = nullptr;
	}

	void retree(Node* node) //recolor or restructure
	{

		Node* cur = node; //나
		Node* par = cur->par; //부모
		Node* grandpar = par->par; //조부모
		Node* greatpar = grandpar->par; //증조부모
		Node* uncle = nullptr; //삼촌을 받아오는 노드, 삼촌이 없을 경우 삼촌색깔은 BLACK이다.
		int uncle_color = BLACK; //일단 BLACK으로 존재하고 삼촌이 존재할경우 삼촌의 색깔을 받아온다.


		if (cur->color == RED && par->color == RED) //douuble red 다시확인
		{
			if (grandpar->left == par) //삼촌노드를 찾는다.
			{
				uncle = grandpar->right;
			}
			else if (grandpar->right == par)
			{
				uncle = grandpar->left;
			}

			if (uncle != nullptr) //삼촌이 존재하면 삼촌의 색깔을 받아온다.
				uncle_color = uncle->color;
			else //삼촌이 존재하지 않으면 BLACK
				uncle_color = BLACK;


			if (uncle_color == BLACK) //삼촌의 색깔이 BLACK이라면 restructure
			{
				if (par == grandpar->left) //par이 grandpar의 왼쪽자식일때
				{
					if (par != nullptr && cur == par->left) //par이 grandpar의 왼쪽 자식이고 cur이 par의 왼쪽자식일때
					{//왼쪽으로 3개있는 경우 par위치의 노드가 grandpar위치로 가야한다. grandpar은 par의 right로 가게된다.
						if (par->right != nullptr) //par의 
							par->right->par = grandpar;

						grandpar->left = par->right; 


						if (greatpar != nullptr) //증조부모가 존재한다면 grandpar의 위치를 확인하고 par을 연결해주어야한다.
						{
							if (greatpar->left == grandpar)
								greatpar->left = par;
							else
								greatpar->right = par;
						}
						par->par = greatpar; //par이 grandpar위치로 가기때문에 greatpar과 par을 연결해준다.

						grandpar->par = par;
						par->right = grandpar;

						//restructuring 이후 색깔 배치
						par->color = BLACK;
						par->left->color = RED;
						par->right->color = RED;

						if (this->root == grandpar)
							this->root = par;
					}
					else if (par != nullptr)//par이 grandpar의 왼쪽 자식이고 cur이 par의 오른쪽 자식일때
					{
						par->par = cur; //cur이 grandpar의 자리에 가야한다. par의 부모로 cur을 지정한다.
						if (cur->left != nullptr)
							cur->left->par = par;

						par->right = cur->left; //cur의 왼쪽자식이 par의 오른쪽 자식이된다.
						cur->left = par; //cur의 왼쪽에 par이 오고 par의 부모가 cur이되는것이다.

						grandpar->par = cur; //grandpar의 부모에 cur을 넣어준다.
						if (cur->right != nullptr)
							cur->right->par = grandpar;

						grandpar->left = cur->right; //cur의 오른쪽 자식은 grandpar의 왼쪽 자식이된다.
						cur->right = grandpar; //cur의 오른 쪽에 grandpar가 들어온다.
						//-> cur의 왼쪽자식 par, 오른쪽 자식 grandpar가 된다.

						if (greatpar != nullptr) //증조부모의 노드가 존재한다면
						{
							if (greatpar->left == grandpar)
								greatpar->left = cur;
							else
								greatpar->right = cur;

							cur->par = greatpar; //cur의 부모로 증조부모를 선택한다.
						}
						else //증조부모 노드가 없다면 cur->par은 nullptr이된다.
							cur->par = nullptr;

						//restructuring 이후 색깔 배치
						cur->color = BLACK;
						par->color = RED;
						cur->right->color = RED;

						if (this->root == grandpar) //root를 체크하고 변경해준다.
							this->root = cur;
					}


				}
				else // par이 grandpar의 오른쪽 자식일 때
				{
					if (cur == par->right) // par이 grandpar의 오른쪽 자식이고 cur이 par의 오른쪽 자식일때
					{//오른쪽으로 뻗어있는 경우
						if (par->left != nullptr) //par의 왼쪽 자식이 존재하는경우
						{
							par->left->par = grandpar; 
							grandpar->right = par->left; //grandpar의 오른쪽 자식으로 par의 왼쪽을 넣어준다.
						}
						else//par의 왼쪽 자식이 존재하지 않는경우 grandpar의 오른쪽 자식이 nullptr이된다.
							grandpar->right = nullptr; 



						if (greatpar != nullptr) //증조부모가 존재하는경우
						{//par이 grandpar의 자리가 되기때문에 greatpar과 연결해 준다.
							if (greatpar->left == grandpar) 
								greatpar->left = par;
							else
								greatpar->right = par;

							par->par = greatpar;
						}
						else //증조부모가 존재하지 않는경우 par의 par을 nullptr로 설정한다.
							par->par = nullptr;

						grandpar->par = par; //grandpar의 par가 par이되면서 
						par->left = grandpar; //par의 왼쪽 자식이 grandpar가 된다.

						//restructuring 이후 색깔 배치
						par->color = BLACK;
						par->left->color = RED;
						par->right->color = RED;

						if (this->root == grandpar) //root를 항상 체크하고 변경해준다.
							this->root = par;
					}
					else // par이 grandpar의 오른쪽 자식이고 cur이 par의 왼쪽 자식일 때
					{//오른쪽 왼쪽으로 꺽여있는 형태이다.

						//cur이 grandpar의 자리에 가야한다. 
						if (cur->right != nullptr) //cur의 오른쪽 자식이 존재한다면
						{
							cur->right->par = par;
							par->left = cur->right; //par의 왼쪽으로 넣어준다.
						}
						else
							par->left = nullptr;

						par->par = cur; // par의 par을 cur로지정해주고
						cur->right = par;//cur의 오른쪽 자식을 par로 지정한다.

						if (cur->left != nullptr) //cur의 왼쪽 자식이 존재한다면
						{
							cur->left->par = grandpar; //grandpar의 오른쪽 자식으로 연결해준다.
							grandpar->right = cur->left;
						}
						else
							grandpar->right = nullptr;

						grandpar->par = cur; //grandpar의 부모는 cur이되고
						cur->left = grandpar; //cur의 왼쪽 자식은 grandpar가된다.


						if (greatpar != nullptr) //증조부모가 존재한다면 grandpar자리에 cur을 넣어 연결해준다.
						{
							if (greatpar->left == grandpar)
								greatpar->left = cur;
							else
								greatpar->right = cur;

							cur->par = greatpar;
						}
						else //증조부모가 존재하지 않는다면 cur->par은 nullptr로 해준다.
							cur->par = nullptr;

						//restructuring 이후 색깔 배치
						cur->color = BLACK;
						cur->left->color = RED;
						cur->right->color = RED;

						if (this->root == grandpar) //root를 체크한다.
							this->root = cur;
					}
				}
				if (this->root != nullptr) //root는 항상 BLACK이다.
					this->root->color = BLACK;
			}
			else //uncle_color가 RED인경우 Recoloring
			{ 
				uncle->color = BLACK;
				par->color = BLACK;

				if (grandpar != this->root)
					grandpar->color = RED;

				if (grandpar->par != nullptr && grandpar->par->color == RED) //Recoloring이후에는 double red가 또 발생할 수 있다.
					retree(grandpar); //recoloring으로 발생한 double red에대해 retstructuring or recoloring 진행
			}

		}
		return;
	}

	void insertNode(int id, string name, int size, int price) //트리에 노드를 넣어주는 연산이다.
	{

		int depth = 0;
		int key = id;

		if (this->root == nullptr) //root가 nullptr이라면 root에 노드를 삽입해주어야한다.
		{
			Node* newNode = new Node();
			newNode->id = id;
			newNode->name = name;
			newNode->size = size;
			newNode->price = price;
			newNode->color = BLACK; //rootnode는 항상 BLACK이다.

			this->root = newNode; //생성한 노드를 넣어준다.
			root->color = BLACK;
			cout << "0\n";
			return;
		}
		//root 노드가 존재한다면 진행
		Node* cur = root;
		Node* par = nullptr;

		while (cur != nullptr)
		{
			if (cur->id == key) //노드가 존재하는 노드라면 아래처럼 depth만 출력해준다.
			{
				Node* empty = cur;
				while (empty != this->root)
				{
					empty = empty->par;
					depth++;
				}
				cout << depth << "\n";
				return;
			}
			else if (cur->id > key)
			{
				par = cur;
				cur = cur->left;
			}
			else
			{
				par = cur;
				cur = cur->right;
			}
		}
		//노드의 위치를 찾았을 때 노드를 할당하고 자리를 정해준다.
		Node* newNode = new Node();
		newNode->id = id;
		newNode->name = name;
		newNode->size = size;
		newNode->price = price;
		newNode->color = RED;

		if (par->id < key) //nullptr의 par인 par노드 id값과 비교하여 위치 지정
		{
			par->right = newNode;
			newNode->par = par;
		}
		else
		{
			par->left = newNode;
			newNode->par = par;
		}



		if (newNode->par->par != nullptr && newNode->par->color == RED) //double red인경우
		{
			retree(newNode); //내부에서 restructuring, recoloring 을 해준다.
		}

		Node* empty = newNode; //할당된 노드의 depth 확인
		while (empty != this->root)
		{
			empty = empty->par;
			depth++;
		}

		cout << depth << '\n'; //삽입된 노드의 depth를 구하는 연산
		return;
	}

	void FindNode(int id) //id로 노드를 찾는 메소드
	{
		int key = id;
		int depth = 0;
		Node* cur = this->root;

		while (cur != nullptr)//루트부터 nullptr이 나올때까지 반복하며 찾는다.
		{
			if (cur->id == key) //입력된 id(key)와 각노드들 id비교 같으면 출력 후 함수반환
			{

				cout << depth << ' ' << cur->name << ' ' << cur->size << ' ' << cur->price << '\n';
				return;
			}
			else if (cur->id > key) //cur의 id가 key보다 크다면 cur의 왼쪽 자식으로 가게한다.
			{
				cur = cur->left;
			}
			else //cur의 id가 key보다 작다면 cur의 오른쪽 자식으로 가게한다.
			{
				cur = cur->right;
			}
			depth++;
		}
		cout << "NULL\n"; //노드가 없을때까지 찾다가 못찾는다면 NULL출력
		return;
	}

	void updateNode(int id, string name, int size, int price) //값을 업데이트 하는 함수
	{
		int key = id; // FindNode와 똑같이 노드를 찾는다.
		Node* cur = root;
		int depth = 0;
		while (cur != nullptr)
		{
			if (cur->id == key) //일치하는 노드 발견시 아래처럼 값 업데이트
			{
				cur->name = name;
				cur->size = size;
				cur->price = price;
				cout << depth << '\n';
				return;
			}
			else if (cur->id > key) //찾으려는 id(key)가 cur->id보다 작으면 cur->left로 이동한다.
			{
				cur = cur->left;
			}
			else //찾으려는 id(key)가 cur->id보다 크면 cur->right로 이동한다.
			{
				cur = cur->right;
			}
			depth++;
		}
		cout << "NULL\n";
		return;
	}

	void Discount(Node* cur, int start, int end, int discount) //할인하는 함수
	{
		if (cur != nullptr) //인자로 들어온 노드가 nullptr이 아닌경우 아래와같이 작동
		{
			if (cur->id >= start && cur->id <= end) //현재 노드가 start보다 크거나 같고 end보다 작거나 같으면 할인 
				cur->price = (cur->price * (100 - discount)) / 100; //할인을 해준다.

			if (cur->id < end) //현재 노드가 end보다 작으면 오른쪽 자식 노드에 대해서도 Discount실행
				Discount(cur->right, start, end, discount);

			if (cur->id > start) //현재 노드가 start보다 크면 왼쪽 자식 노드에 대해서도 Discount 실행
				Discount(cur->left, start, end, discount);
		}

		return;
	}

};

int main()
{
	int input_count;
	char cmd;
	int id;
	string name;
	int size;
	int price;
	int start;
	int end;
	int discount;
	RBTree* rbtree = new RBTree();
	cin >> input_count; //명령어 개수 입력받는다.
	cin.clear();
	while (input_count--) //명령어 개수만큼 반복문 실행
	{
		cin >> cmd; //각 명령어 입력, I(Insert), F(Find), R(Update), D(Discount)
		if (cmd == 'I')
		{
			cin >> id >> name >> size >> price; 
			rbtree->insertNode(id, name, size, price); //삽입함수 실행
		}
		else if (cmd == 'F')
		{
			cin >> id;
			rbtree->FindNode(id); //FindNode실행
		}
		else if (cmd == 'R')
		{
			cin >> id >> name >> size >> price;
			rbtree->updateNode(id, name, size, price); //업데이트 실행
		}
		else if (cmd == 'D')
		{
			cin >> start >> end >> discount;
			rbtree->Discount(rbtree->root, start, end, discount); //Discunt실행
		}


		cin.clear();
	}
}
