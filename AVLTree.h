#pragma once
#ifndef _AVLTREE_H
#define _AVLTREE_H

struct Node
{
	int val;			//节点的值
	int size;			//节点本身和该节点所有孩子节点总和
	int height;			//路径所经过的边数总和
	Node* left;			//左孩子指针
	Node* right;		//右孩子指针
	Node* parent;		//父节点指针

	Node() : val(0), size(0), height(0), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(int a) : val(a), size(1), height(0), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(int a, Node* p) : val(a), size(1), height(0), left(nullptr), right(nullptr), parent(p) {}
	Node(int a, Node* p, Node* l, Node* r) : val(a), size(1), height(0), left(l), right(r), parent(p) {}
};

#include<vector>
using namespace std;	//为了解决 "vector不是模板问题"
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
