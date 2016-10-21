#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QPlainTextEdit>

#include <cstdio>
#include <cstring>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_lineEdit_returnPressed();

    void on_lineEdit_2_returnPressed();

    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_pushButton_4_clicked();

private:
    void showText(QString fileName, QPlainTextEdit *plainTextWidget);

    Ui::MainWindow *ui;
    QString encryptFile, decryptFile;
    bool keyIsHex = true;
};

#endif // MAINWINDOW_H
