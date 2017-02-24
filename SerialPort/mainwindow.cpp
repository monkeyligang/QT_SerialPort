#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include<QMessageBox>
#include<QTGui>
#include<QLabel>
#include<QtSql>
#include"dbserver.h"
#include <iostream>

QByteArray aa;
int l = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //链接数据库
    if( !this->database->connectToMySQL())
    {
       return;
    }
    ui->setupUi(this);
    setFixedSize(861,678);
    ui->CloseButton->setEnabled(false);//开始“关闭摁钮”不可用
    ui->SendButton->setEnabled(false);
    IsHexReceive = false;
    isStopDisplay = false;

    QStringList comList;//串口号
    comList<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"
          <<"COM7"<<"COM8"<<"COM9"<<"COM10"<<"COM11"<<"COM12"
         <<"COM13"<<"COM14"<<"COM15";
    ui->PortNameBox->addItems(comList);
   // ui->openfile->mousePressEvent(QMouseEvent *ev);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readMyCom() //读串口函数
{
    qint64 len = myCom->bytesAvailable();//读到的数据长度
    if( l == 0 )
    {
        aa = 0;
    }


    //读取串口缓冲区的所有数据给临时变量temp
    if (IsHexReceive)
    {
        QString strBuf, strTemp;
        QByteArray bb;
        bb = 0;
        char* buf = new char[len];
        int realLen = myCom->read(buf, len);//输出真实读出的长度
        QChar ch;
        for(int i = 0; i < len; ++i)
        {
            //QString::arg ( int a, int fieldWidth = 0, int base = 10, const QChar & fillChar = QLatin1Char( ' ' ) ) const
            //16进制输出：fieldWidth表示字符宽度，base表示进制，
            strTemp = QString("%1").arg((unsigned char)buf[i], 2, 16, QChar('0'));//转换为16进制
            strBuf.append(strTemp.toUpper());
            strBuf.append(' ');
            bb.append(strTemp);
        }

        if( len == 6 )
        {
            aa = bb.data();
            l = 0;
        }
        else if( len == 9 )
        {
              aa = bb.data();
              l = 0;
        }
        else if(  len<6 || len<9 )
        {
           if(l!=0)
           {
               aa += bb.data();
               if( l+len == 6 )
               {
                   l = 0;
               }
               else if( l+len == 9 )
               {
                   l =0;
               }
               else
               {
                   l = l+len;
                   aa += bb.data();
               }


           }
           else if(l == 0)
           {
               aa = 0;
               l = len;
               aa += bb.data();
           }
        }


       /* QString a;
        a.append("叠加数据:").append(aa).append("串口长度:").append((int)len+'0').append("串口数据:").append(bb).append("l长度:").append(l+'0');
        ui->textBrowser_3->insertPlainText(a);
        ui->textBrowser_3->insertPlainText("\n");*/

        insert_database(aa,len);


        if(!isStopDisplay)
        {
             ui->textBrowser->moveCursor((QTextCursor::End));
             ui->textBrowser->insertPlainText(strBuf);
             if (ui->AutoChBox->isChecked())
                 ui->textBrowser->insertPlainText("\n");
             ui->textBrowser->moveCursor((QTextCursor::End));
        }
        delete []buf;
        return;
    }
    else
    {
        if(!isStopDisplay){
            QByteArray temp = myCom->readAll();
            //读取数据缓冲区中的所有数据给临时变量temp
            ui->textBrowser->insertPlainText(temp);
            ui->textBrowser->moveCursor((QTextCursor::End));
        }
    }
}

