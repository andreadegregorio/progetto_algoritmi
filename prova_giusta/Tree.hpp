//
// Created by Andrea on 02/08/2019.
//

#ifndef PROVA_GIUSTA_TREE_HPP
#define PROVA_GIUSTA_TREE_HPP


#include <iostream>
#include <vector>
#include <limits>

using namespace std;

template <typename T, typename E>

class Tree {
public:

    Tree(){
        _root = nullptr;
        _current_parent = nullptr;
        PathMax_input = 0;
        PathMin_input = numeric_limits<int>::max();
        _total_power = 0;
    };

    //copy ctor
    Tree(const Tree& to_copy) {
        _root = nullptr;
        copyTree(_root, to_copy._root);
        _output_name = to_copy._output_name;
        PathMin_input = to_copy.PathMin_input;
        PathMax_input = to_copy.PathMax_input;
        pathMin_NomiInput = to_copy.pathMin_NomiInput;
        pathMax_NomiInput = to_copy.pathMax_NomiInput;
        FF_presence = to_copy.FF_presence;
        FF_profondita = to_copy.FF_profondita;
        _total_power = to_copy._total_power;
    };

    ~Tree(){
        deleteTree(_root);
    }

    void addNode(string& name, vector<string>& SpecialEl);
    void addNode(E port);

    void addGateNOT(string& name, vector<string>& SpecialEl);

    void setName(string& str);
    string getName() const;

    void addInput(const vector<T>& values, const vector<string>& labels);
    void addInput(const vector<T>& val_input, const vector<string>& labels_input,
                  const vector<T>& val_FFs, const vector<string>& labels_FFs);


    T getOutput();



    int getPathMax_Input();
    void setPath_Max(const int& max);

    int getPathMin_Input();
    void setPath_Min(const int& min);

    vector<string> getNamePathMax_Input();
    vector<string> getNamePathMin_Input();

    vector<string> getFF_presence();
    vector<int> getFF_profondita();

    float getOutputPower() const;


private:
    struct nodo{

        E porta;              //tipo porta
        T _value;             //valore dell'input

        string _input_name;   //etichetta dell'input
        char _type;           //'p' se porta logica, 'v' se ingresso, 'f' se flipflop, 's' se uscita di un sottocircuito
        int _profondita;

        nodo *left;
        nodo *right;
        nodo *parent;
    };

    float _total_power;
    int PathMax_input;
    int PathMin_input;

    vector<string> pathMax_NomiInput;
    vector<string> pathMin_NomiInput;
    vector<string> FF_presence;
    vector<int> FF_profondita;

    nodo * _root;
    nodo * _current_parent; //deve essere il nodo genitore di ogni foglia che viene aggiunta

    string _output_name;

    void deleteTree(nodo * &n);
    T calcolaUscite(nodo * &n);

    void addInputPostOrder(nodo * &n, const vector<T>& val, const vector<string>& labels);

    void addInputPostOrder(nodo * &n, const vector<T>& val_input, const vector<string>& labels_input,
                                        const vector<T>& val_FFs, const vector<string>& labels_FFs);

    void copyTree(nodo * &n, const nodo * to_copy);

};




template<typename T, typename E>
void Tree<T, E>::deleteTree(nodo* &n){
    if(n != nullptr){
        deleteTree(n->left);
        deleteTree(n->right);
        delete n;
    }
}


