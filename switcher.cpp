#include "switcher.h"
#include "framedSolfege.h"
#include <QScrollBar>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSize>
#include "key.h"


Switcher::Switcher(QWidget *parent) : QWidget(parent)
{
    solfege = 0;
    transKey = 0;
    transposeSwitch = false;
    sustainSwitch = false;

    transposeButton = new QToolButton;
    transposeButton->setCheckable(true);
    transposeButton->setIcon(QIcon(":/img/transpose.png"));
    transposeButton->setIconSize(QSize(40,40));
    transposeButton->setToolTip("transpose");

    sustainButton = new QToolButton;
    sustainButton->setCheckable(true);
    sustainButton->setIcon(QIcon(":/img/sustain.png"));
    sustainButton->setIconSize(QSize(40,40));
    sustainButton->setToolTip("sustain");

    playChordButton = new QToolButton;
    playChordButton->setIcon(QIcon(":/img/play.png"));
    playChordButton->setIconSize(QSize(40,40));
    playChordButton->setToolTip("play chord");

    buttonRow = new QHBoxLayout;
    buttonRow->addWidget(transposeButton);
    buttonRow->addWidget(sustainButton);
    buttonRow->addWidget(playChordButton);
    this->setLayout(buttonRow);

    connect(transposeButton, SIGNAL(clicked()), this, SLOT(toggleTransposeSlot()));
    connect(sustainButton, SIGNAL(clicked()), this, SLOT(toggleSustainSlot()));
    connect(playChordButton, SIGNAL(clicked()), this, SLOT(playSlot()));

    playChordButton->setEnabled(false);

}


void Switcher::mostRecentPressSlot(Key * key)
{
    latest = key;
    if (transposeSwitch == true)
        transpose();
    else if (sustainSwitch == true)
        sustain();
}

void Switcher::transpose()
{

    if (transKey == 0)
        transKey = latest;

    else
    {
        if (transKey->parent() == latest->parent())  //user selected two keys from the same keyboard, so abort
        {
            transposeSwitch = false;
            transposeButton->setChecked(false);
            transKey = 0;
        }
        else
        {
            solfegeKeyWidth = solfege->getKeyboard()->getWhiteKeyWidth();
            solfege->blink();
            solfege->setOffset(calculateOffset());
            transSolfege = solfege->getKey(transSolfegeKeyNum); //because ptr's referent was just deleted
            solfege->keyWidthResize(solfegeKeyWidth);
            scroll();
            transposeSwitch = false;
            transposeButton->setChecked(false);
            transKey = 0;
        }
    }
}

void Switcher::sustain()
{
    int dupIndex = checkDuplicates(chord, latest);
    if ((dupIndex == -1) && (chord.size() < 10))
    {
        chord << latest;
        if (chord.size() == 1)
            playChordButton->setEnabled(true);
    }
    else if ((dupIndex != -1) && latest->directCl)
    {
        sustainSwitch = false;
        chord[dupIndex]->release();
        sustainSwitch = true;
        if (chord.size() == 1) //soon to be 0
        {
            toggleSustainSlot();
            return;
        }

        QList<Key *>::iterator iter;
        iter = chord.begin();

        for (int i = 0; i < dupIndex; i++)
            iter++;

        chord.erase(iter);
    }
}

int Switcher::checkDuplicates(const QList<Key *> & c, const Key * l) const //c for chord, l for latest; returns -1 if match not found, otherwise offset
{
    int size = c.size();

    int i;
    for (i = 0; i < size; i++)
        if (c[i]->keyNumber == l->keyNumber)
            return i;

    return -1;
}

int Switcher::calculateOffset()
{
    int localOffset, globalOffset, previousOffset;

    const char * keyboardType = transKey->parent()->metaObject()->className();
    if (keyboardType[0] == 'P') //i.e., first letter of class "PianoKeySet"
    { //transKey belongs to piano
        transPiano = transKey;
        transSolfege = latest;
    }
    else //latest belongs to piano
    {
        transPiano = latest;
        transSolfege = transKey;
    }

    transSolfegeKeyNum = transSolfege->keyNumber;     //needed for Switcher::scroll()
    transSolfegeKeyPos = transSolfege->absPosition().x();

    globalOffset = transSolfege->keyNumber;
    localOffset = (transPiano->keyNumber) - (transSolfege->keyNumber);
    previousOffset = solfege->getOffset();
    return (localOffset - ((globalOffset - previousOffset) % 12 /* key# if solfege WERE in starting position */) + globalOffset) % 12;

}

void Switcher::toggleTransposeSlot()
{
    int keyboardMargin = solfege->keyboardMargin;

    if (transposeSwitch == false)
    {
        if (sustainSwitch == true)
                toggleSustainSlot();

        transposeSwitch = true;
        transposeButton->setChecked(true);

        if (keyboardMargin != 0)
            solfege->removeMargins();
    }
    else
        {
        transposeSwitch = false;
        transposeButton->setChecked(false);
        }
}


void Switcher::toggleSustainSlot()
{
    if (sustainSwitch == false)
    {
        if (transposeSwitch == true)
            toggleTransposeSlot();

        sustainSwitch = true;
        sustainButton->setChecked(true);

    }
    else
    {
        sustainSwitch = false;
        sustainButton->setChecked(false);
        playChordButton->setEnabled(false);

        for (int i = 0; i < chord.size(); i++)
            chord[i]->release();

        chord.clear();
    }
}

void Switcher::playSlot()
{
    sustainSwitch = false;
    for (int i = 0; i < chord.size(); i++)
        chord[i]->release();
    sustainSwitch = true;
    for (int i = 0; i < chord.size(); i++)
        chord[i]->press(false, true);
}

void Switcher::scroll() const
{
    int gap = transPiano->keyNumber - transSolfegeKeyNum;
    const Key * moveKey = solfege->getKey(transPiano->keyNumber);
    int pixToMove = moveKey->absPosition().x() - transSolfegeKeyPos;

    if (gap > 0) //shift left, i.e. scroll right
        solfege->scrollRight(pixToMove);

    else if (gap < 0) //shift right, i.e. scroll left
        solfege->scrollLeft(-pixToMove);
}
