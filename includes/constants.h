#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QStringList>
#include <QDebug>

//TODO: move more settings to config file
//Common:punctuation marks
const QString dot(".");
const QString comma(",");
const QString space(" ");
const QString rBracket(")");
const QString lBracket("(");
const QString quote2("\"");
const QString quote("\'");
const QString underline("_");

//Description file:type of fields
const QString ME("me");
const QString AV("av");
const QString DV("dv");
const QString ED("ed");
const QString ET("et");
const QString RM("rm");
const QString CB("cb");

//Describing model fields
const QString id("id");//Идентификатор
const QString name("name");//Имя
const QString type("type");//Тип
const QString level("level");//Уровень
const QString dvalue("value");//данные
const QString error("error");//Ошибка
const QString numb("number");//Номер по порядку
const QString repeat("repeat");//дублируюший элемент
const QString rapid("rapid");
const QString prosDesc("prosDesc");//вспомогательный контейнер для обработки описаний от прокуратуры
const QString formId("formId");//соответствие между исходным и конвертируемым(ид)
const QString targetName("targetName");//соответствие между исходным и конвертируемым(имя)
/*
  List of QMap<QString, QString> where each element is
  0001 <-> compactxx45x - for compact data
  0001 <-> code12345 - for simple encoded data
  0001 <-> "" (Empty string) for entire value
  */
const QString compactCode("compact");
const QString simpleCode("code");
const QString correlationValue("correlationValue");
const QString targetDataForConvert("dataforconvert");//дополнительные данные для раскодирования QList<QMap<QString, QString> >   0001 <-> xx45x
const QString dependFields("dependFields"); //List of depend fields
const QString dependId("dependingId");
const QString hint("whatisthis");

//Additions to names:prosure files
const QString F5_names("F5_");

//Items in additional correlation table
const QString itemNamesInForm("itemNames");
const QString corrValuesInForm("corrValues");
const QString codesInForm("codeValues");
const QString typeNameInForm("typeName");
const QString dependIdsInForm("dependIds");
const QString templateNameInForm("templateName");
const QString dependListNames("dependListNames");

const int iTemplate = 0;
const int iTarget = 1;
const int iFunction = 2;

//Excel extensions
const QStringList excelExts(QStringList() << "xls" << "xlsx");

#ifdef DEBUGING
//Helpful qDebug defines
#define qDebug()            qDebug()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qWarning()          qWarning()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qCritical()         qCritical()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qFatal()            qFatal()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#endif

#define CORR_MODEL ((CorrelationModel *)(_view->corrModel))
#define QUERY_MODEL ((querymodel *)(_view->queryModel))
#define MSSQL_QUERY ((mssqlquery *)(_view->mssqlQuery))
#define MODELS(i) ((QModelDescribing *)(_view->models[i]))
#define TREES(i) ((TreeViewModel *)(_view->trees[i]))

#endif // CONSTANTS_H
