#include <iostream>
using namespace std;

class RBtree
{
public:
    struct Node;
    Node *_root;
    Node *_hot;
    struct nodeIter;

    void init(int value);
    void connect34(Node *root, Node *leftchild, Node *rightchild, Node *tree1, Node *tree2, Node *tree3, Node *tree4);
    void DoubleRed(Node *root);
    void DoubleBlack(Node *root);
    Node *find(int value, const int rvalue);
    Node *noRepFind(int value, const int rvalue);
    Node *rankFindNode(int, Node *);
    int findRank(int rvalue, Node *root);

    RBtree() : _root(nullptr), _hot(nullptr) {}

    int valueFindRank(int rvalue);
    nodeIter insert(int value);
    bool remove(int rvalue);
    int size();
    bool empty();
    nodeIter rankFindValue(int rank);
    nodeIter lowNext(int value);
    nodeIter bigNext(int value);
};

struct RBtree::Node
{
    int value;
    bool RBcolor;
    Node *parent;
    Node *leftchild;
    Node *rightchild;
    int sonSize;

    Node(int val = 0, bool color = true, Node *ptr = nullptr, Node *lc = nullptr, Node *rc = nullptr, int rank = 1) : value(val), RBcolor(color), parent(ptr), leftchild(lc), rightchild(rc), sonSize(rank) {}

    Node *succ()
    {
        Node *nextNode = rightchild;
        while (nextNode->leftchild != nullptr)
        {
            nextNode->sonSize--;
            nextNode = nextNode->leftchild;
        }
        return nextNode;
    }

    Node *aboveRank()
    {
        Node *above = this;
        if (leftchild)
        {
            above = above->leftchild;
            while (above->rightchild)
            {
                above = above->rightchild;
            }
        }
        else
        {
            while (above->parent && above->parent->leftchild == above)
                above = above->parent;
            above = above->parent;
        }
        return above;
    }
    Node *underRank()
    {
        Node *under = this;
        if (rightchild)
        {
            under = under->rightchild;
            while (under->leftchild)
            {
                under = under->leftchild;
            }
        }
        else
        {
            while (under->parent && under->parent->rightchild == under)
                under = under->parent;
            under = under->parent;
        }
        return under;
    }
    void updatesonSize()
    {
        sonSize = 1;
        if (leftchild)
        {
            sonSize += leftchild->sonSize;
        }
        if (rightchild)
        {
            sonSize += rightchild->sonSize;
        }
    }
};

struct RBtree::nodeIter
{
    Node *nodePlace;

    nodeIter &operator++()
    {
        nodePlace = nodePlace->underRank();
        return *this;
    }

    nodeIter &operator--()
    {
        nodePlace = nodePlace->aboveRank();
        return *this;
    }

    int operator*()
    {
        return nodePlace->value;
    }
    nodeIter(Node *nodeIn = nullptr) : nodePlace(nodeIn) {}
    nodeIter(nodeIter const &iter) : nodePlace(iter.nodePlace) {}
};

int RBtree::size()
{
    return _root->sonSize;
}

bool RBtree::empty()
{
    return !_root;
}

void RBtree::init(int value)
{
    _root = new Node(value, false, nullptr, nullptr, nullptr, 1);
}

RBtree::Node *RBtree::find(int rvalue, int sonSizeAdd)
{
    Node *temp1 = _root;
    _hot = nullptr;
    while (temp1)
    {
        _hot = temp1;
        temp1->sonSize += sonSizeAdd;
        if (temp1->value > rvalue)
            temp1 = temp1->leftchild;
        else
            temp1 = temp1->rightchild;
    }
    return temp1;
}

RBtree::Node *RBtree::noRepFind(int rvalue, int sonSizeAdd)
{
    Node *temp1 = _root;
    _hot = nullptr;
    while (temp1 && temp1->value != rvalue)
    {
        _hot = temp1;
        temp1->sonSize += sonSizeAdd;
        if (temp1->value > rvalue)
        {
            temp1 = temp1->leftchild;
        }
        else
            temp1 = temp1->rightchild;
    }
    return temp1;
}

