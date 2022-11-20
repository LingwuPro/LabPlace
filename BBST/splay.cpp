#include <iostream>
#define N 100000 + 5
using namespace std;
int _root, idex = 0;
struct Node
{
    int value;
    int leftchild;
    int rightchild;
    int parent;
    int sonSize;
    bool upTurnDown;

    void init(int val, int par)
    {
        value = val;
        parent = par;
        sonSize = 1;
    }
} treeNode[N];

void updateSize(int tnode)
{
    treeNode[tnode].sonSize = treeNode[treeNode[tnode].leftchild].sonSize + treeNode[treeNode[tnode].rightchild].sonSize + 1;
}

void msgDown(int tnode)
{
    if (treeNode[tnode].upTurnDown)
    {
        treeNode[treeNode[tnode].leftchild].upTurnDown ^= 1;
        treeNode[treeNode[tnode].rightchild].upTurnDown ^= 1;

        std::swap(treeNode[tnode].leftchild, treeNode[tnode].rightchild);
    }

    treeNode[tnode].upTurnDown = 0;
}

void zipzap(int tnode)
{
    int parent = treeNode[tnode].parent, grandParent = treeNode[parent].parent;
    bool isRightchild = (treeNode[parent].rightchild == tnode);

    if (treeNode[grandParent].rightchild == parent)
        treeNode[grandParent].rightchild = tnode;
    else
        treeNode[grandParent].leftchild = tnode;

    treeNode[tnode].parent = grandParent;

    if (isRightchild)
    {
        treeNode[parent].rightchild = treeNode[tnode].leftchild;
        treeNode[treeNode[tnode].leftchild].parent = parent;
        treeNode[tnode].leftchild = parent;
    }
    else
    {
        treeNode[parent].leftchild = treeNode[tnode].rightchild;
        treeNode[treeNode[tnode].rightchild].parent = parent;
        treeNode[tnode].rightchild = parent;
    }

    treeNode[parent].parent = tnode;
    // treeNode[parent].sonSize = treeNode[treeNode[parent].leftchild].sonSize + treeNode[treeNode[parent].rightchild].sonSize + 1;
    // treeNode[tnode].sonSize = treeNode[treeNode[tnode].leftchild].sonSize + treeNode[treeNode[tnode].rightchild].sonSize + 1;
    updateSize(parent);
    updateSize(tnode);
}

void splay(int tnode, int rank)
{
    int parent, grandparent;
    while (treeNode[tnode].parent != rank)
    {
        parent = treeNode[tnode].parent;
        grandparent = treeNode[parent].parent;
        if (grandparent != rank)
        {
            if (((treeNode[parent].rightchild == tnode) && (treeNode[grandparent].leftchild == parent)) || ((treeNode[parent].leftchild == tnode) && treeNode[grandparent].rightchild == parent))
            {
                zipzap(tnode);
            }
            else
            {
                zipzap(parent);
            }
        }
        zipzap(tnode);
    }
    if (!rank)
    {
        _root = tnode;
    }
}

void insert(int value)
{
    int place = _root, Parent = 0;
    while (place)
    {
        Parent = place;
        if (value > treeNode[place].value)
        {
            place = treeNode[place].rightchild;
        }
        else
        {
            place = treeNode[place].leftchild;
        }
    }
    place = ++idex;
    if (Parent)
    {
        if (value > treeNode[Parent].value)
            treeNode[Parent].rightchild = place;
        else
            treeNode[Parent].leftchild = place;
    }
    treeNode[place].init(value, Parent);
    splay(place, 0);
}

int rankFindNode(int rank)
{
    int place = _root;
    while (true)
    {
        msgDown(place);
        if (treeNode[treeNode[place].leftchild].sonSize >= rank)
            place = treeNode[place].leftchild;
        else if (treeNode[treeNode[place].leftchild].sonSize + 1 == rank)
            return place;
        else
        {
            rank -= treeNode[treeNode[place].leftchild].sonSize + 1;
            place = treeNode[place].rightchild;
        }
    }
    return 0;
}
int num;
void output(int tnode)
{
    msgDown(tnode);
    if (treeNode[tnode].leftchild)
        output(treeNode[tnode].leftchild);
    if (treeNode[tnode].value > 0 && treeNode[tnode].value < num + 1)
        cout << treeNode[tnode].value << ' ';
    if (treeNode[tnode].rightchild)
        output(treeNode[tnode].rightchild);
}

int main()
{
    int op, leftEdge, rightEdge;
    cin >> num >> op;
    for (int i = 0; i <= num + 1; i++)
    {
        insert(i);
    }

    for (int i = 0; i < op; i++)
    {
        cin >> leftEdge >> rightEdge;
        leftEdge = rankFindNode(leftEdge);
        rightEdge = rankFindNode(rightEdge + 2);
        splay(leftEdge, 0);
        splay(rightEdge, leftEdge);
        treeNode[treeNode[rightEdge].leftchild].upTurnDown = 1 - treeNode[treeNode[rightEdge].leftchild].upTurnDown;
    }
    output(_root);
    // for (int i = 1; i <= treeNode[_root].sonSize; i++)
    // {
    //     cout << treeNode[i].value << '|';
    // }
    return 0;
}