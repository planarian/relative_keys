#include "piano.h"
#include "pianoKeySet.h"
#include <QPainter>


Piano::Piano(const PianoSynth * s, const Instrument * a, const Switcher * swi, QWidget *parent) : Instrument(s, a, swi, parent)
{
    keyboard = new PianoKeySet(audio, instr, toggles, this);
    createLayout();

}

void Piano::setSynth(const PianoSynth * newSynth)
{
    audio = newSynth;
    delete keyboard;
    keyboard = new PianoKeySet(audio, instr, toggles, this);
    createLayout();
}

void Piano::setInstrument(const Instrument * newInstr)
{
    instr = newInstr;
    delete keyboard;
    keyboard = new PianoKeySet(audio, instr, toggles, this);
    createLayout();
}

void Piano::paintEvent(QPaintEvent *)
{
    QPainter background(this);
    background.fillRect(rect(), Qt::black);

}

