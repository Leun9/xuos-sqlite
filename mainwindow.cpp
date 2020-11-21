#include "mainwindow.h"
#include "ui_mainwindow.h"

#define XUOSDB this->database_

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (InitDB() != 0)
    {
        QMessageBox::warning(this, "错误", "初始化失败，错误信息：" + XUOSDB.lastError().text());
        exit (1);
    }

    /* 初始化控件 */

    /*** tab student ***/
    QStringList qstrListStu({"学号", "姓名", "性别", "年龄", "院系", "奖学金"});
    ui->tbwStu->setColumnCount(6);
    ui->tbwStu->setHorizontalHeaderLabels(qstrListStu);
    //ui->tbwStu->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tbwStu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->leStuNum->setValidator(new QIntValidator());
    ui->leStuBd->setValidator(new QIntValidator());
    ui->leStuNumd->setValidator(new QIntValidator());

    /*** tab course ***/
    QStringList qstrListCou({"课程号", "课程名", "先行课程", "学分"});
    ui->tbwCou->setColumnCount(4);
    ui->tbwCou->setHorizontalHeaderLabels(qstrListCou);
    ui->tbwCou->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->leCouNum->setValidator(new QIntValidator());
    ui->leCouPnum->setValidator(new QIntValidator());
    ui->leCouCredit->setValidator(new QIntValidator());
    ui->leCouNumd->setValidator(new QIntValidator());

    /*** tab grade ***/
    QStringList qstrListGrd({"学号", "课程号", "分数"});
    ui->tbwGrd->setColumnCount(3);
    ui->tbwGrd->setHorizontalHeaderLabels(qstrListGrd);
    ui->tbwGrd->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->leGrdSno->setValidator(new QIntValidator());
    ui->leGrdCno->setValidator(new QIntValidator());
    ui->leGrdGrade->setValidator(new QIntValidator());

    /*** tab stuinfo ***/
    QStringList qstrListSInfo1({"学号", "姓名", "性别", "年龄", "院系", "奖学金"});
    ui->tbwStuInfo1->setColumnCount(6);
    ui->tbwStuInfo1->setRowCount(1);
    ui->tbwStuInfo1->setHorizontalHeaderLabels(qstrListSInfo1);
    ui->tbwStuInfo1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbwStuInfo1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tbwStuInfo1->verticalHeader()->hide();
    QStringList qstrListSInfo2({"课程号", "课程名", "先行课程", "学分", "成绩"});
    ui->tbwStuInfo2->setColumnCount(5);
    ui->tbwStuInfo2->setHorizontalHeaderLabels(qstrListSInfo2);
    ui->tbwStuInfo2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->leStuInfo->setValidator(new QIntValidator());

    /*** tab gradeSummary ***/
    QStringList qstrListSum({"系别", "平均分", "最高分", "最低分", "优秀率", "不及格人数"});
    ui->tbwSum1->setColumnCount(6);
    ui->tbwSum1->setRowCount(1);
    ui->tbwSum1->setHorizontalHeaderLabels(qstrListSum);
    ui->tbwSum1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbwSum1->verticalHeader()->hide();
    //ui->tbwSum1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tbwSum2->setColumnCount(6);
    ui->tbwSum2->setHorizontalHeaderLabels(qstrListSum);
    ui->tbwSum2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*** tab Rank ***/
    QStringList qstrListRank({"学号", "姓名", "课程号", "课程名", "分数"});
    ui->tbwRank->setColumnCount(5);
    ui->tbwRank->setHorizontalHeaderLabels(qstrListRank);
    ui->tbwRank->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /* 显示信息 */
    DisplayStudents();
    DisplayCourses();
    DisplayGrades();
    DisplaySummarys();
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
    sql_query.exec(sql_open_foreign_key);
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

#define INS_ERR_RET(errMsg, RETCODE) \
{ \
    QMessageBox::warning(this, "错误", QString("插入失败，错误信息：") + errMsg); \
    return RETCODE; \
}

#define PAD_SINGLE_QUOTES(x) (QString("'") + x + QString("'"))

