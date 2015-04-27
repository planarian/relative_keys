#include <QSizePolicy>
#include "instrument.h"
#include "solfegeKeySet.h"
#include "key.h"
#include <QHBoxLayout>


Instrument::Instrument(const PianoSynth * a, const Instrument * i, const Switcher * swi, QWidget *parent) : QWidget(parent)
{
    audio = a;
    instr = i;
    toggles = swi;
}

void Instrument::press(int keyNum, bool chordPlay) const
{
    keyboard->press(keyNum, chordPlay);
}

void Instrument::release(int keyNum) const
{
    keyboard->release(keyNum);
}

const Key * Instrument::getKey(int i) const
{
    return keyboard->getKey(i);
}

void Instrument::createLayout()
{
    delete this->layout();
    instLayout = new QHBoxLayout;

    instLayout->addWidget(keyboard);

    this->setLayout(instLayout);
}
