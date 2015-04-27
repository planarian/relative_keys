#ifndef PIANOKEYSET_H
#define PIANOKEYSET_H

#include "keySet.h"

class QWidget;
class WhiteKey;
class FullBlackKey;
class PianoSynth;
class Instrument;
class Switcher;

class PianoKeySet : public KeySet
{
    Q_OBJECT

public:
    explicit PianoKeySet(const PianoSynth * a, const Instrument * i, const Switcher * swi, QWidget *parent = 0);
    ~PianoKeySet() {}

private:
    void arrangeBlackKeys();
    void createWhiteKeys();
    void createBlackKeys();

};

#endif // PIANOKEYSET_H

