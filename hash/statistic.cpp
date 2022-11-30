#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;
vector<string> nameIn;
vector<int> hasIn;
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
    string temp;
    ifstream myfile("hdu.txt");
    freopen("data6.in", "w", stdout);
    while (getline(myfile, temp)) //按行读取字符串
    {
        istringstream sin(temp);
        string cuf;

        int ID;
        getline(sin, cuf, ' ');
        getline(sin, cuf, ' ');
        // cout << cuf << endl;
        nameIn.push_back(cuf);
    }

    srand((int)time(0));
    int n = llrand() % 500000;
    int tempNum;
    for (int i = 0; i < n; i++)
    {
        cout << "0 ";
        tempNum = llrand() % nameIn.size();
        hasIn.push_back(tempNum);
        cout << nameIn[tempNum] << ' ';
        cout << (llrand() % 100000) - 50000 << endl;
    }
    int n2 = llrand() % 500000;
    for (int i = 0; i < n2; i++)
    {
        cout << "1 ";
        tempNum = llrand() % hasIn.size();
        cout << nameIn[hasIn[tempNum]] << endl;
    }
    cout << '2' << endl;
}
