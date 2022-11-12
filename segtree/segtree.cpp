#include <iostream>
#include <stack>
#include <queue>
#include <stdlib.h>
#include <algorithm>
using namespace std;
#define IsRoot(x) (!((x).parent))
#define IsLChlid(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChlid(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) (IsLChlid(*(p)) ? (p)->parent->rc : (p)->parent->lc)
#define uncle(x) (IsLChlid(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChlid(x) ? (x).parent->lc : (x).parent->rc))
#define BinNodePosi(T) BinNode<T> *
#define stature(p) ((p) ? (p)->height : -1)
typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor;
template <class T>
void release(T *x)
{
    if (x)
    {
        delete x;
    }
}
template <typename T>
struct BinNode
{
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;

    BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED) : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}

    int size();
    BinNodePosi(T) insertAsLC(T const &);
    BinNodePosi(T) insertAsRC(T const &);
    BinNodePosi(T) succ();
    template <typename VST>
    void travLevel(VST &);
    template <typename VST>
    void travPre(VST &);
    template <typename VST>
    void travIn(VST &);
    template <typename VST>
    void travPost(VST &);
    bool operator<(BinNode const &bn) { return data < bn.data; }
    bool operator==(BinNode const &bn) { return data == bn.data; }
    /*DSA*/ BinNodePosi(T) zig();
    /*DSA*/ BinNodePosi(T) zag();
};
template <class T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    return lc = new BinNode(e, this);
}
template <class T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rc = new BinNode(e, this);
}
template <typename T>

class BinTree
{
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);
    void updateHeightAbove(BinNodePosi(T) x);

public:
    BinTree() : _size(0), _root(NULL) {}
    ~BinTree()
    {
        if (0 < _size)
            remove(_root);
    }
    int size() const { return _size; }
    bool empty() const { return !_root; }
    BinNodePosi(T) root() const { return _root; }
    BinNodePosi(T) insertAsRoot(T const &e);
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&T1);
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&T1);
    int remove(BinNodePosi(T) x);
    BinTree<T> *secede(BinNodePosi(T) x);
    template <typename VST>
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    }
    template <typename VST>
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }
    template <typename VST>
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    }
    template <typename VST>
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    }
    bool operator<(BinTree<T> const &t)
    {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator==(BinTree<T> const &t)
    {
        return _root && t._root && (_root == t._root);
    }
}; // BinTree
template <class T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x高度
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}
template <class T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新高度
{
    while (x)
    {
        updateHeight(x);
        x = x->parent;
    }
}
template <class T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e) //将e当做根节点插入二叉树
{
    _size = 1;
    _root = new BinNode<T>(e);
    return _root;
}
template <class T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e) // e插入为x的左节点
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}
template <class T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e) // e插入为x的右节点
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}
template <class T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S)
{
    x->lc = S->_root;
    if (S->_root)
    {
        x->lc->parent = x;
    }
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    release(x);
    S = NULL;
    return x;
}
template <class T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S)
{
    x->rc = S->_root;
    if (S->_root)
    {
        x->rc->parent = x;
    }
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    release(x);
    S = NULL;
    return x;
}
template <class T>
static int removeAt(BinNodePosi(T) x)
{
    if (!x)
        return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    release(x);
    return n;
}
template <class T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}
template <class T>
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = NULL;
    S->_size = x->size();
    _size -= S->_size;
    return S;
}
//递归版
template <class T, class VST>
void travPre_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}
template <class T, class VST>
void travIn_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}
template <class T, class VST>
void travPost_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

//迭代版
template <class T, class VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST &visit, stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        visit(x->data);
        S.push(x->rc);
        x = x->lc;
    }
}
template <class T, class VST>
void travPre_I(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S;
    while (1)
    {
        visitAlongLeftBranch(x, visit, S);
        if (S.empty())
            break;
        x = S.top();
        S.pop();
    }
}