void RBtree::connect34(Node *nroot, Node *nleftchild, Node *nrightchild,
                       Node *ntree1, Node *ntree2, Node *ntree3, Node *ntree4)
{
    nleftchild->leftchild = ntree1;

    if (ntree1 != nullptr)
        ntree1->parent = nleftchild;

    nleftchild->rightchild = ntree2;

    if (ntree2 != nullptr)
        ntree2->parent = nleftchild;

    nrightchild->leftchild = ntree3;

    if (ntree3 != nullptr)
        ntree3->parent = nrightchild;

    nrightchild->rightchild = ntree4;

    if (ntree4 != nullptr)
        ntree4->parent = nrightchild;

    nroot->leftchild = nleftchild;
    nleftchild->parent = nroot;
    nroot->rightchild = nrightchild;
    nrightchild->parent = nroot;

    nleftchild->updatesonSize();
    nrightchild->updatesonSize();
    nroot->updatesonSize();
}

void RBtree::DoubleRed(Node *root)
{
    Node *nodeParent, *uncle, *grandParent;
    while ((!root->parent) || root->parent->RBcolor)
    {
        if (root == _root)
        {
            _root->RBcolor = false;
            return;
        }
        nodeParent = root->parent;
        if (!nodeParent->RBcolor) // no double red to deal with
            return;
        grandParent = root->parent->parent;
        uncle = (grandParent->leftchild == root->parent) ? (grandParent->rightchild) : (grandParent->leftchild);
        if (uncle && uncle->RBcolor)
        {
            grandParent->RBcolor = true;
            uncle->RBcolor = false;
            nodeParent->RBcolor = false;
            root = grandParent;
        }
        else
        {
            if (nodeParent && grandParent->leftchild == nodeParent)
            {
                if (root && root->parent->leftchild == root)
                {
                    nodeParent->parent = grandParent->parent;
                    if (grandParent == _root)
                    {
                        _root = nodeParent;
                    }
                    else if (grandParent->parent->leftchild == grandParent)
                    {
                        grandParent->parent->leftchild = nodeParent;
                    }
                    else
                    {
                        grandParent->parent->rightchild = nodeParent;
                    }
                    connect34(nodeParent, root, grandParent, root->leftchild, root->rightchild, nodeParent->rightchild, uncle);
                    nodeParent->RBcolor = false;
                    grandParent->RBcolor = true;
                }
                else
                {
                    root->parent = grandParent->parent;
                    if (grandParent == _root)
                        _root = root;
                    else if (grandParent->parent->leftchild == grandParent)
                        grandParent->parent->leftchild = root;
                    else
                        grandParent->parent->rightchild = root;
                    connect34(root, nodeParent, grandParent, nodeParent->leftchild, root->leftchild, root->rightchild, uncle);
                    root->RBcolor = false;
                    grandParent->RBcolor = true;
                }
            }
            else
            {
                if (root && root->parent->leftchild == root)
                {
                    root->parent = grandParent->parent;
                    if (grandParent == _root)
                        _root = root;
                    else if (grandParent->parent->leftchild == grandParent)
                    {
                        grandParent->parent->leftchild = root;
                    }
                    else
                        grandParent->parent->rightchild = root;
                    connect34(root, grandParent, nodeParent, uncle, root->leftchild, root->rightchild, nodeParent->rightchild);
                    root->RBcolor = false;
                    grandParent->RBcolor = true;
                }
                else
                {
                    nodeParent->parent = grandParent->parent;
                    if (grandParent == _root)
                    {
                        _root = nodeParent;
                    }
                    else if (grandParent->parent->leftchild == grandParent)
                        grandParent->parent->leftchild = nodeParent;
                    else
                        grandParent->parent->rightchild = nodeParent;

                    connect34(nodeParent, grandParent, root, uncle, nodeParent->leftchild, root->leftchild, root->rightchild);
                    nodeParent->RBcolor = false;
                    grandParent->RBcolor = true;
                }
            }
            return;
        }
    }
}

