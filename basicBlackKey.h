#ifndef BASICBLACKKEY_H
#define BASICBLACKKEY_H

#include "key.h"

class Switcher;

class BasicBlackKey : public Key
{
    Q_OBJECT
public:
    BasicBlackKey(const Switcher *, const CustomColor & , const CustomColor & , QWidget * parent = 0);
    ~BasicBlackKey() {}
    void press(bool directClick = false, bool chordPlay = false);
    void release(bool directClick = false);

signals:
    void pressSignal(bool chordPlay = false);
    void releaseSignal(bool directClick = false);

public slots:
    void sympatheticPressSlot(bool chordPlay = false);
    void sympatheticReleaseSlot();

};

#endif // BASICBLACKKEY_H
