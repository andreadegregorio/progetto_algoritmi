#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include "FileManager.h"
#include "LogicGates.h"
#include "Tree.hpp"


using namespace std;

int Find_Path_Max(vector<string> FF_names, vector<int> FF_profondita, vector<vector<Tree<value_t, LogicGates*>>>& outs, const int& num_circuito);
int Find_Path_Min(vector<string> FF_names, vector<int> FF_profondita, vector<vector<Tree<value_t, LogicGates*>>>& outs, const int& num_circuito);


int main(int argc, char* argv[]) {

    string str, nome_circuito, nome_file_in;
    FileManager f_reader;
    vector<float> consumi_FF;
    vector<vector<Tree<value_t, LogicGates*>>> uscite;
    int tipo_circuito, choice;
    if(argc != 2){
        cerr << "Errore: numero di argomenti da linea di comando errato" << endl;
        exit(-1);
    }
    str = argv[1];
    ifstream myfile(R"(C:\Users\ACER\CLionProjects\Esempi\)" + str);
    if(!myfile.is_open()){
        cerr << "Errore apertura file" << endl;
        exit(-1);
    }
    cout << "Inserisci file contenente i consumi delle porte:" << endl;
    cin >> nome_file_in;
    ifstream file_in(R"(C:\Users\ACER\CLionProjects\Esempi\)" + nome_file_in);
    if (!file_in.is_open()) {
        cerr << "Errore apertura file" << endl;
        exit(-1);
    }
    consumi_FF = f_reader.leggiFileConsumi(file_in);
    file_in.close();
    //tipo circuito definisce quale circuito è stato inserito
    //la funzione readFile ritorna 0 se combinatorio, 1 se sequenziale, 2 se composto
    tipo_circuito = f_reader.readFile(myfile, uscite);
    myfile.close();
    cout << "Scegliere l'operazione da eseguire:" << endl;
    cout << "[1] Simulazione" << endl;
    cout << "[2] Analisi" << endl;
    cout << "[3] Uscire" << endl;
    cin >> choice;

    while(choice != 3) {
        switch (choice) {
            case 1: {
                cout << "inserire il file contenente gli input da fornire al circuito:" << endl;
                cin >> str;
                cout << "Dove si desidera ricevere i risultati?:" << endl;
                cout << "[1] su schermo" << endl;
                cout << "[2] su file" << endl;
                cin >> choice;
                string nome_file_out;
                ofstream file_out;
                if((choice != 1) && (choice != 2))
                    cout << "E' stata effettuata una scelta non valida" << endl;
                if (choice == 2){
                    cout << "Inserire nome del file:" << endl;
                    cin >> nome_file_out;
                    file_out.open(R"(C:\Users\ACER\CLionProjects\Esempi\)" + nome_file_out);
                    if(!file_out.is_open()){
                        cerr << "Errore apertura file" << endl;
                        exit(-1);
                    }
                }
                int clk = 1;
                myfile.open(R"(C:\Users\ACER\CLionProjects\Esempi\)" + str);
                if (!myfile.is_open()) {
                    cerr << "Errore apertura file" << endl;
                    exit(-1);
                }
                char val;
                vector<value_t> valori_input;
                vector<vector<value_t>> memorie_FFs(uscite.size());
                for (int i = 0; i < uscite.size(); ++i) {                        //inizializzazione matrice di memorie dei flipflop per ogni circuito
                    memorie_FFs[i].resize(f_reader.getNum_FFs(i), x);
                }
                myfile >> val;
                valori_input.push_back((value_t) (val - '0'));
                //vector<float> Potenza(uscite[uscite.size()-1].size()-f_reader.getNum_FFs(uscite.size()-1), 0);
                while (!myfile.eof()) {
                    vector<float> Potenza(uscite[uscite.size()-1].size()-f_reader.getNum_FFs(uscite.size()-1), 0);
                    for (int i = 1; i < f_reader.getNum_input(uscite.size() - 1); ++i) {        //ciclo per inserire i valori di input
                        myfile >> val;
                        valori_input.push_back((value_t) (val - '0'));
                    }
                    for (int i = 0; i < uscite.size() - 1; ++i) {
                        //risolvo tutti i sottocircuiti e poi inserisco i risultati ottenuti come ingressi
                        if (f_reader.getNum_FFs(i) == 0) {
                            //caso in cui il sottocircuito è combinatorio
                            for (int j = 0; j < uscite[i].size(); ++j) {
                                uscite[i][j].addInput(valori_input, f_reader.getNomi_input(i));
                                valori_input.push_back(uscite[i][j].getOutput());
                                bool flag = false;
                                for(int m = 0; m < uscite[uscite.size()-1].size() && !flag; ++m){
                                    for(int n = 0; n < f_reader.getConi_Logici()[uscite.size()-1][m].size() && !flag; ++n){
                                        if(uscite[i][j].getName()== f_reader.getConi_Logici()[uscite.size()-1][m][n]) {
                                            flag = true;
                                            Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += uscite[i][j].getOutputPower();
                                        }
                                    }
                                }
                            }
                        } else {
                            //caso in cui il sottocircuito è sequenziale
                            for (int j = 0; j < f_reader.getNum_FFs(i); ++j) {                //ciclo per calcolare le uscite di ogni flipflop per ogni ciclo di clk
                                //aggiungo uscite numeriche all'albero del flipflop
                                uscite[i][j].addInput(valori_input, f_reader.getNomi_input(i), memorie_FFs[i], f_reader.getNomi_FFs(i));
                                bool flag = false;
                                for(int m = 0; m < uscite[uscite.size()-1].size() && !flag; ++m){
                                    for(int n = 0; n < uscite[uscite.size()-1][m].getFF_presence().size() && !flag; ++n){
                                        if(uscite[i][j].getName() == uscite[uscite.size()-1][m].getFF_presence()[n]) {
                                            flag = true;
                                            if (memorie_FFs[i][j] == zero && uscite[i][j].getOutput() == uno)
                                                Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += consumi_FF[0];
                                            if (memorie_FFs[i][j] == uno && uscite[i][j].getOutput() == zero)
                                                Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += consumi_FF[1];
                                        }
                                    }
                                }
                            }
                            for (int j = f_reader.getNum_FFs(i); j < uscite[i].size(); ++j) {                  //ciclo per calcolare le uscite
                                uscite[i][j].addInput(valori_input, f_reader.getNomi_input(i), memorie_FFs[i], f_reader.getNomi_FFs(i));
                                valori_input.push_back(uscite[i][j].getOutput());
                                bool flag = false;
                                for(int m = 0; m < uscite[uscite.size()-1].size() && !flag; ++m){
                                    for(int n = 0; n < f_reader.getConi_Logici()[uscite.size()-1][m].size() && !flag; ++n){
                                        if(uscite[i][j].getName()==f_reader.getConi_Logici()[uscite.size()-1][m][n]) {
                                            flag = true;
                                            Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += uscite[i][j].getOutputPower();
                                        }
                                    }
                                }
                            }
                            for (int j = 0; j < f_reader.getNum_FFs(i); ++j) {          //ciclo per aggiornare il vettore delle memorie dei flipflop del sottocircuito i
                                memorie_FFs[i][j] = uscite[i][j].getOutput();
                            }
                        }
                    }
                    for (int i = 0; i < f_reader.getNum_FFs(uscite.size() - 1); ++i) {          //ciclo per calcolare le uscite di ogni flipflop per ogni ciclo di clk
                        //aggiungo uscite numeriche all'albero del flipflop
                        uscite[uscite.size() - 1][i].addInput(valori_input, f_reader.getNomi_input(uscite.size() - 1), memorie_FFs[uscite.size() - 1], f_reader.getNomi_FFs(uscite.size() - 1));
                        bool flag = false;
                        for(int m = 0; m < uscite[uscite.size()-1].size() && !flag; ++m){
                            for(int n = 0; n<uscite[uscite.size()-1][m].getFF_presence().size() && !flag; ++n){
                                if(uscite[uscite.size()-1][i].getName()==uscite[uscite.size()-1][m].getFF_presence()[n]) {
                                    flag = true;
                                    if (memorie_FFs[uscite.size()-1][i] == zero && uscite[uscite.size()-1][i].getOutput() == uno)
                                        Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += consumi_FF[0];
                                    if (memorie_FFs[uscite.size()-1][i] == uno && uscite[uscite.size()-1][i].getOutput() == zero)
                                        Potenza[m-f_reader.getNum_FFs(uscite.size()-1)] += consumi_FF[1];
                                }
                            }
                        }
                    }
                    if (tipo_circuito == 0) {               //se il circuito è combinatorio il ciclo per stampare i risultati è a parte perchè non c'è clock
                        for (int j = 0; j < uscite[0].size(); ++j) {
                            uscite[0][j].addInput(valori_input, f_reader.getNomi_input(0));
                            if(choice != 2)
                                cout << uscite[0][j].getName() << ": " << uscite[0][j].getOutput() << endl;
                            else
                                file_out << uscite[0][j].getName() << ": " << uscite[0][j].getOutput() << endl;
                            Potenza[j] = uscite[0][j].getOutputPower();
                            cout << "Consumo di potenza dell'uscita " << uscite[0][j].getName() << ": " << Potenza[j] << endl;
                        }
                    } else {                              //ciclo per stampare i risultati del circuito sequenziale o composto
                        for (int i = f_reader.getNum_FFs(uscite.size() - 1); i < uscite[uscite.size() - 1].size(); ++i) {                  //ciclo per calcolare le uscite
                            char result_to_print;
                            uscite[uscite.size() - 1][i].addInput(valori_input, f_reader.getNomi_input(uscite.size() - 1), memorie_FFs[uscite.size() - 1], f_reader.getNomi_FFs(uscite.size() - 1));
                            if (uscite[uscite.size() - 1][i].getOutput() == zero)
                                result_to_print = '0';
                            else if (uscite[uscite.size() - 1][i].getOutput() == uno)
                                result_to_print = '1';
                            else
                                result_to_print = 'x';
                            Potenza[i - f_reader.getNum_FFs(uscite.size() - 1)] += uscite[uscite.size() - 1][i].getOutputPower();
                            if(choice != 2)
                                cout << uscite[uscite.size() - 1][i].getName() << ": " << result_to_print << ", colpo di clock n." << clk << endl;
                            else
                                file_out << uscite[uscite.size() - 1][i].getName() << ": " << result_to_print << ", colpo di clock n." << clk << endl;
                            cout << "Consumo di potenza dell'uscita " << uscite[uscite.size() - 1][i].getName() << ": " << Potenza[i - f_reader.getNum_FFs(uscite.size() - 1)] << endl;
                        }
                    }
                    for (int i = 0; i < f_reader.getNum_FFs(uscite.size() - 1); ++i) {          //ciclo per aggiornare il vettore delle memorie dei flipflop
                        memorie_FFs[uscite.size() - 1][i] = uscite[uscite.size() - 1][i].getOutput();
                    }
                    ++clk;
                    valori_input.clear();
                    myfile >> val;
                    valori_input.push_back((value_t) (val - '0'));
                }
                if (choice == 2)
                    file_out.close();
                myfile.close();
                break;
            }
            case 2:{
                //Path massimo
                for(int i=0; i<uscite.size(); ++i) {
                    for (int j = 0; j < f_reader.getNum_FFs(i); j++) {
                        if (Find_Path_Max(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i) > uscite[i][j].getPathMax_Input())
                            uscite[i][j].setPath_Max(Find_Path_Max(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i));
                    }
                    for (int j = f_reader.getNum_FFs(i); j < uscite[i].size(); ++j) {
                        if (Find_Path_Max(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i) > uscite[i][j].getPathMax_Input())
                            uscite[i][j].setPath_Max(Find_Path_Max(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i));
                        if(i==uscite.size()-1)
                            cout << "il path massimo dell'uscita " << uscite[i][j].getName() << " vale " << uscite[i][j].getPathMax_Input() << endl;
                    }
                }
                //Path minimo
                for(int i=0; i<uscite.size(); ++i) {
                    for (int j = 0; j < uscite[i].size(); j++) {
                        if (uscite[i][j].getFF_presence().empty()) {
                            if (uscite[i][j].getName()[0] != 'F' && uscite[i][j].getName()[1] != 'F' && i==uscite.size()-1) {
                                cout << "Il path minimo dell'uscita " << uscite[i][j].getName() << " e' ";
                                for (int k = 0; k < uscite[i][j].getNamePathMin_Input().size(); ++k) {
                                    cout << uscite[i][j].getNamePathMin_Input()[k] << " ";
                                }
                                cout << " e vale: " << uscite[i][j].getPathMin_Input() << endl;
                            }
                        } else {
                            bool flag = true;
                            for (int k = 0; k < uscite[i][j].getFF_presence().size(); ++k) {
                                if (uscite[i][j].getPathMin_Input() > uscite[i][j].getFF_profondita()[k])
                                    flag = false;
                            }
                            if (!flag) {
                                if (Find_Path_Min(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i) < uscite[i][j].getPathMin_Input())
                                    uscite[i][j].setPath_Min(Find_Path_Min(uscite[i][j].getFF_presence(), uscite[i][j].getFF_profondita(), uscite, i));
                            }
                            if (uscite[i][j].getName()[0] != 'F' && uscite[i][j].getName()[1] != 'F' && i==uscite.size()-1)
                                cout << "Il path minimo dell'uscita " << uscite[i][j].getName() << " vale: " << uscite[i][j].getPathMin_Input() << endl << endl;
                        }
                    }
                }
                //Coni logici
                if (tipo_circuito == 0) { // Coni Logici per Circuiti combinatori
                    for (int i = 0; i < uscite[uscite.size()-1].size(); ++i) {
                        cout << endl << "Coni Logici uscita " << uscite[uscite.size()-1][i].getName() << ": ";
                        for (int j = 0; j < f_reader.getConi_Logici()[i].size(); ++j)
                            cout << f_reader.getConi_Logici()[uscite.size()-1][i][j] << " ";
                        cout << endl;
                    }
                }
                //Coni logici per sequenziali
                for(int i=f_reader.getNum_FFs(uscite.size()-1); i<uscite[uscite.size()-1].size(); ++i){
                    cout << "Coni logici dell'uscita " << uscite[uscite.size() - 1][i].getName() << ":";
                    bool flag = false;
                    for(int j = 0; j < f_reader.getConi_Logici()[uscite.size()-1][i].size(); ++j) {
                        for(int m = 0; m < uscite.size()-1; ++m) {
                            for(int n = 0; n < uscite[m].size(); ++n) {
                                if (f_reader.getConi_Logici()[uscite.size() - 1][i][j] == uscite[m][n].getName()) {
                                    flag = true;
                                    for (int k = 0; k < f_reader.getConi_Logici()[m][n].size(); ++k) {
                                        if(!f_reader.getConi_Logici()[m][n][k].empty())
                                            cout << " " << f_reader.getConi_Logici()[m][n][k] << "(uscita " << uscite[m][n].getName() << ")";
                                    }
                                }
                            }
                        }
                        if(!flag){
                            if(!f_reader.getConi_Logici()[uscite.size()-1][i][j].empty())
                                cout << " " << f_reader.getConi_Logici()[uscite.size()-1][i][j];
                        }
                        flag = false;
                    }
                    cout << endl;
                }
                cout << endl;

                break;
            }
            default:
                cout << "Scelta effettuata non valida" << endl;
        }
        cout << "Scegliere l'operazione da eseguire:" << endl;
        cout << "[1] Simulazione" << endl;
        cout << "[2] Analisi" << endl;
        cout << "[3] Uscire" << endl;
        cin >> choice;
    }
    return 0;
}


