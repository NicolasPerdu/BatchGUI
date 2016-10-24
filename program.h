
#ifndef PROGRAM_H
#define PROGRAM_H

#include <QString>
#include <QDebug>
#include <QProcess>

class Program {
public:
    Program(QString out, QString exe, QString arg) {
        this->out = out;
        this->exe = exe;
        this->arg = arg;
        this->next = NULL;

        //qWarning() << "program : " << out << ", " << exe << ", " << arg << ", " << endl;
    }

    ~Program() {
        if(next != NULL)
            delete(next);
    }

    void printPid() {
        qWarning() << pids << endl;
    }

    bool pidExist(qint64 id) {
        return pids.indexOf(id) != -1;
    }

    void addPid(qint64 id) {
        pids.push_back(id);
    }

    void deletePid(qint64 id) {
        if(pids.indexOf(id) != -1) {
            pids.erase(pids.begin() + pids.indexOf(id));
        }
    }

    Program* getProgram() {
        return next;
    }

    QString getOut() {
        return out;
    }

    QString getExe() {
        return exe;
    }

    QString getArg() {
        return arg;
    }

    void setProg(Program *prog) {
        this->next = prog;
    }

private:
    QString out, exe, arg;
    Program* next;
    QVector<qint64> pids;
};

#endif // PROGRAM_H
