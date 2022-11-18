#include "reglasYhechos.hpp"

double max(double a, double b)
{
    return ((a < b) ? b : a);
}

double min(double a, double b)
{
    return ((a > b) ? b : a);
}

double gt0(double a, double b)
{
    return a+b*(1-a);
}

double lt0(double a, double b)
{
    return a+b*(1+a);
}

double other(double a, double b)
{
    return (a+b)/(1-min(a,b));
}

regla::regla(string nombre, double factor, bool tipo)
{
    this->nombre = nombre;
    this->factor = factor;
    if(tipo)
        this->tipo = min;
    else
        this->tipo=max;
    this->tipob=tipo;
}

regla::regla(string nombre, double factor, bool tipo, list<hecho*> hechos)
{
    this->nombre = nombre;
    this->factor = factor;
    this->hechos = hechos;
    if(tipo)
        this->tipo = min;
    else
        this->tipo=max;
    this->tipob=tipo;
}

regla::~regla()
{

}

bool regla::operator<(const regla otro) const
{
    return this->nombre<otro.nombre;
}

bool regla::operator==(const regla otro) const
{
    return this->nombre == otro.nombre;
}

void regla::addHecho(hecho* hecho)
{
    this->hechos.push_front(hecho);
}

double regla::resolver(map<hecho*, double> mapa)
{
    list<hecho*>::iterator it = this->hechos.begin();
    double res;
    double aux;
    res=-1.1;
    if(this->tipob)
        res=1.1;
    while(it!=this->hechos.end())
    {
        if(mapa.count(*it))
            aux = mapa.at(*it);
        else
            aux=(*it)->resolver(mapa);
        res = this->tipo(res,aux);
        it++;
    }
    return res*this->factor;
}


hecho::hecho(string nombre)
{
    this->nombre = nombre;
}

hecho::hecho(string nombre, list<regla*> productoras)
{
    this->nombre = nombre;
    this->productoras = productoras;
}

hecho::~hecho()
{

}

bool hecho::operator<(const hecho otro) const
{
    return this->nombre < otro.nombre;
}

bool hecho::operator==(const hecho otro) const
{
    return this->nombre == otro.nombre;
}

void hecho::addProductora(regla* regla)
{
    this->productoras.push_front(regla);
}

double hecho::resolver(map<hecho*, double> mapa)
{
    list<regla*> productoras = this->productoras;
    if(productoras.size()==0) return 0.0;
    if(productoras.size()==1) return productoras.front()->resolver(mapa);
    list<regla*>::iterator it = this->productoras.begin();
     double a=(*it)->resolver(mapa);
    it++;
    double b=(*it)->resolver(mapa);
    double (*funRet)(double, double);
    if(a>=0 && b>=0) funRet=gt0;
    else if(a<=0 && b>=0) funRet=lt0; 
    else funRet=other;
    return funRet(a,b);
}

void regla::imprimirHechos()
{
    list<hecho*>::iterator it;
    for(it=this->hechos.begin();it!=this->hechos.end();it++)
        cout<<(*it)->getNombre()<<" ";
    cout<<endl;
}

void hecho::imprimirProductoras()
{
    list<regla*>::iterator it;
    for(it=this->productoras.begin();it!=this->productoras.end();it++)
    {
        cout<<(*it)->getNombre()<<":"<<endl;
        (*it)->imprimirHechos();
    }
    cout<<this->productoras.size()<<endl;
}
