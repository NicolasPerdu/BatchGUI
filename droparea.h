#ifndef DROPAREA_H
#define DROPAREA_H

#include "process.h"
#include "program.h"
#include <QTableWidget>
#include <QProcess>

class MainWindow;
class QMimeData;

class DropArea : public QTableWidget {
    Q_OBJECT

public:
    DropArea(QWidget *parent = 0);

    ~DropArea() {
        for(int i = 0; i < progs.size(); i++)
            delete(progs[i]);

        progs.clear();

        for(int i = 0; i < procs.size(); i++)
            delete(procs[i]);

        procs.clear();
    }

    void addProgram(Program* prog) {
        progs.push_back(prog);
    }

    void startProg(Program* p, QString file, QString path);

public slots:
    void clear();
    void launch();

    void error(QProcess::ProcessError error);
    void started();
    void finished(int, QProcess::ExitStatus);

signals:
    void changed(const QMimeData *mimeData = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
    MainWindow *parent;
    QTableWidget *table;
    QStringList listFile;

    QVector<Program*> progs;
    QVector<Process*> procs;
};

#endif // DROPAREA_H