template<typename T, typename E>
void Tree<T, E>::addNode(string& name, vector<string>& SpecialEl) {

    if(_root == nullptr){
        _root = new nodo;
        _root->_profondita = 1;
        _root->_input_name = name;

        if((name[0]=='F' && name[1]=='F')) {
            _root->_type = 'f';
            FF_presence.push_back(_root->_input_name);
            FF_profondita.push_back(_root->_profondita);
        }
        else {
            bool flag = false;
            for(int i = 0; i < SpecialEl.size() && !flag; ++i) {
                if (_root->_input_name == SpecialEl[i]) {
                    flag = true;
                    FF_presence.push_back(SpecialEl[i]);
                }
            }
            if(flag) {
                _root->_type = 's';
                FF_profondita.push_back(_root->_profondita);
            }
            else {
                _root->_type = 'v';
                if (_root->_profondita > PathMax_input) {
                    PathMax_input = _root->_profondita;
                    pathMax_NomiInput.push_back(_root->_input_name);
                }
                if (_root->_profondita < PathMin_input) {
                    PathMin_input = _root->_profondita;
                    pathMin_NomiInput.push_back(_root->_input_name);
                }
            }
        }

        _root->left = nullptr;
        _root->right = nullptr;
        _root->parent = nullptr;
        return;
    }

    if(_current_parent->left == nullptr){

        _current_parent->left = new nodo;
        _current_parent->left->_profondita = _current_parent->_profondita + 1;
        _current_parent->left->_input_name = name;

        if((name[0]=='F' && name[1]=='F')) {
            _current_parent->left->_type = 'f';
            FF_presence.push_back(_current_parent->left->_input_name);
            FF_profondita.push_back(_current_parent->left->_profondita);
        }
        else {
            bool flag = false;
            for(int i = 0; i<SpecialEl.size() && !flag; ++i) {
                if (_current_parent->left->_input_name == SpecialEl[i]) {
                    flag = true;
                    FF_presence.push_back(SpecialEl[i]);
                }
            }
            if(flag) {
                _current_parent->left->_type = 's';
                FF_profondita.push_back(_current_parent->left->_profondita);
            }
            else {
                _current_parent->left->_type = 'v';
                if (_current_parent->left->_profondita > PathMax_input) {
                    PathMax_input = _current_parent->left->_profondita;
                    pathMax_NomiInput.clear();
                    pathMax_NomiInput.push_back(_current_parent->left->_input_name);
                } else {
                    if (_current_parent->left->_profondita == PathMax_input)
                        pathMax_NomiInput.push_back(_current_parent->left->_input_name);
                }

                if (_current_parent->left->_profondita < PathMin_input) {
                    PathMin_input = _current_parent->left->_profondita;
                    pathMin_NomiInput.clear();
                    pathMin_NomiInput.push_back(_current_parent->left->_input_name);
                } else {
                    if (_current_parent->left->_profondita == PathMin_input)
                        pathMin_NomiInput.push_back(_current_parent->left->_input_name);
                }
            }
        }

        _current_parent->left->parent =_current_parent;
        _current_parent->left->left = nullptr;
        _current_parent->left->right = nullptr;
    }

    else{

        _current_parent->right = new nodo;
        _current_parent->right->_profondita = _current_parent->_profondita+1;
        _current_parent->right->_input_name = name;

        if((name[0]=='F' && name[1]=='F')) {
            _current_parent->right->_type = 'f';
            FF_presence.push_back(_current_parent->right->_input_name);
            FF_profondita.push_back(_current_parent->right->_profondita);
        }
        else {

            bool flag = false;
            for(int i = 0; i<SpecialEl.size() && !flag; ++i) {
                if (_current_parent->right->_input_name == SpecialEl[i]) {
                    flag = true;
                    FF_presence.push_back(SpecialEl[i]);
                }
            }

            if(flag) {
                _current_parent->right->_type = 's';
                FF_profondita.push_back(_current_parent->right->_profondita);
            }

            else {
                _current_parent->right->_type = 'v';
                if (_current_parent->right->_profondita > PathMax_input) {
                    PathMax_input = _current_parent->right->_profondita;
                    pathMax_NomiInput.clear();
                    pathMax_NomiInput.push_back(_current_parent->right->_input_name);
                } else {
                    if (_current_parent->right->_profondita == PathMax_input)
                        pathMax_NomiInput.push_back(_current_parent->right->_input_name);
                }

                if (_current_parent->right->_profondita < PathMin_input) {

                    PathMin_input = _current_parent->right->_profondita;
                    pathMin_NomiInput.clear();
                    pathMin_NomiInput.push_back(_current_parent->right->_input_name);
                } else {
                    if (_current_parent->right->_profondita == PathMin_input)
                        pathMin_NomiInput.push_back(_current_parent->right->_input_name);
                }
            }
        }

        _current_parent->right->parent=_current_parent;
        _current_parent->right->left= nullptr;
        _current_parent->right->right= nullptr;

        bool flag = false;

        if(_current_parent == _root)

            return;

        _current_parent=_current_parent->parent;

        while(!flag && (_current_parent!=_root)){
            if(_current_parent->left!= nullptr)   //se il figlio sinistro di current_parent è nullptr vuol dire che
                flag= true;                       //current_parent è un NOT (perchè ha solo figlio destro e non ha sinistro)
            else{                                 //quindi current_element deve essere aggiornato al suo genitore
                _current_parent = _current_parent->parent;
            }
        }
    }
}


