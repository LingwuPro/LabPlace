#include <iostream>
#include <vector>
using namespace std;

int calcu(int n, vector<vector<int>> a)
{
    if (n == 2)
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    else
    {
        int sum = 0;
        for (int k = 0; k < n; k++)
        {
            vector<vector<int>> b;
            for (int i = 1; i < n; i++)
            {
                vector<int> c;
                for (int j = 0; j < n; j++)
                {
                    if (j == k)
                        continue;
                    c.push_back(a[i][j]);
                }
                b.push_back(c);
            }
            sum = k % 2 == 0 ? sum + a[0][k] * calcu(n - 1, b) : sum - a[0][k] * calcu(n - 1, b);
        }
        return sum;
    }
}

int main()
{
    vector<vector<int>> a;
    if (hrz == 1)
    {
        cout << x << endl;
    }
    else
    {
        for (int i = 0; i < hrz; i++)
        {
            vector<int> b;
            for (int j = 0; j < hrz; j++)
            {
                b.push_back(xMatrix[i][j]);
            }
            a.push_back(b);
        }
        cout << calcu(hrz, a) << endl;
    }
}
