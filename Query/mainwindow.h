#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "BKTree.h"
#include "TST.h"


class Record;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initDB(QString dBName);

private slots:
    void on_buttonStart_clicked();

    void on_lineEditWhere_textEdited(const QString &arg1);

    void on_comboBoxOperation_currentIndexChanged(int index);

    void on_comboBoxTables_currentIndexChanged(int index);


    void on_buttonSetDB_clicked();

private:
    Ui::MainWindow *ui;
    TST<Record> m_tst;
    BKTree<Record*> m_bkTree;

};

#endif // MAINWINDOW_H





#ifndef RECORD_H
#define RECORD_H


class Record
{
public:
    Record(){}
    Record(QString index,int id):m_wordIndex(index),m_id(id){m_row.clear();}
    Record(const Record &rec):m_wordIndex(rec.m_wordIndex),m_id(rec.m_id){m_row=rec.m_row;}

    ~Record(){}

    void addData(QString data){m_row.push_back(data);}

    int find(Record & rec){
        return m_wordIndex.toStdString().find(rec.m_wordIndex.toStdString());

    }
    bool operator < (const Record &rec){return m_wordIndex.toStdString()<rec.m_wordIndex.toStdString();}

    bool operator > (const Record &rec){return m_wordIndex.toStdString()>rec.m_wordIndex.toStdString();}

    bool operator == (const Record &rec){return m_wordIndex.toStdString()==rec.m_wordIndex.toStdString();}

    QString m_wordIndex;
    int m_id;
    QStringList m_row;

};

#endif // RECORD_H


