#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef struct NODE { //트리 노드 : NODE

	int key; //키 값
	int treeSize; //트리 크기
	struct NODE* left; //왼쪽 자식
	struct NODE* right;//오른쪽 자식
	struct NODE* parent;//부모

}NODE;

typedef struct LIST { //방문 경로 : LIST

	struct NODE* node; //방문한 노드
	struct LIST* next; //다음 리스트 원소

}LIST;


void insertItem(NODE* root, int k); //중복 키 처리하면서 원소 삽입
int findAllRange(NODE* root, int k1, int k2); //k1, k2 범위 안의 원소의 개수 구하기 => O(logN)
void freeMemory(NODE* root); // root 부터 전체 메모리 해제

NODE* getNode(); //새로운 노드 만드는 함수
int isExternal(NODE* node); // 외부 노드인지 반환하는 함수
NODE* treeSearch(NODE* v, int k); //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수  => O(logN)
void expandExternal(NODE* v); //v 노드를 내부노드로 만드는 함수

LIST* getList();  //리스트 원소 만들어 반환하는 함수
void addLast(LIST* p, NODE* v); //리스트 추가하는 함수
NODE* treeSearchCommon1(LIST* p, NODE* v, int k); //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수(가장 좌측) => O(logN)
NODE* treeSearchCommon2(LIST* p, NODE* v, int k); //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수(가장 우측) => O(logN)
NODE* treeSearch2(NODE* v, int k); //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수 => O(logN)
NODE* commonNode(LIST** p1, LIST** p2); //p1 p2 경로에서 동일한 경로는 삭제 => O(logN)
void freeList(LIST* p); //리스트 메모리 해제



int main() {

	int n;
	NODE* root = getNode(); // root가 될 빈 노드 하나 생성

	scanf("%d", &n); //정수 개수 입력 받기
	for (int i = 0; i < n; i++) // n 개의 정수 입력 받아 차례로 트리에 삽입
	{
		int key;
		scanf("%d", &key);
		insertItem(root, key);
	}

	int m; 
	scanf("%d", &m); // 질의 개수 m 입력 받기
	for (int i = 0; i < m; i++) // 키의 범위 입력 받아 질의에 대한 답 출력
	{
		int k1, k2;
		scanf("%d%d", &k1, &k2);
		printf("%d\n", findAllRange(root, k1, k2));
		
	}
	
	freeMemory(root); // 메모리 해제

	return 0;
}




void insertItem(NODE* root, int k) { //중복 키 처리하면서 원소 삽입

	NODE* w = treeSearch(root, k); // 삽입 위치 찾기
	while (!isExternal(w)) //원소가 중복되면 가장 오른쪽에 삽입하도록 이동
	{
		w = treeSearch(w->right, k);
	}

	w->key = k;
	expandExternal(w);
}

int findAllRange(NODE* root, int k1, int k2) { //k1, k2 범위 안의 원소의 개수 구하기 => O(logN)

	LIST* p1, * p2;
	p1 = getList(); //k1 까지의 경로 저장하는 리스트
	p2 = getList(); //k2 까지의 경로 저장하는 리스트

	NODE* v1, * v2, * v;
	v1 = treeSearchCommon1(p1, root, k1); //k1을 키로 가진 노드 중 가장 좌측 원소 구하기 & 경로 구하기 => O(logN)
	v2 = treeSearchCommon2(p2, root, k2); //k2을 키로 가진 노드 중 가장 우측 원소 구하기 & 경로 구하기 => O(logN)
	v = commonNode(&p1, &p2); // p1, p2에서 마지막 공통 노드 v 전까지 원소 삭제

	int c = 0; // 범위 안의 노드 개수

	for (LIST* p = p1->next; p != NULL; p = p->next) //v가 아닌 노드에 대해서 => O(logN)
	{
		
		if (!isExternal(p->node)) {
			NODE* u = p->node->right; // 오른쪽 자식
			if (p->next == NULL || u != p->next->node) //오른쪽 자식이 v1으로 가는 경로에 없는 경우
			{
				if (!isExternal(u)) //오른쪽 자식이 내부 노드인 경우
				{
					c += u->treeSize;
				}
				c++;
			}
		}
	}

	for (LIST* p = p2->next; p != NULL; p = p->next) //v가 아닌 노드에 대해서 => O(logN)
	{
		
		if (!isExternal(p->node)) {
			NODE* u = p->node->left;// 왼쪽 자식
			if (p->next == NULL || u != p->next->node) //왼쪽 자식이 v2으로 가는 경로에 없는 경우
			{
				if (!isExternal(u))// 왼쪽 자식이 내부 노드인 경우
				{
					c += u->treeSize;
				}
				c++;
			}
		}
	}

	if (!isExternal(v)) c++; // 공통 노드가 내부 노드이면

	freeList(p1); // 리스트 메모리 해제
	freeList(p2); // 리스트 메모리 해제

	return c;
}

void freeMemory(NODE* root) { // root 부터 전체 메모리 해제

	if (root == NULL) return;
	else
	{
		freeMemory(root->left);
		freeMemory(root->right);
		free(root);
	}
}