template <class T>
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        S.push(x);
        x = x->lc;
    }
}
template <class T, class VST>
void travIn_I(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S;
    while (1)
    {
        goAlongLeftBranch(x, S);
        if (S.empty())
            break;
        x = S.top();
        S.pop();
        visit(x->data);
        x = x->rc;
    }
}
template <class T>
static void gotoHLVFL(stack<BinNodePosi(T)> &S)
{
    while (BinNodePosi(T) x = S.top())
    {
        if (HasLChild(*x))
        {
            if (HasRChild(*x))
            {
                S.push(x->rc);
            }
            S.push(x->lc);
        }
        else
        {
            S.push(x->rc);
        }
    }
    S.pop();
}
template <class T, class VST>
void travPost_I(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S;
    if (x)
    {
        S.push(x);
    }
    while (!S.empty())
    {
        if (S.top() != x->parent) //如果不是父节点，则一定是右兄弟节点，因此此时要伸进去。
        {
            gotoHLVFL(S);
        }
        x = S.top();
        S.pop();
        visit(x->data);
    }
}
template <class T>
template <class VST>
void BinNode<T>::travLevel(VST &visit)
{
    queue<BinNodePosi(T)> Q;
    Q.push(this);
    while (!Q.empty())
    {
        BinNodePosi(T) x = Q.front();
        Q.pop();
        visit(x->data);
        if (HasLChild(*x))
            Q.push(x->lc);
        if (HasRChild(*x))
            Q.push(x->rc);
    }
}

template <class T>
stack<BinNodePosi(T)> ancestor_buff;
template <class T>
void get_ancestor(BinNodePosi(T) son)
{
    if (son->parent == nullptr)
    {
        return;
    }
    ancestor_buff<T>.push(son->parent);
    get_ancestor(son->parent);
}

template <class T>
BinNodePosi(T) get_uncle(BinNodePosi(T) son)
{
    if (son->parent == nullptr)
    {
        return nullptr;
    }
    return sibling(son);
}
void coutData(double data)
{
    cout << data << ' ';
}

