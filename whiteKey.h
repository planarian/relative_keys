#ifndef WHITEKEY_H
#define WHITEKEY_H

#include "key.h"

class Instrument;
class PianoSynth;
class Switcher;

class WhiteKey : public Key
{
    Q_OBJECT
public:
    WhiteKey(const PianoSynth *, const Instrument *, const Switcher *, const CustomColor &, const CustomColor &, QWidget * parent = 0);
    ~WhiteKey() {}
    void press(bool directClick = false, bool chordPlay = false);
    void release(bool directClick = false); 

protected:
    const PianoSynth * audio;
    const Instrument * instr;

};

#endif // WHITEKEY_H
