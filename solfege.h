#ifndef SOLFEGE_H
#define SOLFEGE_H

#include "instrument.h"

class QWidget;
class QPainter;
class PianoSynth;
class SolfegeKeySet;
class Switcher;
class Key;
class FramedSolfege;

class Solfege : public Instrument
{
    Q_OBJECT
public:
    Solfege(PianoSynth *, Instrument *, Switcher *, int, QWidget * parent = 0);
    ~Solfege() {}
    void setSynth(const PianoSynth *);
    void setInstrument(const Instrument *);
    void setOffset(int);
    SolfegeKeySet * getKeyboard() const;
    virtual QSize sizeHint() const;
    int getOffset() const { return offset; }
    void keyWidthResize(int keyWidth, int margin = 0);
    void blink();
    bool isBright() const { return blinking; } //tests whether keyboard background has been turned white

protected:
    void paintEvent(QPaintEvent *);
    int offset;

private:
    void resetKeyboard();
    FramedSolfege * framedParent;
    bool blinking;

};

#endif // SOLFEGE_H
