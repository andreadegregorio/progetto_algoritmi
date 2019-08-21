//
// Created by Edoardo on 19/06/2019.
//

#include "Grafo.h"

Grafo::Grafo() {

}

Grafo::Grafo(int num_nodi) {

}

Grafo::~Grafo() {

}

void Grafo::addNodo(string nome_nodo) {
    nodi.emplace_back(nome_nodo, 0);
}

void Grafo::addArco(int i, int j) {
    archi[i][j]=true;
}

void Grafo::removeNodo(int posizione) {
    nodi.erase(nodi.begin()+ posizione);
}

void Grafo::removeArco(int i, int j) {
    archi[i][j]=false;
}

bool Grafo::isConnected(int i, int j) {
    return archi[i][j];
}

void Grafo::removeUltimoNodo() {
    nodi.pop_back();
}

int Grafo::trovaPath(vector<Grafo::nodo> vettore_nodi, vector<vector<bool> > matrice_archi, int scelta) {
    int res=0;

    //ricerca path / etichettatura livelli
    for(unsigned long long int i=nodi.size()-1; i>=0 ; i--){
        for(unsigned long long int j=0; j<nodi.size(); j++){
            if(archi[i][j] == true && (nodi[j].etichetta==0 || nodi[j].etichetta > nodi[i].etichetta+1)){
                nodi[j].etichetta=nodi[i].etichetta+1; //incremento il livello del nodo nel momento in cui c'è la connessione e il nodo ha livello maggiore o non ha livello minore di quello che gli stiamo assegnando
            }
        }
    }

    switch(scelta){ //in base alla scelta si calcola path min o path max
        case 1:
            //return path min
            res=nodi[0].etichetta;
            for(unsigned long long int i=0; i<nodi.size() ; i++){
                if(nodi[i].etichetta < res)
                    res=nodi[i].etichetta;
            }
            break;

        case 2:
            //return path max
            res=nodi[0].etichetta;
            for(unsigned long long int i=0; i<nodi.size() ; i++){
                if(nodi[i].etichetta > res)
                    res=nodi[i].etichetta;
            }
            break;
    }

    return res;
}


