#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>
class QSlider;
class QVBoxLayout;
class QLabel;
class MainWindow;
class FramedSolfege;
class QScrollBar;

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeDialog(QWidget * parent = 0,  Qt::WindowFlags f = 0);

public slots:
    void widthResizeSlot(int newSize);
    void heightResizeSlot(int newSize);

private:
    MainWindow * parentWindow;
    QSlider * widthSlider, * heightSlider;
    QVBoxLayout * layout;
    QLabel * widthLabel, * heightLabel;
    FramedSolfege * solfege;
    QScrollBar * solfegeScrollBar;

};

#endif // RESIZEDIALOG_H
