#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <map>
#include <queue>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;

/*
    auto start = system_clock::now();
    -----Function to Track-----
    auto end = system_clock::now();
 
    duration<double> elapsed_seconds = end-start;
    time_t end_time = system_clock::to_time_t(end);
 
    cout << "finished computation at " << ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
*/

class Tracker
{
private:
    time_point<system_clock> start, end;
    string name;
    int counter;

public:
    Tracker(string name = "") : name(name), counter(0){};
    void setName(string name)
    {
        this->name = name;
    }
    void setCounter()
    {
        counter = 0;
    }
    void Start()
    {
        start = system_clock::now();
        counter++;
    }
    void Stop(int print = 1)
    {
        end = system_clock::now();
        if (print)
            PrintAll();
    }
    void PrintAll()
    {
        duration<double> elapsed_seconds = end - start;
        time_t end_time = system_clock::to_time_t(end);
        if (name != "")
            cout << "Function " + name << endl;
        cout << "Step " << counter << ") Finished computation at " << ctime(&end_time) << "Delta: " << elapsed_seconds.count() << "s\n\n";
    }
    void Delta()
    {
        duration<double> elapsed_seconds = end - start;
        cout << "Delta: " << elapsed_seconds.count() << "s\n\n";
    }
};
class Customer
{

public:
    int x, y, reward;
    Customer()
    {
        cin >> x >> y >> reward;
    }
};

class PathStatus
{
public:
    int idNode, score;
    string path;
    PathStatus(int idNode, int score, string path) : idNode(idNode), score(score), path(path) {}
};

class PathStatusQueue : public PathStatus
{
public:
    int x, y, startScore;
    PathStatusQueue(int idNode, int score, string path, int x, int y) : PathStatus(idNode, score, path), x(x), y(y), startScore(score){};
};

class CellStatus
{
private:
    int globalScore, price, conflict; //always max
    map<int, PathStatus *> paths;

public:
    CellStatus(int price) : globalScore(0), price(price), conflict(0){};
    bool Test()
    {
        if (price == 0)
            return true;
        return false;
    }
    bool tryCell(PathStatusQueue const &agentPath, char move)
    {
        int newScore = agentPath.score - price;
        if (price > 0 && newScore > 0 && agentPath.path.size() <= 75)
        {
            PathStatus *targetStatus = paths[agentPath.idNode];
            if (targetStatus != NULL)
            {
                //exist
                if (newScore > (*targetStatus).score)
                {
                    globalScore += newScore - (*targetStatus).score;
                    (*targetStatus).path = agentPath.path + move;
                    (*targetStatus).score = newScore;
                    return true;
                }
                return false;
            }
            else
            {
                paths[agentPath.idNode] = new PathStatus(agentPath.idNode, newScore, agentPath.path + move);
                globalScore += newScore;
                if (paths.size() > 1)
                    return false;
                return true;
            }
        }
        return false;
    }
    bool changed(PathStatusQueue const &agentPath)
    {
        PathStatus *targetStatus = paths[agentPath.idNode];
        if (targetStatus->score == agentPath.score)
            return true;
        return false;
    }
};

