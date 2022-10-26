//#define needValue 0
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <math.h>
#include "D:\\library\\eigen\\Eigen\\Dense"

using namespace std;
using namespace Eigen;

template <class T>
bool cmp(T x, T y)
{
    return x < y;
}

// operator the vector;

template <class T>
class vecTor
{
public:
    int _size = 0;
    int _capacity = 0;
    T *_elem = 0;
    vecTor()
    {
        _elem = new T[10]; //存储单元
        _capacity = 10;    //容量
        _size = 0;         //实际存入的数据大小
    }
    ~vecTor() { delete[] _elem; } //析构函数
    int size() const { return _size; }
    bool empty() const { return !_size; }
    vecTor<T> &operator=(vecTor<T> const &);
    T &operator[](int r) const //重定义[]
    {
        return _elem[r];
    }
    void expand();
    void shrink();
    int find(T const &a, int ptr, int end); //无序状态下的查找函数，O（n）
    int insert(int r, T const &a);
    int remove(int fir, int end);
    T remove(int r);
    int search(T const &a); //有序状态下的查找函数，O（logn）
    void sort();
    void push(T const &a) { insert(size(), a); } //相当于vector中的pushback
    void clear()                                 //置空
    {
        _size = 0;
        for (int i = 0; i < _capacity; i++)
        {
            _elem[i] = 0;
        }
    }
};
template <class T>
vecTor<T> &vecTor<T>::operator=(vecTor<T> const &a)
{
    if (_elem)
        delete[] _elem;
    _capacity = 2 * a.size();
    _elem = new T[_capacity];
    _size = 0;
    int ptr = 0;
    while (ptr < a.size())
    {
        _elem[_size++] = a._elem[ptr++];
    }
}
template <class T>
void vecTor<T>::expand()
{
    if (_size < _capacity)
        return;
    T *oldElem = _elem;
    _capacity = max(10, _capacity * 2);
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i];
    }
    delete[] oldElem;
}
template <class T>
void vecTor<T>::shrink()
{
    if (_size > _capacity / 4)
        return;
    T *oldElem = _elem;
    _capacity /= 2;
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i];
    }
    delete[] oldElem;
}
template <class T>
int vecTor<T>::find(T const &a, int fir, int ptr)
{
    while (fir < ptr)
    {
        if (_elem[ptr] == a)
        {
            break;
        }
        ptr--;
    }
    return ptr;
}
template <class T>
int vecTor<T>::insert(int r, T const &a)
{
    expand();
    for (int i = _size; i > r; i--)
    {
        _elem[i] = _elem[i - 1];
    }
    _elem[r] = a;
    _size++;
    return r;
}
template <class T>
int vecTor<T>::remove(int fir, int end)
{
    int type = fir;
    if (fir >= end)
        return -1;
    while (end < _size)
    {
        _elem[fir++] = _elem[end++];
    }
    _size = fir;
    shrink();
    return type;
}
template <class T>
T vecTor<T>::remove(int r)
{
    T temp = _elem[r];
    remove(r, r + 1);
    return temp;
}
template <class T>
int vecTor<T>::search(T const &a)
{
    int front = 0, rear = _size, mid = -1;
    while (front < rear)
    {
        mid = (front + rear) / 2;
        if ((a < _elem[mid]))
            rear = mid;
        else if (a > _elem[mid])
            front = mid + 1;
        else
            return mid;
    }
    return -1;
}
template <class T>
void vecTor<T>::sort()
{
    std::sort(_elem, _elem + _size, cmp);
}

template <class T>
class stAck : public vecTor<T>
{
public:
    T pop()
    {
        if (vecTor<T>::size() == 0)
        {
            cout << "the stack is underflow!" << endl;
            cout << "Maybe you forget something!" << endl;
            exit(-1);
        }
        return vecTor<T>::remove(vecTor<T>::size() - 1);
    }
    T &top() { return (*this)[vecTor<T>::size() - 1]; }
};

