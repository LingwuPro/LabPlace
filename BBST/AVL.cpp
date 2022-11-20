#include <iostream>
using namespace std;
#define nodeNum 1000000
struct treeNode
{
    treeNode *leftChild, *rightChild;
    int value;
    int height;
    int sonSize;
    int repNum;
    treeNode(int cinValue)
    {
        leftChild = nullptr;
        rightChild = nullptr;
        value = cinValue;
        height = 1;
        sonSize = 1;
        repNum = 1;
    }
    treeNode() : leftChild(nullptr), rightChild(nullptr), value(0), height(1),
                 sonSize(1), repNum(1) {}
};

struct AVLtree
{
    int totalSize;
    treeNode *root;
    treeNode node[nodeNum];
    AVLtree() : root(nullptr), totalSize(0) {}

    int height(treeNode *here)
    {
        if (!here)
            return 0;
        return here->height;
    }

    int size(treeNode *here)
    {
        if (!here)
            return 0;
        return here->sonSize;
    }

    // treeNode *valueFindNode(int rvalue)
    // {
    //     treeNode *temp = root;
    //     while (temp)
    //     {
    //         if (temp->value == rvalue)
    //             return temp;
    //         if (temp->value > rvalue)
    //             temp = temp->leftChild;
    //         else
    //             temp = temp->rightChild;
    //     }
    //     cout << "Can't find it!" << endl
    //          << "error in 'valueFindNode'!" << endl;
    //     return temp;
    // }

    treeNode *levoversion(treeNode *parent)
    {
        treeNode *temp = parent->rightChild;
        parent->rightChild = temp->leftChild;
        temp->leftChild = parent;

        parent->height = max(height(parent->leftChild), height(parent->rightChild)) + 1;
        parent->sonSize = size(parent->leftChild) + size(parent->rightChild) + parent->repNum;

        temp->height = max(height(temp->leftChild), height(temp->rightChild)) + 1;
        temp->sonSize = size(temp->leftChild) + size(temp->rightChild) + temp->repNum;

        return temp; // return the new parent;
    }

    treeNode *dextrorotation(treeNode *parent)
    {
        treeNode *temp = parent->leftChild;
        parent->leftChild = temp->rightChild;
        temp->rightChild = parent;

        parent->height = max(height(parent->leftChild), height(parent->rightChild)) + 1;
        parent->sonSize = size(parent->leftChild) + size(parent->rightChild) + parent->repNum;

        temp->height = max(height(temp->leftChild), height(temp->rightChild)) + 1;
        temp->sonSize = size(temp->leftChild) + size(temp->rightChild) + temp->repNum;

        return temp; // return the new parent;
    }

    treeNode *levoDext(treeNode *grandparent)
    {
        treeNode *temp = grandparent;
        temp->leftChild = levoversion(temp->leftChild);
        temp = dextrorotation(temp);
        return temp;
    }

    treeNode *dextLevo(treeNode *grandparent)
    {
        treeNode *temp = grandparent;
        temp->rightChild = dextrorotation(temp->rightChild);
        temp = levoversion(temp);
        return temp;
    }

    treeNode *banlanceTree(treeNode *Root)
    {
        // if (!Root || !Root->leftChild || !Root->rightChild)
        //     return Root;
        if (height(Root->leftChild) == height(Root->rightChild) + 2)
        {
            if (height(Root->leftChild->leftChild) <= height(Root->leftChild->rightChild))
                Root = levoDext(Root);
            else
            {
                Root = dextrorotation(Root);
            }
        }
        else if (height(Root->rightChild) == height(Root->leftChild) + 2)
        {
            if (height(Root->rightChild->rightChild) > height(Root->rightChild->leftChild))
            {
                Root = levoversion(Root);
            }
            else
                Root = dextLevo(Root);
        }
        return Root;
    }

    treeNode *findMinValue(treeNode *Root)
    {
        if (!Root)
            return nullptr;
        while (Root->leftChild)
            Root = Root->leftChild;
        return Root;
    }

    treeNode *nodeInsertValue(treeNode *Root, int rvalue)
    {
        if (!Root) // create new node
        {
            Root = node + totalSize;
            Root->value = rvalue;
            totalSize++;
            return Root;
        }
        if (Root->value == rvalue)
            Root->repNum++;
        else if (Root->value > rvalue)
        {
            Root->leftChild = nodeInsertValue(Root->leftChild, rvalue);
        }
        else
        {
            Root->rightChild = nodeInsertValue(Root->rightChild, rvalue);
        }

        Root->height = max(height(Root->leftChild), height(Root->rightChild)) + 1;
        Root->sonSize = size(Root->leftChild) + size(Root->rightChild) + Root->repNum;

        return banlanceTree(Root);
    }

