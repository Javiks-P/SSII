#include "reglasYhechos.hpp"
#include <iostream>
#include <fstream>
#include <string>

map<hecho, regla> hecho_regla;
map<regla, hecho> regla_hecho;

hecho inicializar(map<hecho,double> &mapa, string bc, string bh);

int main(int argc, char * argv[])
{
    
    if(argc!=3)
    {
        cout<<"Error: uso\n SBR-FC.exe BasedeConocimientos(fichero de texto) BasedeReglas(fichero de texto)"<<endl;
        exit(EXIT_FAILURE);
    }
    
    

    map<hecho,double> mapa = map <hecho,double>();
    hecho objetivo = inicializar(mapa, argv[1], argv[2]);
    cout<<objetivo.resolver(mapa);

}

hecho inicializar(map<hecho,double> &mapa, string bc, string bh)
{
    ifstream fileBC(bc);
    ifstream fileBH(bh);

    if(!fileBC.is_open())
    {
        cerr<<"Error: no se ha podido abrir el fichero "<<bc<<endl;
        exit(1);
    }
    if(!fileBH.is_open())
    {
        cerr<<"Error: no se ha podido abrir el fichero "<<bh<<endl;
        exit(1);
    }
    //variables para BC y BH
    string linea;
    map<string, hecho> nombres = map<string,hecho>();
    string nomRegla;
    int numReglas;
    int numHechos;
    int index;
    int flag; // para terminar el bucle que reconoce los hechos para una regla
    int tipo; //determinar el tipo de una regla
    string nombreHecho;
    list<hecho> hechosRegla;
    float factor;
    list<regla> reglas;
    bool flagH=0;

    if(!getline(fileBC,linea))
    {
        cerr<<"Error en la base de conocimientos"<<endl;
        exit(EXIT_FAILURE);
    }
    numReglas=stoi(linea);
    getline(fileBC,linea);
    while(linea.size())
    {
        nomRegla = "";
        for(index =0;linea[index]!=':';++index)
            nomRegla+=linea[index];
        for(index+=1;linea[index]==' ';++index);
        if(linea[index++]!='S' || linea[index++]!='i')
        {
            cerr<<"Error en la estructura de la base de hechos"<<endl;
            exit(EXIT_FAILURE);
        }
        for(index+=1;linea[index]==' ';++index);
        flag = 0;
        tipo = -1;
        hechosRegla = list<hecho>();
        while(!flag)
        {
            nombreHecho="";
            for(;linea[index]!=' ';++index)
                nombreHecho+=linea[index];
            if(nombreHecho=="Entonces") flag=1;
            else if(nombreHecho=="o" && (tipo==-1 ||tipo==O))
                tipo= O;
            else if(nombreHecho=="y" && (tipo==-1 || tipo==Y))
                tipo=Y;
            else if((nombreHecho=="y" && tipo==O) || (nombreHecho=="o" && tipo==O))
            {
                cerr<<"Incoherencia en el uso de hechos para las reglas"<<endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                if(!nombres.count(nombreHecho))
                {
                    nombres.insert({nombreHecho, hecho(nombreHecho)});
                }
                hechosRegla.push_front(nombres.at(nombreHecho));
                
            }
            for(;linea[index]==' ';++index);
        }
        if(tipo==-1) tipo=O;
        nombreHecho="";
        for(;linea[index]!=',';++index)
            nombreHecho+=linea[index];
        for(;linea[index]!='=';++index);
        factor = stof(linea.substr(++index));
        if(!nombres.count(nombreHecho))
        {
            nombres.insert({nombreHecho,hecho(nombreHecho)});
        }
            
        nombres.at(nombreHecho).addProductora(regla(nomRegla,factor,tipo,hechosRegla));
        hechosRegla.clear();
        getline(fileBC,linea);
        if (!linea.size())
        {
            cout<<"nombre: "<<nombreHecho<<endl;
            nombres.at(nombreHecho).imprimirProductoras();
        }
        
    }
    
    if(!getline(fileBH,linea))
    {
        cerr<<"Error en la base de conocimientos"<<endl;
        exit(EXIT_FAILURE);
    }
    numHechos=stoi(linea);
    while(!flagH)
    {
        getline(fileBH,linea);
        nombreHecho ="";
        for(index=0;linea[index]!=','&& linea[index]!='\0';++index)
            nombreHecho+=linea[index];
        if(linea[index]=='\0' || linea.size()==0)
        {
            flagH=1;
            break;
        }
        for(;linea[index]!='='; ++index);
        factor = stof(linea.substr(++index));
        if(!nombres.count(nombreHecho))
        {
            cerr<<"linea 146"<<endl;
            exit(EXIT_FAILURE);
        }
        mapa.insert({nombres.at(nombreHecho),factor});
    }
    getline(fileBH,linea);
    if(!nombres.count(linea))
    {
        cerr<<"linea 150"<<endl;
        exit(EXIT_FAILURE);
    }
    fileBC.close();
    fileBH.close();
    return nombres.at(linea);
}