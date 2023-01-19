#ifndef _RBTREE_HPP_
#define _RBTREE_HPP_

#include "RBTreeNode.hpp"
#include <iostream>
using namespace std;

template<typename T>
void FreeTree(RBTreeNode<T>* curNode);

template<typename T>
class RBTree
{

public :
    RBTreeNode<T>* treeRoot;

public :
    RBTree();
    RBTree(RBTreeNode<T>* node);
    ~RBTree();

    friend void FreeTree(RBTreeNode<T>* curNode); // 释放红黑树的内存

    bool Insert(T data); // 插入节点
    bool RightRotate(RBTreeNode<T>* curNode); // 右旋
    bool LeftRotate(RBTreeNode<T>* curNode); // 左旋

    bool Get(T data); // 查看数据是否存在

    void ViewTree(RBTreeNode<T>* curNode); // 查看树的结构

    void Output(RBTreeNode<T>* curNode, bool left, string const& indent); // 可视化红黑树
    void ViewTreeByGraphic(RBTreeNode<T>* curNode); // 可视化红黑树
};

template<typename T>
RBTree<T>::RBTree() : treeRoot(nullptr)
{

}

template<typename T>
RBTree<T>::RBTree(RBTreeNode<T>* node) : treeRoot(node) {}

template<typename T>
RBTree<T>::~RBTree()
{
    FreeTree(treeRoot);
}

template<typename T>
void FreeTree(RBTreeNode<T>* curNode)
{
    if(curNode->right) FreeTree(curNode->right);
    if(curNode->left)  FreeTree(curNode->left);

    if(curNode != nullptr) // 回溯删除所有节点，释放堆上的内存
    {
        // cout << "This node is deleted : " << curNode->data << '\n';
        delete curNode;
        curNode = nullptr;
        return ;
    }
}