// void RBtree::DoubleRed(Node *nn)
// {
//     while ((!(nn->parent)) || nn->parent->RBcolor)
//     {
//         if (nn == _root)
//         {
//             _root->RBcolor = false;
//             return;
//         }
//         Node *nodeParent = nn->parent;
//         if (!(nodeParent->RBcolor))
//             return; ////No double-red
//         Node *uncle = (nn->parent->parent->leftchild == nn->parent) ? (nn->parent->parent->rightchild) : (nn->parent->parent->leftchild);
//         Node *grandParent = nn->parent->parent;
//         if (uncle != NULL && uncle->RBcolor)
//         { ////RR-2
//             grandParent->RBcolor = true;
//             uncle->RBcolor = false;
//             nodeParent->RBcolor = false;
//             nn = grandParent;
//         }
//         else
//         { ////RR-1
//             if (nodeParent != NULL && nodeParent->parent->leftchild == nodeParent)
//             {
//                 if (nn != NULL && nn->parent->leftchild == nn)
//                 {
//                     nodeParent->parent = grandParent->parent;
//                     if (grandParent == _root)
//                         _root = nodeParent;
//                     else if (grandParent->parent->leftchild == grandParent)
//                         grandParent->parent->leftchild = nodeParent;
//                     else
//                         grandParent->parent->rightchild = nodeParent;
//                     connect34(nodeParent, nn, grandParent, nn->leftchild, nn->rightchild, nodeParent->rightchild, uncle);
//                     nodeParent->RBcolor = false;
//                     grandParent->RBcolor = true;
//                 }
//                 else
//                 {
//                     nn->parent = grandParent->parent;
//                     if (grandParent == _root)
//                         _root = nn;
//                     else if (grandParent->parent->leftchild == grandParent)
//                         grandParent->parent->leftchild = nn;
//                     else
//                         grandParent->parent->rightchild = nn;
//                     connect34(nn, nodeParent, grandParent, nodeParent->leftchild, nn->leftchild, nn->rightchild, uncle);
//                     nn->RBcolor = false;
//                     grandParent->RBcolor = true;
//                 }
//             }
//             else
//             {
//                 if (nn != NULL && nn->parent->leftchild == nn)
//                 {
//                     nn->parent = grandParent->parent;
//                     if (grandParent == _root)
//                         _root = nn;
//                     else if (grandParent->parent->leftchild == grandParent)
//                         grandParent->parent->leftchild = nn;
//                     else
//                         grandParent->parent->rightchild = nn;
//                     connect34(nn, grandParent, nodeParent, uncle, nn->leftchild, nn->rightchild, nodeParent->rightchild);
//                     nn->RBcolor = false;
//                     grandParent->RBcolor = true;
//                 }
//                 else
//                 {
//                     nodeParent->parent = grandParent->parent;
//                     if (grandParent == _root)
//                         _root = nodeParent;
//                     else if (grandParent->parent->leftchild == grandParent)
//                         grandParent->parent->leftchild = nodeParent;
//                     else
//                         grandParent->parent->rightchild = nodeParent;
//                     connect34(nodeParent, grandParent, nn, uncle, nodeParent->leftchild, nn->leftchild, nn->rightchild);
//                     nodeParent->RBcolor = false;
//                     grandParent->RBcolor = true;
//                 }
//             }
//             return;
//         }
//     }
// }

RBtree::nodeIter RBtree::insert(int value)
{
    Node *temp1 = find(value, 1);
    if (_hot == nullptr)
    {
        init(value);
        return nodeIter(_root);
    }
    temp1 = new Node(value, true, _hot, nullptr, nullptr, 1);
    if (_hot->value <= value)
        _hot->rightchild = temp1;
    else
        _hot->leftchild = temp1;
    DoubleRed(temp1);
    return nodeIter(temp1);
}

