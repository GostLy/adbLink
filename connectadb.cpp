#include "connectadb.h"
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QTimer>
#include "logfile.h"

#ifdef Q_OS_WIN
#include <windows.h>
#elif defined(Q_OS_UNIX)
#include <unistd.h>
#include <signal.h>
#endif

Qt::HANDLE myHandle;

QString connectadb(const QString &cstring)
{
    logfile("Starting connection process");

    QProcess run_command;
    run_command.setProcessChannelMode(QProcess::MergedChannels);
    run_command.start(cstring);

    if (!run_command.waitForStarted())
    {
        logfile("Failed to start the process.");
        return QString();
    }

    logfile("Attempting to connect ...");

    bool forceTerminated = false;

    QTimer timer;
    timer.setSingleShot(true);
    timer.start(5000); // 5 seconds timeout

    QEventLoop loop;

    QObject::connect(&timer, &QTimer::timeout, [&]() {
#ifdef Q_OS_WIN
        // Get the process ID using QProcess::processId()
        DWORD processId = run_command.processId();
        HANDLE processHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, processId);
        ::TerminateProcess(processHandle, 0);
        ::CloseHandle(processHandle);

        logfile("Connection process terminated.");
#elif defined(Q_OS_UNIX)
            ::kill(run_command.processId(), SIGKILL);
            logfile("IP not responding.");
#endif
        forceTerminated = true;
        loop.quit();
    });

    QObject::connect(&run_command, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int, QProcess::ExitStatus) {
        if (!forceTerminated)
        {
            logfile("Connecting.");
        }
        loop.quit();
    });

    loop.exec();

    if (run_command.state() != QProcess::NotRunning) {
        run_command.terminate();
        run_command.waitForFinished();

    }

    QString command = run_command.readAll();

    logfile("Process output");

    return command;
}
