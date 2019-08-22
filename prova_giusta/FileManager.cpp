//
// Created by Andrea on 02/08/2019.
//

#include "FileManager.h"

FileManager::FileManager() {

    for(int i=0; i<8; ++i){
        for(int j=0; j<2; ++j){
            _consumi[i][j] = 0;
        }
    }
}


int FileManager::readFile(ifstream &f, vector<vector<Tree<value_t, LogicGates*>>>& outs) {

    vector<string> nomi_circuiti, SpecialElements;
    string str, keyword, nome_output;
    int to_return;
    num_circuiti=0;

    while(!f.eof()) {
        bool flag = false;
        int num_output = 0;

        _coni_logici.resize(num_circuiti + 1);
        _num_input.resize(num_circuiti + 1);
        _num_FFs.resize(num_circuiti + 1);
        _nomi_input.resize(num_circuiti + 1);
        _nomi_FFs.resize(num_circuiti + 1);
        nomi_circuiti.resize(num_circuiti + 1);
        outs.resize(num_circuiti + 1);

        _num_input[num_circuiti] = 0;

        f >> keyword >> nomi_circuiti[num_circuiti] >> keyword;  //lettura prima riga
        f >> keyword;

        if (keyword != "input")
            f >> keyword;

        while (keyword != "output") {   //lettura dal primo input fino alla parola output
            while (!flag) {    //lettura di una riga di input
                _nomi_input[num_circuiti].resize(_num_input[num_circuiti] + 1);
                f >> _nomi_input[num_circuiti][_num_input[num_circuiti]];
                if (_nomi_input[num_circuiti][_num_input[num_circuiti]].find(',') == numeric_limits<size_t>::max())
                    flag = true;
                else
                    _nomi_input[num_circuiti][_num_input[num_circuiti]] = _nomi_input[num_circuiti][_num_input[num_circuiti]].substr(0, _nomi_input[num_circuiti][_num_input[num_circuiti]].size() - 1);

                if (int pos = _nomi_input[num_circuiti][_num_input[num_circuiti]].find('[') !=
                              numeric_limits<size_t>::max()) {  //caso in cui l'input venga inserito come vettore

                    stringstream st;
                    string s;
                    int n;
                    st << _nomi_input[num_circuiti][_num_input[num_circuiti]].substr(pos + 1, _nomi_input[num_circuiti][_num_input[num_circuiti]].find(']') - pos - 1);
                    st >> n;
                    s = _nomi_input[num_circuiti][_num_input[num_circuiti]].substr(0, pos);
                    _nomi_input[num_circuiti].resize(_num_input[num_circuiti] + n);
                    for (int i = 0; i < n; ++i) {
                        _nomi_input[num_circuiti][_num_input[num_circuiti] + i] = s + "[" + to_string(i) + "]";
                    }

                    _num_input[num_circuiti] += n - 1;

                }

                ++_num_input[num_circuiti];
            }

            f >> keyword;
            flag = false;

        }

        while (!flag) {  //lettura della riga degli output

            f >> nome_output;
            if (nome_output.find(',') == numeric_limits<size_t>::max())
                flag = true;
            else
                nome_output = nome_output.substr(0, nome_output.size() - 1);

            outs[num_circuiti].resize(num_output + 1);      //il vettore uscite deve avere tanti elementi quante sono le uscite

            outs[num_circuiti][num_output].setName(nome_output);   //inserisco l'etichetta dell'uscita nel corrispondente albero

            num_output++;

        }

        f >> keyword;
        f >> keyword;

        if (keyword == "assign") {
            to_return = 0;
            for (int i = 0; i < num_output; ++i) {   //leggo riga per riga le espressioni relative a ogni uscita
                char da_saltare;
                f >> nome_output >> keyword;
                f.get(da_saltare);
                getline(f, str);
                _coni_logici[num_circuiti].resize(outs[num_circuiti].size()); //poniamo la dimensione di _coni_logici uguale a quella del vettore uscite
                generaNodi(str, outs[num_circuiti][i], SpecialElements, i);   //creo l'albero relativo all'uscita i
                f >> keyword;

            }
        } else {
            if((keyword[0]=='F') && (keyword[1]=='F') && (keyword[2]<'9' && keyword[2]>'0')) {
                to_return = 1;
                _num_FFs[num_circuiti] = 0;
                int i = 0;

                while (keyword != "assign") {
                    char da_saltare;
                    _nomi_FFs[num_circuiti].push_back(keyword);
                    ++_num_FFs[num_circuiti];
                    outs[num_circuiti].emplace_back();
                    f >> keyword;
                    f.get(da_saltare);
                    getline(f, str);
                    _coni_logici[num_circuiti].resize(outs[num_circuiti].size()); //poniamo la dimensione di _coni_logici uguale a quella del vettore uscite
                    generaNodi(str, outs[num_circuiti][i], SpecialElements, i);  //creo l'albero relativo all'uscita i che è l'ingresso di un certo FF
                    outs[num_circuiti][i].setName(_nomi_FFs[num_circuiti][i]);
                    f >> keyword;
                    ++i;
                }

                for (i = _num_FFs[num_circuiti]; i < num_output + _num_FFs[num_circuiti]; ++i) {   //leggo riga per riga le espressioni relative a ogni uscita

                    char da_saltare;
                    f >> nome_output >> keyword;
                    outs[num_circuiti][i].setName(nome_output);
                    f.get(da_saltare);
                    getline(f, str);
                    generaNodi(str, outs[num_circuiti][i], SpecialElements, i);   //creo l'albero relativo all'uscita i
                    f >> keyword;
                }
            }

            else{
                to_return = 2;
                char da_saltare;
                int i=0;

                while(keyword!="assign" && !((keyword[0]=='F') && (keyword[1]=='F') && (keyword[2]<'9' && keyword[2]>'0'))) {
                    flag = false;
                    i=0;
                    while (i < num_circuiti && !flag) {
                        if (keyword == nomi_circuiti[i])
                            flag = true;
                        else
                            ++i;

                    }

                    if (!flag) {
                        cerr << "Errore: nome circuito combinatorio o sequenziale inserito non riconosciuto" << endl;
                        exit(-4);
                    }

                    f >> keyword;
                    if (keyword != "instance")
                        cerr << "Errore: formato file errato" << endl;

                    f >> da_saltare;

                    for (int j = 0; j < _num_input[i]; ++j) {
                        int pos1, pos2;
                        string etichetta1, etichetta2, tmp;
                        f >> etichetta1 >> da_saltare;
                        f.get(da_saltare);
                        f >> etichetta2;
                        etichetta1 = etichetta1.substr(1, etichetta1.size());
                        etichetta2 = etichetta2.substr(0, etichetta2.size() - 1);

                        flag = false;

                        for (int k = 0; k < _num_input[i] && !flag; ++k) {
                            if (_nomi_input[i][k] == etichetta1) {
                            pos1 = k;
                            flag = true;
                            }
                        }

                        flag = false;

                        for (int k = 0; k < _num_input[num_circuiti] && !flag; ++k) {
                            if (_nomi_input[num_circuiti][k] == etichetta2) {
                                pos2 = k;
                                flag = true;
                            }
                        }

                        _nomi_input[i].resize(_num_input[num_circuiti]);
                        tmp = _nomi_input[i][pos2];
                        _nomi_input[i][pos2] = _nomi_input[i][pos1];
                        _nomi_input[i][pos1] = tmp;
                    }

                    flag = false;

                    for(int j=0; j<outs[i].size()-_num_FFs[i]; ++j){
                        string etichetta1, etichetta2, tmp;

                        if(!flag) {
                            f >> etichetta1 >> da_saltare;
                            f.get(da_saltare);
                            f >> etichetta2;
                            etichetta1 = etichetta1.substr(1, etichetta1.size());
                            if(etichetta2[etichetta2.size()-1]==')')
                                flag = true;

                            etichetta2 = etichetta2.substr(0, etichetta2.size() - 1);
                        }

                        if(outs[i][j + _num_FFs[i]].getName()!=etichetta1){
                            _nomi_input[num_circuiti].push_back("-");
                            flag = true;
                        }
                        else {
                            _nomi_input[num_circuiti].push_back(etichetta2);
                            outs[i][j + _num_FFs[i]].setName(etichetta2);
                            SpecialElements.push_back(etichetta2);
                        }

                    }

                    f >> keyword;

                }

                _num_FFs[num_circuiti] = 0;
                i = 0;

                while (keyword != "assign") {
                    _nomi_FFs[num_circuiti].push_back(keyword);
                    ++_num_FFs[num_circuiti];
                    outs[num_circuiti].emplace_back();
                    f >> keyword;
                    f.get(da_saltare);
                    getline(f, str);
                    _coni_logici[num_circuiti].resize(outs[num_circuiti].size()); //poniamo la dimensione di _coni_logici uguale a quella del vettore uscite
                    generaNodi(str, outs[num_circuiti][i], SpecialElements, i);  //creo l'albero relativo all'uscita i che è l'ingresso di un certo FF
                    outs[num_circuiti][i].setName(_nomi_FFs[num_circuiti][i]);
                    f >> keyword;
                    ++i;
                }

                for (i = _num_FFs[num_circuiti]; i < num_output + _num_FFs[num_circuiti]; ++i) {   //leggo riga per riga le espressioni relative a ogni uscita
                    f >> nome_output >> keyword;
                    outs[num_circuiti][i].setName(nome_output);
                    f.get(da_saltare);
                    getline(f, str);
                    generaNodi(str, outs[num_circuiti][i], SpecialElements, i);   //creo l'albero relativo all'uscita i
                    f >> keyword;
                }
            }
        }

        ++num_circuiti;

    }

    gestisciConiLogici(outs);

    return to_return;
}



