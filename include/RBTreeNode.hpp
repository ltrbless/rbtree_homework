#ifndef _RBTREENODE_HPP_
#define _RBTREENODE_HPP_


enum Color  // 1 is red，0 is black node
{
    Black = 0, 
    Red
};


template<typename T> 
class RBTreeNode
{

public :
    T data;
    Color color; // 1 代表红色， 0 代表黑色
    RBTreeNode* father;  // 父节点
    RBTreeNode* left;    // 左孩子
    RBTreeNode* right;   // 右孩子

public :
    RBTreeNode(); 
    void RBTreeNodeSet(T data); // 配合默认构造函数使用，使用默认构造函数没有初始值，但每个节点必须包含值，所以该函数可以另外设置初始值或者修改初始值

    RBTreeNode(T data);
    ~RBTreeNode();    
};

template<typename T>
RBTreeNode<T>::RBTreeNode() : color(Red), father(nullptr), left(nullptr), right(nullptr)
{

}

template<typename T>
RBTreeNode<T>::RBTreeNode(T data) : data(data), color(Red) , father(nullptr), left(nullptr), right(nullptr)
{

}

template<typename T>
void RBTreeNode<T>::RBTreeNodeSet(T data)
{
    this->data = data;
}

template<typename T>
RBTreeNode<T>::~RBTreeNode()
{
    this->father = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

#endif