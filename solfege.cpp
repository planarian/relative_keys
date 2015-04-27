#include "solfege.h"
#include "solfegeKeySet.h"
#include "keySet.h"
#include "key.h"
#include "whiteKey.h"
#include "framedSolfege.h"
#include <QPainter>
#include <QHBoxLayout>


Solfege::Solfege(PianoSynth * a, Instrument * i, Switcher * swi, int o, QWidget * parent):
    Instrument(a, i, swi, parent), offset(o)
{
    blinking = false;
    keyboard = new SolfegeKeySet(audio, instr, swi, o, this);
    framedParent = qobject_cast<FramedSolfege *> (parent);
    connect(keyboard, SIGNAL(notVisibleSignal(Key *)), framedParent, SLOT(notVisibleSlot(Key*)));
    createLayout();
}

void Solfege::setSynth(const PianoSynth * newSynth)
{
    audio = newSynth;
    resetKeyboard();
}

void Solfege::setInstrument(const Instrument * newInstr)
{
    instr = newInstr;
    resetKeyboard();
}

void Solfege::setOffset(int newOffset)
{
    offset = newOffset;
    resetKeyboard();
    keyboard->show();
}

SolfegeKeySet * Solfege::getKeyboard() const
{
    return qobject_cast<SolfegeKeySet *> (keyboard);
}

void Solfege::paintEvent(QPaintEvent *)
{
    QPainter background(this);
    background.fillRect(rect(), isBright() ? Qt::gray : Qt::black);
}

QSize Solfege::sizeHint() const
{
    return QSize(1600,250);
}

void Solfege::resetKeyboard()
{
    instLayout->removeWidget(keyboard);
    int innerMargin = getKeyboard()->getInnerMargin();
    delete keyboard;
    keyboard = new SolfegeKeySet(audio, instr, toggles, offset, this);
    connect(keyboard, SIGNAL(notVisibleSignal(Key *)), framedParent, SLOT(notVisibleSlot(Key*)));
    instLayout->addWidget(keyboard);
    getKeyboard()->setInnerMargins(innerMargin);
    //createLayout();
}

void Solfege::keyWidthResize(int keyWidth, int margin)
{
    int newWidth, newInnerMargin;

    int numKeys = keyboard->getNumWhiteKeys();
    newWidth = keyWidth * numKeys + 36/*magic number*/ + margin;
    newInnerMargin = (keyWidth > 23) ? ((keyWidth - 23/*max key width before keyset margin needs expanding*/) / 2) : 0;
    if (newInnerMargin >= 0)
        getKeyboard()->setInnerMargins(newInnerMargin);
    resize(newWidth + (2 * newInnerMargin), height());
}

void Solfege::blink()
{
    if (isBright())
    {
       blinking = false;
       this->update();
    }
    else
    {
        blinking = true;
        this->update();
    }
}
