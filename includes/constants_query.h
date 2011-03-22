#ifndef CONSTANTS_QUERY_H
#define CONSTANTS_QUERY_H
#include <QString>

//Sql field types
const QString VCHAR("VCHAR");
const QString VCHARMAX("VARCHAR(500)");
const QString INTEGER("INT");

//Sql
//DB names and connection string
const QString masterDB("master");
const QString criminalDB("CriminalDB"); //to config
const QString driver("SQL Native Client");
const QString server("localhost\\SQLEXPRESS");//to config
const QString saUID("sa"); //to config
const QString saPWD("NNSU");//to config

const QString sqlServerSettings("SQL_Server_Settings");
const QString srvName("Server");
const QString dtbsName("Database");
const QString userName("User");
const QString pwdUser("Password");
const QString dbpath("Path");

const QString connectString("DRIVER={%1};SERVER=%2;DATABASE=%3;UID=%4;PWD=%5;");
const QString createDBReqSimple("CREATE DATABASE %1");
const QString createDBReqAdvanced("CREATE DATABASE %1 ON PRIMARY (NAME=%1, FILENAME='%2/%1.mdf', SIZE=20MB, MAXSIZE=100MB, FILEGROWTH=20%) LOG ON (NAME=%1_log, FILENAME='%2/%1_log.ldf', SIZE=10MB, MAXSIZE=50MB, FILEGROWTH=20%)");
//Requests
const QString createTbl("CREATE TABLE");
const QString insertTbl("INSERT INTO");
const QString valuesTbl("VALUES");
const QString dropTbl("DROP TABLE");
//Demo tbl name
const QString demoNameTbl("DemoTblName");
//Table names
const QString genericNameTbl("generic");
const QString figurantNameTbl("figurant");
const QString locusNameTbl("locus_delicti");
const QString weaponNameTbl("weapon");

const QStringList tblNames(QStringList()<<genericNameTbl<<figurantNameTbl<<locusNameTbl<<weaponNameTbl);//to query constant



const QString updateTbl("UPDATE");// to query constant
const QString setTbl("SET");// to query constant
const QString whereTbl("WHERE");// to query constant
const QString alterTbl("ALTER TABLE");// to query constant
const QString addTbl("ADD");// to query constant

#endif // CONSTANTS_QUERY_H