int Find_Path_Max(vector<string> FF_names, vector<int> FF_profondita, vector<vector<Tree<value_t, LogicGates*>>>& outs, const int& num_circuito){
    if (FF_names.empty())
        return 0;
    else {
        int max = 0;
        for (int i = 0 ; i < FF_names.size() ; i++){
            if (FF_names[i][FF_names[i].size() - 1] == 'N') {
                FF_names[i]=FF_names[i].substr(0, FF_names[i].size() - 1);
            }
            if(FF_names[i][0]=='F' && FF_names[i][1]=='F') {
                for (int j = 0; j < outs[num_circuito].size(); j++) {
                    if (FF_names[i] == outs[num_circuito][j].getName()) {
                        if (outs[num_circuito][j].getPathMax_Input() + FF_profondita[i] > max) {
                            max = outs[num_circuito][j].getPathMax_Input() + FF_profondita[i];
                        }
                    }
                }
            }
            else{
                for(int k = 0; k < outs.size()-1; ++k) {
                    for (int j = 0; j < outs[k].size(); j++) {
                        if (FF_names[i] == outs[k][j].getName()) {
                            if (outs[k][j].getPathMax_Input() + FF_profondita[i] > max) {
                                max = outs[k][j].getPathMax_Input() + FF_profondita[i];
                            }
                        }
                    }
                }
            }
        }
        return max;
    }
}


