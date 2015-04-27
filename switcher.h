#ifndef SWITCHER_H
#define SWITCHER_H

#include <QWidget>

class Solfege;
class Instrument;
class QToolButton;
class QHBoxLayout;
class Key;
class ResizeDialog;
class FramedSolfege;

class Switcher: public QWidget
{
    Q_OBJECT
public:
    Switcher(QWidget *parent = 0);
    ~Switcher() {}
    void setSolfege(FramedSolfege * k) { solfege = k; }

    QList<Key *> chord;
    bool transposeSwitch, sustainSwitch;

public slots:
    void mostRecentPressSlot(Key *);

private slots:
    void toggleTransposeSlot();
    void toggleSustainSlot();
    void playSlot();

private:
    int checkDuplicates(const QList<Key *> &, const Key *) const;
    int calculateOffset();
    void transpose();
    void sustain();
    void scroll() const;
    FramedSolfege * solfege;
    Key * transKey, * latest;
    const Key * transPiano;
    const Key * transSolfege;
    QToolButton * transposeButton, * sustainButton, * playChordButton;
    QHBoxLayout * buttonRow;
    int transSolfegeKeyNum, transSolfegeKeyPos, solfegeKeyWidth;

};

#endif // SWITCHER_H