int priority(const char str) //评定运算符优先级
{
    int temp;
    switch (str)
    {
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '*':
        return 2;
        break;
    case '/':
        return 2;
        break;
    case '^':
        return 3;
        break;
    }
    cout << ("error in priority!");
    exit(-1);
}
double add(double a, double b) { return (a + b); } //四则运算加法
double sub(double a, double b) { return (a - b); } //四则运算减法
double mul(double a, double b) { return (a * b); } //四则运算乘法
double div(double a, double b) { return (a / b); } //四则运算除法
double calculate(string str1)
{
    string str;
    for (int i = 0; i < str1.length(); i++)
    {
        if (str1[i] != '-')
        {
            str += str1[i];
        }
        else if (i != 0 && ((str1[i - 1] >= '0' && str1[i - 1] <= '9') || str1[i - 1] == ')'))
        {
            str += "+0-";
        }
        else
            str += "0-";
    }
    stAck<string> Symbol; //符号存储栈
    stAck<string> Number; //数据存储栈，改为后缀表达式后也存储运算符
    string temp;          //暂存数字
    string op;            //暂存运算符
    for (int i = 0; i <= str.length(); i++)
    {
        if (i == str.length()) //如果已经读到了最后，把temp里边的元素堆到数字栈中。
        {
            if (temp.empty() == false)
            {
                Number.push(temp);
                temp.clear();
            }
            continue;
        }
        if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
        {
            temp += str[i]; //如果是数据，就存到temp里边（-作负号时也一并存入）
        }
        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^')
        {
            if (temp.empty() == false) //读到运算符，就把未压栈的数字压栈。
            {
                Number.push(temp);
                temp.clear();
            }
            while (1)
            {
                if (Symbol.empty() == true || Symbol.top().c_str()[0] == '(' || priority(str[i]) > priority(Symbol.top().c_str()[0])) //低等级的运算符直接压栈
                {
                    op = str[i];
                    Symbol.push(op);
                    break;
                }
                else
                {
                    op = Symbol.pop(); //否则先把之前的所有低等级符号弹栈再直接存入到num里边
                    Number.push(op);
                }
            }
        }
        else
        {
            if (str[i] == '(')
            {
                op = str[i];
                Symbol.push(op);
            }
            else if (str[i] == ')')
            {
                while (Symbol.top().c_str()[0] != '(')
                {
                    if (temp.empty() == false)
                    {
                        Number.push(temp);
                        temp.clear();
                    }
                    op = Symbol.top();
                    Symbol.pop();
                    Number.push(op);
                }
                Symbol.pop();
            }
            else
            {
                cout << "Wrong in!" << endl;
                exit(-1);
            }
        }
    }
    while (!Number.empty())
    {
        temp = Number.top();
        Symbol.push(temp);
        Number.pop();
    }

    double temp1, x1, y1;
    stAck<double> s1;
    while (Symbol.empty() == false)
    {
        if (Symbol.top().c_str()[0] >= '0' && Symbol.top().c_str()[0] <= '9' || Symbol.top().c_str()[0] == '-' && Symbol.top().c_str()[1] >= '0' && Symbol.top().c_str()[1] <= '9')
        {
            double dbl = atof(Symbol.pop().c_str());
            s1.push(dbl);
        }
        else
        {
            x1 = s1.pop();
            y1 = s1.pop();
            switch (Symbol.pop()[0])
            {
            case '+':
                temp1 = add(x1, y1);
                break;
            case '-':
                temp1 = sub(y1, x1);
                break;
            case '*':
                temp1 = mul(x1, y1);
                break;
            case '/':
                temp1 = div(y1, x1);
                break;
            case '^':
                temp1 = pow(y1, x1);
                break;
            }
            s1.push(temp1);
        }
    }
    return s1.top();
}
bool checkX(char x)
{
    if ((x >= '0' && x <= '9') || x == '.')
    {
        return true;
    }
    if (x == '+' || x == '-' || x == '*' || x == '/' || x == '^')
    {
        return true;
    }
    if (x == '(' || x == ')')
    {
        return true;
    }
    return false;
}

vecTor<string> funcName;
vecTor<string> funcStr;
vecTor<string> funcX;
int theX;

