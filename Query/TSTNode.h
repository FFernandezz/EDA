#ifndef TSTNODE_H
#define TSTNODE_H

#include <iostream>
#include <map>

using namespace std;
template<typename T>

class TSTNode
{
public:
    TSTNode(){}
    TSTNode(T dato){tDato=dato; iKey=0;listWordEnd.clear();nPtrSon[0]=nPtrSon[1]=nPtrSon[2]=0;}
    ~TSTNode(){if(nPtrSon[0]) delete nPtrSon[0];if(nPtrSon[1]) delete nPtrSon[1];if(nPtrSon[2]) delete nPtrSon[2];listWordEnd.clear();}
    TSTNode<T>* nPtrSon[3];
    T tDato;
    int iKey;
    map<int,T> listWordEnd;
};

#endif // TSTNODE_H
