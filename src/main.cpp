#include<iostream>
#include<cstring>
#include<sstream>

#include"RBTree.hpp"
#include"RBTreeNode.hpp"

using namespace std;

int a[30] = {0, 12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17};

int main()
{
    RBTree<int> Tree;
    cout << "\nOrder of inserting of all nodes.\n\n";
    for(int i = 1; i <= 20; i++ )
    {
        cout << a[i] << ' ';
        Tree.Insert(a[i]); // 插入节点
    }
    cout << '\n';
    cout << "\nPre-order traversal of red-black tree : \n\n";
    Tree.ViewTree(Tree.treeRoot); // 红黑树的线序遍历
    Tree.ViewTreeByGraphic(Tree.treeRoot); // 可视化红黑树
    // for(int i = 1; i <= 20; i++ )
    // {
    //     cout << i << " : " << Tree.Get(a[i]) << '\n';
    // }
}