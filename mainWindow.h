#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QMdiSubWindow;
class QMdiArea;
class QCloseEvent;
class Piano;
class PianoSynth;
class Switcher;
class FramedSolfege;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget * parent = 0);
    FramedSolfege * getFramedSolfege() const { return s; }
    void solfegeHeightResize(int newHeight);
    void solfegeKeyWidthResize(int newWidth);
    void setSolfegeWindowMaxWidth();
    int solfegeKeyWidth, solfegeInternalHeight;

private slots:
    void showHelp();
    void showAbout();

private:
    void initializeSwitcher();
    void initializePiano();
    void initializeSolfege();
    void createMenus();
    void readSettings();
    void writeSettings();
    QMdiSubWindow * solfegeWindow, * pianoWindow;
    Piano * p;
    FramedSolfege * s;
    QMdiArea * area;
    PianoSynth * syn;
    Switcher * swi;
    QMenu * fileMenu, * helpMenu;
    QAction * exitAction, * helpAction, * aboutAction;
    int pianoX, pianoY, pianoWidth, pianoHeight, solfegeX, solfegeY,
        solfegeExternalWidth, solfegeExternalHeight, solfegeScroll,
        transpose;
    int solfegeTimer, solfegeWindowWidthThickness;

protected:
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *); //completes solfege initialization

};

#endif // MAINWINDOW_H
