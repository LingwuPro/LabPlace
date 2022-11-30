#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

#define M 1333331
#define p 31
#define startkey 39 // f(x)=sigma(x*p^k)+startkey

#define inmod 1
#define outmod 1
#define textin "data6.in"
#define textout "output5_data6.txt"

#define byte_110 (3 << 6)
#define byte_1110 (7 << 5)

using namespace std;

//#if outmod == 0
// struct keyNum *hash1[M + 5]; // remember to delete[]!!!
// struct keyNum
// {
//     int key;
//     int value;
//     struct keyNum *next;
// };
// void chainInsert(int key, int value);
// int chainSearch(int key); // return value, or -1;
// void chainDelete();
//#endif
//#if outmod == 1
// struct keySqu
// {
//     int key;
//     int value;
// } hash2[M + 5];
// void squareInsert(int key, int value);
// int squareSearch(int key);
//#endif
// #if outmod == 2
// struct keyLine
// {
//     int key;
//     int value;
// } hash3[M + 5];
// void lineInsert(int key, int value);
// int lineSearch(int key);
// #endif

#if inmod == 1
int utf2key(string temp)
{
    vector<int> num;
    int utfnum;
    long long sum = 0;
    int Size = temp.size();
    for (int i = 0; i < Size;)
    {
        if ((i & byte_110) == byte_110)
        {
            utfnum = ((int)(unsigned char)temp.c_str()[i]) << 8;
            utfnum += (int)(unsigned char)temp.c_str()[i + 1];
            i += 2;
            num.push_back(utfnum);
        }
        else if ((i & byte_1110) == byte_1110)
        {
            utfnum = ((int)(unsigned char)temp.c_str()[i]) << 16;
            utfnum += ((int)(unsigned char)temp.c_str()[i + 1]) << 8;
            utfnum += (int)(unsigned char)temp.c_str()[i + 2];
            i += 3;
            num.push_back(utfnum);
        }
        else
        {
            num.push_back((int)(unsigned char)temp.c_str()[i]);
            i++;
        }
    }
    for (auto i : num)
    {
        sum *= p;
        sum += i;
    }
    sum *= p;
    sum += startkey;
    return abs((int)sum);
}
#endif

#if inmod == 0
int ascii2key(string temp)
{
    long long sum = 0;
    for (auto i : temp)
    {
        sum *= p;
        sum += (int)(unsigned char)i;
    }
    sum *= p;
    sum += startkey;
    return abs((int)sum);
}
#endif

#if outmod == 0
struct keyNum *hash1[M + 5]; // remember to delete[]!!!
struct keyNum
{
    int key;
    int value;
    struct keyNum *next;
};
void chainInsert(int key, int value);
void chainInsert(int key, int value)
{
    int hashCode = abs(key % M);
    struct keyNum *temp = hash1[hashCode], *parent = nullptr;
    while (temp)
    {
        parent = temp;
        temp = temp->next;
    }
    temp = new keyNum;
    temp->key = key;
    temp->value = value;
    temp->next = nullptr;
    if (parent)
    {
        parent->next = temp;
    }
    else
    {
        hash1[hashCode] = temp;
    }
}

int chainSearch(int key)
{
    int hashCode = abs(key % M);
    struct keyNum *temp = hash1[hashCode];
    while (temp)
    {
        if (temp->key == key)
            return temp->value;
        else
            temp = temp->next;
    }
    return -1;
}

void chainDelete()
{
    struct keyNum *temp1, *temp2;
    for (int i = 0; i < M; i++)
    {
        if (hash1[i])
        {
            temp1 = hash1[i];
            while (temp1)
            {
                temp2 = temp1;
                temp1 = temp1->next;
                delete temp2;
            }
        }
    }
}
#endif

#if outmod == 1
struct keySqu
{
    int key;
    int value;
} hash2[M + 5];