    treeNode *nodeRemoveValue(treeNode *Root, int rvalue)
    {
        treeNode *temp = Root;
        if (!Root)
            return nullptr;
        if (Root->value == rvalue)
        {
            if (Root->repNum - 1)
                Root->repNum--;
            else if (Root->leftChild && Root->rightChild)
            {
                temp = findMinValue(Root->rightChild);
                Root->value = temp->value;
                Root->repNum = temp->repNum;
                temp->repNum = 1;
                Root->rightChild = nodeRemoveValue(Root->rightChild, Root->value);
            }
            else if (Root->leftChild)
            {
                Root = Root->leftChild;
            }
            else if (Root->rightChild)
            {
                Root = Root->rightChild;
            }
            else
            {
                Root = nullptr;
                return Root;
            }
        }
        else if (Root->value < rvalue)
            Root->rightChild = nodeRemoveValue(Root->rightChild, rvalue);
        else
            Root->leftChild = nodeRemoveValue(Root->leftChild, rvalue);

        Root->height = max(height(Root->leftChild), height(Root->rightChild)) + 1;
        Root->sonSize = size(Root->leftChild) + size(Root->rightChild) + Root->repNum;

        return banlanceTree(Root);
    }

    void insert(int rvalue) // 1 used
    {
        root = nodeInsertValue(root, rvalue);
    }
    void remove(int rvalue) // 2 used
    {
        // totalSize--;
        root = nodeRemoveValue(root, rvalue);
        // if (root == nullptr && totalSize != 0)
        // {
        //     cout << rvalue << " is not exist!" << endl;
        //     exit(-1);
        // }
    }
    int valueFindRank(int rvalue) // 3 used
    {
        treeNode *temp = root;
        int rank = 0;
        while (temp)
        {
            if (temp->value == rvalue)
                return rank + size(temp->leftChild) + 1;
            if (temp->value > rvalue)
                temp = temp->leftChild;
            else
            {
                rank += size(temp->leftChild) + temp->repNum;
                temp = temp->rightChild;
            }
        }
        // cout << "Can't find it!" << endl
        //      << "error in 'valueFindRank'!" << endl;
        return rank + 1;
    }
    treeNode *rankFindNode(treeNode *parent, int rank) // 4 used
    {
        // if (!parent)
        // {
        //     cout << "Can't find it!" << endl
        //          << "error in 'rankFindNode'!" << endl;
        // }
        if (rank <= size(parent->leftChild))
            return rankFindNode(parent->leftChild, rank);
        if (rank > size(parent->leftChild) + parent->repNum)
            return rankFindNode(parent->rightChild, rank - size(parent->leftChild) - parent->repNum);
        return parent;
    }
    treeNode *bigNext(int rvalue) // 5 used
    {
        treeNode *temp = root, *ans = nullptr;
        while (temp)
        {
            if (temp->value > rvalue)
            {
                if (!ans || temp->value < ans->value)
                    ans = temp;
                temp = temp->leftChild;
            }
            else
                temp = temp->rightChild;
        }
        return ans;
    }
    treeNode *lowNext(int rvalue) // 6 used
    {
        treeNode *temp = root, *ans = nullptr;
        while (temp)
        {
            if (temp->value < rvalue)
            {
                if (!ans || temp->value > ans->value)
                    ans = temp;
                temp = temp->rightChild;
            }
            else
                temp = temp->leftChild;
        }
        return ans;
    }
};
AVLtree atree;
treeNode *temp1;
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
            atree.insert(num);
            break;
        case 2:
            atree.remove(num);
            break;
        case 3:
            ans = atree.valueFindRank(num);
            cout << ans << endl;
            break;
        case 4:
            ans = atree.rankFindNode(atree.root, num)->value;
            cout << ans << endl;
            break;
        case 5:
            temp1 = atree.lowNext(num);
            if (temp1)
                cout << temp1->value << endl;
            else
                cout << "-1" << endl;
            break;
        case 6:
            temp1 = atree.bigNext(num);
            if (temp1)
                cout << temp1->value << endl;
            else
                cout << "-1" << endl;
            break;
        default:
            cout << "wrong in!" << endl;
            exit(-1);
        }
    }
    return 0;
}