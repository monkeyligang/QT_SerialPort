#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "win_qextserialport.h"
#include "dbserver.h"


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

    void on_OpenButton_clicked();

    void readMyCom();//声明读串口槽函数

    void on_CloseButton_clicked();

    void on_SendButton_clicked();

    void on_SaveButton_clicked();

    void saveFile();

    void open();


    void on_Stop_clicked();

    void on_ClearButton_clicked();

    void on_textBrowser_windowIconTextChanged(const QString &iconText);

    void sendData();
    void on_AutoChBox_stateChanged(int arg1);

    void on_HexReceive_stateChanged(int arg1);

    void on_ClearSendButton_clicked();

    void on_HexSendBox_stateChanged(int arg1);

    void MainWindow::insert_database(QByteArray buffer, int len);//插库

private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *myCom;//声明对象，对串口
    bool IsHexReceive;//16进制
    bool IsHexSend;
    bool as;//换行标志符
    bool isStopDisplay;
    QMouseEvent *ev;
    QByteArray sendBuf;
    dbserver *database;

    QString ctime;
    //插库数据
    int value;
    int DestinationID;
    int DataFlag;
    int PackageCount;
    int NodeID;
    int Noce;
    int PackageAll;
    int PackageSeg;
    double Data;
};

#endif // MAINWINDOW_H
