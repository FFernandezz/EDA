#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItem>
#include <QMessageBox>


using namespace std;

const QString HostName="localhost";
const QString DatabaseName= "world";
const QString UserName= "";
const QString Password= "";
const int Port=5432;
QSqlDatabase db =QSqlDatabase::addDatabase("QPSQL");




static int distance(Record* record1, Record* record2)
{
    string word1=record1->m_wordIndex.toStdString();

    string word2=record2->m_wordIndex.toStdString();

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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->labelRange->setVisible(false);
    ui->spinBoxRange->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDB(QString dBName)
{
    db.setHostName(HostName);
    db.setDatabaseName(dBName);
    db.setUserName(UserName);
    db.setPassword(Password);
    db.setPort(Port);

    if(db.open() != true){
        QMessageBox::critical(this, tr("Connection Failed!"),db.lastError().text());
        ui->statusBar->showMessage("Connection Failed!");

    }
    else{
        ui->statusBar->showMessage("Connection Successful!",5000);
        ui->comboBoxTables->clear();
        ui->comboBoxTables->addItems(db.tables());

    }
}


void MainWindow::on_buttonStart_clicked()
{
    bool whereActivated=false;
    bool selectAll=false;


    QString columnNames=ui->lineEditSelect->text().remove(" ");
    if(columnNames=="*") selectAll=true;

    QString whereColumn=ui->lineEditWhere->text().remove(" ");
    if (whereColumn!=""){
        whereActivated=true;

        if(!selectAll  && columnNames.contains(whereColumn, Qt::CaseInsensitive)==false){
            columnNames+=","+whereColumn;
        }
    }

    QString query= "SELECT "+columnNames +" FROM " +ui->lineEditFrom->text();

    QSqlQueryModel* model= new QSqlQueryModel(ui->tableQuery);
    model->setQuery(query);


    QStringList columnNameList= columnNames.split(",");


    if(whereActivated){
        if(selectAll){
            columnNameList.clear();
            for (register int i = 0; i < model->columnCount(); ++i)
                columnNameList.push_back(model->headerData( i, Qt::Horizontal ).toString());
        }

        //similar to

        if(ui->comboBoxOperation->currentIndex()==0){

            m_bkTree=BKTree<Record*>(distance);

            for (int i = 0; i < model->rowCount(); ++i) {
                Record* rec=new Record(model->record(i).value(whereColumn).toString(),i);
                for(register int j=0; j<columnNameList.size();++j){
                    rec->addData(model->record(i).value(columnNameList[j]).toString());
                }
                m_bkTree.insert(rec);
            }

            vector<Record*> result =m_bkTree.find(new Record(ui->lineEditOperation->text(),-1),ui->spinBoxRange->value());



            QStandardItemModel* myModel= new QStandardItemModel(ui->tableQuery);
            myModel->clear();
            for (int i=0;i<columnNameList.size();++i){
                myModel->insertColumn(i);
                myModel->setHeaderData(i, Qt::Horizontal,columnNameList[i]);
            }
            for (register unsigned int i=0;i<result.size();++i){
                myModel->insertRow(i);
                for (register int j=0;j<result[i]->m_row.size();++j){
                    myModel->setItem(i,j,new QStandardItem(result[i]->m_row[j]));
                }
            }


            ui->tableQuery->setModel(myModel);
        }

        //contains
        else if(ui->comboBoxOperation->currentIndex()==1){

            /*QString query= "SELECT "+columnNames +" FROM " +ui->lineEditFrom->text()+
                    " WHERE "+ ui->lineEditWhere->text() + " LIKE '%" + ui->lineEditOperation->text()+"%'";

            model= new QSqlQueryModel(ui->tableQuery);
            model->setQuery(query);


            ui->tableQuery->setModel(model);*/


            for (int i = 0; i < model->rowCount(); ++i) {
                for (auto &it: model->record(i).value(whereColumn).toString().split(" ")){
                    Record rec(it,i);
                    for(register int j=0; j<columnNameList.size();++j){
                        rec.addData(model->record(i).value(columnNameList[j]).toString());
                    }
                    m_tst.add(rec,rec.m_id);
                }
            }
            Record recQuery(ui->lineEditOperation->text(),-1);
            vector<Record> result =m_tst.find(recQuery);



            QStandardItemModel* myModel= new QStandardItemModel(ui->tableQuery);
            myModel->clear();
            for (int i=0;i<columnNameList.size();++i){
                myModel->insertColumn(i);
                myModel->setHeaderData(i, Qt::Horizontal,columnNameList[i]);
            }
            for (register unsigned int i=0;i<result.size();++i){
                myModel->insertRow(i);
                for (register int j=0;j<result[i].m_row.size();++j){
                    myModel->setItem(i,j,new QStandardItem(result[i].m_row[j]));
                }
            }


            ui->tableQuery->setModel(myModel);
        }
    }
    else
        ui->tableQuery->setModel(model);

}



void MainWindow::on_lineEditWhere_textEdited(const QString &arg1)
{
    if (arg1!=""){
        ui->comboBoxOperation->setEnabled(true);
        ui->lineEditOperation->setEnabled(true);
    }
    else{
        ui->comboBoxOperation->setEnabled(false);
        ui->lineEditOperation->setEnabled(false);

        ui->comboBoxOperation->setCurrentIndex(-1);
    }
}


void MainWindow::on_comboBoxOperation_currentIndexChanged(int index)
{

    if(index== 0){
        ui->labelRange->setVisible(true);
        ui->spinBoxRange->setVisible(true);
    }
    else{
        ui->labelRange->setVisible(false);
        ui->spinBoxRange->setVisible(false);
    }
}

void MainWindow::on_comboBoxTables_currentIndexChanged(int )
{
    QString query= "SELECT * FROM "+ui->comboBoxTables->currentText();

    QSqlQueryModel* model= new QSqlQueryModel(ui->tableTables);
    model->setQuery(query);

    ui->tableTables->setModel(model);
}



void MainWindow::on_buttonSetDB_clicked()
{
    if (ui->lineDataBase->text()!=""){
        initDB(ui->lineDataBase->text());
    }
}