int main()
{
    double n;         // cin data
    int s, s1;        // cin flag
    bool flag = true; // check to quit
    BinTree<double> tree;
    BinNodePosi(double) temp_now;
    cout << "Please give your root value." << endl;
    cin >> n;
    tree.insertAsRoot(n);
    temp_now = tree.root();
    while (flag)
    {
        cout << "****************************************************" << endl;
        cout << "Please set your binary tree:" << endl;
        cout << "if you want to give the left-child value, cin 1." << endl;
        cout << "if you want to give the right-child vlaue,cin 2." << endl;
        cout << "if you want to come back the parent palce, cin 3." << endl;
        cout << "if you want to quit, cin 4." << endl;
        cin >> s;
        switch (s)
        {
        case 1:
            if (HasLChild(*temp_now))
            {
                cout << "wrong in!" << endl;
                exit(-1);
            }
            cout << "this is the left child." << endl;
            cout << "please cin value:";
            cin >> n;
            temp_now = tree.insertAsLC(temp_now, n);
            break;
        case 2:
            if (HasRChild(*temp_now))
            {
                cout << "wrong in!" << endl;
                exit(-1);
            }
            cout << "this is the right child." << endl;
            cout << "please cin value:";
            cin >> n;
            temp_now = tree.insertAsRC(temp_now, n);
            break;
        case 3:
            if (!HasParent(*temp_now))
            {
                cout << "wrong in!" << endl;
                exit(-1);
            }
            temp_now = temp_now->parent;
            cout << "now you have back to the parent place." << endl;
            break;
        default:
            flag = false;
            break;
        }
    }
    flag = true;
    bool temp1 = true;
    while (flag)
    {
        cout << "if you want to Preorder traversal, cin 1." << endl;
        cout << "if you want to Middle order traversal,cin 2." << endl;
        cout << "if you want to Postorder traversal, cin 3." << endl;
        cout << "if you want to level traversal, cin 4." << endl;
        cout << "if you want to get the parent data, cin 5." << endl;
        cout << "if you want to get the sibiling data, cin 6." << endl;
        cout << "if you want to quit, cin 7." << endl;
        cin >> s;
        switch (s)
        {
        case 1:
            cout << "this is the iteration model:" << endl;
            travPre_I(tree.root(), coutData);
            cout << endl;
            cout << "this is the recursion model:" << endl;
            travPre_R(tree.root(), coutData);
            cout << endl;
            break;
        case 2:
            cout << "this is the iteration model:" << endl;
            travIn_I(tree.root(), coutData);
            cout << endl;
            cout << "this is the recursion model:" << endl;
            travIn_R(tree.root(), coutData);
            cout << endl;
            break;
        case 3:
            cout << "this is the iteration model:" << endl;
            travPost_I(tree.root(), coutData);
            cout << endl;
            cout << "this is the recursion model:" << endl;
            travPost_R(tree.root(), coutData);
            cout << endl;
            break;
        case 4:
            cout << "the answer is:" << endl;
            tree.root()->travLevel(coutData);
            cout << endl;
            break;
        case 5:
            temp1 = true;
            cout << "you may need to choose the iterator's place:" << endl;
            temp_now = tree.root();
            while (temp1)
            {
                cout << "the iterator's height is: " << temp_now->height << " and the data is: " << temp_now->data << endl;
                cout << "cin 1 means goto left child;2 means right one;3 means its parent;or 4 means quit." << endl;
                cin >> s1;
                switch (s1)
                {
                case 1:
                    if (temp_now->lc == nullptr)
                    {
                        cout << "no lc!" << endl;
                        break;
                    }
                    temp_now = temp_now->lc;
                    break;
                case 2:
                    if (temp_now->rc == nullptr)
                    {
                        cout << "no rc" << endl;
                        break;
                    }
                    temp_now = temp_now->rc;
                    break;
                case 3:
                    if (temp_now->parent == nullptr)
                    {
                        cout << "no parent!" << endl;
                        break;
                    }
                    temp_now = temp_now->parent;
                    break;
                default:
                    temp1 = false;
                    break;
                }
            }
            get_ancestor(temp_now);
            if (ancestor_buff<double>.empty())
            {
                cout << "null of this!" << endl;
            }
            else
                cout << "the answer is:" << endl;
            while (!ancestor_buff<double>.empty())
            {
                cout << ancestor_buff<double>.top()->data << " ";
                ancestor_buff<double>.pop();
            }
            cout << endl;
            break;
        case 6:
            temp1 = true;
            cout << "you may need to choose the iterator's place:" << endl;
            temp_now = tree.root();
            while (temp1)
            {
                cout << "the iterator's height is: " << temp_now->height << " and the data is: " << temp_now->data << endl;
                cout << "cin 1 means goto left child;2 means right one;3 means its parent;or 4 means quit." << endl;
                cin >> s1;
                switch (s1)
                {
                case 1:
                    if (temp_now->lc == nullptr)
                    {
                        cout << "no lc!" << endl;
                        break;
                    }
                    temp_now = temp_now->lc;
                    break;
                case 2:
                    if (temp_now->rc == nullptr)
                    {
                        cout << "no rc" << endl;
                        break;
                    }
                    temp_now = temp_now->rc;
                    break;
                case 3:
                    if (temp_now->parent == nullptr)
                    {
                        cout << "no parent!" << endl;
                        break;
                    }
                    temp_now = temp_now->parent;
                    break;
                default:
                    temp1 = false;
                    break;
                }
            }
            if (get_uncle(temp_now) == nullptr)
            {
                cout << "null of this" << endl;
            }
            else
            {
                cout << "the answer is:" << get_uncle(temp_now)->data << endl;
            }
            break;
        default:
            flag = false;
            break;
        }
    }
    return 0;
}