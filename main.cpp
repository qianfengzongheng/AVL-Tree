#include<iostream>
#include"AVLTree.h"
#include<vector>
#include<algorithm>
using namespace std;

void testForAVLTree()
{
	//�������100���ڵ����֣�[0, 99]
	//srand((unsigned)time(0));	//ͨ��ʱ���������������
	//vector<int> nums(20);
	//for (int i = 0; i < 20; ++i)
	//{
	//	nums[i] = rand() % 100;
	//}
	//sort(nums.begin(), nums.end());
	//for (int i : nums)
	//{
	//	cout << i << " ";
	//}
	//cout << endl;

	//���ҵ�1λС������
	vector<int> nums({ 1, 2, 3, 4, 5, 6 });
	AVLTree oneTree(nums);
	int r = oneTree.avlSearch(1);
	if (r == -1)
	{
		cout << "û���ҵ���Ҫ�ҵ���" << endl;
	}
	else
	{
		cout << "�ҵ�1��С������" << r << endl;
	}
	cout << endl;

	//����ֵΪ3�Ľڵ�
	oneTree.avlInsert(3);
	vector<int> res = oneTree.getInOrderNums();
	for (int i : res)
	{
		cout << i << " ";
	}
	cout << endl;

	//ɾ��ֵΪ4�Ľڵ�
	bool ret = oneTree.avlDeleteForOne(4);
	if (ret)
	{
		cout << "avlDeleteForOneɾ���ɹ�" << endl;
	}
	else
	{
		cout << "avlDeleteForOneûɾ���ɹ�" << endl;
	}
	res = oneTree.getInOrderNums();
	for (int i : res)
	{
		cout << i << " ";
	}
	cout << endl;

	//ɾ������ֵΪ3�Ľڵ�
	bool ret1 = oneTree.avlDeleteForAll(10);
	if (ret1)
	{
		cout << "avlDeleteForAllɾ���ɹ�" << endl;
	}
	else
	{
		cout << "avlDeleteForAllûɾ���ɹ�" << endl;
	}
	res = oneTree.getInOrderNums();
	for (int i : res)
	{
		cout << i << " ";
	}
	cout << endl;
}

int main()
{
	testForAVLTree();

	return 0;
}