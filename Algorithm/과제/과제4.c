#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef struct NODE { //Ʈ�� ��� : NODE

	int key; //Ű ��
	int treeSize; //Ʈ�� ũ��
	struct NODE* left; //���� �ڽ�
	struct NODE* right;//������ �ڽ�
	struct NODE* parent;//�θ�

}NODE;

typedef struct LIST { //�湮 ��� : LIST

	struct NODE* node; //�湮�� ���
	struct LIST* next; //���� ����Ʈ ����

}LIST;


void insertItem(NODE* root, int k); //�ߺ� Ű ó���ϸ鼭 ���� ����
int findAllRange(NODE* root, int k1, int k2); //k1, k2 ���� ���� ������ ���� ���ϱ� => O(logN)
void freeMemory(NODE* root); // root ���� ��ü �޸� ����

NODE* getNode(); //���ο� ��� ����� �Լ�
int isExternal(NODE* node); // �ܺ� ������� ��ȯ�ϴ� �Լ�
NODE* treeSearch(NODE* v, int k); //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�  => O(logN)
void expandExternal(NODE* v); //v ��带 ���γ��� ����� �Լ�

LIST* getList();  //����Ʈ ���� ����� ��ȯ�ϴ� �Լ�
void addLast(LIST* p, NODE* v); //����Ʈ �߰��ϴ� �Լ�
NODE* treeSearchCommon1(LIST* p, NODE* v, int k); //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�(���� ����) => O(logN)
NODE* treeSearchCommon2(LIST* p, NODE* v, int k); //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�(���� ����) => O(logN)
NODE* treeSearch2(NODE* v, int k); //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ� => O(logN)
NODE* commonNode(LIST** p1, LIST** p2); //p1 p2 ��ο��� ������ ��δ� ���� => O(logN)
void freeList(LIST* p); //����Ʈ �޸� ����



int main() {

	int n;
	NODE* root = getNode(); // root�� �� �� ��� �ϳ� ����

	scanf("%d", &n); //���� ���� �Է� �ޱ�
	for (int i = 0; i < n; i++) // n ���� ���� �Է� �޾� ���ʷ� Ʈ���� ����
	{
		int key;
		scanf("%d", &key);
		insertItem(root, key);
	}

	int m; 
	scanf("%d", &m); // ���� ���� m �Է� �ޱ�
	for (int i = 0; i < m; i++) // Ű�� ���� �Է� �޾� ���ǿ� ���� �� ���
	{
		int k1, k2;
		scanf("%d%d", &k1, &k2);
		printf("%d\n", findAllRange(root, k1, k2));
		
	}
	
	freeMemory(root); // �޸� ����

	return 0;
}




void insertItem(NODE* root, int k) { //�ߺ� Ű ó���ϸ鼭 ���� ����

	NODE* w = treeSearch(root, k); // ���� ��ġ ã��
	while (!isExternal(w)) //���Ұ� �ߺ��Ǹ� ���� �����ʿ� �����ϵ��� �̵�
	{
		w = treeSearch(w->right, k);
	}

	w->key = k;
	expandExternal(w);
}

int findAllRange(NODE* root, int k1, int k2) { //k1, k2 ���� ���� ������ ���� ���ϱ� => O(logN)

	LIST* p1, * p2;
	p1 = getList(); //k1 ������ ��� �����ϴ� ����Ʈ
	p2 = getList(); //k2 ������ ��� �����ϴ� ����Ʈ

	NODE* v1, * v2, * v;
	v1 = treeSearchCommon1(p1, root, k1); //k1�� Ű�� ���� ��� �� ���� ���� ���� ���ϱ� & ��� ���ϱ� => O(logN)
	v2 = treeSearchCommon2(p2, root, k2); //k2�� Ű�� ���� ��� �� ���� ���� ���� ���ϱ� & ��� ���ϱ� => O(logN)
	v = commonNode(&p1, &p2); // p1, p2���� ������ ���� ��� v ������ ���� ����

	int c = 0; // ���� ���� ��� ����

	for (LIST* p = p1->next; p != NULL; p = p->next) //v�� �ƴ� ��忡 ���ؼ� => O(logN)
	{
		
		if (!isExternal(p->node)) {
			NODE* u = p->node->right; // ������ �ڽ�
			if (p->next == NULL || u != p->next->node) //������ �ڽ��� v1���� ���� ��ο� ���� ���
			{
				if (!isExternal(u)) //������ �ڽ��� ���� ����� ���
				{
					c += u->treeSize;
				}
				c++;
			}
		}
	}

	for (LIST* p = p2->next; p != NULL; p = p->next) //v�� �ƴ� ��忡 ���ؼ� => O(logN)
	{
		
		if (!isExternal(p->node)) {
			NODE* u = p->node->left;// ���� �ڽ�
			if (p->next == NULL || u != p->next->node) //���� �ڽ��� v2���� ���� ��ο� ���� ���
			{
				if (!isExternal(u))// ���� �ڽ��� ���� ����� ���
				{
					c += u->treeSize;
				}
				c++;
			}
		}
	}

	if (!isExternal(v)) c++; // ���� ��尡 ���� ����̸�

	freeList(p1); // ����Ʈ �޸� ����
	freeList(p2); // ����Ʈ �޸� ����

	return c;
}

void freeMemory(NODE* root) { // root ���� ��ü �޸� ����

	if (root == NULL) return;
	else
	{
		freeMemory(root->left);
		freeMemory(root->right);
		free(root);
	}
}