double calculX(string str, string changeStr, bool isFunc)
{
    bool hasLeftBrac = false, hasRightBrac = false, findSth = false;
    string func;
    int i = -1;
    int saveStart, saveLen = 0;
    while (i < (int)str.length())
    {
        hasLeftBrac = false;
        hasRightBrac = false;
        findSth = false;
        saveLen = 0;
        while (checkX(str[++i]))
        {
            if (i >= str.length())
            {
                break;
            }
        }
        if (i >= str.length())
            break;
        if (str[i - 1] != '+' && str[i - 1] != '-' && str[i - 1] != '*' && str[i - 1] != '/' && str[i - 1] != '(')
        {
            cout << "wrong name!";
            exit(-1);
        }
        saveStart = i;
        while (str.c_str()[i] != '+' && str.c_str()[i] != '-' && str.c_str()[i] != '*' && str.c_str()[i] != '/' && str.c_str()[i] != '^' && i <= str.length() - 1)
        {
            if (hasLeftBrac == false && str.c_str()[i] == ')')
            {
                break;
            }
            findSth = true;
            saveLen++;
            if (str.c_str()[i] == '(')
            {
                hasLeftBrac = true;
            }
            else if (str.c_str()[i] == ')' && hasLeftBrac == true)
            {
                hasRightBrac = true;
                hasLeftBrac = false;
            }
            i++;
        }
        if (hasRightBrac == true)
        {
            int temp1 = saveStart;
            while (str[temp1] != '(')
            {
                func += str[temp1++];
            }
            for (int m = 0; m < funcName.size(); m++)
            {
                if (funcName[m] == func && isFunc == true)
                {
                    func.clear();
                    func += '(';
                    func += funcStr[m];
                    func += ')';
                    str.replace(saveStart, saveLen, func);
                    i = -1;
                }
            }
        }
        else if (findSth == true)
        {
            string Namer;
            Namer += '(';
            for (int j = saveStart; j < saveStart + saveLen; j++)
            {
                Namer += str.c_str()[j];
            }
            if (Namer != funcX[theX] && isFunc == true)
            {
                cout << "wrong name of X!";
                exit(-1);
            }
            str.replace(saveStart, saveLen, changeStr);
        }
    }
    // cout << str;
    double answer = calculate(str);
    return answer;
}

void defFunc(string str)
{
    int lenOfName = -1;
    bool start = false;
    string X;
    while (1)
    {
        if (str.c_str()[++lenOfName] == '=')
            break;
        if (str[lenOfName] == '(')
        {
            start = true;
        }
        if (start == true && str[lenOfName] != ')')
        {
            X += str[lenOfName];
        }
    }
    funcX.push(X);
    string temp;
    for (int i = 0; i < lenOfName; i++)
    {
        if (str.c_str()[i] == '(')
            break;
        temp += str.c_str()[i];
    }
    for (int i = 0; i < funcName.size(); i++)
    {
        if (temp == funcName[i])
        {
            cout << "Duplicate definition!";
            exit(-1);
        }
    }
    funcName.push(temp);
    temp.clear();
    for (int i = lenOfName + 1; i < str.length(); i++)
    {
        temp += str.c_str()[i];
    }
    funcStr.push(temp);
}
double runFunc(string str)
{
    string Name, num;
    int strPtr = 0;
    while (1)
    {
        if (strPtr >= str.length())
        {
            cout << "Run Funcation Error!";
            exit(-1);
        }
        else if (str[strPtr] == '(')
        {
            strPtr++;
            break;
        }
        Name += str.c_str()[strPtr++];
    }
    int place = funcName.find(Name, 0, funcName.size());
    if (place == 0)
    {
        cout << "Didn't define this function!";
        exit(-1);
    }
    while (1)
    {
        if (strPtr >= str.length())
        {
            cout << "Run Funcation Error!";
            exit(-1);
        }
        else if (str.c_str()[strPtr] == ')')
            break;
        num += str.c_str()[strPtr++];
    }
    theX = place;
    return calculX(funcStr[place], num, true);
}

