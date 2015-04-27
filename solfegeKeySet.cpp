#include "solfegeKeySet.h"
#include <QHBoxLayout>
#include "basicBlackKey.h"
#include "whiteKey.h"
#include "fullBlackKey.h"
#include <QString>

SolfegeKeySet::SolfegeKeySet(const PianoSynth * a, const Instrument * i, const Switcher * swi, const int offset, QWidget * parent):
    KeySet(a, i, swi, parent), pitchOffset(offset)
{   

    top = new int;
    bottom = new int;
    left = new int;
    right = new int;

    pitchTranspose();
    createWhiteKeys();
    createBlackKeys();
    mapKeys();
    initializeVar();
    assignNumbers();
    colorTranspose();
}

void SolfegeKeySet::createBlackKeys()
{
    CustomColor inactiveColor; //placeholder
    for (int x = 0; x < blackLocations.size(); ++x)
    {

        blackKeys_upper << new FullBlackKey(audio, instr, toggles, darkCyan_, inactiveColor, this);
        blackKeys_lower << new BasicBlackKey(toggles, darkCyan_, inactiveColor, this);

        connect(blackKeys_upper[x], SIGNAL(pressSignal(bool)), blackKeys_lower[x], SLOT(sympatheticPressSlot(bool)));
        connect(blackKeys_upper[x], SIGNAL(releaseSignal(bool)), blackKeys_lower[x], SLOT(sympatheticReleaseSlot()));
        connect(blackKeys_lower[x], SIGNAL(pressSignal(bool)), blackKeys_upper[x], SLOT(sympatheticPressSlot(bool)));
        connect(blackKeys_lower[x], SIGNAL(releaseSignal(bool)), blackKeys_upper[x], SLOT(sympatheticReleaseSlot()));

        connect(blackKeys_upper[x], SIGNAL(switcherSignal(Key*)), toggles, SLOT(mostRecentPressSlot(Key*)), Qt::QueuedConnection);
        connect(blackKeys_lower[x], SIGNAL(switcherSignal(Key*)), toggles, SLOT(mostRecentPressSlot(Key*)), Qt::QueuedConnection);

        connect(blackKeys_upper[x], SIGNAL(notVisibleSignal(Key*)), this, SLOT(notVisibleSlot(Key*)));
        connect(blackKeys_lower[x], SIGNAL(notVisibleSignal(Key*)), this, SLOT(notVisibleSlot(Key*)));
     }
}

void SolfegeKeySet::createWhiteKeys()
{
    CustomColor inactiveColor;
    for (int x = 0; x < (88 - blackLocations.size()); ++x)
    {

        whiteKeys << new WhiteKey(audio, instr, toggles, darkCyan_, inactiveColor, this);
        layout->addWidget(whiteKeys[x]);
        connect(whiteKeys[x], SIGNAL(switcherSignal(Key*)), toggles, SLOT(mostRecentPressSlot(Key*)), Qt::QueuedConnection);
        connect(whiteKeys[x], SIGNAL(notVisibleSignal(Key*)), this, SLOT(notVisibleSlot(Key*)));
    }
}

void SolfegeKeySet::arrangeBlackKeys()
{
    int keyWidth = whiteKeys[0]->width() * 4 / 5 + 1;
    int keyHeight = whiteKeys[0]->height() / 3;
    int offset = whiteKeys[0]->width() - (keyWidth / 2);
    int xLocation, yLocation_upper = whiteKeys[0]->pos().y() - 1,
            yLocation_lower = (whiteKeys[0]->pos().y()) +  (whiteKeys[0]->height() - keyHeight) + 1;

    for (int x = 0; x < blackKeys_upper.length(); ++x)
    {
        xLocation = blackLocations[x];
        blackKeys_upper[x]->resize(keyWidth, keyHeight);
        blackKeys_lower[x]->resize(keyWidth, keyHeight);
        if (xLocation != -1) //set black key AHEAD of current white key
        {
            blackKeys_upper[x]->move(whiteKeys[xLocation]->pos().x() + offset, yLocation_upper);
            blackKeys_lower[x]->move(whiteKeys[xLocation]->pos().x() + offset, yLocation_lower);
        }
        else //set black key BEHIND current white key
        {
            blackKeys_upper[x]->move(whiteKeys[0]->pos().x() - (keyWidth / 2), yLocation_upper);
            blackKeys_lower[x]->move(whiteKeys[0]->pos().x() - (keyWidth / 2), yLocation_lower);
        }
    }
}

void SolfegeKeySet::assignNumbers()
{
    KeySet::assignNumbers();

    for (int i = 0; i < blackLocations.size(); i++)
        blackKeys_lower[i]->keyNumber = blackKeys_upper[i]->keyNumber;
}



