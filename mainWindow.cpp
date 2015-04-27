#include "mainWindow.h"
#include "piano.h"
#include "solfege.h"
#include "pianoSynth.h"
#include "switcher.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QFile>
#include <QDataStream>
#include "framedSolfege.h"
#include <QScrollBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>


MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent)
{

    setWindowTitle("Relative Keys");
    //setWindowIcon(QIcon(":/img/relativeKeys.ico"));
    createMenus();

    readSettings();

    area = new QMdiArea(this);
    syn = new PianoSynth;

    setCentralWidget(area);

    initializeSwitcher();
    initializePiano();
    initializeSolfege();

    swi->setSolfege(s); //enables swi to set the offset on s
    p->setInstrument(s->getKeyboard());


}

void MainWindow::solfegeHeightResize(int newHeight)
{
    int oldHeight = s->getKeyboard()->height();
    int difference = newHeight - oldHeight;

    s->heightResize(newHeight);
    solfegeInternalHeight = newHeight;
    solfegeWindow->setFixedHeight(solfegeWindow->height() + difference);

}

void MainWindow::solfegeKeyWidthResize(int newWidth)
{

    s->keyWidthResize(newWidth);
    setSolfegeWindowMaxWidth();
    solfegeKeyWidth = newWidth;
}

void MainWindow::setSolfegeWindowMaxWidth()
{
    int maxSolfegeExternalWidth = s->getKeyboard()->width() + solfegeWindowWidthThickness;
    solfegeWindow->setMaximumWidth(maxSolfegeExternalWidth);

}

void MainWindow::timerEvent(QTimerEvent * event) //completes solfege initialization
{
    if (event->timerId() == solfegeTimer)
    {
        solfegeWindowWidthThickness = this->frameGeometry().width() - this->geometry().width();
        setSolfegeWindowMaxWidth();
        solfegeExternalHeight = solfegeWindow->height();
        solfegeWindow->resize(solfegeExternalWidth, solfegeExternalHeight);
        solfegeWindow->setFixedHeight(solfegeExternalHeight);
        killTimer(solfegeTimer);
        solfegeTimer = 0;
    }
}

void MainWindow::initializeSwitcher()
{
    swi = new Switcher(this);
    QMdiSubWindow * switcherWindow = area->addSubWindow(swi);
    switcherWindow->move(0,0);
    switcherWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    switcherWindow->setFixedSize(switcherWindow->sizeHint());
}

void MainWindow::initializePiano()
{
    p = new Piano(syn, 0, swi);
    pianoWindow = area->addSubWindow(p);
    pianoWindow->setWindowFlags(Qt::CustomizeWindowHint);
    pianoWindow->setMinimumSize(400,100);
    pianoWindow->resize(pianoWidth,pianoHeight);
    pianoWindow->move(pianoX,pianoY);
}

void MainWindow::initializeSolfege()
{
    s = new FramedSolfege(syn,p,swi,transpose,this);
    s->keyWidthResize(solfegeKeyWidth);
    s->heightResize(solfegeInternalHeight);

    solfegeWindow = area->addSubWindow(s);
    solfegeWindow->setWindowFlags(Qt::CustomizeWindowHint);
    solfegeWindow->setMinimumWidth(30);
    solfegeWindow->move(solfegeX,solfegeY);
    //CHECK THAT LOCATION OF FOLLOWING LINE IS CROSS-PLATFORM COMPATIBLE (otherwise place in MainWindow::timerEvent)
    s->horizontalScrollBar()->setValue(solfegeScroll); //BUG: WHY DOES THIS IMPOSE AN ARBITRARY MAXIMUM?!
    solfegeTimer = startTimer(100); //will complete initialization after windows are drawn
}

void MainWindow::createMenus()
{
    exitAction = new QAction("E&xit", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    helpAction = new QAction("&Instructions", this);
    connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelp()));

    aboutAction = new QAction("&About Relative Keys", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(exitAction);
    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);

}

void MainWindow::showHelp()
{
    QMessageBox::about(this, "Instructions",
                       "Relative Keys is a tool for transposing the solfege sequence to any of the 88 keys on a standard piano. "
                       "This is useful for developing functional pitch recognition, as it allows the playing of scales, melodies, "
                       "and chords in any key without reference to absolute pitches. However, these pitches can still be seen on the "
                       "standard keyboard, making it a good aid for transcription. "
                       "<p> To <strong>transpose</strong>, click the arrow button followed by a key from the piano keyboard and then "
                       "the solfege value to which you want that key assigned. The next time that solfege value is clicked, the newly "
                       "assigned pitch will sound. <p>To play a chord, click the <strong>chord</strong> button followed by up to "
                       "ten notes from either keyboard (selected notes can be unselected by clicking them a second time). Clicking the "
                       "<strong>play</strong> button will cause the selected pitches to sound together. Clicking the chord button a second "
                       "time releases them. <p> The length of the solfege keyboard's window can be adjusted by dragging its edge; "
                       "click the small unmarked button in the window's lower-right corner for controls that adjust the width and height "
                       "of the keys.");
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Relative Keys",
                       "<h2>Relative Keys 1.0</h2>"
                       "<p>&copy; 2013 Jay Davis"
                       "<p>Free for any use"
                       "<p>Developed with <a href = 'http://qt.digia.com'>Qt</a> and <a href = 'http://sourceforge.net/apps/trac/fluidsynth/'>FluidSynth</a>");
}

void MainWindow::readSettings()
{
    QFile settings("settings.dat");
    if (!settings.open(QIODevice::ReadOnly))
    { //defaults
        solfegeX = 120;
        solfegeY = 350;
        solfegeExternalWidth = 700;
        //solfegeExternalHeight = 303;
        solfegeKeyWidth = 40;
        solfegeInternalHeight = 250;
        solfegeScroll = 630;
        transpose = 0;

        pianoX = 15;
        pianoY = 100;
        pianoWidth = 1000;
        pianoHeight = 220;

    }
     else
     {
        QDataStream in(&settings);
        in >> solfegeX >> solfegeY >> solfegeExternalWidth >> solfegeExternalHeight
           >> solfegeKeyWidth >> solfegeInternalHeight >> solfegeScroll >> transpose
           >> pianoX >> pianoY >> pianoWidth >> pianoHeight;
     }
}

void MainWindow::writeSettings()
{
    //gather settings
    solfegeX = solfegeWindow->x();
    solfegeY = solfegeWindow->y();
    solfegeExternalWidth = solfegeWindow->width();
    solfegeExternalHeight = solfegeWindow->height();
    solfegeScroll = s->horizontalScrollBar()->value();
    transpose = s->getOffset();
    pianoX = pianoWindow->x();
    pianoY = pianoWindow->y();
    pianoWidth = pianoWindow->width();
    pianoHeight = pianoWindow->height();

    //write to file
    QFile settings("settings.dat");
    if (!settings.open(QIODevice::WriteOnly))
        qWarning("Cannot open file for writing: %s\n", qPrintable(settings.errorString()));
    else {
           QDataStream out(&settings);
           out << solfegeX << solfegeY << solfegeExternalWidth << solfegeExternalHeight
               << solfegeKeyWidth << solfegeInternalHeight << solfegeScroll
               << transpose << pianoX << pianoY << pianoWidth << pianoHeight;
        }
}


void MainWindow::closeEvent(QCloseEvent * quit)
{
    writeSettings();
    quit->accept();
}
