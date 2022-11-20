#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int llrand()
{
    int output = rand() + rand() + rand();
    output += (rand() + rand() + rand()) * 10;
    output += (rand() + rand() + rand()) * 100;
    output += (rand() + rand() + rand()) * 1000;
    output += (rand() + rand() + rand()) * 10000;
    output += (rand() + rand() + rand()) * 100000;

    return abs(output);
}
int main()
{
    int n = (llrand() % 100000) + 1;
    int m = (llrand() % 100000) + 1;
    cout << n << ' ' << m << endl;
    for (int i = 0; i < m; i++)
    {
        int l = (llrand() % n) + 1;
        int r = (llrand() % (n - l)) + l + 1;
        cout << l << ' ' << r << endl;
    }
}