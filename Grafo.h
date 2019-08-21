//
// Created by Edoardo on 19/06/2019.
//

#ifndef PROVA_PROGETTO_GRAFO_H
#define PROVA_PROGETTO_GRAFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Grafo {
public:
    Grafo();
    Grafo(int num_nodi);
    ~Grafo();
    void addNodo(string nome_nodo);
    void addArco(int i, int j);
    void removeNodo(int posizione);
    void removeUltimoNodo(); //senza posizione esegue il pop_back
    void removeArco(int i, int j);
    bool isConnected(int i, int j);
    //void stampaOutput(vector <vector<bool> > matrice_archi); //implementare in modo che stampi gli output del circuito
    int trovaPath(vector<nodo> vettore_nodi, vector < vector<bool> > matrice_archi, int scelta);

private:
    struct nodo{
        string nome; //nome del nodo: a,b,x,y,AND,FF#
        int etichetta=0; //livello di cammino del nodo
    };
    vector<nodo> nodi; //i nodi del grafo sono gli input, le porte/flip flop e gli output
    vector< vector<bool> > archi; //matrice di collegamenti del circuito

};


#endif //PROVA_PROGETTO_GRAFO_H
