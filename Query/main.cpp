#include "mainwindow.h"
#include <QApplication>


int levenshteinDistance(string word1, string word2)
{
    /*int N1 = word1.size();
    int N2 = word2.size();
    int i=0, j=0;
    vector<int> vect(N2+1);
    for ( i = 0; i <= N2; i++ )
        vect[i] = i;
    for ( i = 0; i < N1; i++ ) {
        vect[0] = i+1;
        int corner = i;
        for ( j = 0; j < N2; j++ ) {
            int upper = vect[j+1];
            if ( word1[i] == word2[j] )
                vect[j+1] = corner;
            else
                vect[j+1] = min(vect[j], min(upper, corner)) + 1;
            corner = upper;
        }
    }
    return vect[N2];*/

    int s1len = word1.size();
    int s2len = word2.size();

    auto column_start = (decltype(s1len))1;

    auto column = new decltype(s1len)[s1len + 1];
    std::iota(column + column_start, column + s1len + 1, column_start);

    for (auto x = column_start; x <= s2len; x++) {
        column[0] = x;
        auto last_diagonal = x - column_start;
        for (auto y = column_start; y <= s1len; y++) {
            auto old_diagonal = column[y];
            auto possibilities = {
                column[y] + 1,
                column[y - 1] + 1,
                last_diagonal + (word1[y - 1] == word2[x - 1]? 0 : 1)
            };
            column[y] = std::min(possibilities);
            last_diagonal = old_diagonal;
        }
    }
    auto result = column[s1len];
    delete[] column;
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    BKTree<string> bk(&levenshteinDistance);

    bk.insert("book");
    bk.insert("BOOKS");
    bk.insert("cake");
    bk.insert("boo");
    bk.insert("cape");
    bk.insert("cart");
    bk.insert("boon");
    bk.insert("cook");


    bk.save("prueba");
    bk.toPNG("prueba");


    for(auto &iter:bk.find("caqe",1))
        cout<<iter<<endl;


    return a.exec();
}
