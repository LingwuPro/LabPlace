#include <bits/stdc++.h>
#include <limits>
#include <stack>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <list>
#define Rank int
#define NumOfVer 80
using namespace std;
vector<int> answerTable;
double epsilon = 0;
bool cmp1(int a, int b) { return a < b; }
using VStatus = enum { UNDISCOVERED,
                       DISCOVERED,
                       VISITED };
using EType = enum { UNDETERMINED,
                     TREE,
                     CROSS,
                     FORWARD,
                     BACKWARD };

struct node
{
    node *parent;
    node *next;
    int ID;
    int thisPlace;
    VStatus staint;
    // node(node *p, node *n, int i) : parent(p), next(n), ID(i)
};

struct Verintx
{
    int data; // ID
    int inDegree, outDegree;
    VStatus status; // State
    int dTime, fTime;
    Rank parent;  // parent's ID
    int priority; // the smallest time that I could come here.
                  // if the startTime is bigger than it, give it up.
    Verintx(int const &d = (int)0) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
                                     dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {
    }
};
struct edgeMsg
{
    int data;
    int startTime, endTime;
    int weight;
    bool operator<(const edgeMsg &a) const { return weight > a.weight; }
};
bool cmp(edgeMsg a, edgeMsg b)
{
    return a.weight > b.weight;
}

struct Edge
{
    vector<int> data; // Flight ID
    vector<int> startTime, endTime;
    vector<int> weight; // fee
    vector<int> model;
    EType type;
    Edge() : data(0), weight(0), type(UNDETERMINED), startTime(0), endTime(0), model(0) {}
};

// graph class
class GraphMatrix
{
public:
    int n;
    int e;
    Verintx V[NumOfVer];
    Edge E[NumOfVer][NumOfVer];
    vector<node> GE;

    GraphMatrix()
    {
        this->n = this->e = 0;
    }
    ~GraphMatrix()
    {
        for (int i = 0; i < NumOfVer; i++)
        {
            node *ptr = &(GE[i]);
            node *temp;
            while (ptr->next)
            {
                temp = ptr->next;
                if (ptr->ID != 0)
                    delete ptr;
                ptr = temp;
            }
        }
    }
    int &verintx(Rank i) { return V[i].data; }
    int inDegree(Rank i) { return V[i].inDegree; }
    int outDegree(Rank i) { return V[i].outDegree; }
    Rank firstNbr(Rank i) { return nextNbr(i, this->n); }
    Rank nextNbr(Rank i, Rank j)
    {
        while ((-1 < j) && (!exists(i, --j)))
            ;
        return j;
    }
    void reset()
    {
        for (Rank i = 0; i < n; i++)
        {
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;
            parent(i) = -1;
            priority(i) = INT_MAX;
            for (Rank j = 0; j < n; j++)
                if (exists(i, j))
                    type(i, j) = UNDETERMINED;
        }
    }
    VStatus &status(Rank i) { return V[i].status; }
    int &dTime(Rank i) { return V[i].dTime; }
    int &fTime(Rank i) { return V[i].fTime; }
    Rank &parent(Rank i) { return V[i].parent; }
    int &priority(Rank i) { return V[i].priority; }

    bool exists(Rank i, Rank j)
    {
        return (i < this->n) && (j < this->n) && E[i][j].data.empty() == false;
    }
    EType &type(Rank i, Rank j) { return E[i][j].type; }
    int &edge(Rank i, Rank j, Rank k) { return E[i][j].data[k]; }
    int &weight(Rank i, Rank j, Rank k) { return E[i][j].weight[k]; }
    void bfs_g(int s);
    void BFS_g(int v, int &clock);
    void dfs(int s);
    void DFS(int v, int &clock);
    int BFS1(int v, int s);
    void DFS1(int v, int s, int times);
    void digkstra(int v, int s);
};

