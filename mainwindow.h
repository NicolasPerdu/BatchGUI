#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QDialogButtonBox;
class QMimeData;
class QPushButton;
class QTableWidget;
class DropArea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void clearConfigLabel();
    void setLaunch(bool value);
    ~MainWindow();

public slots:
    //void updateFormatsTable(const QMimeData *mimeData);

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
    void open();
    void about();

private:
    //Menu
    void createActions();
    void createMenus();

   QMenu *fileMenu;
   QMenu *helpMenu;
   QLabel *configLabel;
   QAction *openAct;
   QAction *aboutAct;

   //Body
   DropArea *dropArea;
   QString configFile;

   QPushButton *launchButton;
   QPushButton *clearButton;
   QPushButton *quitButton;
   QDialogButtonBox *buttonBox;
};

#endif // MAINWINDOW_H
