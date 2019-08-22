//
// Created by Andrea on 02/08/2019.
//

#ifndef PROVA_GIUSTA_LOGICGATES_H
#define PROVA_GIUSTA_LOGICGATES_H


#include <string>

using namespace std;

enum value_t {zero , uno , x }; //zero = 0, uno = 1, x = 2 (commento da togliere)

class LogicGates {
public:

    LogicGates();
    ~LogicGates();

    virtual value_t get_output(value_t& a, value_t& b) = 0 ;
    virtual float getPower() const = 0;
    string getName() const;

protected:

    string _name;
    value_t _previous;
    float _to_one, _to_zero, _power;

};


class AND : public LogicGates{

public:

    AND(const float& val1, const float& val2){

        _name = "AND";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;

    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class OR : public LogicGates{

public:

    OR(const float& val1, const float& val2){

        _name = "OR";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;

    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class NOT : public LogicGates{

public:

    NOT(const float& val1, const float& val2){

        _name = "NOT";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;

    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class XOR : public LogicGates{

public:

    XOR(const float& val1, const float& val2){

        _name = "XOR";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;

    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class NAND : public LogicGates{

public:

    NAND(const float& val1, const float& val2){

        _name = "NAND";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;
    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class NOR : public LogicGates{

public:

    NOR(const float& val1, const float& val2){

        _name = "NOR";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;
    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};



class XNOR : public LogicGates{

public:

    XNOR(const float& val1, const float& val2){

        _name = "XNOR";
        _to_one = val1;
        _to_zero = val2;
        _power = 0;
        _previous = x;
    }

    value_t get_output(value_t& a , value_t& b);

    float getPower() const;

};


#endif //PROVA_GIUSTA_LOGICGATES_H
