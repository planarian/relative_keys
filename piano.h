#ifndef PIANO_H
#define PIANO_H

#include "instrument.h"

class QPainter;
class Instrument;
class PianoKeySet;
class PianoSynth;
class Instrument;
class Switcher;

class Piano : public Instrument
{
    Q_OBJECT
public:
    explicit Piano(const PianoSynth *, const Instrument *, const Switcher *, QWidget * parent = 0);
    ~Piano() {}
    void setSynth(const PianoSynth *);
    void setInstrument(const Instrument *);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // PIANO_H