// 2.4
struct proNode
{
    int placeNum;
    int startTime;
    int Endtime;
    int planeID;
    bool operator>(const proNode &x) const
    {
        return Endtime < x.Endtime;
    }
    bool operator<(const proNode &x) const
    {
        return Endtime > x.Endtime;
    }
    proNode(const proNode &a)
    {
        placeNum = a.placeNum;
        startTime = a.startTime;
        Endtime = a.Endtime;
    }
    proNode(int a = 0, int b = 0, int c = 0) : placeNum(a), startTime(b), Endtime(c) {}
};
priority_queue<proNode, vector<proNode>, greater<proNode>> priQueue;
proNode init(int a, int b, int c)
{
    static proNode x{a, b, c};
    return x;
}
int nowTime;
int path[NumOfVer], minTime[NumOfVer];
vector<int> ansbot, tempForEndplace, tempForFlight;
void GraphMatrix::digkstra(int v, int s)
{
    proNode temp = init(V[v].data, 0, 0);
    proNode aim;
    int min = INT_MAX;

    for (int i = 0; i < NumOfVer; i++)
    {
        for (int j = 0; j < E[v][i].data.size(); j++)
        {
            if (E[v][i].data.size() == 0)
            {
                continue;
            }
            while (!priQueue.empty())
            {
                priQueue.pop();
            }
            for (int i = 0; i < NumOfVer; i++)
            {
                minTime[i] = INT_MAX;
            }

            memset(path, 0, sizeof(path));
            path[v] = -1;
            minTime[v] = 0;
            temp.startTime = E[v][i].startTime[j];
            temp.Endtime = E[v][i].endTime[j];
            temp.placeNum = i;
            minTime[i] = 0;
            path[i] = v;
            priQueue.push(temp);
            while (!priQueue.empty())
            {
                aim = priQueue.top();
                int thisPlace = aim.placeNum;
                nowTime = aim.Endtime;
                if (aim.placeNum == s)
                {
                    // for (int i = 0; i < 80; i++)
                    // {
                    //     std::cout << i << ':' << path[i] << '|';
                    // }
                    // std::cout << endl;
                    min = (aim.Endtime - E[v][i].startTime[j]) < min ? (aim.Endtime - E[v][i].startTime[j]) : min;
                    priQueue.pop();
                    continue;
                }
                priQueue.pop();
                for (int i = 0; i < NumOfVer; i++)
                {
                    for (int j = 0; j < (int)E[thisPlace][i].data.size(); j++)
                    {
                        if (nowTime > E[thisPlace][i].startTime[j])
                            continue;
                        if (minTime[i] > E[thisPlace][i].endTime[j])
                        {
                            path[i] = thisPlace;
                            minTime[i] = E[thisPlace][i].endTime[j];
                            temp.Endtime = E[thisPlace][i].endTime[j];
                            temp.placeNum = i;
                            priQueue.push(temp);
                        }
                    }
                }
            }
        }
    }
    if (min != INT_MAX)
    {
        std::cout << min << ' ';
    }
    else
    {
        std::cout << "-1" << ' ';
    }
}

// 2.1 uesed
void GraphMatrix::bfs_g(int s)
{
    answerTable.push_back(s);
    reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
            BFS_g(v, clock);
    } while (s != (v = (++v % n)));
}
void GraphMatrix::BFS_g(int v, int &clock)
{
    queue<int> Q;
    status(v) = DISCOVERED;
    Q.push(v);
    while (!Q.empty())
    {
        int v = Q.front();
        Q.pop();
        dTime(v) = ++clock;
        node *u = GE[v].next;

        while (u)
        {
            if (UNDISCOVERED == status(u->thisPlace))
            {
                answerTable.push_back(u->thisPlace);
                status(u->thisPlace) = DISCOVERED;
                Q.push(u->thisPlace);
                type(v, u->thisPlace) = TREE;
                parent(u->thisPlace) = v;
            }
            else
                type(v, u->thisPlace) = CROSS;
            u = u->next;
        }
    }
    status(v) = VISITED;
}
void GraphMatrix::dfs(int s)
{
    reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v))
        {
            DFS(v, clock);
        }
    } while (s != (v = (++v % n)));
}
void GraphMatrix::DFS(int v, int &clock)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    node *u = GE[v].next;
    while (u)
    {
        if (status(u->thisPlace) == UNDISCOVERED)
        {
            parent(u->thisPlace) = v;
            DFS(u->thisPlace, clock);
        }
        u = u->next;
    }
    status(v) = VISITED;
    fTime(v) = ++clock;
}

// 2.2 used
int GraphMatrix::BFS1(int v, int s)
{
    queue<int> Q;
    int times = 0;
    status(v) = DISCOVERED;
    Q.push(v);
    while (!Q.empty())
    {
        int v = Q.front();
        Q.pop();
        for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
        {
            if (UNDISCOVERED == status(u))
            {
                status(u) = DISCOVERED;
                Q.push(u);
                parent(u) = v;
                // std::cout << u << '|';
                if (u == s)
                {
                    while (u != 0)
                    {
                        u = V[u].parent;
                        times++;
                    }
                    return times - 4;
                }
            }
        }
    }
    return INT_MAX;
}

