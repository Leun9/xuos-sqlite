#include "mainwindow.h"
#include "ui_mainwindow.h"

#define XUOSDB this->database_

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowMaximized);
    if (InitDB() != 0)
    {
        QMessageBox::warning(this, "错误", "初始化失败，错误信息：" + XUOSDB.lastError().text());
        exit (1);
    }

    /*** 初始化控件 ***/
    QStringList qstrList({"学号", "姓名", "性别", "出生日期", "院系", "奖学金"});
    ui->tbwStu->setColumnCount(6);
    ui->tbwStu->setHorizontalHeaderLabels(qstrList);
    //ui->tbwStu->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tbwStu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*** 显示信息 ***/
    DisplayStudents();
}

MainWindow::~MainWindow()
{
    delete ui;
    XUOSDB.close();
}


int MainWindow::InitDB() {
    XUOSDB = QSqlDatabase::addDatabase("QSQLITE");
    XUOSDB.setDatabaseName(".userdata");
    //XUOSDB.setUserName(sql_username);
    //XUOSDB.setPassword(sql_passwd);
    if (!XUOSDB.open()) return 1;
    QSqlQuery sql_query;
    sql_query.exec(sql_create_tb_student);
    sql_query.exec(sql_create_tb_course);
    sql_query.exec(sql_create_tb_sc);
    return 0;
}

int MainWindow::DisplayStudents() {
    QSqlQuery sql_query(sql_select_tb_student);
    if (!sql_query.exec()) return 1;

    ui->tbwStu->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwStu->insertRow(row);
        for (int i = 0; i < 6; ++i)
            ui->tbwStu->setItem(row, i, new QTableWidgetItem(sql_query.value(i).toString()));
        row++;
    }
    return 0;
}

int MainWindow::InsertStudent() {
    QSqlQuery sql_query(sql_insert_student);
    // TODO : 数据合法性检查
    sql_query.addBindValue(ui->leStuNum->text());
    sql_query.addBindValue(ui->leStuName->text());
    sql_query.addBindValue(ui->cbStuSex->currentText());
    sql_query.addBindValue(ui->leStuBd->text());
    sql_query.addBindValue(ui->leStuDep->text());
    sql_query.addBindValue(ui->cbStuSch->currentText());
    if(!sql_query.exec())
    {
        QMessageBox::warning(this, "错误", "插入失败，错误信息：" + sql_query.lastError().text());
        return 1;
    }
    ui->leStuNum->clear();
    ui->leStuName->clear();
    //ui->cbStuSex->clear();
    ui->leStuBd->clear();
    ui->leStuDep->clear();
    //ui->cbStuSch->clear();
    DisplayStudents();
    return 0;
}

void MainWindow::on_btnStuIns_clicked()
{
    InsertStudent();
}
