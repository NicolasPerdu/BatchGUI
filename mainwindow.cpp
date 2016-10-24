#include <QtWidgets>
#include <QtDebug>

#include "droparea.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
   QWidget *widget = new QWidget;

   configLabel = NULL;

   dropArea = new DropArea(this);

   configLabel = new QLabel("<b>Config file : </b>" + configFile);
   configLabel->setTextFormat(Qt::RichText);
   configLabel->setWordWrap(true);
   configLabel->adjustSize();

   launchButton = new QPushButton(tr("Launch"));
   clearButton = new QPushButton(tr("Clear"));
  // quitButton = new QPushButton(tr("Quit"));

   buttonBox = new QDialogButtonBox;
   buttonBox->addButton(launchButton, QDialogButtonBox::ActionRole);

   buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
   //buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

   //connect(quitButton, &QAbstractButton::pressed, this, &exit);
   connect(clearButton, &QAbstractButton::pressed, dropArea, &DropArea::clear);
   connect(launchButton, &QAbstractButton::pressed, dropArea, &DropArea::launch);

   QVBoxLayout *mainLayout = new QVBoxLayout(widget);

   mainLayout->addWidget(configLabel);
   mainLayout->addWidget(dropArea);
   mainLayout->addWidget(buttonBox);

  // widget->setLayout(mainLayout);

   createActions();
   createMenus();

   setLaunch(false);
   setWindowTitle(tr("BatchGUI"));
   resize(480, 480);
   setCentralWidget(widget);
}

void MainWindow::clearConfigLabel() {
    configFile = "";

    if(configLabel != NULL) {
        configLabel->setText("<b>Config file : </b>" + configFile);
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.exec(event->globalPos());
}

void MainWindow::setLaunch(bool value){
     launchButton->setEnabled(value);
}

void MainWindow::open() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("XML Files (*.xml)"));

    if(filename == "")
        return;

    QFile file(filename);
    bool errorFound = false;
    //qWarning() << "Open" << endl;

    if(file.open(QIODevice::ReadOnly)) {
        qWarning() << "Reading the file" << endl;
        QXmlStreamReader xml(&file);

        configFile = file.fileName();
        configLabel->setText("<b>Config file : </b> " + configFile);

        xml.readNextStartElement(); // Go to the root element
        QString name = xml.name().toString();

        if (name == "root") { // Find the root
            while (xml.readNextStartElement()) {
                 QString name2 = xml.name().toString();

                 if(name2 == "prog") {
                     QString out, exe, arg;

                     QXmlStreamAttributes attr = xml.attributes();

                     if(attr.hasAttribute("out")) {
                         out = attr.value("out").toString();
                     }

                     if(attr.hasAttribute("exe")) {
                         exe = attr.value("exe").toString();
                     }

                     if(attr.hasAttribute("arg")) {
                         arg = attr.value("arg").toString();
                     }

                     qWarning() << "parent :" << out << endl;

                     // Base program
                     Program *prog = NULL;

                     if(exe != "") {
                         prog = new Program(out, exe, arg);
                     }

                     int i = 1;

                     while(xml.readNextStartElement()) {
                           QString out2, exe2, arg2;
                           QString name3 = xml.name().toString();

                           if(name3 == "prog") {
                               QXmlStreamAttributes attr2 = xml.attributes();

                               if(attr2.hasAttribute("out")) {
                                   out2 = attr2.value("out").toString();
                               }

                               if(attr2.hasAttribute("exe")) {
                                   exe2 = attr2.value("exe").toString();
                               }

                               if(attr2.hasAttribute("arg")) {
                                   arg2 = attr2.value("arg").toString();
                               }

                               //qWarning() << "child :" << out2 << endl;

                               if(exe2 != "") {
                                   Program* lastProg = prog;

                                   while(lastProg->getProgram() != NULL) {
                                       lastProg = lastProg->getProgram();
                                   }

                                   qWarning() << "set next program " << i << " : " << out2 << ", " << exe2 << ", " << arg2 << endl;
                                   lastProg->setProg(new Program(out2, exe2, arg2));
                                   i++;
                               }
                           }
                     }

                     dropArea->addProgram(prog);
                 }
            }
        }
    } else {
        errorFound = true;
        QMessageBox::warning(this, tr("BatchGUI"),tr("An error has occured when parsing the config file."));
        //qWarning() << "Error reading file" << endl;
    }
    qWarning() << "end of the parsing XML" << endl;

    setLaunch(!errorFound);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About BatchGUI"),
            tr("<div align=\"center\">The application <b>BatchGUI</b> is created by Glusoft.<br/>"
               "<a href=\"http://glusoft.com/tuto/BatchGUI.php\">Official page</a></div>"));
}


void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);


    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

MainWindow::~MainWindow() {
}
