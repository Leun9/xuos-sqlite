#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "sql.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStuIns_clicked();
    void on_btnStuDel_clicked();
    void on_btnStuUpd_clicked();
    void on_btnCouIns_clicked();
    void on_btnCouDel_clicked();
    void on_btnCouUpd_clicked();
    void on_btnGrdIns_clicked();
    void on_btnGrdUpd_clicked();
    void on_btnGrdDel_clicked();
    void on_btnStuInfo_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database_;

    int InitDB();
    int DisplayStudents();
    int InsertStudent();
    int DeleteStudent();
    int UpdateStudent();
    int DisplayCourses();
    int InsertCourse();
    int DeleteCourse();
    int UpdateCourse();
    int DisplayGrades();
    int InsertGrade();
    int DeleteGrade();
    int UpdateGrade();
    int GetStuInfo();
};

#endif // MAINWINDOW_H
