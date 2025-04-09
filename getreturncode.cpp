#include "getreturncode.h"

#include "logfile.h"
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>

bool getreturncode(const QString &cstring)
{





  QProcess run_command;
  run_command.setProcessChannelMode(QProcess::MergedChannels);
  run_command.start(cstring);

  run_command.waitForStarted();


  while(run_command.state() != QProcess::NotRunning)
      qApp->processEvents();

  QString command=run_command.readAll();


 bool success = (run_command.exitCode() == 0);

// if (!success)
//    { logfile("ERROR: "+cstring);
//      logfile("ERROR: "+command); }

  return success;

}