template<typename T, typename E>
void Tree<T, E>::addNode(E port) {

    if (_root == nullptr) {   //se l'albero è vuoto aggiungi una porta del tipo passato come parametro

        _root = new nodo;
        _root->_profondita = 1;
        _root->_type = 'p';
        _root->porta = port;
        _root->left = nullptr;
        _root->right = nullptr;
        _root->parent = nullptr;
        _current_parent = _root;
        return;

    }

    if (_current_parent->porta->getName() == "NOT") {    //aggiunge la figlia di una porta NOT
        _current_parent->right = new nodo;
        _current_parent->right->_profondita = _current_parent->_profondita + 1;
        _current_parent->right->porta = port;
        _current_parent->right->_type = 'p';
        _current_parent->right->parent = _current_parent;
        _current_parent->right->left = nullptr;
        _current_parent->right->right = nullptr;
        _current_parent = _current_parent->right;
        return;

    }

    if (_current_parent->left == nullptr) {
        _current_parent->left = new nodo;
        _current_parent->left->_profondita = _current_parent->_profondita + 1;
        _current_parent->left->porta = port;
        _current_parent->left->_type = 'p';
        _current_parent->left->parent = _current_parent;
        _current_parent->left->left = nullptr;
        _current_parent->left->right = nullptr;
        _current_parent = _current_parent->left;

    } else {
        _current_parent->right = new nodo;
        _current_parent->right->_profondita = _current_parent->_profondita + 1;
        _current_parent->right->porta = port;
        _current_parent->right->_type = 'p';
        _current_parent->right->parent = _current_parent;
        _current_parent->right->left = nullptr;
        _current_parent->right->right = nullptr;
        _current_parent = _current_parent->right;
    }
}


template<typename T, typename E>
void Tree<T, E>::setName(string &str) {
    _output_name = str;
}



template<typename T, typename E>
string Tree<T, E>::getName() const {
    return _output_name;
}



template<typename T, typename E>
void Tree<T, E>::addGateNOT(string& name, vector<string>& SpecialEl) { //aggiungere il caso in cui l'ingresso del NOT è un flipflop

    _current_parent->right = new nodo;
    _current_parent->right->_profondita = _current_parent->_profondita +1;
    _current_parent->right->_input_name = name;

    if((name[0]=='F' && name[1]=='F'))
        _current_parent->right->_type='f';

    else {
        bool flag = false;
        for(int i = 0; i<SpecialEl.size() && !flag; ++i) {
            if (_current_parent->right->_input_name == SpecialEl[i]) {
                flag = true;
                FF_presence.push_back(SpecialEl[i]);
            }
        }

        if(flag) {
            _current_parent->right->_type = 's';
            FF_profondita.push_back(_current_parent->right->_profondita);
        }
        else {
            _current_parent->right->_type = 'v';
            if (_current_parent->right->_profondita > PathMax_input) {
                PathMax_input = _current_parent->right->_profondita;
                pathMax_NomiInput.clear();
                pathMax_NomiInput.push_back(_current_parent->right->_input_name);
            } else {
                if (_current_parent->right->_profondita == PathMax_input)
                    pathMax_NomiInput.push_back(_current_parent->right->_input_name);
            }
            if (_current_parent->right->_profondita < PathMin_input) {
                PathMin_input = _current_parent->right->_profondita;
                pathMin_NomiInput.clear();
                pathMin_NomiInput.push_back(_current_parent->right->_input_name);
            } else {
                if (_current_parent->right->_profondita == PathMin_input)
                    pathMin_NomiInput.push_back(_current_parent->right->_input_name);
            }
        }
    }

    _current_parent->right->parent =_current_parent;
    _current_parent->right->left = nullptr;
    _current_parent->right->right= nullptr;

    bool flag = false;

    if(_current_parent == _root)
        return;

    _current_parent=_current_parent->parent;

    while(!flag && (_current_parent!=_root)){
        if(_current_parent->left!= nullptr)   //se il figlio sinistro di current_parent è nullptr vuol dire che
            flag = true;                       //current_parent è un NOT (perchè ha solo figlio destro e non ha sinistro)
        else{                                 //quindi current_element deve essere aggiornato al suo genitore
            _current_parent=_current_parent->parent;
        }
    }
}




template<typename T, typename E>
T Tree<T, E>::calcolaUscite(nodo* &n) {

    if(n == nullptr)
        return T();

    if(n->_type == 'v' || n->_type == 'f' || n->_type == 's')
        return n->_value;

    T result;
    T a = calcolaUscite(n->left);
    T b = calcolaUscite(n->right);

    result = n->porta->get_output(a, b);
    _total_power += n->porta->getPower();
    return result;
}