//打开按钮
void MainWindow::on_OpenButton_clicked()
{
    QString portName;
    portName.append("\\\\.\\").append(ui->PortNameBox->currentText());//获取串口名
    myCom = new Win_QextSerialPort(portName,QextSerialBase::EventDriven);//定义串口对象
    myCom->setPortName(portName);
    myCom->open(QIODevice::ReadWrite);//以读写方式打开串口
    //设置波特率
    if(ui->BaudRateBox->currentText()==tr("1200"))
        myCom->setBaudRate(BAUD1200);
    else if(ui->BaudRateBox->currentText()==tr("2400"))
        myCom->setBaudRate(BAUD2400);
    else if(ui->BaudRateBox->currentText()==tr("4800"))
        myCom->setBaudRate(BAUD4800);
    else if(ui->BaudRateBox->currentText()==tr("9600"))
        myCom->setBaudRate(BAUD9600);
    else if(ui->BaudRateBox->currentText()==tr("14400"))
        myCom->setBaudRate(BAUD14400);
    else if(ui->BaudRateBox->currentText()==tr("19200"))
        myCom->setBaudRate(BAUD19200);
    else if(ui->BaudRateBox->currentText()==tr("38400"))
        myCom->setBaudRate(BAUD38400);
    else if(ui->BaudRateBox->currentText()==tr("57600"))
        myCom->setBaudRate(BAUD57600);
    else if(ui->BaudRateBox->currentText()==tr("128000"))
        myCom->setBaudRate(BAUD128000);
    else if(ui->BaudRateBox->currentText()==tr("115200"))
        myCom->setBaudRate(BAUD115200);

    //设置数据位
    if(ui->DataBitsBox->currentText()==tr("5"))
        myCom->setDataBits(DATA_5);
    else if(ui->DataBitsBox->currentText()==tr("6"))
        myCom->setDataBits(DATA_6);
    else if(ui->DataBitsBox->currentText()==tr("7"))
        myCom->setDataBits(DATA_7);
    else if(ui->DataBitsBox->currentText()==tr("8"))
        myCom->setDataBits(DATA_8);

    //设置奇偶校验位
    if(ui->ParityBox->currentText()==tr("NONE"))
        myCom->setParity(PAR_NONE);
   else if(ui->ParityBox->currentText()==tr("ODD"))
        myCom->setParity(PAR_ODD);
   else if(ui->ParityBox->currentText()==tr("EVEN"))
        myCom->setParity(PAR_EVEN);

    //设置停止位
    if(ui->StopBitBox->currentText()==tr("1"))
         myCom->setStopBits(STOP_1);
    else if(ui->StopBitBox->currentText()==tr("2"))
         myCom->setStopBits(STOP_2);
    myCom->setFlowControl(FLOW_OFF);//设置为无数据流控制
    myCom->setTimeout(500);//延时

    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    //信号和槽函数连接，当串口缓冲区有数据时，进行读串口操作
    if (!myCom->isOpen())
    {
         QMessageBox::critical(this, QString::fromLocal8Bit("错误"),
                 QString::fromLocal8Bit("无法打开串口")+myCom->portName()+QString::fromLocal8Bit("\n指定的串口不存在或者被占用"));
         return;
    }
    ui->OpenButton->setEnabled(false);//打开串口后“打开串口摁钮”不可用
    ui->CloseButton->setEnabled(true);//打开串口后”关闭串口摁钮“可用
    ui->SendButton->setEnabled(true);
}
//关闭按钮
void MainWindow::on_CloseButton_clicked()
{
    myCom->close();
    ui->OpenButton->setEnabled(true); //关闭串口后“打开串口”按钮可用
    ui->CloseButton->setEnabled(false); //关闭串口后“关闭串口”按钮不可用
    ui->BaudRateBox->setEnabled(true); //设置各个组合框可用
    ui->DataBitsBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->StopBitBox->setEnabled(true);
    ui->PortNameBox->setEnabled(true);
    ui->SendButton->setEnabled(false);
}
//发送数据
void MainWindow::sendData()
{
    QString str=ui->SendEdit->text();
    if (str=="")
    {
         QMessageBox::warning(this, tr("Send File"), tr("Nothing"));
         return;
    }//发送数据为空
    if (!myCom->isOpen())
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"),
                    QString::fromLocal8Bit("串口没有打开") + myCom->portName());
        return;
    }//串口没有打开
    //16j进制发送
    if(IsHexSend)
    {
        if(!str.endsWith(' '))
            str.append(' ');
        QString strTemp;
        int len = str.length();
        sendBuf.clear();
        for(int i=0;i<len;)
        {
            bool ok;
            int index = str.indexOf(' ',i,Qt::CaseInsensitive);
            if(index<0)
                break;
            strTemp = str.mid(i,index-i);
            i=index+1;
            unsigned short k = strTemp.toUShort(&ok,16);
            sendBuf.append((char)k);
        }
        myCom->write(sendBuf);
        return;
    }
    else
    {
        myCom->write(ui->SendEdit->text().toLatin1());//默认转换为ascll码发送
    }

}

