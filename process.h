#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>

class Process {
public:
    Process(QProcess *proc, QString outputFile, QString path) {
        this->proc = proc;
        this->outputFile = outputFile;
        this->path = path;
    }

    ~Process() {
        if(proc != NULL) {
            proc->kill();
            proc->deleteLater();
        }
    }

    QProcess* getProc() {
        return proc;
    }

    QString getOutputFile() {
        return outputFile;
    }

    QString getPath() {
        return path;
    }

private:
    QProcess *proc;
    QString outputFile, path;
};

#endif // PROCESS_H