void FileManager::generaNodi(string &str, Tree<value_t, LogicGates*> &out, vector<string>& SpecialEl, const int& num_uscita) {

    if(is_input(str)){
        out.addNode(str, SpecialEl);
        _coni_logici[num_circuiti][num_uscita].push_back(str); // aggiungo l'input in _coni_logici alla cella relativa all'uscita num_uscita
        return;
    }

    if(str.substr(0, str.find(' '))=="NOT") {  //eccezione per il caso not perche è l'unico che ha un solo ingresso
        string op, exp;
        op = "NOT";
        exp = generaExp2(str);
        out.addNode(generaPorta(op));

        if (is_input(exp)) {
            out.addGateNOT(exp, SpecialEl); //right
            bool flag = true;

            for(int j=0; j<_coni_logici[num_uscita].size() && flag; ++j) {// se è un input già presente in _coni_logici[num_uscita]

                if(exp == _coni_logici[num_circuiti][num_uscita][j])
                    flag = false;

            }

            if(flag)
                _coni_logici[num_circuiti][num_uscita].push_back(exp);
        } else {
            generaNodi(exp, out, SpecialEl, num_uscita);
        }
    }

    else{
        string exp1, op, exp2;
        exp1 = generaExp1(str);
        op = generaOp(str, exp1);
        exp2 = generaExp2(str);
        out.addNode(generaPorta(op));

        if(is_input(exp1)){
            out.addNode(exp1, SpecialEl); //left
            bool flag = true;

            for(int j=0;j<_coni_logici[num_circuiti][num_uscita].size() && flag; j++) {// se è un input già presente in _coni_logici

                if(exp1 == _coni_logici[num_circuiti][num_uscita][j])
                    flag = false;

            }

            if(flag)
                _coni_logici[num_circuiti][num_uscita].push_back(exp1);
        }

        else{
            generaNodi(exp1, out, SpecialEl, num_uscita);
        }

        if(is_input(exp2)){
            out.addNode(exp2, SpecialEl); //right
            bool flag = true;

            for(int j=0;j<_coni_logici[num_circuiti][num_uscita].size() && flag; j++) { // se è un input già presente in _coni_logici

                if(exp2 == _coni_logici[num_circuiti][num_uscita][j])
                    flag = false;

            }

            if(flag)
                _coni_logici[num_circuiti][num_uscita].push_back(exp2);

        }

        else{
            generaNodi(exp2, out, SpecialEl, num_uscita);
        }

    }

}


