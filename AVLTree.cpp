#include "AVLTree.h"

/**
 * @brief �ҵ���node�ڵ�Ϊ���ڵ�������У����һ�����ֵĽڵ㣬ʵ���Ͼ���avl��������������õ��������е����һ��ֵ�Ľڵ�
 * @param node �����ĸ��ڵ�
 * @return ��������һ���ڵ�
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
 * @brief AVL���Ĺ��캯����ͨ��һ����С������������飬����һ��AVL��
 * @param vec ��С�������������
*/
AVLTree::AVLTree(vector<int>& vec)
{
	if (!vec.empty())
	{
		root = build(vec, 0, vec.size() - 1, nullptr);
	}
}

/**
 * @brief AVL�����캯�����������õĽ���������ͨ������Ĵ�С������������齨AVL��
 * @param vec ��С�������������
 * @param left �ֶ�����������±�
 * @param right �ֶ�����������±�
 * @param parent �����������½ڵ�ĸ��ڵ�
 * @return �������ɹ������ظ�AVL���ĸ��ڵ�root�����򷵻�nullptr
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
 * @brief Ѱ�ҵ�k��С��ֵ��������������ۣ�
 *				1. ��ǰ�ڵ�����������ܽڵ���С��k-1��˵����k��С��ֵһ�������ڵ�ǰ�ڵ������������Ϊ��ǰ�ڵ�����������ܽڵ���+��ǰ�ڵ�<=k-1��ת�Ƶ���������ʱ��Ҫ���ȥ1����ΪҪ�ѵ�ǰ�ڵ�Ҳ��ȥ
 *				2. ��ǰ�ڵ���������ܽڵ�������k-1��˵����ǰ�ڵ����Ҫ�ҵĵ�k��С��ֵ��
 *				3. ��ǰ�ڵ���������ܽڵ�������k-1��˵����k��С��ֵ���ڵ�ǰ�ڵ���������С�
 * @param k Ҫ�ҵĵ�k��С������1 <= k <= n��n��ʾAVL���Ľڵ���
 * @return �ҵ��������������λ�õ�ֵ�����򷵻�-1
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
		else if (leftNum == k - 1)//�ҵ��ˣ���ǰ�ڵ���ǵ�k��С����
		{
			break;
		}
		else
		{
			node = node->left;
		}
	}
	return node ? node->val : -1;//�Ҳ����ͷ���-1
}

/**
 * @brief ��AVL���в���һ���ڵ㣬����������㣬�����Ȱ�ֵΪv�Ľڵ���뵽������
 * @param v ������ڵ��ֵ
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

		//����½ڵ�
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
 * @brief ɾ��1��ֵΪv�Ľڵ㣬�����vֵ�Ľڵ���ڶ������ֻ��ɾ������һ��
 * @param v Ҫɾ���Ľڵ��ֵ
 * @return ɾ���ɹ�����true�����򷵻�false
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

		flag = true;	//˵���ҵ���ֵΪv�Ľڵ㣬ֻҪ�ҵ�һ�Σ���˵��ɾ�������ɹ����С���ɾ����ֵΪv�Ľڵ�֮����ô��Ҫֱ������whileѭ��

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
 * @brief �õ�AVL����������µĴ�С���������
 * @return ���ظ�����
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
 * @brief �Խڵ������ת�������������������������������������������ת�ı����Ǹı�ĳ�ڵ������ָ���ָ�����һ���ڵ㸸ָ���ָ��
 * @param node ������ת�����Ľڵ�
*/
void AVLTree::rotate(Node* node)
{
	Node* parent = node->parent;
	Node* grandParent = parent->parent;
	if (!grandParent)	 //��ƽ��Ľڵ���root������Ѱ��root���������и߶Ƚϸߵ�������������Ϊ�µ�root
	{
		root = node;
		node->parent = nullptr;
	}
	else	//��ƽ��Ľڵ㲻��root
	{
		relink(grandParent, node, parent == grandParent->left);
	}

	if (node == parent->left)	//���node��parent�����
	{
		relink(parent, node->right, true);
		relink(node, parent, false);
	}
	else	//���node��parent���ұ�
	{
		relink(parent, node->left, false);
		relink(node, parent, true);
	}
}

/**
 * @brief AVL����ĳ�ڵ�ɾ�������������һ��private��������粻��ֱ�ӵ��ã������ʹ�õ�ֻ����һ�����غ���avlDelete(int v)
 * @param node Ҫɾ���Ľڵ�
*/
void AVLTree::avlDelete(Node* node)
{
	//������һ���delete����֮�󣬰�����˵Ҫɾ���Ľڵ㲻���������Һ��ӣ������������ֱ�ӷ���
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
		if (parent->left == node)	//nodeλ�ڸ��ڵ�����
		{
			parent->left = child;
		}
		else		//nodeλ�ڸ��ڵ���ұ�
		{
			parent->right = child;
		}
	}
	node->parent = node;
	delete node;
}

