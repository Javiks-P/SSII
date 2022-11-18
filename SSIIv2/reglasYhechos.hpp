#ifndef REGLASYHECHOS_HPP
#define REGLASYHECHOS_HPP

#include <iostream>
#include <list>
#include <string>
#include <Map>

//tipos
#define Y 1
#define O 0

using namespace std;

class hecho;

class regla
{
private:
    string nombre; //id de la regla (debe ser único)
    list<hecho*> hechos;
    double factor;
    bool tipob;
    double(*tipo)(double, double);

public:
    regla(string nombre, double factor, bool tipo);
    regla(string nombre,double factor,bool tipo, list<hecho*> hechos);
    ~regla();
    bool operator< (const regla otro) const;
    bool operator== (const regla otro) const;
    void addHecho(hecho* Hecho);
    double resolver(map <hecho*, double> hechosBase);
    //Se borran
    void imprimirHechos();
    string getNombre(){return this->nombre;}
};


class hecho
{
private:
    string nombre; //id del hecho (debe ser único)
    list<regla*> productoras;
public:
    hecho(string nombre);
    hecho(string nombre, list<regla*> productoras);
    ~hecho();
    bool operator< (const hecho otro) const;
    bool operator== (const hecho otro) const;
    void addProductora(regla* regla);
    double resolver(map <hecho*, double> hechosBase);
    //se borran
    void imprimirProductoras();
    string getNombre(){return this->nombre;}
};

#endif