/*************************************/
/***insertItem() 에서 사용되는 함수***/
/*************************************/

NODE* getNode() { //새로운 노드 만드는 함수

	NODE* newNode = (NODE*)malloc(sizeof(NODE)); //메모리 할당
	newNode->left = newNode->right = NULL; //자신의 왼쪽, 오른쪽 자식 없음
	newNode->treeSize = 0; //부트리 크기 초기화

	return newNode; // 만든 노드 반환
}

int isExternal(NODE* node) { // 외부 노드인지 반환하는 함수

	return node->left == NULL && node->right == NULL;
}

NODE* treeSearch(NODE* v, int k) { //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수  => O(logN)
								   //insertItem() 에서 사용(즉, 삽입 위치를 반환하는 함수) treeSearch2에서 부트리 개수 증가하는 코드 추가한 버전

	v->treeSize++; //부트리의 크기 증가
	if (isExternal(v)) //외부 노드라면(k라는 key를 가진 노드가 없다면)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //해당 노드를 찾았을 때
	else if (k < key) return treeSearch(v->left, k);//현재 노드가 가진 키가 찾는 값보다 클 때
	else return treeSearch(v->right, k); //k > key 현재 노드가 가진 키가 찾는 값보다 작을 때

}

void expandExternal(NODE* v) { //v 노드를 내부노드로 만드는 함수

	v->left = getNode(); //왼쪽 자식 생성
	v->right = getNode();//오른쪽 자식 생성
	v->left->parent = v; //왼쪽 자식의 부모를 현재 노드와 연결
	v->right->parent = v;//오른쪽 자식의 부모를 현재 노드와 연결
}




/*************************************/
/**findAllRange() 에서 사용되는 함수**/
/*************************************/


LIST* getList() { //리스트 원소 만들어 반환하는 함수

	LIST* newList = (LIST*)malloc(sizeof(LIST));
	newList->next = NULL;

	return newList;
}

void addLast(LIST* p, NODE* v) { //리스트 추가하는 함수

	LIST* pnext = getList(); //새로운 리스트 원소 만들기
	pnext->node = v;

	p->next = pnext; //기존 리스트 맨 뒤에 추가

}

NODE* treeSearchCommon1(LIST* p, NODE* v, int k) { //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수(가장 좌측) => O(logN)

	addLast(p, v);
	if (isExternal(v)) //외부 노드라면(k라는 key를 가진 노드가 없다면)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //해당 노드를 찾았을 때
	else if (k < key) return treeSearchCommon1(p->next, v->left, k);//현재 노드가 가진 키가 찾는 값보다 클 때
	else return treeSearchCommon1(p->next, v->right, k); //k > key 현재 노드가 가진 키가 찾는 값보다 작을 때

}

NODE* treeSearchCommon2(LIST* p, NODE* v, int k) { //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수(가장 우측) => O(logN)

	addLast(p, v);
	if (isExternal(v)) //외부 노드라면(k라는 key를 가진 노드가 없다면)
	{
		return v;
	}

	int key = v->key;

	if (k == key) { //해당 노드를 찾았을 때

		while (!isExternal(treeSearch2(v->right, k))) //동일 키 처리
		{
			v = treeSearchCommon2(p->next, v->right, k);
		}

		return v; 
	}
	else if (k < key) return treeSearchCommon2(p->next, v->left, k);//현재 노드가 가진 키가 찾는 값보다 클 때
	else return treeSearchCommon2(p->next, v->right, k); //k > key 현재 노드가 가진 키가 찾는 값보다 작을 때

}

NODE* treeSearch2(NODE* v, int k) { //k라는 key 값을 가지고 있는 노드의 위치 찾아서 반환하는 함수 => O(logN)

	if (isExternal(v)) //외부 노드라면(k라는 key를 가진 노드가 없다면)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //해당 노드를 찾았을 때
	else if (k < key) return treeSearch2(v->left, k);//현재 노드가 가진 키가 찾는 값보다 클 때
	else return treeSearch2(v->right, k); //k > key 현재 노드가 가진 키가 찾는 값보다 작을 때

}

NODE* commonNode(LIST** p1, LIST** p2) { //p1 p2 경로에서 동일한 경로는 삭제 => O(logN)

	NODE* common = NULL;
	LIST* pnext1, * pnext2;

	for (LIST* i1 = (*p1)->next, *i2 = (*p2)->next; i1 != NULL && i2 != NULL; i1 = i1->next, i2 = i2->next)
	{
		if (i1->node == i2->node) {

			// 같은 경로는 삭제
			//(맨 앞에 헤더 있기 때문에 헤더부터 삭제됨 => 전체 돌면 동일한 경로 중 가장 마지막노드 부터 p1, p2 가 시작됨)
			pnext1 = (*p1)->next;
			pnext2 = (*p2)->next;

			free(*p1);
			free(*p2);

			*p1 = pnext1;
			*p2 = pnext2;

			common = i1->node;
		}
		else break;
	}

	return common;
}

void freeList(LIST* p) { //리스트 메모리 해제
	LIST* next;

	for (LIST* i = p; i != NULL; i = next)
	{
		next = i->next;
		free(i);
	}
}


