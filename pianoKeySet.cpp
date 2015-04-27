#include "pianoKeySet.h"
#include <QHBoxLayout>
#include "whiteKey.h"
#include "fullBlackKey.h"


PianoKeySet::PianoKeySet(const PianoSynth * a, const Instrument * i, const Switcher * swi, QWidget *parent) :
    KeySet(a, i, swi, parent)
{

    // determine which white keys have black keys
    blackLocations << 0 << 2 << 3 << 5 << 6 << 7 << 9 << 10 << 12 << 13 << 14
                   << 16 << 17 << 19 << 20 << 21 << 23 << 24 << 26 << 27 << 28
                   << 30 << 31 << 33 << 34 << 35 << 37 << 38 << 40 << 41 << 42
                   << 44 << 45 << 47 << 48 << 49;

    createWhiteKeys();
    createBlackKeys();
    mapKeys();
    initializeVar();
    assignNumbers();

}

void PianoKeySet::arrangeBlackKeys()
{
    int keyWidth = whiteKeys[0]->width() * 4 / 5 - 1;
    int keyHeight = whiteKeys[0]->height() / 1.6;
    int offset = whiteKeys[0]->width() - (keyWidth / 2);
    int xLocation, yLocation = whiteKeys[0]->pos().y() - 1;

    for (int x = 0; x < blackKeys_upper.length(); ++x)
    {
        xLocation = blackLocations[x];
        blackKeys_upper[x]->resize(keyWidth, keyHeight);
        blackKeys_upper[x]->move(whiteKeys[xLocation]->pos().x() + offset, yLocation);
    }
}

void PianoKeySet::createWhiteKeys()
{

    for (int x = 0; x < 52; ++x)
    {
        whiteKeys << new WhiteKey(audio, instr, toggles, gray_, white_, this);
        layout->addWidget(whiteKeys[x]);

        connect(whiteKeys[x], SIGNAL(switcherSignal(Key*)), toggles, SLOT(mostRecentPressSlot(Key*)), Qt::QueuedConnection);
    }
}


void PianoKeySet::createBlackKeys()
{
    for (int x = 0; x < 36; ++x)
    {
        blackKeys_upper << new FullBlackKey(audio, instr, toggles, gray_, black_, this);
        connect(blackKeys_upper[x], SIGNAL(switcherSignal(Key *)), toggles, SLOT(mostRecentPressSlot(Key*)), Qt::QueuedConnection);
    }
}
