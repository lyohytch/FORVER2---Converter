#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<" Testing regexp\n";
    QFile filein("example");
    if(!filein.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning()<<" Error opening file";
        return -1;
    }

    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(codec);

    QString text = filein.readAll();
    qDebug() << "Can encode : "<<codec->canEncode(text);// False if cracozyabry
    qDebug()<< " Text => "<< text;
    QStringList textSplitted = text.split(QRegExp("\\n"));
    QStringList capturedText;
    //QStringList list1, list2, list3;
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]+)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)");

    foreach(QString textLine, textSplitted)
    {
        if (search.indexIn(textLine) != -1)
        {
            capturedText = search.capturedTexts();
            qDebug()<<" Captured => "<<capturedText;
        }
        //if( !capturedText.isEmpty() && !(capturedText.at(0)).isEmpty())
        //{
         //   qDebug()<<"Captured.cap(1) "<<capturedText.at(1)<<" Captured.cap(3) => "<< capturedText.at(3) <<" Captured.cap(5) => "<<capturedText.at(5);
        //}
    }
/*
    int pos = 0;
    while ( (pos = search.indexIn(text, pos)) != -1)
    {
        list1 << search.cap(1);
        list2 << search.cap(2);
        list3 << search.cap(3);
        pos += search.matchedLength();
    }
    qDebug()<<" List1 => "<<list1;
    qDebug()<<" List2 => "<<list2;
    qDebug()<<" List3 => "<<list3;
*/
   // qDebug()<<" textSplitted => "<< textSplitted;

    return a.exec();
}
