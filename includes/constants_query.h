#ifndef CONSTANTS_QUERY_H
#define CONSTANTS_QUERY_H
#include <QString>

//Sql field types
const QString VCHAR("VCHAR");//to query constant
const QString VCHARMAX("VARCHAR(500)");//to query constant
const QString INTEGER("INT");//to query constant

//Sql
//DB names and connection string
const QString masterDB("master"); //to query_constants.h
const QString criminalDB("CriminalDB"); //to config
const QString driver("SQL Native Client"); // to query constant
const QString server("localhost\\SQLEXPRESS");//to config
const QString saUID("sa"); //to config
const QString saPWD("NNSU");//to config

const QString connectString("DRIVER={%1};SERVER=%2;DATABASE=%3;UID=%4;PWD=%5;");//to query constant
const QString createDBReq(QString("CREATE DATABASE %1").arg(criminalDB));//to query constant
//Requests
const QString createTbl("CREATE TABLE");//to query constant
const QString insertTbl("INSERT INTO");//to query constant
const QString valuesTbl("VALUES");//to query constant
const QString dropTbl("DROP TABLE");//to query constant
//Demo tbl name
const QString demoNameTbl("DemoTblName");//to query constant
//Table names
const QString genericNameTbl("generic");//to query constant
const QString figurantNameTbl("figurant");//to query constant
const QString locusNameTbl("locus_delicti");//to query constant
const QString weaponNameTbl("weapon");

const QStringList tblNames(QStringList()<<genericNameTbl<<figurantNameTbl<<locusNameTbl<<weaponNameTbl);//to query constant



const QString updateTbl("UPDATE");// to query constant
const QString setTbl("SET");// to query constant
const QString whereTbl("WHERE");// to query constant
const QString alterTbl("ALTER TABLE");// to query constant
const QString addTbl("ADD");// to query constant

#endif // CONSTANTS_QUERY_H
