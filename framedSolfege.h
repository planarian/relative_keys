#ifndef FRAMEDSOLFEGE_H
#define FRAMEDSOLFEGE_H

#include <QScrollArea>
#include "solfege.h"

class PianoSynth;
class Instrument;
class Switcher;
class ResizeDialog;
class QPushButton;
class Key;
class Timer;
class MainWindow;

class FramedSolfege : public QScrollArea
{
    Q_OBJECT
public:
    explicit FramedSolfege(PianoSynth *, Instrument *, Switcher *, int, QWidget * parent = 0);
    void heightResize(int newHeight) const;
    void keyWidthResize(int newWidth) const;
    void scrollLeft(int);
    void scrollRight(int);
    void setOffset(int);
    int getOffset();
    int minimumKeyWidth(); //smallest key width guaranteeing keyboard not narrower than viewport
    void removeMargins();
    const Solfege * getKeyboard() const;
    const Key * getKey(int) const;
    void blink();
    int keyboardMargin; //negative indicates size of left margin, positive indicates size of right margin

public slots:
    void valueChangedSlot(int);
    void notVisibleSlot(Key *); //relays FROM solfegeKeySet. Scrolls until key visible.
    void removeMarginSlot();
    void blinkSlot();

protected:
    //void resizeEvent(QResizeEvent *);

private:
    void setKeyBoardMarginLeft(int);
    void setKeyBoardMarginRight(int);
    Solfege * keyboard;
    ResizeDialog * stretchBox;
    QPushButton * stretchButton;
    MainWindow * parentWindow;
    int * leftMargin, * rightMargin, * topMargin, * bottomMargin;
    int scrollBarLast;
    QTimer * marginTimer, * blinkTimer;
    volatile bool justResized;


private slots:
    void resizeSlot();

 };

#endif // FRAMEDSOLFEGE_H
