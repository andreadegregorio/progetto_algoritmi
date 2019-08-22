//
// Created by Andrea on 02/08/2019.
//

#ifndef PROVA_GIUSTA_FILEMANAGER_H
#define PROVA_GIUSTA_FILEMANAGER_H

#include <fstream>
#include <limits>
#include <sstream>
#include "Tree.hpp"
#include "LogicGates.h"

using namespace std;

class FileManager {

public:

    FileManager();


    int readFile(ifstream &f, vector<vector<Tree<value_t, LogicGates*>>>& outs);
    vector<float> leggiFileConsumi(ifstream &f);


    int getNum_input(const int& num_circuito) const;
    vector<string> getNomi_input(const int& num_circuito) const;

    int getNum_FFs(const int& num_circuito) const;
    vector<string> getNomi_FFs(const int& num_circuito) const;

    vector<vector <vector<string>>> getConi_Logici() const ;
    void clearCono_Logico(const int& num_circuito, const int& i , const int& j);



private:

    int num_circuiti;
    float _consumi[7][2]; // 8 elementi di base, 2 valori di potenza delle due possibili transizioni
    vector<int> _num_input, _num_FFs;
    vector<vector<string>> _nomi_input, _nomi_FFs;
    vector<vector <vector<string>>> _coni_logici;

    void generaNodi(string &str, Tree<value_t, LogicGates*>& out, vector<string>& SpecialEl, const int& num_uscita);
    string generaExp1(string& str);
    string generaOp(string& str, string& exp);
    string generaExp2(string& str);
    LogicGates* generaPorta(string& porta);
    void gestisciConiLogici(vector<vector<Tree<value_t, LogicGates*>>>& outs);
    bool is_input(string& exp);
};


#endif //PROVA_GIUSTA_FILEMANAGER_H
