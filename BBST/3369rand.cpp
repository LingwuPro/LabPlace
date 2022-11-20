#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int node[100005], size = 0, realsize = 0;
int llrand()
{
    int output = rand();
    output += rand() + rand() + rand();
    output += (rand() + rand() + rand()) * 100000;
    // output += (rand() + rand() + rand()) * 1000000;
    return abs(output);
}
int main()
{
    srand(clock());
    int op = llrand() % 100000;
    cout << op << endl;
    for (int i = 0; i < op; i++)
    {
        int n = (llrand() % 14) + 1;
        if (n > 7 || n < 1)
        {
            n = 1;
        }
        else if (n == 7)
        {
            n = 2;
        }
        if (n == 1 || realsize > 5)
            cout << n << ' ';
        else
        {
            n = 1;
            cout << n << ' ';
        }
        int num, temp, temp1;
        switch (n)
        {
        case 1:
            num = (llrand() % 1000000) - 500000;
            cout << num << endl;
            node[size++] = num;
            realsize++;
            break;
        case 2:
            temp = llrand() % (size + 1);
            while (!node[temp])
            {
                temp = llrand() % (size + 1);
            }
            cout << node[temp] << endl;
            node[temp] = 0;
            realsize--;
            break;
        case 3:
            temp = llrand() % (size + 1);
            while (!node[temp])
            {
                temp = llrand() % (size + 1);
            }
            cout << node[temp] << endl;
            break;
        case 4:
            num = (llrand() % realsize) + 1;
            cout << num << endl;
            break;
        case 5:
            temp = llrand() % (size + 1);
            while (!node[temp])
            {
                temp = llrand() % (size + 1);
            }
            temp1 = llrand() % (size + 1);
            while (!node[temp1] && node[temp] != node[temp1])
            {
                temp1 = llrand() % (size + 1);
            }
            cout << max(node[temp], node[temp1]) << endl;
            break;
        case 6:
            temp = llrand() % size + 1;
            while (!node[temp])
            {
                temp = llrand() % size + 1;
            }
            temp1 = llrand() % (size + 1);
            while (!node[temp1] && node[temp] != node[temp1])
            {
                temp1 = llrand() % (size + 1);
            }
            cout << min(node[temp], node[temp1]) << endl;
            break;
        }
    }
}