#include "basicBlackKey.h"
#include "switcher.h"
#include <QString>

BasicBlackKey::BasicBlackKey(const Switcher * swi, const CustomColor & a, const CustomColor & i, QWidget * parent) : Key(swi, a, i, parent)
{
    keyType = 2;
}

void BasicBlackKey::press(bool directClick, bool chordPlay)
{
    directCl = directClick;
    if ((isPressed == 0) && ((chordPlay == true) || (toggles->chord.size() < 10)))
    {
        isPressed = 1;
        if ((directClick == true) || (chordPlay == true))
        {
            emit pressSignal(chordPlay);
            if (toggles->sustainSwitch == true)
                emit switcherSignal(qobject_cast<Key*> (this));
        }
        update();
    }
    else if ((directClick == true) && (toggles->sustainSwitch == true))
        emit switcherSignal(qobject_cast<Key *> (this));
    if (toggles->transposeSwitch == false)
        checkIfVisible();
}

void BasicBlackKey::release(bool directClick)
{
    directCl = directClick;
    if ((isPressed == 1) && (toggles->sustainSwitch == false))
    {
        isPressed = 0;
        emit releaseSignal();
        if ((directClick == true) && (toggles->transposeSwitch == true))
            emit switcherSignal(qobject_cast<Key *> (this));
        update();
    }
}

void BasicBlackKey::sympatheticPressSlot(bool chordPlay)
{
    press(false, chordPlay);
}

void BasicBlackKey::sympatheticReleaseSlot()
{
    release();
}
