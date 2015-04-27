#include "whiteKey.h"
#include "instrument.h"
#include "pianoSynth.h"
#include "switcher.h"

WhiteKey::WhiteKey(const PianoSynth * au, const Instrument * ins, const Switcher * swi,
                   const CustomColor & a, const CustomColor & i, QWidget * parent) :
                   Key(swi, a, i, parent), audio(au), instr(ins)
{
    keyType = 0;
}

void WhiteKey::press(bool directClick, bool chordPlay)
{
    directCl = directClick;
    if ((isPressed == 0) && ((chordPlay == true) || (toggles->chord.size() < 10)))
    {
        isPressed = 1;
        if (audio != 0)
            audio->press(midiNumber);
        if (instr != 0)
            instr->press(keyNumber, chordPlay);
        update();
    }
    else if (toggles->sustainSwitch == true)
        emit switcherSignal(qobject_cast<Key *> (this));
    if (toggles->transposeSwitch == false)
        checkIfVisible();
}

void WhiteKey::release(bool directClick)
{
    directCl = directClick;
    if ((isPressed == 1) && (toggles->sustainSwitch == false))
    {
        isPressed = 0;
        if (audio != 0)
            audio->release(midiNumber);
        if (instr != 0)
            instr->release(keyNumber);
        update();
    }
    else if (toggles->transposeSwitch == true)
        emit switcherSignal(qobject_cast<Key *> (this));

}
