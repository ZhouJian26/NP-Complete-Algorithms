/*
    ho una matrice vector 
    calcolo per ogni riga un valori rispetto ad una funzione
    calcolo per ogni colonna un valore rispetto ad una funzione
    sorto le righe secondo una funzione
    sorto le colonne secondo una funzione
    seleziono il primo elemento ed applico ad un intervallo o tutti rispetto ad una funzione di valutazione la prossima scelta
        cosi si creano percorsi
*/
#include "photo-matching.h"
#include <vector>
#include <bitset>

using namespace std;
/*
    _rowOrder se 0 allora crescente altrimenti descescente
*/

template <class T, class typeScoreRow>
T PhotoMatching(T _matrix, int _rowOrder)
{
    vector<vector<typeScoreRow>> scoreRow;
    vector<vector<int>> scoreColums;
    T tempMatrix;

    // creo vettore con index riga originario e quanti uni ha
    for (auto &x : _matrix)
        scoreRow.push_back({x - scoreRow.begin(), *x.count()});

    // creo vettore con index colonna originario e quanti uni ha in colonna
    for (int i = 0; i < _matrix[0].size(); i++)
        scoreColums.push_back({i, 0});

    for (auto &x : _matrix)
        for (int i = 0; i < x.size; i++)
            if (x[i])
                scoreColums[i]++;

    sort(scoreRow.begin(), scoreRow.end(), [](vector<int> &a, vector<int> &b) {
        if (_rowOrder == 0)
            return a[1] < b[1];
        return a[1] > b[1];
    });

    // creo la matrice di lavoro sortata
    for (auto &x : scoreRow)
        tempMatrix.push_back(_matrix[x[0]]);
}