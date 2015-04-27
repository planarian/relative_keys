#include "framedSolfege.h"
#include "solfege.h"
#include <QPushButton>
#include "resizeDialog.h"
#include <QScrollBar>
#include <QHBoxLayout>
#include "key.h"
#include <QTimer>
#include "mainWindow.h"

FramedSolfege::FramedSolfege(PianoSynth * syn, Instrument * inst, Switcher * swi, int offset, QWidget *parent) : QScrollArea(parent)
{
    keyboard = new Solfege(syn, inst, swi, offset, this);

    parentWindow = qobject_cast<MainWindow *> (parent);
    keyboardMargin = 0;
    justResized = false;
    leftMargin = new int;
    rightMargin = new int;
    topMargin = new int;
    bottomMargin = new int;

    scrollBarLast = horizontalScrollBar()->value();
    keyboard->instLayout->getContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWidget(keyboard);
    stretchButton = new QPushButton();

    marginTimer = new QTimer(this);
    connect(marginTimer, SIGNAL(timeout()), this, SLOT(removeMarginSlot()));

    blinkTimer = new QTimer(this);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkSlot()));

    connect(stretchButton, SIGNAL(clicked()), this, SLOT(resizeSlot()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));

    addScrollBarWidget(stretchButton,Qt::AlignRight);
    stretchButton->setMaximumWidth(stretchButton->height());

}

void FramedSolfege::heightResize(int newHeight) const
{
    keyboard->resize(keyboard->width(), newHeight);
}

void FramedSolfege::keyWidthResize(int newWidth) const
{
    keyboard->keyWidthResize(newWidth, abs(keyboardMargin));
}

void FramedSolfege::scrollLeft(int numPixels)
{
    int current = horizontalScrollBar()->value();
    int pixAfterCollision = numPixels - current;

    if (pixAfterCollision > 0)
    {
        if (keyboardMargin > 0)
        {
            setKeyBoardMarginRight(-keyboardMargin);
            horizontalScrollBar()->setValue(0);
            setKeyBoardMarginLeft(pixAfterCollision);
            horizontalScrollBar()->setValue(0);
        }
        else
        {
            setKeyBoardMarginLeft(pixAfterCollision);
            horizontalScrollBar()->setValue(current - numPixels);
        }
    }
    else horizontalScrollBar()->setValue(current - numPixels);
}

void FramedSolfege::scrollRight(int numPixels)
{
    int current = horizontalScrollBar()->value();
    int pixAfterCollision = horizontalScrollBar()->maximum() - (numPixels + current);

    if (pixAfterCollision < 0)
    {
        if (keyboardMargin < 0)
        {
            setKeyBoardMarginLeft(keyboardMargin);
            horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
            setKeyBoardMarginRight(-pixAfterCollision);
            horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
        }
        else
        {
            setKeyBoardMarginRight(-pixAfterCollision);
            horizontalScrollBar()->setValue(current + numPixels);
        }
    }
    else horizontalScrollBar()->setValue(current + numPixels);
}

void FramedSolfege::setKeyBoardMarginLeft(int offset)
{
    *leftMargin += offset;
    keyboard->instLayout->setContentsMargins(*leftMargin, *topMargin, *rightMargin, *bottomMargin);
    justResized = true;
    keyboard->resize(keyboard->width() + offset, keyboard->height());
    keyboardMargin -= offset;
    justResized = false;
}

void FramedSolfege::setKeyBoardMarginRight(int offset)
{
    *rightMargin += offset;
    keyboard->instLayout->setContentsMargins(*leftMargin, *topMargin, *rightMargin, *bottomMargin);
    justResized = true;
    keyboard->resize(keyboard->width() + offset, keyboard->height());
    keyboardMargin += offset;
    justResized = false;
}

void FramedSolfege::setOffset(int offset)
{
   keyboard->setOffset(offset);
}

int FramedSolfege::getOffset()
{
    return keyboard->getOffset();
}

const Solfege * FramedSolfege::getKeyboard() const
{
    return keyboard;
}

const Key * FramedSolfege::getKey(int keyNum) const
{
    return keyboard->getKey(keyNum);
}


void FramedSolfege::valueChangedSlot(int newValue)
{
   int difference = scrollBarLast - newValue;
   scrollBarLast = newValue;

   if (!justResized) //bug fix: setKeyboardMargin methods trigger valueChanged signal -> setKeyboardMargin cycle
   {
       if (difference > 0) //scroll left
       {
           if (keyboardMargin > 0)
                {
                    if (difference < keyboardMargin)
                        setKeyBoardMarginRight(-difference);
                    else setKeyBoardMarginRight(-keyboardMargin);
                }
       }
       else if (difference < 0) //scroll right
           if (keyboardMargin < 0)
           {
               if (difference > -100) //anything bigger is a hidden-key jump
                   horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->minimum()); //hack to fix Qt bug(?) that prevents correct scrollbar stretching
               if (difference > keyboardMargin)
                   setKeyBoardMarginLeft(difference);
               else setKeyBoardMarginLeft(keyboardMargin);

           }
   }
   justResized = false;
}

void FramedSolfege::resizeSlot()
{
    if (keyboardMargin != 0)
        removeMargins();
    stretchBox = new ResizeDialog(parentWindow, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    stretchBox->show();
}

void FramedSolfege::notVisibleSlot(Key * invisibleKey)
{
    if (keyboardMargin < 0)
        setKeyBoardMarginLeft(keyboardMargin); //prevents overjump when there's a left margin

    int viewportCornerX = (viewport()->mapToGlobal(geometry().topRight())).x();
    int viewportWidth = viewport()->width();
    int viewportEdgeToKey = (invisibleKey->mapToGlobal(QPoint(0,0))).x() - viewportCornerX;
    int viewportCenterToKey = viewportWidth/2 + viewportEdgeToKey;

    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + viewportCenterToKey);
    //by inputting values outside scroll range possibly uses undefined behavior
}

int FramedSolfege::minimumKeyWidth()
{
    int viewportWidth = maximumViewportSize().width();
    int numWhiteKeys = getKeyboard()->getNumWhiteKeys();
    return (viewportWidth - 36 /*magic num*/ - abs(keyboardMargin))/numWhiteKeys /* int div */ + 1;
}

void FramedSolfege::removeMargins()
{
    marginTimer->start(1);
}

void FramedSolfege::removeMarginSlot()
{
    if (keyboardMargin > 0)
    {
        if (keyboardMargin > 25) /*number dictates speed of scroll*/
            scrollLeft(25);
        else scrollLeft(keyboardMargin);
    }
    else if (keyboardMargin < 0)
    {
        if (keyboardMargin < -25)
            scrollRight(25);
        else scrollRight(-keyboardMargin);
        horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->minimum()); //hack to fix Qt bug(?) that prevents correct scrollbar stretching
    }
    else marginTimer->stop();
}

void FramedSolfege::blink()
{
   keyboard->blink();
   blinkTimer->start(300);
}

void FramedSolfege::blinkSlot()
{
    keyboard->blink();
    blinkTimer->stop();
}
