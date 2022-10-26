#include <iostream>
// operator the stack;
typedef struct linkNode
{
    double data;
    struct linkNode *next;
} linkNode;
class stAck
{
public:
    linkNode *Top; // top->next为最后一个入栈的元素；
    int cnt;       // number of member in stack;
    linkNode *createNode(double data);
    void destoryNode(linkNode *node);
    void InsertAfterPtr(linkNode *ptr, linkNode *node);
    void DeleteNextPtr(linkNode *ptr);
    int empty()
    {
        return (Top->next == NULL);
    }
    void push(double data);
    double top();
    double pop();
    void InitStack();
    void ClearStack();
};

linkNode *stAck::createNode(double data)
{
    linkNode *node;

    node = new linkNode;
    node->data = data;
    node->next = NULL;

    return node;
}
void stAck::destoryNode(linkNode *node)
{
    delete node;
}
void stAck::InsertAfterPtr(linkNode *ptr, linkNode *node)
{
    linkNode *next = ptr->next;
    node->next = next;
    ptr->next = node;
}
void stAck::DeleteNextPtr(linkNode *ptr) //从链表中删除ptr结点的下一个结点
{
    linkNode *next = ptr->next->next;
    destoryNode(ptr->next);
    ptr->next = next;
}
void stAck::push(double data)
{
    linkNode *node = createNode(data);
    InsertAfterPtr(Top, node);
    cnt++;
}
double stAck::top()
{
    if (empty())
    {
        printf()
            printf("underflow\n");
        return 0;
    }
    else
    {
        return Top->next->data;
    }
}
double stAck::pop()
{
    double ret = top();
    DeleteNextPtr(Top);
    cnt--;
    return ret;
}
void stAck::InitStack()
{
    double data = 0;
    Top = createNode(data);
    cnt = 0;
}
void stAck::ClearStack()
{
    while (!empty())
        pop();
}