int Find_Path_Min(vector<string> FF_names, vector<int> FF_profondita, vector<vector<Tree<value_t, LogicGates*>>>& outs, const int& num_circuito){
    int min = numeric_limits<int>::max();
    for (int i = 0 ; i < FF_names.size() ; i++){
        if (FF_names[i][FF_names.size() - 1] == 'N')
            FF_names[i]=FF_names[i].substr(0, FF_names.size() - 1);
        if(FF_names[i][0]=='F' && FF_names[i][1]=='F') {
            for (int j = 0; j < outs[num_circuito].size(); j++) {
                if (FF_names[i] == outs[num_circuito][j].getName()) {
                    if (outs[num_circuito][j].getPathMin_Input() + FF_profondita[i] < min) {
                        min = outs[num_circuito][j].getPathMin_Input() + FF_profondita[i];
                    }
                }
            }
        }
        else{
            for(int k=0; k<outs.size()-1; ++k) {
                for (int j = 0; j < outs[k].size(); j++) {
                    if (FF_names[i] == outs[k][j].getName()) {
                        if (outs[k][j].getPathMin_Input() + FF_profondita[i] < min) {
                            min = outs[k][j].getPathMin_Input() + FF_profondita[i];
                        }
                    }
                }
            }
        }
    }
    return min;
}