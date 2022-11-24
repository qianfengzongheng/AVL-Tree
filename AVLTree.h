#pragma once
#ifndef _AVLTREE_H
#define _AVLTREE_H

struct Node
{
	int val;			//�ڵ��ֵ
	int size;			//�ڵ㱾��͸ýڵ����к��ӽڵ��ܺ�
	int height;			//·���������ı����ܺ�
	Node* left;			//����ָ��
	Node* right;		//�Һ���ָ��
	Node* parent;		//���ڵ�ָ��

	Node() : val(0), size(0), height(0), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(int a) : val(a), size(1), height(0), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(int a, Node* p) : val(a), size(1), height(0), left(nullptr), right(nullptr), parent(p) {}
	Node(int a, Node* p, Node* l, Node* r) : val(a), size(1), height(0), left(l), right(r), parent(p) {}
};

#include<vector>
using namespace std;	//Ϊ�˽�� "vector����ģ������"
class AVLTree
{
private:
	Node* root;

	void rotate(Node* node);
	void avlDelete(Node* node);
	void calculate(Node* node);
	void rebalance(Node* node);
	bool isBalance(Node* node);
	int getSize(Node* node);
	int getHeight(Node* node);
	Node* childHeight(Node* node);
	Node* childchildHeight(Node* node);
	Node* searchSubTree(Node* node, int v);
	Node* restructure(Node* node);
	static void relink(Node* parent, Node* child, bool existLeft);
	static Node* subTreeFirst(Node* node);
	static Node* subTreeLast(Node* node);
	
public:
	AVLTree() : root(nullptr) {}
	AVLTree(vector<int>& vec);

	Node* build(vector<int>& vec, int left, int right, Node* parent);
	int avlSearch(int k);
	void avlInsert(int v);
	bool avlDeleteForOne(int v);
	bool avlDeleteForAll(int v);
	vector<int> getInOrderNums();

};
#endif
