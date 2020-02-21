#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;
class Book
{
private:
    int reward, id, spreadLib;
    float rarity;
    bool taken;

public:
    Book(int id) : id(id), spreadLib(0), taken(false)
    {
        cin >> reward;
    };
    int getReward() { return reward; }
    bool isTaken() { return taken; }
    int getId()
    {
        return id;
    }
    int takeIt()
    {
        taken = true;
        reward = 0;
        return id;
    }
    void plusSpread()
    {
        spreadLib++;
        rarity = (1.0 * reward) / spreadLib;
    }
    void minusSpread()
    {
        spreadLib--;
        if (spreadLib > 0)
            rarity = (1.0 * reward) / spreadLib;
        else
            rarity = 0;
    }
    float getRarity() { return rarity; }
};
class Library
{
private:
    int id, signupTime, perDayBooks, nBookTaken;
    //bool taken;
    float rarityOverallBooks, scoreLib;
    vector<Book *> books;
    string responseFinal;

public:
    Library(int id, vector<Book *> &vBooksVector, int remDays) : id(id), nBookTaken(0), /* taken(false),*/ responseFinal("")
    {
        int idBook, nBook;
        cin >> nBook >> signupTime >> perDayBooks;
        for (int i = 0; i < nBook; i++)
        {
            cin >> idBook;
            books.push_back(vBooksVector[idBook]);
            vBooksVector[idBook]->plusSpread();
        }
        sort(books.begin(), books.end(), [](Book *a, Book *b) { return a->getReward() < b->getReward(); });
        updateLibScore(remDays);
    }
    bool takeIt(int remDays)
    {
        int toPick = (remDays - signupTime) * perDayBooks; // how many book can take
        nBookTaken = 0;
        string booksStr = "";
        if (toPick <= 0)
            return false;
        sort(books.begin(), books.end(), [](Book *a, Book *b) {
            if (a->getReward() != b->getReward())
                return a->getReward() < b->getReward();
            return a->getRarity() < b->getRarity();
        });
        for (auto &x : books) // books is already sorted
        {
            if (toPick) // exeeded limit book that can be take
            {
                if (!x->isTaken())
                {
                    int idBook = x->takeIt();
                    booksStr += to_string(idBook) + " ";
                    nBookTaken++;
                    toPick--;
                }
            }
            else
            {
                if (!x->isTaken())
                    x->minusSpread();
            }
        }
        if (nBookTaken > 0)
        {
            responseFinal = to_string(id) + " " + to_string(nBookTaken) + '\n' + booksStr + '\n';
            return true;
        }
        return false;
    }
    void printResponse()
    {
        cout << responseFinal;
    }
    int getTimeSign() { return signupTime; }
    int getNumBookTaken() { return nBookTaken; }
    void updateLibScore(int remDays)
    {
        rarityOverallBooks = 0;
        nBookTaken = 0;
        scoreLib = 0.0;
        int toPick = (remDays - signupTime) * perDayBooks; // how many book can take
        for (auto &x : books)
        {
            if (!toPick)
                break;
            if (!x->isTaken())
            {
                rarityOverallBooks += x->getRarity();
                scoreLib += x->getReward();
                nBookTaken++;
                toPick--;
            }
        }
        return;
        scoreLib = scoreLib / (signupTime + remDays); //avg of lib score
    }
    float getLibScore() { return scoreLib; }
    float getRarityOverallBooks() { return rarityOverallBooks; }
    int duration() { return nBookTaken / perDayBooks; }
};
bool compLibOp(Library *a, Library *b)
{
    if (a->getLibScore() != b->getLibScore())
        return a->getLibScore() < b->getLibScore();
    if (a->getNumBookTaken() != b->getNumBookTaken())
        return a->getNumBookTaken() < b->getNumBookTaken();
    return a->getRarityOverallBooks() < b->getRarityOverallBooks();
}
int main()
{
    vector<Book *> BooksVector;
    //vector<Book *> BookHeap;
    vector<Library *> LibraryHeap;
    //vector<Library *> LibraryVector;
    vector<Library *> response;
    int nBooks, nLib, days, remDays;
    bool needReHeap;
    cin >> nBooks >> nLib >> days;
    Book *pBook;
    Library *pLibrary;
    for (int i = 0; i < nBooks; i++)
    {
        pBook = new Book(i);
        BooksVector.push_back(pBook);
        //BookHeap.push_back(pBook);
    }
    for (int i = 0; i < nLib; i++)
    {
        pLibrary = new Library(i, BooksVector, days);
        LibraryHeap.push_back(pLibrary);
        //LibraryVector.push_back(pLibrary);
    }
    remDays = days;
    make_heap(LibraryHeap.begin(), LibraryHeap.begin(), &compLibOp);
    while (remDays > 0 && nBooks > 0 && LibraryHeap.size())
    {
        needReHeap = false;
        pLibrary = LibraryHeap.front();
        //bool isNullStr = pLibrary->takeIt(remDays);
        if (pLibrary->takeIt(remDays))
        {
            response.push_back(pLibrary);
            remDays -= pLibrary->getTimeSign();
            nBooks -= pLibrary->getNumBookTaken();
            needReHeap = true;
        }
        pop_heap(LibraryHeap.begin(), LibraryHeap.end(), &compLibOp);
        LibraryHeap.pop_back();
        if (needReHeap)
        {
            for (auto &x : LibraryHeap)
                x->updateLibScore(remDays);
            make_heap(LibraryHeap.begin(), LibraryHeap.begin(), &compLibOp);
        }
    }
    cout << response.size() << endl;
    for (auto &res : response)
        res->printResponse();
}