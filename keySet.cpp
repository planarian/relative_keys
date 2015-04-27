#include "keySet.h"
#include "key.h"
#include <QMouseEvent>
#include <QHBoxLayout>


KeySet::KeySet(const PianoSynth * a, const Instrument * i, const Switcher * swi, QWidget *parent) :
    QWidget(parent), audio(a), instr(i), toggles(swi), keyMap(89)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);

}

void KeySet::press(int keyNum, bool chordPlay) const
{
    keyMap[keyNum]->press(false, chordPlay);
}

void KeySet::release(int keyNum) const
{
    keyMap[keyNum]->release();
}


void KeySet::mapKeys()
{
    int whiteCount = 0, blackCount = 0, combinedCount = 0;

    keyMap[0] = 0; //first cell unused;

    do  //copies members of whiteKeys and blackKeys to a single 88-key vector, converted to MIDI numbers (21 - 108)
    {
        if ((blackCount == 0) && (blackLocations[blackCount] == -1)) //keyboard starts with black key
        {
            keyMap[1] = (Key *) blackKeys_upper[0]; //why doesn't qobject_cast work here?
            blackCount++;
            combinedCount++;
        }

        keyMap[combinedCount + 1] = (Key *) whiteKeys[whiteCount]; //why doesn't qobject_cast work here?

        if (blackCount < blackKeys_upper.size()) //CHECK THIS
            if (blackLocations[blackCount] == whiteCount)
            {
                keyMap[combinedCount + 2] = (Key *) blackKeys_upper[blackCount]; //why doesn't qobject_cast work here?
                blackCount++;
                combinedCount++;
            }
        whiteCount++;
        combinedCount++;
    } while (combinedCount < 88);
}

void KeySet::initializeVar() //prevents null pointer access
{
    recentPressed = (QWidget *) whiteKeys[0];
    previousPressed = (QWidget *) whiteKeys[1];
}

void KeySet::assignNumbers()
{
    for (int i = 0; i < 88; i++)  //Assign key numbers (1-88) and MIDI numbers (21 - 108)
    {
        keyMap[i + 1]->keyNumber = i + 1;
        keyMap[i + 1]->midiNumber = i + 21;
    }

}

void KeySet::resizeEvent(QResizeEvent *)
{
    arrangeBlackKeys(); //this must be called on first draw
}

void KeySet::mousePressEvent(QMouseEvent * event)
{

    QWidget * temp = (childAt(event->pos()));

    previousPressed = recentPressed;
    if (temp != 0)
    {
        recentPressed = temp;
        (qobject_cast<Key*> (recentPressed))->press(true);
    }

}

void KeySet::mouseReleaseEvent(QMouseEvent * )
{
     (qobject_cast<Key *> (recentPressed))->release(true);
}

void KeySet::mouseMoveEvent(QMouseEvent * event)
{

    QWidget * temp = childAt(event->pos());

    previousPressed = recentPressed;

    if ((temp != 0) && !temp->visibleRegion().isEmpty())
    {
        recentPressed = temp;
        if (recentPressed != previousPressed)
        {
            (qobject_cast<Key *> (previousPressed))->release(true);
            (qobject_cast<Key*> (recentPressed))->press(true);
        }
    }
}



CustomColor KeySet::yellow_ = {QColor(238, 242, 55, 255), QColor(244, 248, 135, 255), QColor(117, 120, 7, 255)};
CustomColor KeySet::orange_yellow_ = {QColor(239, 217, 58, 255), QColor(245, 232, 137, 255), QColor(118, 104, 10, 255)};
CustomColor KeySet::orange_ = {QColor(235, 192, 63, 255), QColor(243, 218, 139, 255), QColor(116, 90, 12, 255)};
CustomColor KeySet::red_orange_ = {QColor(237, 136, 61, 255), QColor(244, 184, 138, 255), QColor(117, 56, 11, 255)};
CustomColor KeySet::red_ = {QColor(239, 58, 58, 255), QColor(245, 137, 137, 255), QColor(118, 10, 10, 255)};
CustomColor KeySet::violet_red_ = {QColor(211, 86, 108, 255), QColor(228, 154, 167, 255), QColor(101, 27, 39, 255)};
CustomColor KeySet::violet_ = {QColor(174, 123, 174, 255), QColor(206, 176, 206, 255), QColor(79, 49, 79, 255)};
CustomColor KeySet::blue_violet_ = {QColor(148, 120, 177, 255), QColor(191, 174, 208, 255), QColor(64, 47, 81, 255)};
CustomColor KeySet::blue_ = {QColor(119, 119, 179, 255), QColor(173, 173, 209, 255), QColor(46, 46, 82, 255)};
CustomColor KeySet::blue_green_ = {QColor(117, 173, 181, 255), QColor(172, 206, 210, 255), QColor(45, 79, 83, 255)};
CustomColor KeySet::green_ = {QColor(114, 184, 117, 255), QColor(170, 213, 172, 255), QColor(43, 85, 45, 255)};
CustomColor KeySet::yellow_green_ = {QColor(183, 235, 63, 255), QColor(213, 243, 139, 255), QColor(85, 116, 12, 255)};
CustomColor KeySet::white_ = {QColor(242,243,218,255), Qt::white, QColor(94, 97, 31, 255)};
CustomColor KeySet::gray_ = {QColor(167, 167, 167, 255), QColor(212, 212, 212, 255), QColor(63, 63, 63, 255)};
CustomColor KeySet::black_ = {Qt::black, gray_.standard, Qt::black};
CustomColor KeySet::darkCyan_ = {Qt::darkCyan /*rgb = 0,128,128*/, QColor(0, 170, 170, 255), QColor(0, 91, 91, 255) /*exception to formula*/};