template<typename T>
bool RBTree<T>::Insert(T data)
{
    if(treeRoot == nullptr) // 1. 红黑树为空树的情况，将节点作为根节点，并且设置为黑色
    {
        treeRoot = new RBTreeNode<T>(data);
        treeRoot->color = Black;
        return 1;
    }

    RBTreeNode<T>* tmpTreeRoot = treeRoot;
    RBTreeNode<T>* curNode = new RBTreeNode<T>(data); // 当前需要插入的节点
    int flag = -1; // 标记是插入左边还是右边或者是值相等，左边用1标记，右边用2标记，相等用3标记。

    while(tmpTreeRoot != nullptr) // 查找需要插入节点的父节点
    {
        if(curNode->data < tmpTreeRoot->data) // 在当前节点的左边
        {
            if(tmpTreeRoot->left != nullptr) tmpTreeRoot = tmpTreeRoot->left; // 往左走
            else{
                flag = 1; // 左边
                break; // 如果左边是空的，那么该空的位置就应该插入新的节点，所以跳出
            }
        }
        else if(curNode->data > tmpTreeRoot->data)
        {
            if(tmpTreeRoot->right != nullptr) tmpTreeRoot = tmpTreeRoot->right; // 往右走
            else{
                flag = 2; // 右边
                break;
            }
        }
        else
        {
            tmpTreeRoot->data = curNode->data;
            flag = 3; // 相等
            break;
        }
    }

    // 查看插入时父节点的相关数据
    // cout << "Insert data = " << data << " flag = " << flag << " father data = " << tmpTreeRoot->data << " color = " << tmpTreeRoot->color << '\n';

    // 目前 tmpTreeRoot 为需要插入的节点的父节点
    if(flag == 3) return 1;  // 2. 插入点为Key已存在的情况

    if(flag == 1) // 插入节点
    {
        tmpTreeRoot->left = curNode;
        curNode->father = tmpTreeRoot;
    }
    else if(flag == 2)
    {
        tmpTreeRoot->right = curNode;
        curNode->father = tmpTreeRoot;
    }


    if(tmpTreeRoot->color == Black) return 1; // 3. 如果父节点为黑色，返回

    // cout << "hello\n";

    // 4. 插入节点的父节点为红色，需要修复

    RBTreeNode<T>* uncleNode = nullptr;

    while(  curNode->father != nullptr && // 当前节点不是根节点
            curNode->father->father != nullptr && // 当前节点有爷爷节点
            curNode->father->color == Red) // 当前节点的父节点是红色节点
    {

//        grandfather
//            / \ 
//           /   \ 
//      father    ?(4.1)
//        / \ 
//(4.2.1)?   ?(4.2.2)

        RBTreeNode<T>* fatherNode = curNode->father;
        RBTreeNode<T>* grandFatherNode = fatherNode->father;

        if(fatherNode == grandFatherNode->left) // 4.2.1 父节点为左子树
        {
            uncleNode = grandFatherNode->right;
            if(uncleNode != nullptr && uncleNode->color == Red) // 4.1 叔叔节点存在且为红色
            {
                fatherNode->color = Black;
                uncleNode->color = Black;
                if(grandFatherNode->father != nullptr) grandFatherNode->color = Red; // 如果为空那么该节点是父节点，不能是红色
                curNode = grandFatherNode;
            }
            else  // 4.2 叔叔节点为空节点或者为黑色节点
            {
                if(curNode == fatherNode->left) // 4.2.1.1 当前节点在父节点的左边，LL(双红的情况)
                // 1. 将父节点设置为黑色，将爷爷节点设置为红色
                // 2. 对爷爷节点进行右旋
                {
                    fatherNode->color = Black;
                    grandFatherNode->color = Red;
                    RightRotate(grandFatherNode); // 右旋
                }
                else if(curNode == fatherNode->right) // 4.2.1.2 当前节点的父节点的右边，LR双红
                {
                    curNode = curNode->father;
                    LeftRotate(curNode);
                }
            }
        }
        
//        grandfather
//            / \ 
//           /   \ 
//      ?(4.1)   father
//                / \ 
//        (4.2.1)?   ?(4.2.2)
        else // 4.2.2 父节点为右子树
        {
            uncleNode = grandFatherNode->left;
            if(uncleNode != nullptr && uncleNode->color == Red) 
// 4.1 叔叔节点存在，且为红色。
// 叔叔节点的判断位置有些乱，是因为理论要先能找到叔叔节点，
// 所以需要先进行4.2.1/2节的判断，也就是插入节点的父节点是爷爷节点的左孩子还是右孩子
            {
                fatherNode->color = Black;
                uncleNode->color = Black;
                if(grandFatherNode->father != nullptr) grandFatherNode->color = Red;
                curNode = grandFatherNode;
            }
            else
            {
                if(curNode == fatherNode->right) // 4.2.2.1 RR 双红
                {
                    fatherNode->color = Black;
                    grandFatherNode->color = Red;
                    LeftRotate(grandFatherNode);
                }
                else if(curNode == fatherNode->left)// 4.2.2.2 RL双红
                {
                    curNode = curNode->father;
                    RightRotate(curNode);
                }
            }
        }
    }
    return 1;
}

template<typename T>
bool RBTree<T>::RightRotate(RBTreeNode<T>* curNode)
{
    RBTreeNode<T>* curNodeLeft = curNode->left;
    RBTreeNode<T>* curNodeFather = curNode->father;

    // 操作1 ：把当前节点的左孩子换成当前节点的左节点的右孩子
    curNode->left = curNodeLeft->right;
    if(curNode->left != nullptr) curNode->left->father = curNode;  // 判断当前节点的左节点的右孩子是否为空

    // 操作2 ：把当前节点的作为当前节点的左节点的右孩子
    curNodeLeft->right = curNode;
    curNode->father = curNodeLeft;

    // 操作3 ：把当前节点的父节点与当前节点的左节点进行连接

    if(curNodeFather == nullptr) // curNodeLeft为根节点
    {
        curNodeLeft->father = curNodeFather;
        treeRoot = curNodeLeft; // 这里需要注意，需要把根节点更新
    }
    else if(curNodeFather->left == curNode)
    {
        curNodeFather->left = curNodeLeft;
        curNodeLeft->father = curNodeFather;
    }
    else
    {
        curNodeFather->right = curNodeLeft;
        curNodeLeft->father = curNodeFather;
    }

    return 1;
}

