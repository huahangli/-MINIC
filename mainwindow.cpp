/****************************************************/
/* File: mainwindow.cpp                             */
/* The mainwindow of MINIC compiler GUI             */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "MINIC.h"


extern int linepos; /* current position in LineBuf */
extern int bufsize; /* current size of buffer string */
extern int EOF_flag; /* corrects ungetNextChar behavior on EOF */


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MINI C");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"),"", tr("文本文件(*.minic *.cpp *.c *.h)"));
    if(fileName.isEmpty()){
        }
        else {
                QFile f(fileName);
                if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui->textEdit->setText("Fail！！");
                    return;
                }
                QTextStream txtInput(&f);
                ui->textEdit->clear();
                ui->textEdit->setText(txtInput.readAll());//显示txt文件内容
                f.close();
        }
}


void MainWindow::on_pushButton_3_clicked()
{
    QString inp=ui->textEdit->toPlainText();
    QFile fin("input.minic");
    fin.open(QIODevice::WriteOnly| QIODevice::Text);
    fin.write(inp.toUtf8());
    fin.close();

    lineno=0;
    linepos = 0; /* current position in LineBuf */
    bufsize = 0; /* current size of buffer string */
    EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */
    char * input="input.minic";

    mini_c(input);

    QString fileName="out.txt";
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textBrowser_2->setText("Fail！！");
        return;
    }
    QTextStream txtInput(&f);
    ui->textBrowser_2->clear();
    ui->textBrowser_2->setText(txtInput.readAll());//显示txt文件内容
}


void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("保存文件"),
            "",
            tr("文本文件 (*.minic )"));
    if(fileName.isEmpty())
    {}
    else
    {
        QString output=ui->textBrowser_2->toPlainText();
        QFile f(fileName);
        if(!f.open(QIODevice::WriteOnly| QIODevice::Text))
        {
            ui->textBrowser_2->setText("Fail！！");
            return;
        }
        f.write(output.toUtf8());
        f.close();
    }
}

