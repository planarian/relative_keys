#ifndef CUSTOMCOLOR_H
#define CUSTOMCOLOR_H

struct CustomColor
{
    QColor standard;
    QColor light; //adjust standard color's lum to 200
    QColor dark; //adjust standard color's lum to 60
};

#endif // CUSTOMCOLOR_H
