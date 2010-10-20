#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QDebug>

//Сюда добавлять все константы
//Common
const QString dot(".");
const QString comma(",");
const QString space(" ");
const QString rBracket(")");
const QString lBracket("(");
const QString quote2("\"");
const QString quote("\'");
//Sql
const QString createTbl("CREATE TABLE");
const QString insertTbl("INSERT INTO");
const QString valuesTbl("VALUES");
const QString dropTbl("DROP TABLE");
const QString nameTbl("DemoTblName");
const QString updateTbl("UPDATE");
const QString setTbl("SET");
const QString whereTbl("WHERE");
const QString alterTbl("ALTER TABLE");
const QString addTbl("ADD");
const QString emptyCell("EMPTY");


//Sql field types
const QString VCHAR("VCHAR");
const QString VCHARMAX("VARCHAR(500)");
const QString INTEGER("INT");


//Description file constants
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

//Data model fields
const QString generic("generic_");
const QString figurant("figurant_");
const QString locus("locus_delicti_");
const QString weapon("weapon_");

//Correlation
const QString number("number");
const QString function("function");
//target
const QString TargetSign("TargetSign");
const QString TargetData("TargetData");
const QString TargetDesc("TargetDesc");
//template
const QString TemplateSign("TemplateSign");
const QString TemplateData("TemplateData");
const QString TemplateDesc("TemplateDesc");
//table record
const QString TableRecord("TableRecord");
//table lists
const QString tempList("tempList");
const QString targList("targList");
const QString funcList("funcList");
const QString cmf(".cmf");

const int iTemplate = 0;
const int iTarget = 1;
const int iFunction = 2;

//Type function
const int agef = 0;
const int concatf = 1;
const int nonef = 2;

//Describing definitions
const int TARGETDESC = 0;
const int TEMPLATEDESC = 1;

//Predefined file paths
#ifdef Q_OS_LINUX
const QString demopath("Resources/Description/Demo/sprav_d.txt");
const QString target1path("Resources/Description/4Views/Sprav1.txt");
const QString target2path("Resources/Description/4Views/Sprav2.txt");
const QString target3path("Resources/Description/4Views/Sprav3.txt");
const QString target4path("Resources/Description/4Views/Sprav4.txt");
const QString prosPathF1("Resources/Description/PROS/F1.TXT");
const QString prosPathF2("Resources/Description/PROS/F2.TXT");
const QString prosPathF5("Resources/Description/PROS/F5.TXT");
const QString prosPathF12("Resources/Description/PROS/F12.TXT");
#elif defined Q_OS_WIN32 && !defined DEBUGING
const QString demopath("Resources\\Description\\sprav_d.txt");
const QString target1path("Resources\\Description\\Sprav1.txt");
const QString target2path("Resources\\Description\\Sprav2.txt");
const QString target3path("Resources\\Description\\Sprav3.txt");
const QString target4path("Resources\\Description\\Sprav4.txt");
#else
const QString demopath("..\\Resources\\Description\\Demo\\sprav_d.txt");
const QString target1path("..\\Resources\\Description\\4Views\\Sprav1.txt");
const QString target2path("..\\Resources\\Description\\4Views\\Sprav2.txt");
const QString target3path("..\\Resources\\Description\\4Views\\Sprav3.txt");
const QString target4path("..\\Resources\\Description\\4Views\\Sprav4.txt");
const QString prosPathF1("..\\Resources\\Description\\PROS\F1.TXT");
const QString prosPathF2("..\\Resources\\Description\\PROS\F2.TXT");
const QString prosPathF5("..\\Resources\\Description\\PROS\F5.TXT");
const QString prosPathF12("..\\Resources\\Description\\PROS\F12.TXT");
#endif

//Helpful qDebug defines
#define qDebug()            qDebug()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qWarning()          qWarning()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qCritical()         qCritical()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="
#define qFatal()            qFatal()<<__FILE__<<"("<<__LINE__<<")"<<"=="<<__FUNCTION__<<"=="

#endif // CONSTANTS_H