string FileManager::generaExp1(string &str) {

    string to_return;
    to_return=str.substr(0, str.find(' '));       //caso in cui l'espressione sia composta solo da un ingresso

    if(to_return.find('(')==numeric_limits<size_t>::max())
        return to_return;

    int l;
    l=str.length();
    int i,cont,pos_i=0,pos_f=0;
    bool flag = false;
    cont=0;

    for(i=0;i<l && !flag;i++){
        if(str[i]=='('){
            cont++;
            if(cont==1)
                pos_i=i;
        }

        if(str[i]==')'){
            cont--;
            if(cont==0){
                pos_f = i;
                flag = true;
            }
        }
    }

    to_return = str.substr(pos_i+1,pos_f-1);
    return to_return;
}



string FileManager::generaOp(string &str, string &exp) {

    string to_return;
    int pos;

    if(is_input(exp))
        pos=exp.size()+1;
    else
        pos=exp.size()+3;

    return str.substr(pos, str.find(' ', pos) - pos);
}


string FileManager::generaExp2(string &str) {

    string to_return;
    to_return = str.substr(str.find_last_of(' ') + 1, str.size() - str.find_last_of(' '));  //caso in cui l'espressione sia composta solo da un ingresso

    if(to_return.find(')')==numeric_limits<size_t>::max())
        return to_return;

    size_t i , pos_i = 0 , pos_f = 0 , l = str.length() ;
    int cont = 0 , flag = 0 ;

    for( i=l-1 ; i>0 && flag==0 ; i-- ){
        if( str[i]==')' ){
            cont++;
            if( cont==1 )
                pos_f = i;
        }

        if( str[i]=='(' ){
            cont--;
            if( cont==0 ){
                pos_i = i;
                flag = 1;
            }
        }
    }

    return str.substr( pos_i+1 , pos_f-pos_i-1 );
}