void MainWindow::on_SendButton_clicked()
{
   this->sendData();
}

//保存数据
void MainWindow::on_SaveButton_clicked()
{
   saveFile();
}
//保存数据
void MainWindow::saveFile()
{
    QString tempData=ui->textBrowser->toPlainText();
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Text Files(*.txt)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Save File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out<<tempData;
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }
}
//打开文件
void MainWindow::open(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "/home",
                                                   QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
}


void MainWindow::on_Stop_clicked()
{
    isStopDisplay = !isStopDisplay;
    if (isStopDisplay)
        ui->Stop->setText(QString::fromLocal8Bit("继续接受"));
    else
       ui->Stop->setText(QString::fromLocal8Bit("停止接受"));
}

void MainWindow::on_ClearButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_textBrowser_windowIconTextChanged(const QString &iconText)
{
    //始终是紧跟最后的信息
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_AutoChBox_stateChanged(int arg1)
{
    as=(arg1==0?false:true);
}

//接受数据转换为16进制
void MainWindow::on_HexReceive_stateChanged(int arg1)
{
    IsHexReceive = !IsHexReceive;
}


void MainWindow::on_ClearSendButton_clicked()
{
    ui->SendEdit->clear();
}
//发送数据转换为16进制
void MainWindow::on_HexSendBox_stateChanged(int arg1)
{
    IsHexSend = !IsHexSend;
}
//插库
void MainWindow::insert_database(QByteArray buffer, int len)
{
    value = 0;
   // QByteArray buffer = buff.toHex();
    //解码包类型
    for(int k=0;k<2;k++)
    {
        value = value*16+switchNum(buffer[0+k]);
    }


    if(value == 1 && buffer.size() == 12 )
    {

       unlockPocket_InterestPackage(buffer.data(),buffer.size(),value,DestinationID,DataFlag,PackageCount,NodeID,Noce);

       QString a;
       a.append("interest:").append(value+'0').append(DestinationID+'0').append(DataFlag+'0').append(PackageCount+'0').append(NodeID+'0').append(Noce+'0');
       ui->textBrowser_3->insertPlainText(a);
       ui->textBrowser_3->insertPlainText("\n");
       ui->textBrowser_3->moveCursor((QTextCursor::End));

       QDateTime atime=QDateTime::currentDateTime();
       ctime=atime.toString("yyyyMMddhhmmsszzz");
       ctime+="000";
       //QSqlQuery query;
       //query.exec("insert into interest(ctime,value,DestinationID,DataFlag,PackageCounter,LastID,Noce)values(ctime,value,DestinationID,DataFlag,PackageCounter,LastID,Noce)");

       database->insertMySql_interest(ctime,value,DestinationID,DataFlag,PackageCount,NodeID,Noce);

    }
    else if(value == 2 && buffer.size() == 18 )
    {
       unlockPocket_DataPackage(buffer.data(),buffer.size(),value,DestinationID,DataFlag,PackageCount,PackageAll,PackageSeg,Data);

       QString a;
       a.append("Data:").append(value+'0').append(DestinationID+'0').append(DataFlag+'0').append(PackageCount+'0').append(PackageAll+'0').append(PackageSeg+'0').append((int)Data+'0');
       ui->textBrowser_3->insertPlainText(a);
       ui->textBrowser_3->insertPlainText("\n");
       ui->textBrowser_3->moveCursor((QTextCursor::End));

       QDateTime atime=QDateTime::currentDateTime();
       ctime=atime.toString("yyyyMMddhhmmsszzz");
       ctime+="000";

       database->insertMySql_datapackage(ctime,value,DestinationID,DataFlag,PackageCount,PackageAll,PackageSeg,Data);
    }

}
