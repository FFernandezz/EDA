#ifndef BKTREE_H
#define BKTREE_H

#include "BKNode.h"
#include <vector>


template <typename T>
class BKTree
{
public:
    BKTree(){}
    BKTree(typeDistance (*function) (T,  T )){m_root=0;m_currentID=0;m_distanceFunction=function;}
    BKTree(vector<T>& dataList);

    ~BKTree(){if(m_root)delete m_root;}

    void insert(T word);
    void setDistanceFunction(typeDistance (*function) (T,  T ));


    void find(T queryWord, BKNode<T> *&currentNode, vector<T>& solution, typeDistance range=typeDistance(2));
    vector<T> find(T queryWord,typeDistance range=typeDistance(2));

    BKNode<T>* m_root;
    int m_currentID;
    typeDistance (*m_distanceFunction) (T,  T );

    void save(string fileName);
    void toPNG(string fileName);


};
#endif // BKTREE_H


template<typename T>
BKTree<T>::BKTree(vector<T> &dataList)
{
    for (auto &iter:dataList)
        insert(iter);
}

template<typename T>
void BKTree<T>::setDistanceFunction(typeDistance (*function)(T, T))
{
    m_distanceFunction=function;
}



template<typename T>
void BKTree<T>::insert(T word)
{
    if (!m_root){
        m_root=new BKNode<T>(word);
        m_root->m_ID=m_currentID;
        ++m_currentID;

        return;
    }
    BKNode<T>* nodeTemp=m_root;

    while (true){
        typeDistance currentDist=(*m_distanceFunction)(nodeTemp->m_data,word);
        if (nodeTemp->hasSon(currentDist))
            nodeTemp=nodeTemp->getSon(currentDist);

        else{
            nodeTemp->insert(word,currentDist);
            nodeTemp->getSon(currentDist)->m_ID=m_currentID;
            ++m_currentID;
            break;
        }
    }
}

template<typename T>
void BKTree<T>::find(T queryWord, BKNode<T> *&currentNode, vector<T>& solution, typeDistance range)
{
    if(!currentNode) return;

    typeDistance distance= (*m_distanceFunction)(queryWord,currentNode->m_data);

    if(distance<=range)
        solution.push_back(currentNode->m_data);

    for(typeDistance i=distance-range;i<=distance+range;++i)
        if(currentNode->hasSon(i)){
            find(queryWord,currentNode->getSon(i),solution,range);
        }

}

template<typename T>
vector<T> BKTree<T>::find(T queryWord,typeDistance range)
{

    vector<T> solution;
    if (range<0)
        find(queryWord,m_root,solution,2);
    else
        find(queryWord,m_root,solution,range);
    return solution;
}



template<typename T>
void BKTree<T>::save(string fileName)
{
    FILE* dot=fopen((fileName+".dot").c_str(),"w");

    fwrite("digraph G{\nnode [shape=circle];\n",sizeof(char),32,dot);

    if (m_root){
        string buffer=to_string(m_root->m_ID)+"[label="+m_root->m_data+"];\n";

        m_root->print(buffer);

        fwrite(buffer.c_str(),sizeof(char),buffer.length(),dot);
    }

    fwrite("}",sizeof(char),1,dot);
    fclose(dot);
}

template <typename T>
void BKTree<T>::toPNG(string fileName)
{
    string temp="dot -Tpng -l lib.png ";
    if(fileName.substr(fileName.length()-4)!=".dot")
        temp+=fileName+".dot -o "+fileName+".png";
    else
        temp+=fileName+" -o "+fileName.substr(0,fileName.length()-4)+".png";

    system(temp.c_str());
}





