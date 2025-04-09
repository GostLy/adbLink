
#include "logfile.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
  #include <QDir>
#include <QtGlobal>

#ifdef Q_OS_WIN
const QString databasedir = QDir::homePath() + "/AppData/Roaming/.jocala/";
#elif defined(Q_OS_UNIX)
const QString databasedir = QDir::homePath() + "/.jocala/";
#endif

void logfile(const QString& line)
{
   QFile file(databasedir + "adblink.log");
   if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
   {
       QMessageBox::critical(nullptr, "", "Can't create logfile!\n", QMessageBox::Cancel);
       return;
   }

   QTextStream out(&file);
   out << line << endl;
   file.close();
}