// 2.3 used
int cnt = 0;
void GraphMatrix::DFS1(int v, int s, int time)
{
    if (time <= -1)
    {
        if (v == s && time == -1)
        {
            cnt++;
        }
        return;
    }
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
    {
        for (int i = 0; i < E[v][u].data.size(); i++)
        {
            if (E[v][u].startTime[i] > V[v].priority)
            {
                int temp = V[u].priority;
                V[u].priority = E[v][u].endTime[i];
                DFS1(u, s, time - 1);
                V[u].priority = temp;
            }
        }
    }
}
struct edge;
struct verx
{
    int time;
    int place;
    int minFee;
    int minTimes;
    int waitTime;
    int value;
    vector<int> passway;
    vector<edge> edgestart;
    bool operator<(const verx &a)
    {
        return minTimes > a.minTimes;
    }
    bool operator>(const verx &a)
    {
        return minTimes < a.minTimes;
    }
    verx(int t = 0, int p = 0, int f = INT_MAX, int mt = INT_MAX, int wt = INT_MAX, int vl = INT_MAX) : time(t), place(p), minFee(f), minTimes(mt), waitTime(wt), value(vl) {}
};
vector<verx> Verx[NumOfVer];
struct edge
{
    int startPlace, endPlace;
    int startTime, endTime;
    int startplace, endplace;
    int fee;
    int flightID;
    int model; // 9 means from myself to myself
    int ismyself;
    edge(int sp, int ep, int st, int et, int f, int ID, int m, int tp) : startplace(sp), endplace(ep), startTime(st), endTime(et), fee(f), flightID(ID), model(m)
    {
        int temp7;
        if (tp == 1)
        {
            bool flag = 0;
            for (int i = 0; i < Verx[sp].size(); i++)
            {
                if (Verx[sp][i].time == st)
                {
                    startPlace = i;
                    flag = 1;
                    Verx[sp][startPlace].edgestart.push_back(*this);
                    temp7 = Verx[sp][startPlace].edgestart.size() - 1;
                    break;
                }
            }
            if (!flag)
            {
                Verx[sp].push_back(verx(st, sp, INT_MAX, INT_MAX, 0));
                startPlace = Verx[sp].size() - 1;
                Verx[sp][startPlace].edgestart.push_back(*this);
                temp7 = Verx[sp][startPlace].edgestart.size() - 1;
                for (int i = 0; i < Verx[sp].size() - 1; i++) // set up self circle
                {
                    if (Verx[sp][i].time < st)
                    {
                        Verx[sp][i].edgestart.push_back(edge(sp, sp, Verx[sp][i].time, st, 0, 0, 9, 0));
                        Verx[sp][i].edgestart[Verx[sp][i].edgestart.size() - 1].startPlace = i;
                        Verx[sp][i].edgestart[Verx[sp][i].edgestart.size() - 1].endPlace = startPlace;
                    }
                    else if (Verx[sp][i].time > st)
                    {
                        Verx[sp][startPlace].edgestart.push_back(edge(sp, sp, st, Verx[sp][i].time, 0, 0, 9, 0));
                        Verx[sp][startPlace].edgestart[Verx[sp][startPlace].edgestart.size() - 1].endPlace = i;
                        Verx[sp][startPlace].edgestart[Verx[sp][startPlace].edgestart.size() - 1].startPlace = startPlace;
                    }
                    else
                    {
                        std::cout << "error 1!";
                        std::exit(-1);
                    }
                }
            }
            flag = 0;
            for (int i = 0; i < Verx[ep].size(); i++)
            {
                if (Verx[ep][i].time == et)
                {
                    endPlace = i;
                    flag = 1;
                    Verx[sp][startPlace].edgestart[temp7].endPlace = endPlace;
                    break;
                }
            }
            if (!flag)
            {
                Verx[ep].push_back({et, ep, INT_MAX, INT_MAX, 0});
                endPlace = Verx[ep].size() - 1;
                Verx[sp][startPlace].edgestart[temp7].endPlace = endPlace;
                for (int i = 0; i < Verx[ep].size() - 1; i++) // set up self circle
                {
                    if (Verx[ep][i].time < et)
                    {
                        Verx[ep][i].edgestart.push_back(edge(ep, ep, Verx[ep][i].time, et, 0, 0, 9, 0));
                        Verx[ep][i].edgestart[Verx[ep][i].edgestart.size() - 1].startPlace = i;
                        Verx[ep][i].edgestart[Verx[ep][i].edgestart.size() - 1].endPlace = endPlace;
                    }
                    else if (Verx[ep][i].time > et)
                    {
                        Verx[ep][endPlace].edgestart.push_back(edge(ep, ep, et, Verx[ep][i].time, 0, 0, 9, 0));
                        Verx[ep][endPlace].edgestart[Verx[ep][endPlace].edgestart.size() - 1].endPlace = i;
                        Verx[ep][endPlace].edgestart[Verx[ep][endPlace].edgestart.size() - 1].startPlace = endPlace;
                    }

                    else
                    {
                        std::cout << "error 2!";
                        std::exit(-1);
                    }
                }
            }
        }
    }
    edge(const edge &a)
    {
        startPlace = a.startPlace;
        endPlace = a.endPlace;
        startplace = a.startplace;
        endplace = a.endplace;
        startTime = a.startTime;
        endTime = a.endTime;
        fee = a.fee;
        flightID = a.flightID;
        model = a.model;
    }
    edge() {}
} edge1[NumOfVer][NumOfVer];
struct cmpTimes : verx
{
    bool operator()(const verx &a, const verx &b)
    {
        return a.minTimes > b.minTimes;
    }
};
struct cmpFee : verx
{
    bool operator()(const verx &a, const verx &b)
    {
        return a.minFee > b.minFee;
    }
};
struct cmpTimeswithWait : verx
{
    bool operator()(const verx &a, const verx &b)
    {
        if (a.minTimes != b.minTimes)
        {
            return a.minTimes > b.minTimes;
        }
        return a.waitTime > b.waitTime;
        // if (a.value != b.value)
        //     return a.value > b.value;
        // return a.minTimes > b.minTimes;
    }
};
struct cmpFeewithWait : verx
{
    bool operator()(const verx &a, const verx &b)
    {
        if (a.minFee != b.minFee)
        {
            return a.minFee > b.minFee;
        }
        return a.waitTime > b.waitTime;
        // if (a.value != b.value)
        //     return a.value > b.value;
        // return a.minFee > b.minFee;
    }
};
priority_queue<verx, vector<verx>, cmpTimes> save1;
priority_queue<verx, vector<verx>, cmpFee> save2;
priority_queue<verx, vector<verx>, cmpTimeswithWait> save3;
priority_queue<verx, vector<verx>, cmpFeewithWait> save4;
bool hasAnswer = false;
void digkstra(int s, int v, int k, int model, int src1, int src2)
{
    while (!save1.empty())
    {
        save1.pop();
    }
    verx temp1;
    for (int i = 0; i < NumOfVer; i++)
    {
        for (auto j : Verx[i])
        {
            j.minFee = INT_MAX;
            j.minTimes = INT_MAX;
        }
    }
    for (auto i : Verx[s])
    {
        if (model == 1 && (i.time < src1 || i.time > src2))
        {
            continue;
        }
        for (auto j : i.edgestart)
        {

            if (j.model != src1 && model == 3)
            {
                continue;
            }
            if (j.flightID)
            {
                Verx[j.endplace][j.endPlace].minTimes = 1;
                Verx[j.endplace][j.endPlace].passway.push_back(j.flightID);
                save1.push(Verx[j.endplace][j.endPlace]);
            }
        }
    }
    while (!save1.empty())
    {
        temp1 = save1.top();
        // std::cout << temp1.minTimes << '|';
        save1.pop();
        if (temp1.place == v)
        {
            // cout << temp1.minTimes << '|';
            hasAnswer = true;
            if (temp1.minTimes > k)
            {
                std::cout << "-1" << endl;
                return;
            }
            for (int p = 0; p < temp1.passway.size(); p++)
            {
                std::cout << temp1.passway[p] << ' ';
            }
            std::cout << endl;
            return;
        }
        for (auto i : temp1.edgestart)
        {
            //  std::cout << temp1.time << "->" << i.endTime << " is " << Verx[i.endplace][i.endPlace].minTimes << '|';

            if (model == 2 && (Verx[i.endplace][i.endPlace].time < src1 || Verx[i.endplace][i.endPlace].time > src2))
            {
                continue;
            }
            int temp2 = Verx[i.endplace][i.endPlace].minTimes;
            if (i.model == 9)
            {
                Verx[i.endplace][i.endPlace].minTimes = min(temp1.minTimes, Verx[i.endplace][i.endPlace].minTimes);
            }
            else if (model == 3 && i.model != src1)
            {
                continue;
            }
            else
            {
                Verx[i.endplace][i.endPlace].minTimes = temp1.minTimes + 1;
            }
            if (Verx[i.endplace][i.endPlace].minTimes != temp2)
            {
                Verx[i.endplace][i.endPlace].passway.clear();
                for (auto m : temp1.passway)
                {
                    Verx[i.endplace][i.endPlace].passway.push_back(m);
                }
                if (i.flightID)
                    Verx[i.endplace][i.endPlace].passway.push_back(i.flightID);
                save1.push(Verx[i.endplace][i.endPlace]);
                // std::cout << Verx[i.endplace][i.endPlace].minTimes << '|';
            }
        }
    }
}

