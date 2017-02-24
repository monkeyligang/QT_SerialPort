#include<QString>
#ifndef DBSERVER_H
#define DBSERVER_H
#define SIZE 1024

void unlockPocket_InterestPackage(char buffer[SIZE],int datasize,int &value,int &DestinationID,int &DataFlag,int &PackageCount,int &NodeID,int &Noce);
int  switchNum(char ch);//十六进制转换成十进制
void unlockPocket_DataPackage(char buffer[SIZE],int datasize,int &value,int &DestinationID,int &DataFlag,int &PackageCount,int &PackageAll,int &PackageSeg,double &Data);

class dbserver
{
public:
    dbserver();
    ~dbserver();
     bool connectToMySQL();//创建连接
     int insertMySql_interest(QString time,int value,int DestinationID,int DataFlag,int PackageCount,int NodeID,int Noce);
     int insertMySql_datapackage(QString time,int value,int DestinationID,int DataFlag,int PackageCount,int PackageAll,int PackageSeg,double Data);
};

#endif // DBSERVER_H