class Map
{
private:
    vector<vector<int>> mapConverted;
    vector<Customer *> customers;
    vector<int> convertRow(string toConvert)
    {
        vector<int> toRes;
        map<char, int> conversion2 = {{'#', -1}, {'~', 800}, {'*', 200}, {'+', 150}, {'X', 120}, {'_', 100}, {'H', 70}, {'T', 50}};
        //for(auto x = toConvert.begin)
        for_each(toConvert.begin(), toConvert.end(), [&toRes, &conversion2](char &simbol) {
            if (conversion2[simbol] != 0)
                toRes.push_back(conversion2[simbol]);
        });
        return toRes;
    }

public:
    void Create(vector<string> &mapNaive)
    {
        for (auto &x : mapNaive)
        {
            vector<int> temp = convertRow(x);
            mapConverted.push_back(temp);
        }
    }
    void addCustomers(int nCustomer)
    {
        for (int i = 0; i < nCustomer; i++)
            customers.push_back(new Customer());
    };
    void PrintMap()
    {
        for (auto &x : mapConverted)
        {
            for (auto &y : x)
                cout << y << "  ";
            cout << endl;
        }
    }
    void PrintCustomers()
    {
        for (auto &x : customers)
            cout << (*x).reward << endl;
    }
    auto ExpandCustomer();
};
auto Map::ExpandCustomer()
{
    int maxX, maxY;
    Tracker tracker("Expansion Customer Algo");
    queue<PathStatusQueue *> pathQueue;
    PathStatusQueue *targetPath;
    vector<vector<CellStatus *>> expansionMap;
    for (auto &y : mapConverted)
    {
        expansionMap.resize(expansionMap.size() + 1);
        for (auto &x : y)
        {
            expansionMap[expansionMap.size() - 1].push_back(new CellStatus(x));
        }
    }
    maxX = expansionMap[0].size() - 1;
    maxY = expansionMap.size() - 1;
    // Inizialization
    for (int i = 0; i < customers.size(); i++)
    {
        targetPath = new PathStatusQueue(i, (*customers[i]).reward, "", (*customers[i]).x, (*customers[i]).y);
        if (expansionMap[(*targetPath).y][(*targetPath).x]->tryCell(*targetPath, '0'))
            pathQueue.push(targetPath);
        else
            delete targetPath;
    }
    // Expansion

    tracker.Start();
    while (!pathQueue.empty())
    {
        targetPath = pathQueue.front();
        if ((*targetPath).x > 0 &&
            expansionMap[(*targetPath).y][(*targetPath).x - 1]->tryCell(*targetPath, 'L'))
            // go L
            pathQueue.push(new PathStatusQueue(targetPath->idNode, targetPath->score - mapConverted[targetPath->y][targetPath->x - 1], targetPath->path + 'L', targetPath->x - 1, targetPath->y));

        if ((*targetPath).x < maxX &&
            expansionMap[(*targetPath).y][(*targetPath).x + 1]->tryCell(*targetPath, 'R'))
            // go R
            pathQueue.push(new PathStatusQueue(targetPath->idNode, targetPath->score - mapConverted[targetPath->y][targetPath->x + 1], targetPath->path + 'R', targetPath->x + 1, targetPath->y));

        if ((*targetPath).y > 0 &&
            expansionMap[(*targetPath).y - 1][(*targetPath).x]->tryCell(*targetPath, 'U'))
            // go U
            pathQueue.push(new PathStatusQueue(targetPath->idNode, targetPath->score - mapConverted[targetPath->y - 1][targetPath->x], targetPath->path + 'U', targetPath->x, targetPath->y - 1));

        if ((*targetPath).y < maxY &&
            expansionMap[(*targetPath).y + 1][(*targetPath).x]->tryCell(*targetPath, 'D'))
            // go D
            pathQueue.push(new PathStatusQueue(targetPath->idNode, targetPath->score - mapConverted[targetPath->y + 1][targetPath->x], targetPath->path + 'D', targetPath->x, targetPath->y + 1));
        pathQueue.pop();
        delete targetPath;
    }
    tracker.Stop();
};

int main()
{
    int mapX, mapY, nCustomerOffice, nReplyOffice;
    vector<string> mapNaive;
    Map mapRefined;
    cin >> mapX >> mapY >> nCustomerOffice >> nReplyOffice;
    mapRefined.addCustomers(nCustomerOffice);
    for (int i = 0; i <= mapY; i++)
    {
        string iStr;
        getline(cin, iStr);
        if (iStr.size() - 1 == mapX)
            mapNaive.push_back(iStr);
    }
    mapRefined.Create(mapNaive);
    //mapRefined.PrintMap();
    mapRefined.ExpandCustomer();
}