void digkstra(int s, int v, int model, int src1, int src2)
{
    while (!save2.empty())
    {
        save2.pop();
    }
    verx temp1;
    for (int i = 0; i < NumOfVer; i++)
    {
        for (auto j : Verx[i])
        {
            j.minFee = INT_MAX;
            j.minTimes = INT_MAX;
        }
    }
    for (auto i : Verx[s])
    {
        if (model == 1 && (i.time < src1 || i.time > src2))
        {
            continue;
        }
        for (auto j : i.edgestart)
        {

            if (j.model != src1 && model == 3)
            {
                continue;
            }
            if (j.flightID)
            {
                Verx[j.endplace][j.endPlace].minFee = j.fee;
                Verx[j.endplace][j.endPlace].passway.push_back(j.flightID);
                save2.push(Verx[j.endplace][j.endPlace]);
            }
        }
    }
    while (!save2.empty())
    {
        temp1 = save2.top();
        save2.pop();
        if (temp1.place == v)
        {
            hasAnswer = true;
            for (int p = 0; p < temp1.passway.size(); p++)
            {
                std::cout << temp1.passway[p] << ' ';
            }
            std::cout << endl;
            std::cout << temp1.minFee << endl;
            return;
        }
        for (auto i : temp1.edgestart)
        {
            //  std::cout << temp1.time << "->" << i.endTime << " is " << Verx[i.endplace][i.endPlace].minTimes << '|';
            if (model == 2 && (Verx[i.endplace][i.endPlace].time < src1 || Verx[i.endplace][i.endPlace].time > src2))
            {
                continue;
            }
            int temp2 = Verx[i.endplace][i.endPlace].minFee;
            if (i.model == 9)
            {
                Verx[i.endplace][i.endPlace].minFee = min(temp1.minFee, Verx[i.endplace][i.endPlace].minFee);
            }
            else if (model == 3 && i.model != src1)
            {
                continue;
            }
            else
            {
                Verx[i.endplace][i.endPlace].minFee = temp1.minFee + i.fee;
            }
            if (Verx[i.endplace][i.endPlace].minFee != temp2)
            {
                Verx[i.endplace][i.endPlace].passway.clear();
                for (auto m : temp1.passway)
                {
                    Verx[i.endplace][i.endPlace].passway.push_back(m);
                }
                if (i.flightID)
                    Verx[i.endplace][i.endPlace].passway.push_back(i.flightID);
                save2.push(Verx[i.endplace][i.endPlace]);
                // std::cout << Verx[i.endplace][i.endPlace].minTimes << '|';
            }
        }
    }
}