double answerMatrix[30][30] = {0};
double xMatrix[30][30] = {0};
double yMatrix[30][30] = {0};
void add(int hrz, int lat)
{
    memset(answerMatrix, 0, sizeof(answerMatrix));
    for (int i = 0; i < hrz; i++)
    {
        for (int j = 0; j < lat; j++)
        {
            cout << xMatrix[i][j] + yMatrix[i][j] << ' ';
        }
        cout << endl;
    }
}
void sub(int hrz, int lat)
{
    memset(answerMatrix, 0, sizeof(answerMatrix));
    for (int i = 0; i < hrz; i++)
    {
        for (int j = 0; j < lat; j++)
        {
            cout << xMatrix[i][j] - yMatrix[i][j] << ' ';
        }
        cout << endl;
    }
}
void mul(int hrz1, int lat1, int hrz2, int lat2)
{
    if (hrz1 != lat2)
    {
        cout << "error in matrix mul!";
        exit(-1);
    }
    MatrixXd mat1(hrz1, lat1), mat2(hrz2, lat2), mat3(hrz2, lat1);
    for (int i = 0; i < hrz1; i++)
    {
        for (int j = 0; j < lat1; j++)
        {
            mat1(i, j) = xMatrix[i][j];
        }
    }
    for (int i = 0; i < hrz2; i++)
    {
        for (int j = 0; j < lat2; j++)
        {
            mat2(i, j) = yMatrix[i][j];
        }
    }
    mat3 = mat1 * mat2;
    cout << mat3 << endl;
}
void transPose(int hrz, int lat)
{
    MatrixXd mat(hrz, lat);
    for (int i = 0; i < hrz; i++)
    {
        for (int j = 0; j < lat; j++)
        {
            mat(i, j) = xMatrix[i][j];
        }
    }
    cout << mat.transpose() << endl;
}
void deterMinant(int hrz, int lat)
{
    if (hrz == 0 && lat == 0)
    {
        cout << '0' << endl;
        return;
    }
    if (hrz != lat)
    {
        cout << '0' << endl;
        return;
    }
    MatrixXd mat(hrz, lat);
    for (int i = 0; i < hrz; i++)
    {
        for (int j = 0; j < lat; j++)
        {
            mat(i, j) = xMatrix[i][j];
        }
    }
    cout << mat.determinant() << endl;
}

#ifdef needValue
void eigenValues(int hrz, int lat)
{
    MatrixXd mat(hrz, lat);
    for (int i = 0; i < hrz; i++)
    {
        for (int j = 0; j < lat; j++)
        {
            mat(i, j) = xMatrix[i][j];
        }
    }
    EigenSolver<MatrixXd> eV(mat);
    cout << eV.eigenvalues() << endl;
}
#endif

double xTimes, yTimes;
vecTor<double> xNomial, yNomial;
double answNomial[100];
bool xIsUsing = false;

void multiNomialCin()
{
    double cIn;
    if (xIsUsing == false)
    {
        cin >> xTimes;
        if (xTimes > 50)
        {
            cout << "the times of x is too high!";
            exit(-1);
        }
        cout << "请输入表达式,按次数从低到高书写：" << endl;
        for (int i = xTimes - 1; i >= 0; i--)
        {
            cin >> cIn;
            xNomial.push(cIn);
        }
        xIsUsing = true;
    }
    else
    {
        cin >> yTimes;
        if (yTimes > 50)
        {
            cout << "the times of y is too high!";
            exit(-1);
        }
        cout << "请输入表达式" << endl;
        for (int i = yTimes - 1; i >= 0; i--)
        {
            cin >> cIn;
            yNomial.push(cIn);
        }
    }
}

