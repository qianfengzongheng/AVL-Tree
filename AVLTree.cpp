#include "AVLTree.h"

/**
 * @brief 找到以node节点为根节点的子树中，最后一个出现的节点，实际上就是avl树中序遍历下所得到的数组中的最后一个值的节点
 * @param node 子树的根节点
 * @return 子树最后的一个节点
*/
Node* AVLTree::subTreeLast(Node* node)
{
	Node* p = node;
	while (p->right)
	{
		p = p->right;
	}
	return p;
}

/**
 * @brief AVL树的构造函数，通过一个从小到大排序的数组，构建一棵AVL树
 * @param vec 从小到大排序的数组
*/
AVLTree::AVLTree(vector<int>& vec)
{
	if (!vec.empty())
	{
		root = build(vec, 0, vec.size() - 1, nullptr);
	}
}

/**
 * @brief AVL树构造函数中真正调用的建树函数，通过传入的从小到大排序的数组建AVL树
 * @param vec 从小到大排序的数组
 * @param left 分段数组的最左下标
 * @param right 分段数组的最右下标
 * @param parent 建树过程中新节点的父节点
 * @return 当建树成功，返回该AVL树的根节点root，否则返回nullptr
*/
Node* AVLTree::build(vector<int>& vec, int left, int right, Node* parent)
{
	if (left > right) return nullptr;
	int mid = left + (right - left) / 2;
	Node* node = new Node(vec[mid], parent);
	node->left = build(vec, left, mid - 1, node);
	node->right = build(vec, mid + 1, right, node);
	if (node) calculate(node);
	return node;
}

/**
 * @brief 寻找第k个小的值，分三种情况讨论：
 *				1. 当前节点的左子树的总节点数小于k-1，说明第k个小的值一定出现在当前节点的右子树，因为当前节点的左子树的总节点数+当前节点<=k-1，转移到右子树的时候要多减去1是因为要把当前节点也减去
 *				2. 当前节点的左子树总节点数等于k-1，说明当前节点就是要找的第k个小的值；
 *				3. 当前节点的左子树总节点数大于k-1，说明第k个小的值就在当前节点的左子树中。
 * @param k 要找的第k个小的数，1 <= k <= n，n表示AVL数的节点数
 * @return 找到该数，返回这个位置的值，否则返回-1
*/
int AVLTree::avlSearch(int k)
{
	Node* node = root;
	while (node)
	{
		int leftNum = getSize(node->left);
		if (leftNum < k - 1)
		{
			node = node->right;
			k -= leftNum + 1;
		}
		else if (leftNum == k - 1)//找到了，当前节点就是第k个小的数
		{
			break;
		}
		else
		{
			node = node->left;
		}
	}
	return node ? node->val : -1;//找不到就返回-1
}

/**
 * @brief 往AVL数中插入一个节点，如果条件满足，会优先把值为v的节点插入到左子树
 * @param v 所插入节点的值
*/
void AVLTree::avlInsert(int v)
{
	if (!root)
	{
		root = new Node(v);
	}
	else
	{
		Node* node = searchSubTree(root, v);
		bool isAddLeft = (node->val >= v);
		if (node->val == v)
		{
			if (node->left)
			{
				node = subTreeLast(node->left);
				isAddLeft = false;
			}
			else
			{
				isAddLeft = true;
			}
		}

		//添加新节点
		Node* leaf = new Node(v, node);
		if (isAddLeft)
		{
			node->left = leaf;
		}
		else
		{
			node->right = leaf;
		}
		rebalance(leaf);
	}
}

/**
 * @brief 删除1个值为v的节点，即如果v值的节点存在多个，则只能删除其中一个
 * @param v 要删除的节点的值
 * @return 删除成功返回true，否则返回false
*/
bool AVLTree::avlDeleteForOne(int v)
{
	if (!root)
	{
		return false;
	}

	Node* node = searchSubTree(root, v);
	if (node->val != v)
	{
		return false;
	}

	if (node->left && node->right)
	{
		Node* replacement = nullptr;
		if (node->left->height >= node->right->height)
		{
			replacement = subTreeLast(node->left);
		}
		else
		{
			replacement = subTreeFirst(node->right);
		}
		node->val = replacement->val;
		node = replacement;
	}

	Node* parent = node->parent;
	avlDelete(node);
	rebalance(parent);
	return true;

}

