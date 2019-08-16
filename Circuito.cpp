//
// Created by Andrea on 25/07/2019.
//

#include "Circuito.h"
#include <string.h>

Circuito ::Circuito() {};

Circuito ::~Circuito() {};

void Circuito ::coniLogici() {

    ifstream File("coni_logici.txt");  // nome file : "coni_logici" // file prova alternativo : coni_logici1
    if (!File.is_open()) {
        cerr << "Errore lettura file";
        exit(1);
    }
    vector<string> salva_file;
    string line;
    while (getline(File, line)) {       // salvataggio file
        salva_file.push_back(line);
    }
    File.close();

    //for (int j = 0; j <salva_file.size() ; j++)         //stampa documento prova
    //    cout << salva_file.at(j)<<endl;

    for (int i = 0; i < salva_file.size(); i++) {
        stringstream ss;
        string temp, temp1, temp2 ;
        ss << salva_file.at(i);
        ss >> temp;
        //cout << temp << endl;
        if (temp == "assign") {
            ss >> temp1;
            cout << "I valori che contribuiscono all'output " << temp1 << " sono : \t";
            while (!ss.eof()) {
                ss >> temp2;  //funzione
                if (temp2 == "AND" || temp2== "OR" || temp2 == "NOT" || temp2 == "NAND" || temp2 == "NOR" ||
                    temp2 == "XOR" || temp2 == "XNOR" || temp2 == "(" || temp2 == ")" || temp2 == "=" )
                    continue;
                else {
                    //valori_utili.push_back(temp2);
                    cout << temp2 << " ";
                }
            }
            cout << endl;
        }
    }
}
