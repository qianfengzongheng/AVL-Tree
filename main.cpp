#include<iostream>
#include"AVLTree.h"
#include<vector>
#include<algorithm>
using namespace std;

void testForAVLTree()
{
	//随机生成100以内的数字，[0, 99]
	//srand((unsigned)time(0));	//通过时间来设置随机种子
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

	//查找第1位小的数字
	vector<int> nums({ 1, 2, 3, 4, 5, 6 });
	AVLTree oneTree(nums);
	int r = oneTree.avlSearch(1);
	if (r == -1)
	{
		cout << "没有找到你要找的数" << endl;
	}
	else
	{
		cout << "找第1个小的数：" << r << endl;
	}
	cout << endl;

	//插入值为3的节点
	oneTree.avlInsert(3);
	vector<int> res = oneTree.getInOrderNums();
	for (int i : res)
	{
		cout << i << " ";
	}
	cout << endl;

	//删除值为4的节点
	bool ret = oneTree.avlDeleteForOne(4);
	if (ret)
	{
		cout << "avlDeleteForOne删除成功" << endl;
	}
	else
	{
		cout << "avlDeleteForOne没删除成功" << endl;
	}
	res = oneTree.getInOrderNums();
	for (int i : res)
	{
		cout << i << " ";
	}
	cout << endl;

	//删除所有值为3的节点
	bool ret1 = oneTree.avlDeleteForAll(10);
	if (ret1)
	{
		cout << "avlDeleteForAll删除成功" << endl;
	}
	else
	{
		cout << "avlDeleteForAll没删除成功" << endl;
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