bool AVLTree::avlDeleteForAll(int v)
{
	if (!root)
	{
		return false;
	}

	bool flag = false;
	while (Node* node = searchSubTree(root, v))
	{
		if (node->val != v)
		{
			if (flag)
			{
				break;
			}
			else
			{
				return false;
			}

		}

		flag = true;	//说明找到了值为v的节点，只要找到一次，就说明删除操作成功进行。当删除完值为v的节点之后，那么需要直接跳出while循环

		if (node->left && node->right)
		{
			Node* replacement = nullptr;
			if (node->left->height >= node->right->height)
			{
				replacement = subTreeLast(node->left);
			}
			else
			{
				replacement = subTreeFirst(node->right);
			}
			node->val = replacement->val;
			node = replacement;
		}

		Node* parent = node->parent;
		avlDelete(node);
		rebalance(parent);
	}
	return true;
}

#include<stack>
#include<vector>
/**
 * @brief 得到AVL树中序遍历下的从小到大的数组
 * @return 返回该数组
*/
vector<int> AVLTree::getInOrderNums()
{
	vector<int> res;
	stack<Node*> st;
	st.push(root);
	while (!st.empty())
	{
		Node* node = st.top();
		if (node)
		{
			st.pop();
			if (node->right) st.push(node->right);
			st.push(node);
			st.push(nullptr);
			if (node->left) st.push(node->left);
		}
		else
		{
			st.pop();
			node = st.top();
			st.pop();
			res.push_back(node->val);
		}
	}
	return res;
}

/**
 * @brief 对节点进行旋转操作，无论是左旋还是右旋，都是在这里操作，旋转的本质是改变某节点的左右指针的指向和另一个节点父指针的指向
 * @param node 进行旋转操作的节点
*/
void AVLTree::rotate(Node* node)
{
	Node* parent = node->parent;
	Node* grandParent = parent->parent;
	if (!grandParent)	 //不平衡的节点是root，所以寻求root左右子树中高度较高的子树，令其作为新的root
	{
		root = node;
		node->parent = nullptr;
	}
	else	//不平衡的节点不是root
	{
		relink(grandParent, node, parent == grandParent->left);
	}

	if (node == parent->left)	//如果node在parent的左边
	{
		relink(parent, node->right, true);
		relink(node, parent, false);
	}
	else	//如果node在parent的右边
	{
		relink(parent, node->left, false);
		relink(node, parent, true);
	}
}

/**
 * @brief AVL树的某节点删除，这个函数是一个private函数，外界不可直接调用，外界能使用的只是另一个重载函数avlDelete(int v)
 * @param node 要删除的节点
*/
void AVLTree::avlDelete(Node* node)
{
	//经过第一层的delete操作之后，按理来说要删除的节点不可能有左右孩子，所以这种情况直接返回
	if (node->left && node->right)
	{
		return;
	}

	Node* child = node->left ? node->left : node->right;

	if (child)
	{
		child->parent = node->parent;
	}

	if (node == root)
	{
		root = child;
	}
	else
	{
		Node* parent = node->parent;
		if (parent->left == node)	//node位于父节点的左边
		{
			parent->left = child;
		}
		else		//node位于父节点的右边
		{
			parent->right = child;
		}
	}
	node->parent = node;
	delete node;
}

/**
 * @brief 计算该节点的高height和size，height表示在树中节点的高度，是其左右子树中较高子树的高度加上它自身；
 *				size表示在树中该节点的左右子树的size之和加上它自身
 * @param node 要进行height和size计算的节点
*/
void AVLTree::calculate(Node* node)
{
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	node->size = getSize(node->left) + getSize(node->right) + 1;
}

/**
 * @brief 平衡AVL树
 *				每次插入，平衡的检查点都会从插入节点开始，往上检查节点的左右子树是否平衡；
 *				每次删除
 * @param node 检查平衡的起始节点
*/
void AVLTree::rebalance(Node* node)
{
	while (node)
	{
		int oldHeight = node->height;
		int oldSize = node->size;
		if (!isBalance(node))
		{
			node = restructure(childchildHeight(node));
			calculate(node->left);
			calculate(node->right);
		}
		calculate(node);
		if (node->height == oldHeight && node->size == oldSize)
		{
			node = nullptr;
		}
		else
		{
			node = node->parent;
		}
	}
}

