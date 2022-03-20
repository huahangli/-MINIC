/****************************************************/
/* File: mainwindow.h                               */
/* The mainwindow of MINIC compiler GUI             */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>

#include <QTableWidget>
#include <QTableWidgetItem>

#include <stdio.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:


    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
