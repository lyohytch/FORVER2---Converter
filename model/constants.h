#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>
#include <QDebug>

//Common
const QString dot(".");
const QString comma(",");
const QString space(" ");
const QString rBracket(")");
const QString lBracket("(");
const QString quote2("\"");
const QString quote("\'");
const QString underline("_");


const QChar zero('0');
const QChar one('1');

const QChar X_Eng('X');
const QChar k_eng('k');
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
const QString id("id");//�������������
const QString name("name");//���
const QString type("type");//���
const QString level("level");//�������
const QString dvalue("value");//������
const QString error("error");//������
const QString numb("number");//����� �� �������
const QString repeat("repeat");//����������� �������
const QString rapid("rapid");
const QString prosDesc("prosDesc");//��������������� ��������� ��� ��������� �������� �� �����������
const QString formId("formId");//������������ ����� �������� � ��������������(��)
const QString targetName("targetName");//������������ ����� �������� � ��������������(���)
/**
  List of QMap<QString, QString> where each element is
  0001 <-> compactxx45x - for compact data
  0001 <-> code12345 - for simple encoded data
  0001 <-> "" (Empty string) for entire value
  */
const QString compactCode("compact");
const QString simpleCode("code");
const QString correlationValue("correlationValue");

const QString targetDataForConvert("dataforconvert");//�������������� ������ ��� �������������� QList<QMap<QString, QString> >   0001 <-> xx45x
const QString dependFields("dependFields"); //List of depend fields
const QString dependId("dependingId");

const QString codingRules("codingRules"); // Encoding rules as described above


//Data model fields
const QString generic("generic_");
const QString figurant("figurant_");
const QString locus("locus_delicti_");
const QString weapon("weapon_");
const QString generic_F5("generic_F5_");

const QString F5_names("F5_");

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

//Items in additional correlation table
const QString itemNamesInForm("itemNames");
const QString corrValuesInForm("corrValues");
const QString codesInForm("codeValues");
const QString typeNameInForm("typeName");
const QString dependIdsInForm("dependIds");
const QString templateNameInForm("templateName");

//Translations
const QString English("EnglishSetting");
const QString Russian("RussianSetting");
const QString ApplicationName("Converter");
const QString TranslateSettings("Translate");

const QString modelQmPath(":translations/model/Resources/Translations/converter_model_ru.qm");
const QString presenterQmPath(":translations/presenter/Resources/Translations/converter_presenter_ru.qm");
const QString UiQmPath(":translations/ui/Resources/Translations/converter_ui_ru.qm");

const QString actionRussian("actionRussian");
const QString actionEnglish("actionEnglish");

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
const QString demopath("../../Resources/Description/Demo/sprav_d.txt");
const QString target1path("../../Resources/Description/4Views/Sprav1.txt");
const QString target2path("../../Resources/Description/4Views/Sprav2.txt");
const QString target3path("../../Resources/Description/4Views/Sprav3.txt");
const QString target4path("../../Resources/Description/4Views/Sprav4.txt");
const QString prosPathF1("../../Resources/Description/PROS/F1.TXT");
const QString prosPathF2("../../Resources/Description/PROS/F2.TXT");
const QString prosPathF5("../../Resources/Description/PROS/F5.TXT");
const QString prosPathF12("../../Resources/Description/PROS/F12.TXT");
const QString juraPath1("../../Resources/Description/Jura/Pros.txt");
#elif defined Q_OS_WIN32 && !defined DEBUGING
const QString demopath("..\\Resources\\Description\\Demo\\sprav_d.txt");
const QString target1path("..\\Resources\\Description\\4Views\\Sprav1.txt");
const QString target2path("..\\Resources\\Description\\4Views\\Sprav2.txt");
const QString target3path("..\\Resources\\Description\\4Views\\Sprav3.txt");
const QString target4path("..\\Resources\\Description\\4Views\\Sprav4.txt");
const QString prosPathF1("..\\Resources\\Description\\PROS\\F1.TXT");
const QString prosPathF2("..\\Resources\\Description\\PROS\\F2.TXT");
const QString prosPathF5("..\\Resources\\Description\\PROS\\F5.TXT");
const QString prosPathF12("..\\Resources\\Description\\PROS\\F12.TXT");
const QString juraPath1("..\\Resources\\Description\\Jura\\Pros.txt");
#else
const QString demopath("..\\..\\Resources\\Description\\Demo\\sprav_d.txt");
const QString target1path("..\\..\\Resources\\Description\\4Views\\Sprav1.txt");
const QString target2path("..\\..\\Resources\\Description\\4Views\\Sprav2.txt");
const QString target3path("..\\..\\Resources\\Description\\4Views\\Sprav3.txt");
const QString target4path("..\\..\\Resources\\Description\\4Views\\Sprav4.txt");
const QString prosPathF1("..\\..\\Resources\\Description\\PROS\\F1.TXT");
const QString prosPathF2("..\\..\\Resources\\Description\\PROS\\F2.TXT");
const QString prosPathF5("..\\..\\Resources\\Description\\PROS\\F5.TXT");
const QString prosPathF12("..\\..\\Resources\\Description\\PROS\\F12.TXT");
const QString juraPath1("..\\..\\Resources\\Description\\Jura\\Pros.txt");
#endif

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