/**
 * @brief 判断节点的左右子树是否平衡，平衡的条件是左右子树的高度之差的绝对值小于等于1
 * @param node 被检查平衡的节点
 * @return 如果节点左右子树平衡，返回true，否则返回false
*/
bool AVLTree::isBalance(Node* node)
{
	return abs(getHeight(node->left) - getHeight(node->right)) <= 1;
}

/**
 * @brief 得到node节点的size
 * @param node AVL树中的某个节点
 * @return 该节点的size
*/
int AVLTree::getSize(Node* node)
{
	return node != nullptr ? node->size : 0;
}

/**
 * @brief 得到node节点的height
 * @param node AVl树中的某个节点
 * @return 该节点的height
*/
int AVLTree::getHeight(Node* node)
{
	return node != nullptr ? node->height : 0;
}

/**
 * @brief 得到node节点的左右子树中较高子树的根节点
 * @param node AVL树中的某个节点
 * @return node节点的左右子树中较高子树的根节点
*/
Node* AVLTree::childHeight(Node* node)
{
	return (getHeight(node->left) > getHeight(node->right)) ? node->left : node->right;
}

/**
 * @brief 得到node节点的左右子树中较高子树的根节点中的左右子树中较高子树的根节点
 * @param node AVL树的某个节点
 * @return node节点的左右子树中较高子树的根节点中的左右子树中较高子树的根节点。这和childHeight比，得到的是再下一层的根节点
*/
Node* AVLTree::childchildHeight(Node* node)
{
	Node* child = childHeight(node);
	return childHeight(child);
}

/**
 * @brief 搜索AVL树中值为v的节点
 * @param node 值为v的节点
 * @param v 要搜索的值
 * @return 如果成功找到值为v的节点，则返回该节点；否则返回要查找的值插入到AVL树的父节点
*/
Node* AVLTree::searchSubTree(Node* node, int v)
{
	if (node->val < v && node->right)
	{
		return searchSubTree(node->right, v);
	}
	else if (node->val > v && node->left)
	{
		return searchSubTree(node->left, v);
	}
	else
	{
		return node;
	}
}

/**
 * @brief 如果某节点不平衡了，就会触发这个函数，要对node节点进行两次旋转操作或者是对node节点的父节点进行一次旋转操作
 *				LL情况：是指grandparent节点的左孩子是parent节点，parent节点的左孩子是node节点
 *				RR情况：是指grandparent节点的右孩子是parent节点，parent节点的右孩子是node节点
 *				LR情况：是指grandparent节点的左孩子是parent节点，parent节点的右孩子是node节点
 *				RL情况：是指grandparent节点的右孩子是parent节点，parent节点的左孩子是node节点
 * @param node 
 * @return 
*/
Node* AVLTree::restructure(Node* node)
{
	Node* parent = node->parent;
	Node* grandParent = parent->parent;
	if ((node == parent->left) == (parent == grandParent->left))	//如果是LL或RR情况，则只需要对parent节点旋转一次
	{
		rotate(parent);
		return parent;
	}
	else	//如果是LR或RL情况，则需要对node节点旋转两次
	{
		rotate(node);
		rotate(node);
		return node;
	}
}

/**
 * @brief 每一次旋转实际上是改变parent节点的左或右指针的指向，以及child节点的父指针的指向
 * @param parent 要改变左右指针的指向的节点
 * @param child 要改变父指针的指向的节点
 * @param existLeft parent节点左孩子是否存在，如果存在则parent节点左指针的指向变为node节点，如果不存在则parent节点右指针的指向变为node节点
*/
void AVLTree::relink(Node* parent, Node* child, bool existLeft)
{
	if (existLeft)
	{
		parent->left = child;
	}
	else
	{
		parent->right = child;
	}
	if (child)
	{
		child->parent = parent;
	}
}

/**
 * @brief 得到以node节点为根节点的子树中，第一个出现的节点，实际上就是avl树中序遍历下所得到的数组中的第一个值的节点
 * @param node 子树的根节点
 * @return 以node节点为根节点的子树中，第一个出现的节点
*/
Node* AVLTree::subTreeFirst(Node* node)
{
	while (node->left)
	{
		node = node->left;
	}
	return node;
}
