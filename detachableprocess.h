#ifndef DetachableProcess_H
#define DetachableProcess_H


class DetachableProcess : public QProcess
{
public:
    explicit DetachableProcess(QObject *parent = nullptr) : QProcess(parent)
    {
    }

    bool detach()
    {
        if (!waitForStarted())
        {
            return false;
        }
        setProcessState(QProcess::NotRunning);
        return true;
    }
};

#endif // DetachableProcess_H