void squareInsert(int key, int value)
{
    int hashCode = abs(key % M), temp = hashCode;
    int base = 0;
    int cnt = 0;
    bool sign[M + 5] = {false};
    while (1)
    {
        if (cnt >= M)
        {
            cout << "overflow!" << endl;
            return;
        }

        temp = hashCode;

        if (!hash2[(temp + base * base) % M].key && !hash2[(temp + base * base) % M].value)
        {
            hash2[(temp + base * base) % M].key = key;
            hash2[(temp + base * base) % M].value = value;
            return;
        }
        else if (sign[(temp + base * base) % M] == false)
        {
            sign[(temp + base * base) % M] = true;
            cnt++;
        }

        temp = hashCode;
        while (temp - base * base < 0)
            temp += M;

        if (!hash2[(temp - base * base) % M].key && !hash2[(temp - base * base) % M].value)
        {
            hash2[(temp - base * base) % M].key = key;
            hash2[(temp - base * base) % M].value = value;
            return;
        }
        else if (sign[(temp - base * base) % M] == false)
        {
            sign[(temp - base * base) % M] = true;
            cnt++;
        }

        base++;
    }
}
int squareSearch(int key)
{
    int hashCode = abs(key % M), temp = hashCode;
    int base = 0;
    int cnt = 0;
    bool sign[M + 5] = {false};
    while (1)
    {

        if (cnt >= M)
            return -1;

        temp = hashCode;

        if (hash2[(temp + base * base) % M].key == key)
            return hash2[(temp + base * base) % M].value;
        else if (!hash2[(temp + base * base) % M].key && !hash2[(temp + base * base) % M].value)
            return -1;
        else if (sign[(temp + base * base) % M] == false)
        {
            sign[(temp + base * base) % M] = true;
            cnt++;
        }

        temp = hashCode;
        while (temp - base * base < 0)
            temp += M;

        if (hash2[(temp - base * base) % M].key == key)
            return hash2[(temp - base * base) % M].value;
        else if (!hash2[(temp - base * base) % M].key && !hash2[(temp - base * base) % M].value)
            return -1;
        else if (sign[(temp - base * base) % M] == false)
        {
            sign[(temp - base * base) % M] = true;
            cnt++;
        }

        base++;
    }
    return -1;
}
#endif

#if outmod == 2
struct keyLine
{
    int key;
    int value;
} hash3[M + 5];

void lineInsert(int key, int value)
{
    int hashCode = key % M, temp = hashCode;
    int base = 0;
    while (hash3[(base + temp) % M].key || hash3[(base + temp) % M].value)
    {
        if (base >= M)
        {
            cout << "overflow!" << endl;
            return;
        }
        base++;
    }
    hash3[(base + temp) % M].key = key;
    hash3[(base + temp) % M].value = value;
}
int lineSearch(int key)
{
    int hashCode = key % M, temp = hashCode;
    int base = 0;
    while (hash3[(base + temp) % M].key || hash3[(base + temp) % M].value)
    {
        if (hash3[(base + temp) % M].key == key)
            return hash3[(base + temp) % M].value;
        if (base >= M)
            return -1;
        base++;
    }
    return -1;
}
#endif

int main()
{
    // freopen(textin, "r", stdin);
    // freopen(textout, "w", stdout);
    int op, value;
    string input;
    int a = clock();
    while (1)
    {
        cin >> op;
        // cout << op;
        if (op == 2)
            break;
        input.clear();
        if (op == 0)
        {
            cin >> input >> value;
#if inmod == 0
#if outmod == 0
            chainInsert(ascii2key(input), value);
#endif
#if outmod == 1
            squareInsert(ascii2key(input), value);
#endif
#if outmod == 2
            lineInsert(ascii2key(input), value);
#endif
#endif

#if inmod == 1
#if outmod == 0
            chainInsert(utf2key(input), value);
#endif
#if outmod == 1
            squareInsert(utf2key(input), value);
#endif
#if outmod == 2
            lineInsert(utf2key(input), value);
#endif
#endif
        }
        else if (op == 1)
        {
            cin >> input;
#if inmod == 0
#if outmod == 0
            cout << chainSearch(ascii2key(input)) << endl;
#endif
#if outmod == 1
            cout << squareSearch(ascii2key(input)) << endl;
#endif
#if outmod == 2
            cout << lineSearch(ascii2key(input)) << endl;
#endif
#endif

#if inmod == 1
#if outmod == 0
            cout << chainSearch(utf2key(input)) << endl;
#endif
#if outmod == 1
            cout << squareSearch(utf2key(input)) << endl;
#endif
#if outmod == 2
            cout << lineSearch(utf2key(input)) << endl;
#endif
#endif
        }
    }
#if outmod == 0
    chainDelete();
#endif
    int b = clock();
    cout << "calculte time: " << double(b - a) / CLOCKS_PER_SEC << endl;
    return 0;
}