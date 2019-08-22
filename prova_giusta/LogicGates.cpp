//
// Created by Andrea on 02/08/2019.
//

#include "LogicGates.h"


LogicGates::LogicGates() {}
LogicGates::~LogicGates() {}


string LogicGates::getName() const {
    return _name;
}



//override getPower method
float AND::getPower() const {
    return _power;
}


float NAND::getPower() const {
    return _power;
}


float NOR::getPower() const {
    return _power;
}


float OR::getPower() const {
    return _power;
}


float XOR::getPower() const {
    return _power;
}


float NOT::getPower() const {
    return _power;
}


float XNOR::getPower() const {
    return _power;
}



//override gate's output methods
value_t AND::get_output(value_t &a, value_t &b) {
    if (a==uno && b==uno) {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }

    if ((a==uno && b==x) || (a==x && b==uno) || (a==x && b==x)) {
        _previous = x;
        _power = 0;
        return x;
    }
    else {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
}


value_t NAND::get_output(value_t &a, value_t &b) {
    if((a==x && b==uno) || (a==uno && b==x) || (a==x && b==x)) {
        _previous = x;
        _power = 0;
        return x;
    }

    if(a==zero || b==zero) {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }

    else {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
}


value_t NOR::get_output(value_t &a, value_t &b) {
    if((a==x && b==zero) || (a==zero && b==x) || (a==x && b==x)) {
        _previous = x;
        _power = 0;
        return x;
    }

    if (a==uno || b==uno) {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
    else {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }
}


value_t NOT::get_output(value_t &a, value_t &b) {
    if (b==zero) {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }
    else if (b==x) {
        _previous = x;
        _power = 0;
        return x;
    }

    else {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
}


value_t OR::get_output(value_t &a, value_t &b) {
    if (a==uno || b==uno) {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }

    if ((a==zero && b==x) || (a==x && b==zero) || (a==x && b == x)) {
        _previous = x;
        _power = 0;
        return x;
    }
    else {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
}


value_t XNOR::get_output(value_t &a, value_t &b) {
    if(a==x || b==x) {
        _previous = x;
        _power = 0;
        return x;
    }

    if(a==b) {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }
    else {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }
}


value_t XOR::get_output(value_t &a, value_t &b) {
    if(a==x || b==x) {
        _previous = x;
        _power = 0;
        return x;
    }

    if (a==b) {
        if(_previous==uno)
            _power = _to_zero;
        else
            _power = 0;

        _previous = zero;
        return zero;
    }

    else {
        if(_previous==zero)
            _power = _to_one;
        else
            _power = 0;

        _previous = uno;
        return uno;
    }
}