void multiAdd()
{
    memset(answNomial, 0, sizeof(answNomial));
    for (int i = max(xTimes, yTimes) - 1; i >= 0; i--)
    {
        answNomial[i] = xNomial[i] + yNomial[i];
        if (i != max(xTimes, yTimes) - 1 && answNomial[i] > 0)
        {
            cout << '+';
        }
        if (answNomial[i] != 0)
            cout << answNomial[i] << "x^" << i;
    }
    cout << endl;

    xNomial.clear();
    yNomial.clear();
    for (int i = 0; i < max(xTimes, yTimes); i++)
    {
        xNomial[i] = answNomial[i];
    }
    xTimes = max(xTimes, yTimes);
    yTimes = 0;
    xIsUsing = true;
    memset(answNomial, 0, sizeof(answNomial));
}
void multiSub()
{
    memset(answNomial, 0, sizeof(answNomial));
    for (int i = max(xTimes, yTimes) - 1; i >= 0; i--)
    {
        answNomial[i] = xNomial[i] - yNomial[i];
        if (i != max(xTimes, yTimes) - 1 && answNomial[i] > 0)
        {
            cout << '+';
        }
        if (answNomial[i] != 0)
            cout << answNomial[i] << "x^" << i;
    }
    xNomial.clear();
    yNomial.clear();
    for (int i = 0; i < max(xTimes, yTimes); i++)
    {
        xNomial[i] = answNomial[i];
    }
    xTimes = max(xTimes, yTimes);
    yTimes = 0;
    xIsUsing = true;
    memset(answNomial, 0, sizeof(answNomial));
}
void multiMul()
{
    if (xTimes + yTimes > 100)
    {
        cout << "the answer's times is too high!";
        exit(-1);
    }
    memset(answNomial, 0, sizeof(answNomial));
    for (int i = 0; i < xTimes; i++)
    {
        for (int j = 0; j < yTimes; j++)
        {
            answNomial[i + j] += xNomial[i] * yNomial[j];
        }
    }
    for (int i = xTimes + yTimes - 2; i >= 0; i--)
    {
        if (i != xTimes + yTimes - 2 && answNomial[i] > 0)
        {
            cout << '+';
        }
        if (answNomial[i] != 0)
            cout << answNomial[i] << "x^" << i;
    }
    xNomial.clear();
    yNomial.clear();
    for (int i = 0; i < max(xTimes, yTimes); i++)
    {
        xNomial[i] = answNomial[i];
    }
    xTimes = xTimes + yTimes;
    yTimes = 0;
    xIsUsing = true;
    memset(answNomial, 0, sizeof(answNomial));
}
void multiDifferential()
{
    bool hasSomeThing = false;
    for (int i = 1; i < xTimes; i++)
    {
        answNomial[i - 1] = i * xNomial[i];
        if (answNomial[i - 1] != 0)
        {
            hasSomeThing = true;
        }
    }
    if (hasSomeThing == false)
    {
        cout << '0' << endl;
        answNomial[0] = 0;
        xTimes = 0;
        yTimes = 0;
    }
    else
    {
        for (int i = xTimes - 2; i >= 0; i--)
        {
            if (i != xTimes - 2 && answNomial[i] > 0)
            {
                cout << '+';
            }
            if (answNomial[i] != 0)
                cout << answNomial[i];
            if (i != 0)
                cout << "x^" << i;
        }
    }
    xNomial.clear();
    yNomial.clear();
    for (int i = 0; i < xTimes - 1; i++)
    {
        xNomial[i] = answNomial[i];
    }
    xTimes--;
    yTimes = 0;
    xIsUsing = true;
    memset(answNomial, 0, sizeof(answNomial));
}
void multiIntegral()
{
    for (int i = 0; i < xTimes; i++)
    {
        answNomial[i + 1] = xNomial[i] / (i + 1);
    }
    for (int i = xTimes; i > 0; i--)
    {
        if (i != xTimes && answNomial[i] > 0)
        {
            cout << '+';
        }
        if (answNomial[i] != 0)
            cout << answNomial[i] << "x^" << i;
    }
    cout << "+C" << endl;
    xNomial.clear();
    yNomial.clear();
    for (int i = 1; i <= xTimes; i++)
    {
        xNomial[i] = answNomial[i];
    }
    xTimes++;
    yTimes = 0;
    xIsUsing = true;
    memset(answNomial, 0, sizeof(answNomial));
}
int main()
{
    funcName.push("setForWaste");
    funcStr.push("setForWaste");
    funcX.push("setForWaste");
    cout << "请输入你要选择的模式（数字）：" << endl
         << "1.多项式计算" << '\t' << "2.四则运算" << endl
         << "3.矩阵运算" << '\t' << "4.一元表达式运算" << endl
         << "5. 一元函数运算" << '\t' << "6.退出" << endl;
    int mod, hrzOfMat, latOfMat, modA, hrz2Mat, lat2Mat;
    double answer;
    char modl;
    string cIn, cNum;

    while (1)
    {
        cin >> mod;
        switch (mod)
        {
        case 1:
        {
            cout << "请输入初始表达式的项数。" << endl;
            multiNomialCin();
            while (1)
            {
                cout << "是否要用到新表达式,需要输入1,否则输入2" << endl;
                cin >> modl;
                if (modl == '1')
                {
                    cout << "请输入新运算表达式的项数,如要退出输入0" << endl;
                    multiNomialCin();
                }
                cout << "请输入你要做的运算,积分为'^',微分为'%',如要退出输入'#'" << endl;
                cin >> modl;
                switch (modl)
                {
                case '+':
                    multiAdd();
                    break;
                case '-':
                    multiSub();
                    break;
                case '*':
                    multiMul();
                    break;
                case '%':
                    multiDifferential();
                    break;
                case '^':
                    multiIntegral();
                    break;
                case '#':
                    exit(-1);
                    break;
                }
            }
            break;
        }
        case 2:
        {
            cout << "请输入表达式，中间不要带分隔符哦。" << endl;
            cin >> cIn;
            cout << calculate(cIn) << endl;
            break;
        }
        case 3:
        {
            cout << "请输入矩阵行数和列数" << endl;
            cin >> hrzOfMat >> latOfMat;
            cout << "请以此按行输入矩阵" << endl;
            for (int i = 0; i < hrzOfMat; i++)
            {
                for (int j = 0; j < latOfMat; j++)
                {
                    cin >> xMatrix[i][j];
                }
            }
            cout << "是否需要用到第二个矩阵呢?是输入1,否输入2" << endl;
            cin >> modA;
            if (modA == 1)
            {
                cout << "请输入矩阵行数和列数" << endl;
                cin >> hrz2Mat >> lat2Mat;
                cout << "请以此按行输入矩阵" << endl;
                for (int i = 0; i < hrz2Mat; i++)
                {
                    for (int j = 0; j < lat2Mat; j++)
                    {
                        cin >> yMatrix[i][j];
                    }
                }
            }
            cout << "请输入你要做的运算：（/表示转置，=表示求行列式,@表示特征值）" << endl;
            cin >> modl;
            switch (modl)
            {
            case '+':
                if (hrz2Mat != hrzOfMat || lat2Mat != latOfMat)
                {
                    cout << "error!";
                    exit(-1);
                }
                add(hrzOfMat, latOfMat);
                break;
            case '-':
                if (hrz2Mat != hrzOfMat || lat2Mat != latOfMat)
                {
                    cout << "error!";
                    exit(-1);
                }
                sub(hrzOfMat, latOfMat);
                break;
            case '*':
                mul(hrzOfMat, latOfMat, hrz2Mat, lat2Mat);
                break;
            case '/':
                transPose(hrzOfMat, latOfMat);
                break;
            case '=':
                deterMinant(hrzOfMat, latOfMat);
                break;
#ifdef needValue
            case '@':
                eigenValues(hrzOfMat, latOfMat);
                break;
#endif
            }
            break;
        }
        case 4:
        {
            cout << "请输入表达式（无空格）：" << endl;
            cin >> cIn;
            cout << "请输入赋值：（仅输入数据）" << endl;
            cin >> cNum;
            cout << calculX(cIn, cNum, false) << endl;
            break;
        }
        case 5:
        {
            cout << "请输入表达式，如要退出函数模式输入'###'" << endl;
            while (1)
            {
                cIn.clear();
                cNum.clear();
                cin >> cNum;
                if (cNum == "DEF")
                {
                    cin >> cIn;
                    defFunc(cIn);
                }
                else if (cNum == "RUN")
                {
                    cin >> cIn;
                    cout << runFunc(cIn) << endl;
                }
                else if (cNum == "###")
                {
                    break;
                }
                else
                {
                    cout << "wrong model!";
                    exit(-1);
                }
            }
            break;
        }
        case 6:
        {
            cout << "Return! " << endl;
            exit(0);
            break;
        }
        default:
        {
            cout << "wrong model!" << endl;
            exit(-1);
        }
        }
        cout << "如果要继续，请重新选择你要选择的模式；" << endl;
    }

    return 0;
}