RBtree::nodeIter RBtree::lowNext(int value)
{
    Node *temp1 = _root;
    while (temp1)
    {
        _hot = temp1;
        if (temp1->value < value)
        {
            temp1 = temp1->rightchild;
        }
        else
        {
            temp1 = temp1->leftchild;
        }
    }
    if (_hot->value < value)
    {
        temp1 = _hot;
    }
    else
    {
        temp1 = _hot->aboveRank();
    }
    return nodeIter(temp1);
}

RBtree::nodeIter RBtree::bigNext(int value)
{
    Node *temp1 = _root;
    while (temp1)
    {
        _hot = temp1;
        if (temp1->value > value)
        {
            temp1 = temp1->leftchild;
        }
        else
        {
            temp1 = temp1->rightchild;
        }
    }
    if (_hot->value > value)
    {
        temp1 = _hot;
    }
    else
    {
        temp1 = _hot->underRank();
    }
    return nodeIter(temp1);
}

RBtree::Node *RBtree::rankFindNode(int rank, Node *root)
{
    if (!root->leftchild)
    {
        if (rank == 1)
        {
            return root;
        }
        else
        {
            return rankFindNode(rank - 1, root->rightchild);
        }
    }
    else
    {
        if (root->leftchild->sonSize == rank - 1)
            return root;
        else if (root->leftchild->sonSize >= rank)
            return rankFindNode(rank, root->leftchild);
        else
            return rankFindNode(rank - root->leftchild->sonSize - 1, root->rightchild);
    }
}
RBtree::nodeIter RBtree::rankFindValue(int rank)
{
    return nodeIter(rankFindNode(rank, _root));
}

int RBtree::findRank(int value, Node *root)
{
    if (!root)
    {
        return 1;
    }
    else if (root->value >= value)
        return findRank(value, root->leftchild);
    else
        return (1 + ((root->leftchild) ? (root->leftchild->sonSize) : 0) + findRank(value, root->rightchild));
}
int RBtree::valueFindRank(int value)
{
    return findRank(value, _root);
}