void digkstra(int s, int v, int waitTime)
{
    while (!save4.empty())
    {
        save4.pop();
    }
    verx temp1;
    for (int i = 0; i < NumOfVer; i++)
    {
        for (auto j : Verx[i])
        {
            j.minFee = INT_MAX;
            j.minTimes = INT_MAX;
            j.passway.clear();
            j.waitTime = INT_MAX;
            j.value = INT_MAX;
        }
    }
    for (auto i : Verx[s])
    {
        for (auto j : i.edgestart)
        {
            if (j.flightID)
            {
                Verx[j.endplace][j.endPlace].minFee = j.fee;
                Verx[j.endplace][j.endPlace].waitTime = 0;
                Verx[j.endplace][j.endPlace].passway.push_back(j.flightID);
                save4.push(Verx[j.endplace][j.endPlace]);
            }
        }
    }
    while (!save4.empty())
    {
        temp1 = save4.top();
        save4.pop();
        // if (temp1.place == 49 && temp1.time == 3815)
        // {
        //     cout << temp1.waitTime << '|';
        // }
        // cout << temp1.place << '|';
        if (temp1.waitTime > waitTime)
        {
            continue;
        }
        if (temp1.place == v)
        {
            // cout << temp1.waitTime << '|';
            hasAnswer = true;
            for (int p = 0; p < temp1.passway.size(); p++)
            {
                std::cout << temp1.passway[p] << ' ';
            }
            std::cout << endl;
            std::cout << temp1.minFee << endl;
            return;
        }
        for (auto i : temp1.edgestart)
        {
            //  std::cout << temp1.time << "->" << i.endTime << " is " << Verx[i.endplace][i.endPlace].minTimes << '|';
            int temp2 = Verx[i.endplace][i.endPlace].minFee;
            int temp3 = Verx[i.endplace][i.endPlace].waitTime;
            if (i.model == 9)
            {

                if (Verx[i.endplace][i.endPlace].minFee > temp1.minFee || Verx[i.endplace][i.endPlace].waitTime > temp1.waitTime + Verx[i.endplace][i.endPlace].time - temp1.time)
                {
                    Verx[i.endplace][i.endPlace].minFee = temp1.minFee + i.fee;
                    Verx[i.endplace][i.endPlace].waitTime = temp1.waitTime + Verx[i.endplace][i.endPlace].time - temp1.time;
                }
            }
            else
            {
                Verx[i.endplace][i.endPlace].minFee = temp1.minFee + i.fee;
                Verx[i.endplace][i.endPlace].waitTime = temp1.waitTime;
            }
            if (Verx[i.endplace][i.endPlace].minFee != temp2 || Verx[i.endplace][i.endPlace].waitTime != temp3)
            {
                Verx[i.endplace][i.endPlace].passway.clear();
                for (auto m : temp1.passway)
                {
                    Verx[i.endplace][i.endPlace].passway.push_back(m);
                }
                if (i.flightID)
                    Verx[i.endplace][i.endPlace].passway.push_back(i.flightID);
                save4.push(Verx[i.endplace][i.endPlace]);
            }
        }
    }
}

