#ifndef CONSTANTS_PRESENTER_H
#define CONSTANTS_PRESENTER_H
#include <QString>
#include <QDir>
//Translations
const QString English("EnglishSetting");
const QString Russian("RussianSetting");
const QString applicationIni("converter.ini");
const QString TranslateSettings("Translate");

const QString modelQmPath(":translations/model/Resources/Translations/converter_model_ru.qm");
const QString presenterQmPath(":translations/presenter/Resources/Translations/converter_presenter_ru.qm");
const QString UiQmPath(":translations/ui/Resources/Translations/converter_ui_ru.qm");

const QString actionRussian("actionRussian");
const QString actionEnglish("actionEnglish");

//Describing definitions
const int TARGETDESC = 0;
const int TEMPLATEDESC = 1;


//Type function
const int agef = 0;
const int concatf = 1;
const int nonef = 2;

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

const QString cmf(".cmf");

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
const QString demopath("..\\..\\Resources\\Description\\Demo\\sprav_d.txt");
const QString target1path("..\\Resources\\Description\\4Views\\Sprav1.txt");
const QString target2path("..\\..\\Resources\\Description\\4Views\\Sprav2.txt");
const QString target3path("..\\..\\Resources\\Description\\4Views\\Sprav3.txt");
const QString target4path("..\\..\\Resources\\Description\\4Views\\Sprav4.txt");
const QString prosPathF1("..\\..\\Resources\\Description\\PROS\\F1.TXT");
const QString prosPathF2("..\\..\\Resources\\Description\\PROS\\F2.TXT");
const QString prosPathF5("..\\..\\Resources\\Description\\PROS\\F5.TXT");
const QString prosPathF12("..\\..\\Resources\\Description\\PROS\\F12.TXT");
const QString juraPath1("..\\..\\Resources\\Description\\Jura\\Pros.txt");
const QString juraPathExcel(QDir::currentPath() + "\\..\\..\\Resources\\Description\\Jura\\F_5_4.xls");
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
const QString juraPathExcel(QDir::currentPath() + "\\..\\..\\Resources\\Description\\Jura\\F_5_4.xls");
#endif

#endif // CONSTANTS_PRESENTER_H
