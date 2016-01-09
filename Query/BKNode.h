#ifndef BKNODE_H
#define BKNODE_H
#include <map>
#include <iostream>
using namespace std;

typedef int typeDistance;


template <typename T>
class BKNode
{
    BKNode<T>* nullNode=0;
public:
    BKNode(){m_dad=0;m_sons.clear();}
    BKNode(T& data):m_data(data){m_dad=0;m_sons.clear();}
    ~BKNode(){m_sons.clear();}

    void insert(T newSon,typeDistance distance){
        m_sons[distance]=new BKNode <T>(newSon);
        m_sons[distance]->m_dad=this;
    }

    bool hasSon(typeDistance distance){
        if(m_sons[distance])
            return true;
        else{
            m_sons.erase(distance);
            return false;
        }
    }

    BKNode<T>*& getSon(typeDistance distance){
        if(m_sons[distance])
            return m_sons[distance];
        else{
            m_sons.erase(distance);
            return nullNode;
        }
    }
    void print(string& buffer){

        for (auto &iter:m_sons){
            buffer+=to_string(iter.second->m_ID)+"[label="+iter.second->m_data+"];\n"+
                  to_string(m_ID)+" -> "+ to_string(iter.second->m_ID)+"[label="+to_string(iter.first)+"];\n";

            iter.second->print(buffer);
        }
    }

    T m_data;
    int m_ID;
    BKNode<T>* m_dad;

    map<typeDistance,BKNode<T>*> m_sons;

};



#endif // BKNODE_H