/*************************************/
/***insertItem() ���� ���Ǵ� �Լ�***/
/*************************************/

NODE* getNode() { //���ο� ��� ����� �Լ�

	NODE* newNode = (NODE*)malloc(sizeof(NODE)); //�޸� �Ҵ�
	newNode->left = newNode->right = NULL; //�ڽ��� ����, ������ �ڽ� ����
	newNode->treeSize = 0; //��Ʈ�� ũ�� �ʱ�ȭ

	return newNode; // ���� ��� ��ȯ
}

int isExternal(NODE* node) { // �ܺ� ������� ��ȯ�ϴ� �Լ�

	return node->left == NULL && node->right == NULL;
}

NODE* treeSearch(NODE* v, int k) { //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�  => O(logN)
								   //insertItem() ���� ���(��, ���� ��ġ�� ��ȯ�ϴ� �Լ�) treeSearch2���� ��Ʈ�� ���� �����ϴ� �ڵ� �߰��� ����

	v->treeSize++; //��Ʈ���� ũ�� ����
	if (isExternal(v)) //�ܺ� �����(k��� key�� ���� ��尡 ���ٸ�)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //�ش� ��带 ã���� ��
	else if (k < key) return treeSearch(v->left, k);//���� ��尡 ���� Ű�� ã�� ������ Ŭ ��
	else return treeSearch(v->right, k); //k > key ���� ��尡 ���� Ű�� ã�� ������ ���� ��

}

void expandExternal(NODE* v) { //v ��带 ���γ��� ����� �Լ�

	v->left = getNode(); //���� �ڽ� ����
	v->right = getNode();//������ �ڽ� ����
	v->left->parent = v; //���� �ڽ��� �θ� ���� ���� ����
	v->right->parent = v;//������ �ڽ��� �θ� ���� ���� ����
}




/*************************************/
/**findAllRange() ���� ���Ǵ� �Լ�**/
/*************************************/


LIST* getList() { //����Ʈ ���� ����� ��ȯ�ϴ� �Լ�

	LIST* newList = (LIST*)malloc(sizeof(LIST));
	newList->next = NULL;

	return newList;
}

void addLast(LIST* p, NODE* v) { //����Ʈ �߰��ϴ� �Լ�

	LIST* pnext = getList(); //���ο� ����Ʈ ���� �����
	pnext->node = v;

	p->next = pnext; //���� ����Ʈ �� �ڿ� �߰�

}

NODE* treeSearchCommon1(LIST* p, NODE* v, int k) { //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�(���� ����) => O(logN)

	addLast(p, v);
	if (isExternal(v)) //�ܺ� �����(k��� key�� ���� ��尡 ���ٸ�)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //�ش� ��带 ã���� ��
	else if (k < key) return treeSearchCommon1(p->next, v->left, k);//���� ��尡 ���� Ű�� ã�� ������ Ŭ ��
	else return treeSearchCommon1(p->next, v->right, k); //k > key ���� ��尡 ���� Ű�� ã�� ������ ���� ��

}

NODE* treeSearchCommon2(LIST* p, NODE* v, int k) { //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ�(���� ����) => O(logN)

	addLast(p, v);
	if (isExternal(v)) //�ܺ� �����(k��� key�� ���� ��尡 ���ٸ�)
	{
		return v;
	}

	int key = v->key;

	if (k == key) { //�ش� ��带 ã���� ��

		while (!isExternal(treeSearch2(v->right, k))) //���� Ű ó��
		{
			v = treeSearchCommon2(p->next, v->right, k);
		}

		return v; 
	}
	else if (k < key) return treeSearchCommon2(p->next, v->left, k);//���� ��尡 ���� Ű�� ã�� ������ Ŭ ��
	else return treeSearchCommon2(p->next, v->right, k); //k > key ���� ��尡 ���� Ű�� ã�� ������ ���� ��

}

NODE* treeSearch2(NODE* v, int k) { //k��� key ���� ������ �ִ� ����� ��ġ ã�Ƽ� ��ȯ�ϴ� �Լ� => O(logN)

	if (isExternal(v)) //�ܺ� �����(k��� key�� ���� ��尡 ���ٸ�)
	{
		return v;
	}

	int key = v->key;

	if (k == key) return v; //�ش� ��带 ã���� ��
	else if (k < key) return treeSearch2(v->left, k);//���� ��尡 ���� Ű�� ã�� ������ Ŭ ��
	else return treeSearch2(v->right, k); //k > key ���� ��尡 ���� Ű�� ã�� ������ ���� ��

}

NODE* commonNode(LIST** p1, LIST** p2) { //p1 p2 ��ο��� ������ ��δ� ���� => O(logN)

	NODE* common = NULL;
	LIST* pnext1, * pnext2;

	for (LIST* i1 = (*p1)->next, *i2 = (*p2)->next; i1 != NULL && i2 != NULL; i1 = i1->next, i2 = i2->next)
	{
		if (i1->node == i2->node) {

			// ���� ��δ� ����
			//(�� �տ� ��� �ֱ� ������ ������� ������ => ��ü ���� ������ ��� �� ���� ��������� ���� p1, p2 �� ���۵�)
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

void freeList(LIST* p) { //����Ʈ �޸� ����
	LIST* next;

	for (LIST* i = p; i != NULL; i = next)
	{
		next = i->next;
		free(i);
	}
}