void digkstra(int s, int v, int k, int waitTime)
{
    for (int i = 0; i < NumOfVer; i++)
    {
        for (auto j : Verx[i])
        {
            j.minFee = INT_MAX;
            j.minTimes = INT_MAX;
            j.passway.clear();
            j.value = INT_MAX;
            j.waitTime = INT_MAX;
        }
    }
    while (!save3.empty())
    {
        save3.pop();
    }
    verx temp1;
    for (auto i : Verx[s])
    {
        for (auto j : i.edgestart)
        {
            if (j.flightID)
            {
                for (auto m : Verx[j.endplace][j.endPlace].passway)
                {
                    if (m == j.flightID)
                    {
                        continue;
                    }
                }
                Verx[j.endplace][j.endPlace].minTimes = 0;
                Verx[j.endplace][j.endPlace].waitTime = 0;
                Verx[j.endplace][j.endPlace].value = 0;
                Verx[j.endplace][j.endPlace].passway.push_back(j.flightID);
                save3.push(Verx[j.endplace][j.endPlace]);
                // if (j.flightID == 83)
                // {
                //     std::cout << j.endplace << '|' << j.endTime << endl;
                // }
            }
        }
    }
    while (!save3.empty())
    {
        temp1 = save3.top();
        // cout << temp1.waitTime << '|';
        save3.pop();
        // if (temp1.place == 49 && temp1.time == 3835 && epsilon == 0)
        // {
        //     cout << temp1.waitTime << '|' << temp1.minTimes << endl;
        //     for (auto m : temp1.edgestart)
        //     {
        //         cout << m.flightID << '|';
        //     }
        //     cout << endl;
        // }
        // std::cout << temp1.minTimes << '|';
        if (temp1.waitTime > waitTime)
            continue;
        if (temp1.minTimes >= k)
            continue;
        if (temp1.place == v)
        {
            // cout << temp1.minTimes << '|';
            hasAnswer = true;
            // cout << epsilon << endl;
            for (int p = 0; p < temp1.passway.size(); p++)
            {
                if (p != 0 && temp1.passway[p] == temp1.passway[0])
                    continue;
                std::cout << temp1.passway[p] << ' ';
            }
            std::cout << endl;
            // std::cout << temp1.waitTime << '|';
            return;
        }
        for (auto i : temp1.edgestart)
        {
            int temp2 = Verx[i.endplace][i.endPlace].minTimes;
            int temp3 = Verx[i.endplace][i.endPlace].waitTime;
            if (i.model == 9)
            {
                if (Verx[i.endplace][i.endPlace].minTimes > temp1.minTimes || Verx[i.endplace][i.endPlace].waitTime > temp1.waitTime + Verx[i.endplace][i.endPlace].time - temp1.time)
                {
                    Verx[i.endplace][i.endPlace].minTimes = temp1.minTimes;
                    Verx[i.endplace][i.endPlace].waitTime = temp1.waitTime + Verx[i.endplace][i.endPlace].time - temp1.time;
                }
            }
            else
            {
                Verx[i.endplace][i.endPlace].minTimes = temp1.minTimes + 1;
                Verx[i.endplace][i.endPlace].waitTime = temp1.waitTime;
            }
            if (Verx[i.endplace][i.endPlace].minTimes != temp2 || Verx[i.endplace][i.endPlace].waitTime != temp3)
            {
                Verx[i.endplace][i.endPlace].passway.clear();
                for (auto m : temp1.passway)
                {
                    Verx[i.endplace][i.endPlace].passway.push_back(m);
                }
                if (i.flightID)
                    Verx[i.endplace][i.endPlace].passway.push_back(i.flightID);
                save3.push(Verx[i.endplace][i.endPlace]);

                // std::cout << Verx[i.endplace][i.endPlace].minTimes << '|';
            }
        }
    }
}

