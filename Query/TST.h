#ifndef TST_H
#define TST_H
#include "TSTNode.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include <vector>

using namespace std;
template<typename T>


class TST
{
public:
    TST(){}
    ~TST(){if(nPtrRoot)delete nPtrRoot; }
    TSTNode<T>* nPtrRoot=0;
    int iDot=0;
    void add(T &dato, int EndWord);
    void addRecursivo(TSTNode<T> *&ptr, T &dato, int EndWord);
    vector<T> find(T &dato);
    vector<T> findRecursivo(TSTNode<T>* &ptr, T &dato);
    void imprimir_(TSTNode<T> *&p, ostream &os, int &dato);
    void imprimir_dot();
};

#endif // TST_H

template<typename T>
void TST<T>::add(T &dato , int EndWord)
{
    addRecursivo(nPtrRoot,dato,EndWord);
}

template<typename  T>
void TST<T>::addRecursivo(TSTNode<T> *&ptr, T &dato, int EndWord)
{
    if(!ptr){
        ptr=new TSTNode<T>(dato);
        ptr->listWordEnd[EndWord]=dato;
        ptr->iKey=iDot;
        ++iDot;
        return;
    }

    int find1=ptr->tDato.find(dato);
    int find2=dato.find(ptr->tDato);

    if(find1==0){
        ptr->listWordEnd[EndWord]=dato;
        return;
    }
    else if(find2==0){
        ptr->tDato=dato;
        ptr->listWordEnd[EndWord]=dato;
        return;
    }

    else if(dato < ptr->tDato)
        addRecursivo(ptr->nPtrSon[0],dato,EndWord);
    else if(dato > ptr->tDato)
        addRecursivo(ptr->nPtrSon[2],dato,EndWord);
}

template<typename T>
vector<T> TST<T>::find(T &dato)
{
    return findRecursivo(nPtrRoot, dato);
}

template<typename T>
vector<T> TST<T>::findRecursivo(TSTNode<T> *&ptr, T &dato)
{
    if(!ptr){        vector<T> rpt; return rpt;}
    if(ptr->tDato.find(dato)==0){
        vector<T> rpt;
        for(auto &it:ptr->listWordEnd)
        {
            if(it.second == dato)
                rpt.push_back(it.second);
        }

        return rpt;
    }


    else if(dato < ptr->tDato)
        return findRecursivo(ptr->nPtrSon[0],dato);
    else if(dato > ptr->tDato)
        return findRecursivo(ptr->nPtrSon[2],dato);

}

template<typename T>
void TST<T>::imprimir_(TSTNode<T> *&p, ostream &os, int &dato)
{

    if(!p)return;
    for(auto &i:p->vListWordEnd)
    {
        cout<<p->tDato<<" : "<<i.second<<endl;
    }
    if(dato!=p->iKey){os<<dato<<"->"<<p->iKey<<endl;}
    imprimir_(p->nPtrSon[2],os,p->iKey);
    imprimir_(p->nPtrSon[1],os,p->iKey);
    imprimir_(p->nPtrSon[0],os,p->iKey);
    if(p==nPtrRoot){os<<p->iKey<<" [label= "<<"\""<<p->tDato<<"\""<<" ];"<<endl; return;}

    os<<p->iKey<<" [label= "<<"\""<<p->tDato<<"\""<<" ];"<<endl;
}

template<typename T>
void TST<T>::imprimir_dot()
{
    ofstream Esc("TST.dot");
    Esc<<"digraph G {"<<endl;
    if(!nPtrRoot)
    {
        Esc<<"}"<<endl;
        Esc.close();
        system("dot TST.dot -Tpdf -o TST.pdf");
        cout<<endl<<"VER EL PDF: TST.pdf"<<endl;
        return;
    }
    TSTNode<T>* tmp=nPtrRoot;
    imprimir_(tmp,Esc,tmp->iKey);
    Esc<<"}"<<endl;
    Esc.close();
    system("dot TST.dot -Tpdf -o TST.pdf");
    cout<<endl<<"VER EL PDF: TST.pdf"<<endl;
}