void SolfegeKeySet::pitchTranspose()
{
    QList<int> chromatic;
    int whiteCount = 0, blackCount = 0, temp;
    blackLocations.clear();

    chromatic << 1 << 0 << 1 << 1 << 0 << 1
              << 0 << 1 << 1 << 0 << 1 << 0; // A, A#, B, C, C#...G#

    for (int i = 0; i < abs(pitchOffset); i++) //rotates sequence to left or right, depending on the sign
    {
        if (pitchOffset > 0)
        {
            temp = chromatic[11];
            chromatic.pop_back();
            chromatic.push_front(temp);
        }

        else
        {
            temp = chromatic[0];
            chromatic.pop_front();
            chromatic.push_back(temp);
        }
    }

    //rebuilds blackLocations using transposed sequence, tallies the new number of black/white keys

    if (chromatic[0] == 0)
    {
        blackLocations << -1; //indicates that the new keyboard layout begins with a black key
        blackCount++;
    }
    else
        whiteCount++;

    int j;

    for (int i = 1; i < 88; i++)
    {
        j = i % 12;
        if (chromatic[j] == 1)
            whiteCount++;
        else
        {
            blackLocations << whiteCount - 1;
            blackCount++;
        }

    }


}

void SolfegeKeySet::colorTranspose()
{

    for (int i = 1; i < 89; i++)
    {
        switch((i + 11 - pitchOffset) % 12) //divide colors among 88 keys (keyMap[0] is empty)
        {
        case 0: keyMap[i]->inactiveColor = blue_green_; //la
                keyMap[i]->solValue = "la";
                break;
        case 1: keyMap[i]->inactiveColor = green_; //te-li
                keyMap[i]->solValue = "li";
                break;
        case 2: keyMap[i]->inactiveColor = yellow_green_; //ti
                keyMap[i]->solValue = "ti";
                break;
        case 3: keyMap[i]->inactiveColor = yellow_; //do
                keyMap[i]->solValue = "do";
                break;
                keyMap[i]->solValue = "do";
        case 4: keyMap[i]->inactiveColor = orange_yellow_; //ra-di
                keyMap[i]->solValue = "di";
                break;
        case 5: keyMap[i]->inactiveColor = orange_; //re
                keyMap[i]->solValue = "re";
                break;
        case 6: keyMap[i]->inactiveColor = red_orange_; //ri-me
                keyMap[i]->solValue = "ri";
                break;
        case 7: keyMap[i]->inactiveColor = red_; //mi
                keyMap[i]->solValue = "mi";
                break;
        case 8: keyMap[i]->inactiveColor = violet_red_; //fa
                keyMap[i]->solValue = "fa";
                break;
        case 9: keyMap[i]->inactiveColor = violet_; //se-fi
                keyMap[i]->solValue = "fi";
                break;
        case 10:keyMap[i]->inactiveColor = blue_violet_; //sol
                keyMap[i]->solValue = "sol";
                break;
        case 11:keyMap[i]->inactiveColor = blue_; //le-si
                keyMap[i]->solValue = "si";
                break;
        }
    }

    QString upperSolValue;
    for (int i = 0; i < blackKeys_upper.size(); i++)   //color, label stub keys
    {
        upperSolValue = blackKeys_upper[i]->solValue;

        if (upperSolValue == "li")
            blackKeys_lower[i]->solValue = "te";
        else if (upperSolValue == "di")
            blackKeys_lower[i]->solValue = "ra";
        else if (upperSolValue == "ri")
            blackKeys_lower[i]->solValue = "me";
        else if (upperSolValue == "fi")
            blackKeys_lower[i]->solValue = "se";
        else if (upperSolValue == "si")
            blackKeys_lower[i]->solValue = "le";

        blackKeys_lower[i]->inactiveColor = blackKeys_upper[i]->inactiveColor;
    }

}

void SolfegeKeySet::notVisibleSlot(Key * invisibleKey)
{
    emit notVisibleSignal(invisibleKey);
}

void SolfegeKeySet::setInnerMargins(int newMargin)
{
    layout->getContentsMargins(left, top, right, bottom);
    *left = 9/*default margin*/ + newMargin;
    *right = 9 /*default margin*/ + newMargin;
    layout->setContentsMargins(*left, *top, *right, *bottom);
}

const int SolfegeKeySet::getInnerMargin() const
{
    int * side = new int;
    int * dummy = new int;

    layout->getContentsMargins(side, dummy, dummy, dummy);
    return *side - 9 /*default margin*/;

}