int str2time(string &cuf)
{
    int startTimes = 0;
    startTimes += (cuf[2] - '5') * 24 * 60;
    if (cuf[10] == ':')
    {
        startTimes += (cuf[9] - '0') * 60;
        startTimes += (cuf[11] - '0') * 10;
        startTimes += (cuf[12] - '0');
    }
    else
    {
        startTimes += (cuf[9] - '0') * 10 * 60;
        startTimes += (cuf[10] - '0') * 60;
        startTimes += (cuf[12] - '0') * 10;
        startTimes += (cuf[13] - '0');
    }
    return startTimes;
}

int main()
{
    GraphMatrix graph;
    ifstream fin("data.csv");
    // freopen("output.txt", "w", stdout);

    string intmp;
    std::getline(fin, intmp);
    node Node;
    Node.next = nullptr;
    Node.parent = nullptr;
    Node.staint = UNDISCOVERED;
    for (int i = 0; i < NumOfVer; i++)
    {
        graph.GE.push_back(Node);
    }
    while (std::getline(fin, intmp))
    {
        istringstream sin(intmp);
        string cuf;

        int ID;
        const char *cuf1 = cuf.data();
        std::getline(sin, cuf, ',');
        ID = atoi(cuf1);

        std::getline(sin, cuf, ',');

        std::getline(sin, cuf, ',');

        std::getline(sin, cuf, ',');

        int startPlace;
        std::getline(sin, cuf, ',');

        cuf1 = cuf.data();
        startPlace = atoi(cuf1);

        int endPlace;
        std::getline(sin, cuf, ',');
        cuf1 = cuf.data();
        endPlace = atoi(cuf1);

        int startTimes = 0;
        std::getline(sin, cuf, ',');

        startTimes += (cuf[2] - '5') * 24 * 60;
        if (cuf[10] == ':')
        {
            startTimes += (cuf[9] - '0') * 60;
            startTimes += (cuf[11] - '0') * 10;
            startTimes += (cuf[12] - '0');
        }
        else
        {
            startTimes += (cuf[9] - '0') * 10 * 60;
            startTimes += (cuf[10] - '0') * 60;
            startTimes += (cuf[12] - '0') * 10;
            startTimes += (cuf[13] - '0');
        }

        int endTimes = 0;
        std::getline(sin, cuf, ',');

        endTimes += (cuf[2] - '5') * 24 * 60;
        if (cuf[10] == ':')
        {
            endTimes += (cuf[9] - '0') * 60;
            endTimes += (cuf[11] - '0') * 10;
            endTimes += (cuf[12] - '0');
        }
        else
        {
            endTimes += (cuf[9] - '0') * 10 * 60;
            endTimes += (cuf[10] - '0') * 60;
            endTimes += (cuf[12] - '0') * 10;
            endTimes += (cuf[13] - '0');
        }

        std::getline(sin, cuf, ',');

        int model = 0;
        std::getline(sin, cuf, ',');
        cuf1 = cuf.data();
        model = atoi(cuf1);

        int fee = 0;
        std::getline(sin, cuf, ',');
        cuf1 = cuf.data();
        fee = atoi(cuf1);

        graph.E[startPlace][endPlace].weight.push_back(fee);
        graph.E[startPlace][endPlace].data.push_back(ID);
        graph.E[startPlace][endPlace].startTime.push_back(startTimes);
        graph.E[startPlace][endPlace].endTime.push_back(endTimes);
        graph.E[startPlace][endPlace].type = UNDETERMINED;
        graph.E[startPlace][endPlace].model.push_back(model);

        node *Nodel = new node();
        Nodel->ID = ID;
        Nodel->thisPlace = endPlace;
        Nodel->next = nullptr;
        node *ptr = &(graph.GE[startPlace]);

        while (ptr->next)
        {
            ptr = ptr->next;
        }
        ptr->next = Nodel;
        Nodel->parent = ptr;

        if (graph.E[startPlace][endPlace].data.size() == 1)
        {
            graph.e++;
        }

        if (graph.V[startPlace].data == 0)
        {
            graph.V[startPlace].status = UNDISCOVERED;
            graph.V[startPlace].data = startPlace;
            graph.n++;
        }
        graph.V[startPlace].parent = 0;
        graph.V[startPlace].priority = 0;

        edge temp3(startPlace, endPlace, startTimes, endTimes, fee, ID, model, 1);
        edge1[startPlace][endPlace] = temp3;
    }
    ofstream();
    // digkstra(28, 74, 4, 1, 0, 100000); // 2.5.1
    int model1, minimodel, arg1, arg2, arg3, arg4;
    int tempc = INT_MAX;
    string timestring1, timestring2, temp;
    std::cin >> model1;
    switch (model1)
    {
    case 1:
        std::cin >> arg1;
        graph.bfs_g(arg1);
        sort(answerTable.begin(), answerTable.end(), cmp1);
        for (auto i = answerTable.begin(); i != answerTable.end(); i++) // 2.1
        {
            std::cout << *i << ' ';
        }
        std::cout << endl;
        graph.dfs(arg1);
        sort(answerTable.begin(), answerTable.end(), cmp1);
        for (auto i = answerTable.begin(); i != answerTable.end(); i++) // 2.1
        {
            std::cout << *i << ' ';
        }
        std::cout << endl;
        break;
    case 2:
        cin >> arg1;
        for (int i = 1; i < NumOfVer - 1; i++)
        {
            for (int j = 1; j < NumOfVer - 1; j++) // 2.2
            {
                graph.reset();
                tempc = graph.BFS1(i, j);
                if (arg1 == -1 && tempc != INT_MAX)
                {
                    cout << "1 ";
                }
                else if (tempc <= arg1)
                {
                    cout << "1 ";
                }
                else
                    cout << "0 ";
            }
            std::cout << endl;
        }
        break;
    case 3:
        std::cin >> arg1 >> arg2 >> arg3;
        graph.DFS1(arg1, arg2, arg3); // 2.3
        std::cout << cnt << endl;
        break;
    case 4:
        std::cin >> arg1 >> arg2;
        graph.digkstra(arg1, arg2); // 2.4
        break;
    case 5:
        std::cin >> minimodel;
        if (minimodel == 1)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2 >> arg3;
            std::cin >> timestring1 >> temp;
            timestring1 += ' ';
            timestring1 += temp;
            std::cin >> timestring2 >> temp;
            timestring2 += ' ';
            timestring2 += temp;
            digkstra(arg1, arg2, arg3, 1, str2time(timestring1), str2time(timestring2)); // 2.5.1
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else if (minimodel == 2)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2 >> arg3;
            std::cin >> timestring1 >> temp;
            timestring1 += ' ';
            timestring1 += temp;
            std::cin >> timestring2 >> temp;
            timestring2 += ' ';
            timestring2 += temp;
            digkstra(arg1, arg2, arg3, 2, str2time(timestring1), str2time(timestring2)); // 2.5.1
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else if (minimodel == 3)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2 >> arg3 >> arg4;
            digkstra(arg1, arg2, arg3, 3, arg4, 0); // 2.5.3
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else
        {
            std::cout << "wrong in 5!" << endl;
            std::exit(-1);
        }
        break;
    case 6:
        std::cin >> minimodel;
        if (minimodel == 1)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2;
            std::cin >> timestring1 >> temp;
            timestring1 += ' ';
            timestring1 += temp;
            std::cin >> timestring2 >> temp;
            timestring2 += ' ';
            timestring2 += temp;
            digkstra(arg1, arg2, 1, str2time(timestring1), str2time(timestring2)); // 2.5.1
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else if (minimodel == 2)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2;
            std::cin >> timestring1 >> temp;
            timestring1 += ' ';
            timestring1 += temp;
            std::cin >> timestring2 >> temp;
            timestring2 += ' ';
            timestring2 += temp;
            digkstra(arg1, arg2, 2, str2time(timestring1), str2time(timestring2)); // 2.5.1
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else if (minimodel == 3)
        {
            hasAnswer = false;
            std::cin >> arg1 >> arg2 >> arg3;
            digkstra(arg1, arg2, 3, arg3, 0); // 2.5.1
            if (hasAnswer == false)
            {
                std::cout << "-1" << endl;
            }
        }
        else
        {
            std::cout << "wrong in 6!" << endl;
            std::exit(-1);
        }
        break;
    case 7:
        std::cin >> arg1 >> arg2 >> arg3 >> arg4;
        hasAnswer = false;
        digkstra(arg1, arg2, arg3, arg4);
        if (hasAnswer == false)
        {
            std::cout << "-1" << endl;
        }
        break;
    case 8:
        std::cin >> arg1 >> arg2 >> arg3;
        hasAnswer = false;
        digkstra(arg1, arg2, arg3);
        if (hasAnswer == false)
        {
            std::cout << "-1" << endl;
        }
        break;
    default:
        std::cout << "wrong in!" << endl;
        std::exit(-1);
        break;
    }
    return 0;
}