template<typename T>
bool RBTree<T>::LeftRotate(RBTreeNode<T>* curNode)
{
    RBTreeNode<T>* curNodeRight = curNode->right;
    RBTreeNode<T>* curNodeFather = curNode->father;

    // 操作1 ：把当前节点的右孩子换成当前节点的右节点的左孩子
    curNode->right = curNodeRight->left;
    if(curNode->right != nullptr) curNode->right->father = curNode;

    // 操作2 ：把当前节点的作为当前节点的右节点的左孩子
    curNodeRight->left = curNode;
    curNode->father = curNodeRight;

    // 操作3 ：把当前节点的父节点与当前节点的右节点进行连接
    if(curNodeFather == nullptr) // curNodeRight 为根节点
    {
        curNodeRight->father = curNodeFather;
        treeRoot = curNodeRight; // 更新根节点
        // cout << "root data = " << treeRoot->data << '\n';
    }
    else if(curNodeFather->right == curNode) // 当前节点为当前节点父节点的右孩子
    {
        curNodeFather->right = curNodeRight;
        curNodeRight->father = curNodeFather;
    }
    else // 当前节点为当前节点父节点的左孩子
    {
        curNodeFather->left = curNodeRight;
        curNodeRight->father = curNodeFather;
    }
}

template<typename T>
bool RBTree<T>::Get(T data)
{
    RBTreeNode<T>* tmpRoot = treeRoot;
    while(tmpRoot != nullptr)
    {
        if(data < tmpRoot->data) // 往左
        {
            if(tmpRoot->left != nullptr) tmpRoot = tmpRoot->left;
            else break;
        }
        else if(data > tmpRoot->data) // 往右
        {
            if(tmpRoot->right != nullptr) tmpRoot = tmpRoot->right;
            else break;
        }
        else // 找到了
        {
            return 1;
        }
    } 
    return 0; // 未找到
}

template<typename T>
void RBTree<T>::ViewTree(RBTreeNode<T>* curNode) // 线序遍历打印红色树查看正确性
{
    cout << curNode->data << ' ';
    if(curNode->left != nullptr) ViewTree(curNode->left);
    if(curNode->right != nullptr) ViewTree(curNode->right);
}

template<typename T>
void RBTree<T>::Output(RBTreeNode<T>* curNode, bool left, string const& indent)
{
    if (curNode->right)
    {
        Output(curNode->right, false, indent + (left ? "|      " : "       "));
    }
    cout << indent;
    cout << (left ? '\\' : '/');
    cout << "-----";
    cout << curNode->data << "(" << (curNode->color == Red ? 'R' : 'B') << ")" << '\n';
    if(curNode->left)
    {
        Output(curNode->left, true, indent + (left ? "       " : "|      "));
    }
    
}

template<typename T>
void RBTree<T>::ViewTreeByGraphic(RBTreeNode<T>* curNode)
{
    cout << "\n\n-------------------- Red Black Tree ------------------------";
    cout << "\n--------------- B : Black Node R : Red Node -------------------\n\n\n";
    if(curNode->right)
    {
        Output(curNode->right, false, "");
    }
    cout << curNode->data <<  "(" << (curNode->color == Red ? 'R' : 'B') << ")" <<  '\n';
    if(curNode->left)
    {
        Output(curNode->left, true, "");
    }
    cout << "\n\n------------------------------------------------------------\n";
}

#endif