/**
 * @brief ����ýڵ�ĸ�height��size��height��ʾ�����нڵ�ĸ߶ȣ��������������нϸ������ĸ߶ȼ���������
 *				size��ʾ�����иýڵ������������size֮�ͼ���������
 * @param node Ҫ����height��size����Ľڵ�
*/
void AVLTree::calculate(Node* node)
{
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	node->size = getSize(node->left) + getSize(node->right) + 1;
}

/**
 * @brief ƽ��AVL��
 *				ÿ�β��룬ƽ��ļ��㶼��Ӳ���ڵ㿪ʼ�����ϼ��ڵ�����������Ƿ�ƽ�⣻
 *				ÿ��ɾ��
 * @param node ���ƽ�����ʼ�ڵ�
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
 * @brief �жϽڵ�����������Ƿ�ƽ�⣬ƽ������������������ĸ߶�֮��ľ���ֵС�ڵ���1
 * @param node �����ƽ��Ľڵ�
 * @return ����ڵ���������ƽ�⣬����true�����򷵻�false
*/
bool AVLTree::isBalance(Node* node)
{
	return abs(getHeight(node->left) - getHeight(node->right)) <= 1;
}

/**
 * @brief �õ�node�ڵ��size
 * @param node AVL���е�ĳ���ڵ�
 * @return �ýڵ��size
*/
int AVLTree::getSize(Node* node)
{
	return node != nullptr ? node->size : 0;
}

/**
 * @brief �õ�node�ڵ��height
 * @param node AVl���е�ĳ���ڵ�
 * @return �ýڵ��height
*/
int AVLTree::getHeight(Node* node)
{
	return node != nullptr ? node->height : 0;
}

/**
 * @brief �õ�node�ڵ�����������нϸ������ĸ��ڵ�
 * @param node AVL���е�ĳ���ڵ�
 * @return node�ڵ�����������нϸ������ĸ��ڵ�
*/
Node* AVLTree::childHeight(Node* node)
{
	return (getHeight(node->left) > getHeight(node->right)) ? node->left : node->right;
}

/**
 * @brief �õ�node�ڵ�����������нϸ������ĸ��ڵ��е����������нϸ������ĸ��ڵ�
 * @param node AVL����ĳ���ڵ�
 * @return node�ڵ�����������нϸ������ĸ��ڵ��е����������нϸ������ĸ��ڵ㡣���childHeight�ȣ��õ���������һ��ĸ��ڵ�
*/
Node* AVLTree::childchildHeight(Node* node)
{
	Node* child = childHeight(node);
	return childHeight(child);
}

/**
 * @brief ����AVL����ֵΪv�Ľڵ�
 * @param node ֵΪv�Ľڵ�
 * @param v Ҫ������ֵ
 * @return ����ɹ��ҵ�ֵΪv�Ľڵ㣬�򷵻ظýڵ㣻���򷵻�Ҫ���ҵ�ֵ���뵽AVL���ĸ��ڵ�
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
 * @brief ���ĳ�ڵ㲻ƽ���ˣ��ͻᴥ�����������Ҫ��node�ڵ����������ת���������Ƕ�node�ڵ�ĸ��ڵ����һ����ת����
 *				LL�������ָgrandparent�ڵ��������parent�ڵ㣬parent�ڵ��������node�ڵ�
 *				RR�������ָgrandparent�ڵ���Һ�����parent�ڵ㣬parent�ڵ���Һ�����node�ڵ�
 *				LR�������ָgrandparent�ڵ��������parent�ڵ㣬parent�ڵ���Һ�����node�ڵ�
 *				RL�������ָgrandparent�ڵ���Һ�����parent�ڵ㣬parent�ڵ��������node�ڵ�
 * @param node 
 * @return 
*/
Node* AVLTree::restructure(Node* node)
{
	Node* parent = node->parent;
	Node* grandParent = parent->parent;
	if ((node == parent->left) == (parent == grandParent->left))	//�����LL��RR�������ֻ��Ҫ��parent�ڵ���תһ��
	{
		rotate(parent);
		return parent;
	}
	else	//�����LR��RL���������Ҫ��node�ڵ���ת����
	{
		rotate(node);
		rotate(node);
		return node;
	}
}

/**
 * @brief ÿһ����תʵ�����Ǹı�parent�ڵ�������ָ���ָ���Լ�child�ڵ�ĸ�ָ���ָ��
 * @param parent Ҫ�ı�����ָ���ָ��Ľڵ�
 * @param child Ҫ�ı丸ָ���ָ��Ľڵ�
 * @param existLeft parent�ڵ������Ƿ���ڣ����������parent�ڵ���ָ���ָ���Ϊnode�ڵ㣬�����������parent�ڵ���ָ���ָ���Ϊnode�ڵ�
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
 * @brief �õ���node�ڵ�Ϊ���ڵ�������У���һ�����ֵĽڵ㣬ʵ���Ͼ���avl��������������õ��������еĵ�һ��ֵ�Ľڵ�
 * @param node �����ĸ��ڵ�
 * @return ��node�ڵ�Ϊ���ڵ�������У���һ�����ֵĽڵ�
*/
Node* AVLTree::subTreeFirst(Node* node)
{
	while (node->left)
	{
		node = node->left;
	}
	return node;
}