LogicGates *FileManager::generaPorta(string &porta) {

    if(porta == "OR")
        return new OR(_consumi[0][0], _consumi[0][1]);

    if(porta == "AND")
        return new AND(_consumi[1][0], _consumi[1][1]);

    if(porta == "NOT")
        return new NOT(_consumi[2][0], _consumi[2][1]);

    if(porta == "XOR")
        return new XOR(_consumi[3][0], _consumi[3][1]);

    if(porta == "NAND")
        return new NAND(_consumi[4][0], _consumi[4][1]);

    if(porta == "NOR")
        return new NOR(_consumi[5][0], _consumi[5][1]);

    if(porta == "XNOR")
        return new XNOR(_consumi[6][0], _consumi[6][1]);

    cerr << "errore: inserita una porta non riconosciuta" << endl;
    exit(-2);
}


vector<string> FileManager::getNomi_input(const int& num_circuito) const {
    return _nomi_input[num_circuito];
}



int FileManager::getNum_FFs(const int& num_circuito) const {
    return _num_FFs[num_circuito];
}



vector<string> FileManager::getNomi_FFs(const int& num_circuito) const {
    return _nomi_FFs[num_circuito];
}


vector<vector<vector<string>>> FileManager::getConi_Logici() const {
    return _coni_logici;
}



void FileManager::clearCono_Logico(const int& num_circuito, const int& i , const int& j) {
    _coni_logici[num_circuito][i][j].clear();
}



int FileManager::getNum_input(const int& num_circuito) const {
    return _num_input[num_circuito];
}


void FileManager::gestisciConiLogici(vector<vector<Tree<value_t, LogicGates*>>>& outs) {

    for(int m=0; m<outs.size(); ++m) {
        for (int i = 0; i < outs[m].size(); i++) {   // Coni Logici per circuiti sequenziali
            for (int j = 0; j < _coni_logici[m][i].size(); j++) {
                for (int k = 0; k < outs[m].size(); k++) {

                    if(_coni_logici[m][i][j][_coni_logici[m][i][j].size()-1] == 'N')
                        _coni_logici[m][i][j]=_coni_logici[m][i][j].substr(0, _coni_logici[m][i][j].size()-1);

                    if (_coni_logici[m][i][j] == outs[m][k].getName()) {
                        for (int n = 0; n < _coni_logici[m][k].size(); ++n) {
                            _coni_logici[m][i].push_back(_coni_logici[m][k][n]);
                        }

                        _coni_logici[m][i][j].erase();

                    }

                }

            }

        }

    }

    for(int m=0; m<outs.size(); ++m) {
        for (int i = _num_FFs[m]; i < outs[m].size(); ++i) {
            for (int j = 0; j < _coni_logici[m][i].size(); ++j) {

                bool flag = true;

                for (int n = j + 1; n < _coni_logici[m][i].size() && flag; ++n) {
                    if (_coni_logici[m][i][j] == _coni_logici[m][i][n]) {
                        flag = false;
                        _coni_logici[m][i][j].erase();
                    }

                }

            }

        }

    }

}



vector<float> FileManager::leggiFileConsumi(ifstream &f) {

    string porta;
    vector<float> to_ret_FF;
    char da_saltare;
    float val1, val2;
    int i = 0;

    getline(f, porta, ';');
    while(i<8 && !f.eof()){

        f >> val1 >> da_saltare >> val2;
        f.get(da_saltare);

        if(porta == "OR") {
            _consumi[0][0] = val1;
            _consumi[0][1] = val2;
        }

        else if(porta == "AND") {
            _consumi[1][0] = val1;
            _consumi[1][1] = val2;
        }

        else if(porta == "NOT") {
            _consumi[2][0] = val1;
            _consumi[2][1] = val2;
        }

        else if(porta == "XOR") {
            _consumi[3][0] = val1;
            _consumi[3][1] = val2;
        }

        else if(porta == "NAND") {
            _consumi[4][0] = val1;
            _consumi[4][1] = val2;
        }

        else if(porta == "NOR") {
            _consumi[5][0] = val1;
            _consumi[5][1] = val2;
        }

        else if(porta == "XNOR") {
            _consumi[6][0] = val1;
            _consumi[6][1] = val2;
        }

        else{
            to_ret_FF.push_back(val1);
            to_ret_FF.push_back(val2);
        }

        ++i;

        getline(f, porta, ';');

    }

    return to_ret_FF;
}




bool FileManager::is_input(string &exp) {
    return exp.find(' ') == numeric_limits<size_t>::max();
}