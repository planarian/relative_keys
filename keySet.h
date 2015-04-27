#ifndef KEYSET_H
#define KEYSET_H

#include <QWidget>
#include "customColor.h"
#include "whiteKey.h"

class Switcher;
class Instrument;
class FullBlackKey;
class PianoSynth;
class QMouseEvent;
class QHBoxLayout;
class Key;

class KeySet : public QWidget  //abstract base class
{
    Q_OBJECT

public:
    explicit KeySet(const PianoSynth *, const Instrument *, const Switcher *, QWidget *parent = 0);
    virtual ~KeySet() {}
    void press(int, bool) const;
    void release(int) const;
    const Key * getKey(int i) const { return keyMap[i]; }
    int getWhiteKeyWidth() const { return whiteKeys[0]->width(); }
    int getNumWhiteKeys() const { return whiteKeys.size(); }

private:
    virtual void arrangeBlackKeys() = 0;
    virtual void createWhiteKeys() = 0;
    virtual void createBlackKeys() = 0;

protected:
    QHBoxLayout *layout;
    QWidget * recentPressed, * previousPressed;
    QList<WhiteKey *> whiteKeys;
    QList<FullBlackKey *> blackKeys_upper;
    QList<int> blackLocations;
    QVector<Key *> keyMap;
    const PianoSynth * audio;
    const Instrument * instr;
    const Switcher * toggles;

    static CustomColor yellow_, orange_yellow_, orange_, red_orange_,
                       red_, violet_red_, violet_, blue_violet_, blue_,
                       blue_green_, green_, yellow_green_, white_, gray_,
                       black_, darkCyan_;

    void mapKeys();
    void initializeVar();
    void assignNumbers();

    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};

#endif // KEYSET_H