template<typename T, typename E>
void Tree<T, E>::addInputPostOrder(Tree::nodo* &n, const vector<T> &val, const vector<string> &labels) {

    if(n == nullptr)
        return;

    if(n->_type == 'v'){
        int i = 0;
        bool flag = false;
        while((i<labels.size()) && (!flag)){

            if(n->_input_name == labels[i])
                flag = true;
            else
                i++;
        }

        n->_value = val[i];

    }

    addInputPostOrder(n->left, val, labels);
    addInputPostOrder(n->right, val, labels);
}





template<typename T, typename E>
void Tree<T, E>::addInput(const vector<T>& values, const vector<string>& labels) {
    addInputPostOrder(_root, values, labels);
}



template<typename T, typename E>
T Tree<T, E>::getOutput() {
    _total_power = 0;
    return calcolaUscite(_root);
}





template<typename T, typename E>
void Tree<T, E>::addInputPostOrder(Tree::nodo* &n, const vector<T> &val_input,
                                          const vector<string> &labels_input, const vector<T> &val_FFs,
                                          const vector<string> &labels_FFs) {

    if(n == nullptr)
        return;

    if(n->_type == 'v' || n->_type == 's'){
        int i=0;
        bool flag = false;

        while((i<labels_input.size()) && (!flag)){
            if(n->_input_name == labels_input[i])
                flag = true;
            else
                i++;
        }
        n->_value = val_input[i];

    }
    if(n->_type == 'f'){

        int i=0;
        bool flag = false;

        while((i<labels_FFs.size()) && (!flag)){
            if(n->_input_name == labels_FFs[i]) {
                flag = true;
                n->_value = val_FFs[i];
            }
            else if(n->_input_name.substr(0, n->_input_name.size() - 1) == labels_FFs[i]) {
                flag = true;
                if(val_FFs[i]==T(1))
                    n->_value = T(0);
                else if(val_FFs[i]==T(0))
                    n->_value = T(1);
                else
                    n->_value = val_FFs[i];
            }
            else
                i++;
        }

    }

    addInputPostOrder(n->left, val_input, labels_input, val_FFs, labels_FFs);
    addInputPostOrder(n->right, val_input, labels_input, val_FFs, labels_FFs);
}


template<typename T, typename E>
void Tree<T, E>::addInput(const vector<T> &val_input, const vector<string> &labels_input,
                                 const vector<T> &val_FFs, const vector<string> &labels_FFs) {

    addInputPostOrder(_root, val_input, labels_input, val_FFs, labels_FFs);
}




template<typename T, typename E>
void Tree<T, E>::copyTree(Tree::nodo * &n, const nodo * to_copy) {

    if(to_copy == nullptr){
        n = nullptr;
        return;
    }

    else
        n = new nodo;

    n->_type = to_copy->_type;
    n->_input_name = to_copy->_input_name;
    n->_value = to_copy->_value;
    n->porta = to_copy->porta;
    n->parent = to_copy->parent;

    if(to_copy->left == nullptr)
        n->left = nullptr;

    else{
        n->left = new nodo;
        copyTree(n->left, to_copy->left);
    }

    if(to_copy->right == nullptr)
        n->right = nullptr;

    else{
        n->right = new nodo;
        copyTree(n->right, to_copy->right);
    }
}



template<typename T, typename E>
int Tree<T, E>::getPathMax_Input() {
    return PathMax_input;
}



template<typename T, typename E>
int Tree<T, E>::getPathMin_Input() {
    return PathMin_input;
}



template<typename T, typename E>
vector<string> Tree<T, E>::getNamePathMax_Input() {
    return pathMax_NomiInput;
}




template<typename T, typename E>
vector<string> Tree<T, E>::getNamePathMin_Input() {
    return pathMin_NomiInput;
}



template<typename T, typename E>
vector<string> Tree<T, E>::getFF_presence() {
    return FF_presence;
}



template<typename T, typename E>
vector<int> Tree<T, E>::getFF_profondita() {
    return FF_profondita;
}



template<typename T, typename E>
void Tree<T, E>::setPath_Max(const int& max) {
    PathMax_input = max;
}



template<typename T, typename E>
void Tree<T, E>::setPath_Min(const int &min) {
    PathMin_input = min;
}



template<typename T, typename E>
float Tree<T, E>::getOutputPower() const {
    return _total_power;
}



#endif //PROVA_GIUSTA_TREE_HPP
