#include "droparea.h"

#include "mainwindow.h"
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QProcess>

DropArea::DropArea(QWidget *parent) : QTableWidget(parent) {
    this->parent = (MainWindow*) parent;

    QStringList labels;
    labels << tr("Files");

    setColumnCount(1);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalHeaderLabels(labels);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void DropArea::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasImage()) {
    }
    else if (mimeData->hasHtml()) {
    } else if (mimeData->hasText()) {
        foreach (QString format, mimeData->formats()) {
            QTableWidgetItem *formatItem = new QTableWidgetItem(format);
            formatItem->setFlags(Qt::ItemIsEnabled);
            formatItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);

            QString text;
            if (format == QLatin1String("text/plain")) {
                text = mimeData->text().simplified();
            } else if (format == QLatin1String("text/html")) {
                text = mimeData->html().simplified();
            } else if (format == QLatin1String("text/uri-list")) {
                QList<QUrl> urlList = mimeData->urls();

                for (int i = 0; i < urlList.size() && i < 32; ++i)
                    text.append(urlList.at(i).toString() + QLatin1Char(' '));


                QStringList lst = text.split("file:///");

                for(int i = 0; i < lst.size(); i++) {
                    if(lst[i] != "") {
                        int row = rowCount();
                        insertRow(row);
                        setItem(row, 0, new QTableWidgetItem(lst[i].split("/").last()));
                        setItem(row, 1, new QTableWidgetItem(lst[i]));
                        listFile.push_back(lst[i]);
                    }
                }

            } else {
                QByteArray data = mimeData->data(format);
                for (int i = 0; i < data.size() && i < 32; ++i)
                    text.append(QStringLiteral("%1 ").arg(uchar(data[i]), 2, 16, QLatin1Char('0')).toUpper());
            }
        }
    } else if (mimeData->hasUrls()) {

    }
    else {
    }

    resizeColumnsToContents();
    resizeRowsToContents();

    event->acceptProposedAction();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event) {
    clear();
    event->accept();
}

void DropArea::clear() {
    // Clear files added
    setRowCount(0);
    listFile.clear();

    // Delete the programs
    for(int i = 0; i < progs.size(); i++)
        delete(progs[i]);

    progs.clear();

    // Clear label
    parent->clearConfigLabel();

    // Kill every QProcess
    for(int i = 0; i < procs.size(); i++) {
        delete(procs[i]);
        procs[i] = NULL;
    }

    procs.clear();

    emit changed();
}

void DropArea::error(QProcess::ProcessError error) {
    qWarning() << "error launch : " << error << endl;
}

void DropArea::started() {
    //qWarning() << "The QProcess started !!\n";
}

void DropArea::finished(int exitCode, QProcess::ExitStatus exitStatus) {
    // find the program finished
    QProcess *procFinished = (QProcess*) sender();
    QVariant var = procFinished->property("PID");

    qWarning() << "The program " << var.toLongLong() << " is finished : " << exitCode << ", " << exitStatus << endl;

    //QString output(procFinished->readAllStandardOutput());
    //qWarning() << output << endl;

    for(int j = 0; j < progs.size(); j++) {
        for(int i = 0; i < procs.size(); i++) {
            // find the pid of the process
            Program* itProg = progs[j];

            while(itProg != NULL) {
                if(itProg->pidExist((qint64)(var.toLongLong()))) {
                    itProg->deletePid((qint64)(var.toLongLong()));

                    if(itProg->getProgram() != NULL) {
                        //Start the next program
                        startProg(itProg->getProgram(), procs[i]->getOutputFile(), procs[i]->getPath());
                    }
                }
                itProg = itProg->getProgram();
            }
        }
    }

    // Delete the process finished
    Process *procFound = NULL;
    int iFound = -1;

    for(int i = 0; i < procs.size(); i++) {
      if(procs[i]->getProc()->property("PID") == var) {
          procFound = procs[i];
          iFound = i;
          break;
      }
    }

    if(procFound != NULL) {
        procs.erase(procs.begin() + iFound);
        delete(procFound);
    }
}

void DropArea::startProg(Program *p, QString file, QString path) {
    QString program = p->getExe();
    file.replace(" ", "");

    QString copyArg(p->getArg());

    //Find the output file
    QStringList args = p->getOut().split("/");
    QString outputFile;

    for(int i = 0; i < args.size(); i++) {
        if(args[i].contains(".png")) {
            outputFile = args[i];
        }
    }

    outputFile.replace(" ", "");

    QStringList name = file.split('.');

    if(outputFile.contains("$FILE$")) {
        outputFile.replace("$FILE$", name[0]);
    }

    QString outputPath = p->getOut();
    if(outputPath.contains("$FILE$")) {
        outputPath.replace("$FILE$", name[0]);
    }

    //copyArg.replace("$FILE$", name[0]);
    copyArg.replace("$IN$", path + file);
    copyArg.replace("$OUT$", outputPath);
    copyArg.replace("$REPEAT$", path + file);

    QStringList arg;

    if(copyArg.length() > 2) {
      arg = copyArg.split(" ");
    }

    //arg << path + file;

    procs.push_back(new Process(new QProcess(parent), outputFile, path));

    connect(procs.back()->getProc(), &QProcess::started, this, &DropArea::started);
    connect(procs.back()->getProc(), SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
    connect(procs.back()->getProc(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));

    procs.back()->getProc()->start(program, arg);
    procs.back()->getProc()->waitForStarted(-1);

    qint64 pid = procs.back()->getProc()->processId();
    p->addPid(pid); // add pid to the program
    procs.back()->getProc()->setProperty("PID", QVariant((qlonglong)pid));

    qWarning() << "start program : " << program << arg << "(" << procs.back()->getProc()->property("PID").toLongLong() << ")" << endl;
}

void DropArea::launch() {
    for(int j = 0; j < progs.size(); j++) {
        if(progs.size() > 0 && progs[j]->getExe() != "" && progs[j]->getOut() != "" && progs[j]->getArg() != "" && listFile.size() > 0) {
            for(int i = 0; i < listFile.size(); i++) {
                QString name = listFile[i].split('/').back();
                startProg(progs[j], name, listFile[i].mid(0, listFile[i].length() - name.length()));
            }
        } else {
            if(progs.size() == 0) {
                QMessageBox::warning(this, tr("BatchGUI"), tr("You need to select a config file before launching!"));
            }
            else if (listFile.size() == 0) {
                QMessageBox::warning(this, tr("BatchGUI"), tr("You need to add files to process!"));
            }
            else {
                QMessageBox::warning(this, tr("BatchGUI"), tr("One of the program in the config file is incomplete<br/>"
                                                               "The attributes exe, out, and arg must not be empty"));
            }
        }
    }
}