void RBtree::DoubleBlack(Node *root)
{
    while (root != _root)
    {
        Node *nodeParent = root->parent;
        Node *brother = (root->parent->leftchild == root) ? (root->parent->rightchild) : (root->parent->leftchild);
        if (brother->RBcolor)
        { ////BB-1
            brother->RBcolor = false;
            nodeParent->RBcolor = true;
            if (_root == nodeParent)
                _root = brother;
            if (nodeParent->parent)
            {
                if (nodeParent->parent->leftchild == nodeParent)
                    nodeParent->parent->leftchild = brother;
                else
                    nodeParent->parent->rightchild = brother;
            }
            brother->parent = nodeParent->parent;
            if (root != NULL && root->parent->leftchild == root)
            {
                connect34(brother, nodeParent, brother->rightchild, root, brother->leftchild, brother->rightchild->leftchild, brother->rightchild->rightchild);
            }
            else
            {
                connect34(brother, brother->leftchild, nodeParent, brother->leftchild->leftchild, brother->leftchild->rightchild, brother->rightchild, root);
            }
            brother = (root->parent->leftchild == root) ? (root->parent->rightchild) : (root->parent->leftchild);
            nodeParent = root->parent;
        }
        if (brother->leftchild && brother->leftchild->RBcolor)
        { ////BB-3
            bool oldRBc = nodeParent->RBcolor;
            nodeParent->RBcolor = false;
            if (nodeParent->leftchild == root)
            {
                if (nodeParent->parent)
                {
                    if (nodeParent->parent->leftchild == nodeParent)
                        nodeParent->parent->leftchild = brother->leftchild;
                    else
                        nodeParent->parent->rightchild = brother->leftchild;
                }
                brother->leftchild->parent = nodeParent->parent;
                if (_root == nodeParent)
                    _root = brother->leftchild;
                connect34(brother->leftchild, nodeParent, brother, nodeParent->leftchild, brother->leftchild->leftchild, brother->leftchild->rightchild, brother->rightchild);
            }
            else
            {
                brother->leftchild->RBcolor = false;
                if (nodeParent->parent)
                {
                    if (nodeParent->parent->leftchild == nodeParent)
                        nodeParent->parent->leftchild = brother;
                    else
                        nodeParent->parent->rightchild = brother;
                }
                brother->parent = nodeParent->parent;
                if (_root == nodeParent)
                    _root = brother;
                connect34(brother, brother->leftchild, nodeParent, brother->leftchild->leftchild, brother->leftchild->rightchild, brother->rightchild, nodeParent->rightchild);
            }
            nodeParent->parent->RBcolor = oldRBc;
            return;
        }
        else if (brother->rightchild && brother->rightchild->RBcolor)
        { ////BB-3
            bool oldRBc = nodeParent->RBcolor;
            nodeParent->RBcolor = false;
            if (nodeParent->leftchild == root)
            {
                brother->rightchild->RBcolor = false;
                if (nodeParent->parent)
                {
                    if (nodeParent->parent->leftchild == nodeParent)
                        nodeParent->parent->leftchild = brother;
                    else
                        nodeParent->parent->rightchild = brother;
                }
                brother->parent = nodeParent->parent;
                if (_root == nodeParent)
                    _root = brother;
                connect34(brother, nodeParent, brother->rightchild, nodeParent->leftchild, brother->leftchild, brother->rightchild->leftchild, brother->rightchild->rightchild);
            }
            else
            {
                if (nodeParent->parent)
                {
                    if (nodeParent->parent->leftchild == nodeParent)
                        nodeParent->parent->leftchild = brother->rightchild;
                    else
                        nodeParent->parent->rightchild = brother->rightchild;
                }
                brother->rightchild->parent = nodeParent->parent;
                if (_root == nodeParent)
                    _root = brother->rightchild;
                connect34(brother->rightchild, brother, nodeParent, brother->leftchild, brother->rightchild->leftchild, brother->rightchild->rightchild, nodeParent->rightchild);
            }
            nodeParent->parent->RBcolor = oldRBc;
            return;
        }
        if (nodeParent->RBcolor)
        { ////BB-2R
            nodeParent->RBcolor = false;
            brother->RBcolor = true;
            return;
        }
        else
        { ////BB-2B
            brother->RBcolor = true;
            root = nodeParent;
        }
    }
}

bool RBtree::remove(int value)
{
    Node *temp1 = noRepFind(value, -1);
    if (!temp1)
        return false;
    Node *node_suc;
    while (temp1->leftchild || temp1->rightchild)
    { ////迭代寻找真后继
        if (!(temp1->leftchild))
        {
            node_suc = temp1->rightchild;
        }
        else if (!(temp1->rightchild))
        {
            node_suc = temp1->leftchild;
        }
        else
        {
            node_suc = temp1->succ();
        }
        temp1->sonSize--;
        temp1->value = node_suc->value;
        temp1 = node_suc;
    }
    if (!temp1->RBcolor)
    {
        temp1->sonSize--;
        DoubleBlack(temp1);
    }
    if (temp1 == _root)
    {
        _root = NULL;
        delete temp1;
        return true;
    }
    if (temp1->parent->leftchild == temp1)
        temp1->parent->leftchild = NULL;
    else
        temp1->parent->rightchild = NULL;
    delete temp1;
    return true;
}

RBtree::nodeIter noIter;
RBtree rbTree;

int main()
{
    int n, op, num, ans;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> op >> num;
        switch (op)
        {
        case 1:
            rbTree.insert(num);
            break;
        case 2:
            rbTree.remove(num);
            break;
        case 3:
            ans = rbTree.valueFindRank(num);
            cout << ans << endl;
            break;
        case 4:
            ans = *rbTree.rankFindValue(num);
            cout << ans << endl;
            break;
        case 5:
            ans = *rbTree.lowNext(num);
            cout << ans << endl;
            break;
        case 6:
            ans = *rbTree.bigNext(num);
            cout << ans << endl;
            break;
        default:
            break;
        }
    }
    return 0;
}