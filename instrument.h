#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QWidget>
#include "keySet.h"

class QSizePolicy;
class PianoSynth;
class Switcher;
class Key;
class QHBoxLayout;

class Instrument : public QWidget  //abstract base class
{
    Q_OBJECT
public:
    explicit Instrument(const PianoSynth *, const Instrument *, const Switcher *, QWidget *parent = 0);
    virtual ~Instrument() {}
    void press(int, bool) const;
    void release(int) const;
    const Key * getKey(int) const;
    int getWhiteKeyWidth() const { return keyboard->getWhiteKeyWidth(); }
    int getNumWhiteKeys() const {return keyboard->getNumWhiteKeys(); }
    virtual void setSynth(const PianoSynth *) = 0;
    virtual void setInstrument(const Instrument *) = 0;
    QHBoxLayout * instLayout;

protected:
    virtual void paintEvent(QPaintEvent *) = 0;
    void createLayout();
    const PianoSynth * audio;
    const Instrument * instr;
    const Switcher * toggles;
    KeySet * keyboard;

};

#endif // INSTRUMENT_H
