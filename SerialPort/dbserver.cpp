#include "dbserver.h"
#include <QMessageBox>
#include <QtCore/QCoreApplication>
#include <QtSql>

dbserver::dbserver()
{
}

dbserver::~dbserver()
{

}
bool dbserver::connectToMySQL()//连接数据库
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("ndn2015");
    db.setUserName("root");
    db.setPassword("nlg");

    bool ok = db.open();
    if(!ok)
    {
        QMessageBox::critical(0,"Error",db.lastError().text());
        return false;
    }
    return true;
}

int dbserver::insertMySql_interest(QString time,int value,int DestinationID,int DataFlag,int PackageCount,int NodeID,int Noce)
{


       //组合成sql的插入命令
      QSqlQuery query;
      QString mySQL="INSERT INTO interest VALUES(";
       mySQL+=time;
       mySQL+=",";
       mySQL+=QString::number(value);
       mySQL+=",";
       mySQL+=QString::number(DestinationID);
       mySQL+=",";
       mySQL+=QString::number(DataFlag);
       mySQL+=",";
       mySQL+=QString::number(PackageCount);
       mySQL+=",";
       mySQL+=QString::number(NodeID);
       mySQL+=",";
       mySQL+=QString::number(Noce);
       mySQL+=")";
       query.exec(mySQL);
       return 0;
}

int dbserver::insertMySql_datapackage(QString time,int value,int DestinationID,int DataFlag,int PackageCount,int PackageAll,int PackageSeg,double Data)
{

       QSqlQuery query;
       //组合成sql的插入命令
       QString mySQL="INSERT INTO datapackage VALUES(";
       mySQL+=time;
       mySQL+=",";
       mySQL+=QString::number(value);
       mySQL+=",";
       mySQL+=QString::number(DestinationID);
       mySQL+=",";
       mySQL+=QString::number(DataFlag);
       mySQL+=",";
       mySQL+=QString::number(PackageCount);
       mySQL+=",";
       mySQL+=QString::number(PackageAll);
       mySQL+=",";
       mySQL+=QString::number(PackageSeg);
       mySQL+=",";
       mySQL+=QString::number(Data);
       mySQL+=")";
       query.exec(mySQL);
       return 0;
}


int  switchNum(char ch)//十六进制转换成十进制
{
    if(ch>='0'&&ch<='9')
        return ch-'0';
    else if(ch=='a'||ch=='A')
        return 10;
    else if(ch=='b'||ch=='B')
        return 11;
    else if(ch=='c'||ch=='C')
        return 12;
    else if(ch=='d'||ch=='D')
        return 13;
    else if(ch=='e'||ch=='E')
        return 14;
    else
        return 15;
}

void unlockPocket_InterestPackage(char buffer[SIZE],int datasize,int &value,int &DestinationID,int &DataFlag,int &PackageCount,int &NodeID,int &Noce)
{
   value = 1;
   DestinationID = 0;
   DataFlag = 0;
   PackageCount = 0;
   NodeID = 0;
   Noce = 0;
   if(datasize == 12)
   {
       for(int i=0;i<2;i++)//DesID
       {
          DestinationID = DestinationID*16+switchNum(buffer[2+i]);
       }
       for(int i=0;i<2;i++)//DataFlag
       {
          DataFlag = DataFlag*16+switchNum(buffer[4+i]);
       }
       for(int i=0;i<2;i++)//PackageCount
       {
          PackageCount = PackageCount*16+switchNum(buffer[6+i]);
       }
       for(int i=0;i<2;i++)//NodeID
       {
          NodeID = NodeID*16+switchNum(buffer[8+i]);
       }
       for(int i=0;i<2;i++)//Noce
       {
          Noce = Noce*16+switchNum(buffer[10+i]);
       }
   }

}

void unlockPocket_DataPackage(char buffer[SIZE],int datasize,int &value,int &DestinationID,int &DataFlag,int &PackageCount,int &PackageAll,int &PackageSeg,double &Data)
{

    if(datasize == 18)
    {
        value = 2;
        DestinationID = 0;
        DataFlag = 0;
        PackageCount = 0;
        PackageAll = 0;
        PackageSeg = 0;
        Data = 0;
        for(int i=0;i<2;i++)//DesID
        {
           DestinationID = DestinationID*16+switchNum(buffer[2+i]);
        }
        for(int i=0;i<2;i++)//DataFlag
        {
           DataFlag = DataFlag*16+switchNum(buffer[4+i]);
        }
        for(int i=0;i<2;i++)//PackageCount
        {
           PackageCount = PackageCount*16+switchNum(buffer[6+i]);
        }
        for(int i=0;i<2;i++)//PackageALL
        {
           PackageAll=PackageAll*16+switchNum(buffer[8+i]);
        }
        for(int i=0;i<2;i++)//PackageSeg
        {
           PackageSeg = PackageSeg*16+switchNum(buffer[10+i]);
        }
        int a = 0;
        for(int i=0;i<4;i++)//Data
        {
           a = a*16+switchNum(buffer[12+i]);
        }
        Data = 3.3*a/32384;
    }
}

