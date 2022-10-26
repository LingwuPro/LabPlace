#if 1
#include <iostream>
#include <string>
#include <stack>
#include <stdlib.h>

using namespace std;
//有个小bug,不能解析两位数的算数
// bug已解决
//判断符号优先级
int foo(const char str)
{
    int temp;
    switch (str)
    {
    case '+':
        temp = 1;
        break;
    case '-':
        temp = 1;
        break;
    case '*':
        temp = 2;
        break;
    case '/':
        temp = 2;
        break;
    }
    return temp;
}
//计算类
class calculate
{
public:
    //加法
    static int add(int a, int b)
    {
        return a + b;
    }
    //减法
    static int sub(int a, int b)
    {
        return a - b;
    }
    //乘法
    static int mul(int a, int b)
    {
        return a * b;
    }
    //除法
    static int div(int a, int b)
    {
        return a / b;
    }
};
//封装一个char 转 int
// int MytoInt(char c)
//{
//	return c - '0';
// }
int main()
{
    string str;
    cout << "输入表达式(括号要输入字母括号): ";
    cin >> str;

    //将表表达式转换为后缀表达式
    //转换规则
    // 1 从左到右遍历
    // 2 运算数直接输出
    // 3 '('直接压栈 ，然后优先级最低
    // 4 遇到')’弹出栈顶元素，直到'(’为止
    // 5 运算符比较优先级，优先级高于栈压栈，反而弹出然后再比较

    stack<string> stk;  //运算符栈
    stack<string> stk2; //中间结果栈
    string temp;        //零时值
    string op;
    string postfix;
    for (int i = 0; i <= str.length(); i++)
    {
        //压入temp里面剩余值
        if ((!temp.empty()) && (i == str.length()))
        {
            stk2.push(temp);
            temp.clear();
            continue;
        }
        //防止越界访问
        if (i == str.length())
        {
            continue;
        }
        //如果是数字直接压入栈stk2
        if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
        {
            temp += str[i];
        }
        //如果是运算符压入stk
        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            //后面是）会丢掉这个temp
            //每次要判断temp是否为空，如果没有会把空压入stk2导致后面结果不正确
            if (!temp.empty())
            {
                stk2.push(temp);
                temp.clear();
            }
            while (1)
            {
                if (stk.empty() || stk.top().c_str()[0] == '(') // stk为空或者栈顶为"("   stk.top().c_str()[0]取符号
                {
                    op = str[i];
                    stk.push(op); //要输入字母的括号
                    break;
                }
                else if (foo(str[i]) > foo(stk.top().c_str()[0])) //判断符号优先级，大于就压栈
                {
                    op = str[i];
                    stk.push(op);
                    break;
                }
                else
                {
                    op = stk.top();
                    stk.pop();
                    stk2.push(op);
                }
            }
        }
        else //判断（）
        {
            if (str[i] == '(')
            {
                op = str[i];
                stk.push(op);
            }
            else //不是'('    就是')"
            {
                //把'('前的符号出栈，然后'('符号丢弃
                while (stk.top().c_str()[0] != '(')
                {
                    //因为其他步骤都跳过了，故这也要添加
                    if (!temp.empty())
                    {
                        stk2.push(temp);
                        temp.clear();
                    }
                    op = stk.top();
                    stk.pop();
                    stk2.push(op);
                }
                stk.pop();
            }
        }
        //检测符号栈里面还有没有为出栈的符号
        // while (!stk.empty() && (i + 1) == str.length())
        //{
        //	stk2.push(stk.top());
        //	stk.pop();
        //}
    }
    //正序转换
    while (!stk2.empty())
    {
        temp = stk2.top();
        stk.push(temp);
        stk2.pop();
    }
    //创建一个零时值
    stack<string> tmp;
    tmp = stk;
    while (!tmp.empty())
    {
        postfix += tmp.top();
        tmp.pop();
    }
    cout << "中缀表达式: " << str << endl;
    cout << "后缀表达式: " << postfix << endl;

    //通过后缀表达式计算值
    int sum;
    int a, b;
    //在定义1个栈
    stack<int> s1;
    while (!stk.empty())
    {
        if (stk.top().c_str()[0] == '+' || stk.top().c_str()[0] == '-' || stk.top().c_str()[0] == '*' || stk.top().c_str()[0] == '/')
        {
            switch (stk.top().c_str()[0]) // stk.top().c_str()[0]转换成符号字符。例如 "+" 转 '+'
            {
            case '+':
                b = s1.top();
                s1.pop();
                a = s1.top();
                s1.pop();
                sum = calculate::add(a, b);
                s1.push(sum);
                stk.pop();
                break;
            case '-':
                b = s1.top();
                s1.pop();
                a = s1.top();
                s1.pop();
                sum = calculate::sub(a, b);
                s1.push(sum);
                stk.pop();
                break;
            case '*':
                b = s1.top();
                s1.pop();
                a = s1.top();
                s1.pop();
                sum = calculate::mul(a, b);
                s1.push(sum);
                stk.pop();
                break;
            case '/':
                b = s1.top();
                s1.pop();
                a = s1.top();
                s1.pop();
                sum = calculate::div(a, b);
                s1.push(sum);
                stk.pop();
                break;
            }
        }
        else
        {
            int tt = atoi(stk.top().c_str());
            s1.push(tt);
            stk.pop();
        }

        // if (postfix[i] != '+' && postfix[i] != '-' && postfix[i] != '*' && postfix[i] != '/')
        //	s1.push(MytoInt(postfix[i]));
    }
    cout << str << " = " << s1.top() << endl;

    system("pause");
    return 0;
}
#endif