int MainWindow::InsertStudent() {
    QSqlQuery sql_query(sql_insert_student);
    if (ui->leStuNum->text().length() != 9) INS_ERR_RET("学号长度必须为9", 1);
    sql_query.addBindValue(ui->leStuNum->text());
    if (ui->leStuName->text().isEmpty()) INS_ERR_RET("姓名不能为空", 1);
    sql_query.addBindValue(ui->leStuName->text());
    if (ui->cbStuSex->currentText().isEmpty()) INS_ERR_RET("性别不能为空", 1);
    sql_query.addBindValue(ui->cbStuSex->currentText());
    if (ui->leStuBd->text().isEmpty()) INS_ERR_RET("年龄不能为空", 1);
    sql_query.addBindValue(ui->leStuBd->text());
    // 系别可以为空
    sql_query.addBindValue(ui->leStuDep->text());
    if (ui->cbStuSch->currentText().isEmpty()) INS_ERR_RET("奖学金不能为空", 1);
    sql_query.addBindValue(ui->cbStuSch->currentText());
    if(!sql_query.exec()) INS_ERR_RET(sql_query.lastError().text(), 1);

    ui->leStuNum->clear();
    ui->leStuName->clear();
    ui->cbStuSex->setCurrentIndex(0);
    ui->leStuBd->clear();
    ui->leStuDep->clear();
    ui->cbStuSch->setCurrentIndex(0);
    DisplayStudents();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnStuIns_clicked()
{
    InsertStudent();
}

#define DEL_ERR_RET(errMsg, RETCODE) \
{ \
    QMessageBox::warning(this, "错误", QString("删除失败，错误信息：") + errMsg); \
    return RETCODE; \
}

#define STABAR_SHOW_5000(Msg) {ui->statusbar->showMessage(Msg, 5000);}

int MainWindow::DeleteStudent() {
    QSqlQuery sql_query(sql_delete_student);
    sql_query.addBindValue(ui->leStuNumd->text());
    if (!sql_query.exec()) DEL_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生删除

    ui->leStuNumd->clear();
    DisplayStudents();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnStuDel_clicked()
{
    DeleteStudent();
}

#define UPD_ERR_RET(errMsg, RETCODE) \
{ \
    QMessageBox::warning(this, "错误", QString("更新失败，错误信息：") + errMsg); \
    return RETCODE; \
}

int MainWindow::UpdateStudent() {
    if (ui->leStuNum->text().isEmpty()) UPD_ERR_RET("学号为空", 1);
    QString qstr;
    if (!ui->leStuName->text().isEmpty()) qstr += "Sname=" + PAD_SINGLE_QUOTES(ui->leStuName->text()) + ",";
    if (!ui->cbStuSex->currentText().isEmpty()) qstr += "Ssex=" + PAD_SINGLE_QUOTES(ui->cbStuSex->currentText()) + ",";
    if (!ui->leStuBd->text().isEmpty()) qstr += "Sbd=" + ui->leStuBd->text() + ",";
    if (!ui->leStuDep->text().isEmpty()) qstr += "Sdept=" + PAD_SINGLE_QUOTES(ui->leStuDep->text()) + ",";
    if (!ui->cbStuSch->currentText().isEmpty()) qstr += "Scholarship=" + PAD_SINGLE_QUOTES(ui->cbStuSch->currentText()) + ",";
    if (qstr.isEmpty()) UPD_ERR_RET("无更新信息", 1);
    qstr = qstr.mid(0, qstr.length() - 1);
    QSqlQuery sql_query(QString(sql_update_student).arg(qstr).arg(ui->leStuNum->text()));
    if(!sql_query.exec()) UPD_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生修改

    ui->leStuNum->clear();
    ui->leStuName->clear();
    ui->cbStuSex->setCurrentIndex(0);
    ui->leStuBd->clear();
    ui->leStuDep->clear();
    ui->cbStuSch->setCurrentIndex(0);
    DisplayStudents();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnStuUpd_clicked()
{
    UpdateStudent();
}

int MainWindow::DisplayCourses() {
    QSqlQuery sql_query(sql_select_tb_course);
    if (!sql_query.exec()) return 1;

    ui->tbwCou->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwCou->insertRow(row);
        for (int i = 0; i < 4; ++i)
            ui->tbwCou->setItem(row, i, new QTableWidgetItem(sql_query.value(i).toString()));
        row++;
    }
    return 0;
}

int MainWindow::InsertCourse() {
    QSqlQuery sql_query(sql_insert_course);
    if (ui->leCouNum->text().isEmpty()) INS_ERR_RET("课程号不能为空", 1);
    sql_query.addBindValue(ui->leCouNum->text());
    if (ui->leCouName->text().isEmpty()) INS_ERR_RET("课程名不能为空", 1);
    sql_query.addBindValue(ui->leCouName->text());
    // 先行课程可以为空
    sql_query.addBindValue(ui->leCouPnum->text());
    // 学分可以为空
    sql_query.addBindValue(ui->leCouCredit->text());
    if(!sql_query.exec()) INS_ERR_RET(sql_query.lastError().text(), 1);

    ui->leCouNum->clear();
    ui->leCouName->clear();
    ui->leCouPnum->clear();
    ui->leCouCredit->clear();
    DisplayCourses();
    DisplaySummarys();
    return 0;
}

int MainWindow::DeleteCourse() {
    QSqlQuery sql_query(sql_delete_course);
    sql_query.addBindValue(ui->leCouNumd->text());
    if (!sql_query.exec()) DEL_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生删除

    ui->leCouNumd->clear();
    DisplayCourses();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnCouIns_clicked()
{
    InsertCourse();
}

void MainWindow::on_btnCouDel_clicked()
{
    DeleteCourse();
}

int MainWindow::UpdateCourse() {
    if (ui->leCouNum->text().isEmpty()) UPD_ERR_RET("课程号为空", 1);
    QString qstr;
    if (!ui->leCouName->text().isEmpty()) qstr += "Cname=" + PAD_SINGLE_QUOTES(ui->leCouName->text()) + ",";
    if (!ui->leCouPnum->text().isEmpty()) qstr += "Cpno=" + ui->leCouPnum->text() + ",";
    if (!ui->leCouCredit->text().isEmpty()) qstr += "Ccredit=" + ui->leCouCredit->text() + ",";
    if (qstr.isEmpty()) UPD_ERR_RET("无更新信息", 1);
    qstr = qstr.mid(0, qstr.length() - 1);
    QSqlQuery sql_query(QString(sql_update_course).arg(qstr).arg(ui->leCouNum->text()));
    //qDebug() << QString(sql_update_course).arg(qstr).arg(ui->leCouNum->text());
    if(!sql_query.exec()) UPD_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生修改

    ui->leCouNum->clear();
    ui->leCouName->clear();
    ui->leCouPnum->clear();
    ui->leCouCredit->clear();
    DisplayCourses();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnCouUpd_clicked()
{
    UpdateCourse();
}

int MainWindow::DisplayGrades() {
    QSqlQuery sql_query(sql_select_tb_grade);
    if (!sql_query.exec()) return 1;

    ui->tbwGrd->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwGrd->insertRow(row);
        for (int i = 0; i < 3; ++i)
            ui->tbwGrd->setItem(row, i, new QTableWidgetItem(sql_query.value(i).toString()));
        row++;
    }
    return 0;
}

int MainWindow::InsertGrade() {
    QSqlQuery sql_query(sql_insert_grade);
    if (ui->leGrdSno->text().isEmpty()) INS_ERR_RET("学号不能为空", 1);
    sql_query.addBindValue(ui->leGrdSno->text());
    if (ui->leGrdCno->text().isEmpty()) INS_ERR_RET("课程号不能为空", 1);
    sql_query.addBindValue(ui->leGrdCno->text());
    // 分数可以为空
    sql_query.addBindValue(ui->leGrdGrade->text());
    if(!sql_query.exec()) INS_ERR_RET(sql_query.lastError().text(), 1);

    ui->leGrdSno->clear();
    ui->leGrdCno->clear();
    ui->leGrdGrade->clear();
    DisplayGrades();
    DisplaySummarys();
    return 0;
}


void MainWindow::on_btnGrdIns_clicked()
{
    InsertGrade();
}

int MainWindow::DeleteGrade() {
    QSqlQuery sql_query(sql_delete_grade);
    if (ui->leGrdSno->text().isEmpty()) DEL_ERR_RET("学号不能为空", 1);
    sql_query.addBindValue(ui->leGrdSno->text());
    if (ui->leGrdCno->text().isEmpty()) DEL_ERR_RET("课程号不能为空", 1);
    sql_query.addBindValue(ui->leGrdCno->text());
    if (!sql_query.exec()) DEL_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生删除

    ui->leGrdSno->clear();
    ui->leGrdCno->clear();
    ui->leGrdGrade->clear();
    DisplayGrades();
    DisplaySummarys();
    return 0;
}

int MainWindow::UpdateGrade() {
    if (ui->leGrdSno->text().isEmpty()) UPD_ERR_RET("学号为空", 1);
    if (ui->leGrdCno->text().isEmpty()) UPD_ERR_RET("课程号为空", 1);
    QSqlQuery sql_query(sql_update_grade);
    sql_query.addBindValue(ui->leGrdGrade->text());
    sql_query.addBindValue(ui->leGrdSno->text());
    sql_query.addBindValue(ui->leGrdCno->text());
    if(!sql_query.exec()) UPD_ERR_RET(sql_query.lastError().text(), 1);
    // TODO 状态栏显示是否发生修改

    ui->leGrdSno->clear();
    ui->leGrdCno->clear();
    ui->leGrdGrade->clear();
    DisplayGrades();
    DisplaySummarys();
    return 0;

}

void MainWindow::on_btnGrdUpd_clicked()
{
    UpdateGrade();
}

void MainWindow::on_btnGrdDel_clicked()
{
    DeleteGrade();
}

#define QUERY_ERR_RET(errMsg, RETCODE) \
{ \
    QMessageBox::warning(this, "错误", QString("查询失败，错误信息：") + errMsg); \
    return RETCODE; \
}

int MainWindow::GetStuInfo() {
    if (ui->leStuInfo->text().isEmpty()) QUERY_ERR_RET("学号为空", 1);
    QSqlQuery sql_query(sql_stu_info1);
    sql_query.addBindValue(ui->leStuInfo->text());
    if (!sql_query.exec()) QUERY_ERR_RET(sql_query.lastError().text(), 1);
    if (!sql_query.next()) {
        QMessageBox::information(this, "信息", "查无此人");
        return 0;
    }
    for (int i = 0; i < 6; ++i)
        ui->tbwStuInfo1->setItem(0, i, new QTableWidgetItem(sql_query.value(i).toString()));
    sql_query.prepare(sql_stu_info2);
    sql_query.addBindValue(ui->leStuInfo->text());
    sql_query.addBindValue(ui->leStuInfo->text());
    if (!sql_query.exec()) QUERY_ERR_RET(sql_query.lastError().text(), 1);
    ui->tbwStuInfo2->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwStuInfo2->insertRow(row);
        for (int i = 0; i < 5; ++i)
            ui->tbwStuInfo2->setItem(row, i, new QTableWidgetItem(sql_query.value(i).toString()));
        row++;
    }
    return 0;
}

void MainWindow::on_btnStuInfo_clicked()
{
    GetStuInfo();
}

int MainWindow::DisplaySummarys() {
    QSqlQuery sql_query(sql_grade_summary1);
    if (!sql_query.exec()) return 1;
    ui->tbwSum2->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwSum2->insertRow(row);
        ui->tbwSum2->setItem(row, 0, new QTableWidgetItem(sql_query.value(0).toString()));
        if (sql_query.value(6).toInt() == 0) continue;
        ui->tbwSum2->setItem(row, 1, new QTableWidgetItem(QString::number(sql_query.value(1).toDouble(), 'f', 2)));
        ui->tbwSum2->setItem(row, 2, new QTableWidgetItem(sql_query.value(2).toString()));
        ui->tbwSum2->setItem(row, 3, new QTableWidgetItem(sql_query.value(3).toString()));
        ui->tbwSum2->setItem(row, 4, new QTableWidgetItem(QString::number(sql_query.value(4).toDouble()*100, 'f', 1) + "%"));
        ui->tbwSum2->setItem(row, 5, new QTableWidgetItem(sql_query.value(5).toString()));
        row++;
    }
    return 0;
}

int MainWindow::GetDepSummary() {
    if (ui->leSum->text().isEmpty()) QUERY_ERR_RET("系别为空", 1);
    int found = -1;
    for (int i = 0; i < ui->tbwSum2->rowCount(); ++i) {
        if (ui->leSum->text() == ui->tbwSum2->item(i, 0)->text()) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        QMessageBox::information(this, "信息", "系别不存在");
        return 0;
    }
    for (int i = 0; i < 6; ++i)
        ui->tbwSum1->setItem(0, i, new QTableWidgetItem(*ui->tbwSum2->item(found, i)));

    return 0;
}

void MainWindow::on_btnSum_clicked()
{
    GetDepSummary();
}

int MainWindow::GetDepRank() {
    if (ui->leRank->text().isEmpty()) QUERY_ERR_RET("系别为空", 1);
    int found = -1;
    for (int i = 0; i < ui->tbwSum2->rowCount(); ++i) {
        if (ui->leRank->text() == ui->tbwSum2->item(i, 0)->text()) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        QMessageBox::information(this, "信息", "系别不存在");
        return 0;
    }
    QSqlQuery sql_query(sql_grade_rank);
    sql_query.addBindValue(ui->leRank->text());
    if (!sql_query.exec()) QUERY_ERR_RET(sql_query.lastError().text(), 1);
    ui->tbwRank->setRowCount(0);
    int row = 0;
    while (sql_query.next()) {
        ui->tbwRank->insertRow(row);
        for (int i = 0; i < 5; ++i)
            ui->tbwRank->setItem(row, i, new QTableWidgetItem(sql_query.value(i).toString()));
        row++;
    }

    return 0;
}

void MainWindow::on_btnRank_clicked()
{
    GetDepRank();
}

int MainWindow::DeleteUselessCourse() {
    QSqlQuery sql_query(sql_delete_useless_course);
    if (!sql_query.exec()) DEL_ERR_RET("", 1);
    DisplayCourses();
    DisplaySummarys();
    return 0;
}

void MainWindow::on_btnCouAutoDel_clicked()
{
    DeleteUselessCourse();
}
