#include "calculcator.h"
#include "ui_calculcator.h"
#include "calc.cpp"
#include <string>
#include <vector>
#include <map>

vector<string> postfix_expression;
vector<string> varibles_names;

calculcator::calculcator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::calculcator)
{
    ui->setupUi(this);
    setFixedSize(size());
}

calculcator::~calculcator()
{
    delete ui;
}

void calculcator::on_pushButton_clicked()
{
    string input_expression = ui->lineEdit->text().toStdString();
    // Errors processing
    try{
        avtomat(input_expression);
    }
    catch(runtime_error& e){
        ui->lineEdit_2->setText(QString::fromStdString(e.what()));
        return;
    }

    vector<string> expression_tokens = tokenize_expression(input_expression);
    postfix_expression = infix2postfix(expression_tokens);

    // Output postfix notation
    string postfix_string;
    for(string i: postfix_expression)
        postfix_string += i + " ";

    ui->lineEdit_2->setText(QString::fromStdString(postfix_string));


    // Read constants
    BigNumber pi, e;
    pi.read_string(PI_NUM);
    e.read_string(E_NUM);
    map<string, BigNumber> varibles = {
            {"pi", pi}, {"e", e}
        };


    // Read varibles
    varibles_names = get_varibles_names(postfix_expression);


    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    for(int i=0; i<varibles_names.size(); i++){
        ui->tableWidget->insertRow(0);
    }

    for(int i=0; i<varibles_names.size(); i++){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem());
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
        ui->tableWidget->item(i, 0)->setText(QString::fromStdString(varibles_names[i]));
        ui->tableWidget->item(i, 0)->setFlags(ui->tableWidget->item(i, 0)->flags() ^ Qt::ItemIsEditable);
    }

    if(!varibles_names.empty())
        return;

    // Calculate
    try{
        BigNumber answer = calculation(postfix_expression, varibles);
        ui->lineEdit->setText(QString::fromStdString(answer.get_string()));
    }
    catch(runtime_error& e){
        ui->lineEdit_2->setText(QString::fromStdString(e.what()));
        return;
    }
}

void calculcator::on_pushButton_2_clicked()
{

    // Read constants
    BigNumber pi, e, value;
    pi.read_string(PI_NUM);
    e.read_string(E_NUM);
    map<string, BigNumber> varibles = {
            {"pi", pi}, {"e", e}
        };


    for(int i=0; i<varibles_names.size(); i++){
        QString var_name = ui->tableWidget->item(i, 0)->text();
        QString var_val = ui->tableWidget->item(i, 1)->text();
        if(!is_number(var_val.toStdString()))
            return;

        BigNumber value;
        value.read_string(var_val.toStdString());
        varibles[var_name.toStdString()] = value;
    }

    // Calculate
    try{
        BigNumber answer = calculation(postfix_expression, varibles);
        ui->lineEdit->setText(QString::fromStdString(answer.get_string()));
    }
    catch(runtime_error& e){
        ui->lineEdit_2->setText(QString::fromStdString(e.what()));
        return;
    }
}
