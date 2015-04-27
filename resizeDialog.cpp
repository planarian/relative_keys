#include "resizeDialog.h"
#include "mainWindow.h"
#include "framedSolfege.h"
#include <QSlider>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QLabel>

ResizeDialog::ResizeDialog(QWidget * parent, Qt::WindowFlags f): QDialog(parent, f)
{    
    parentWindow = qobject_cast<MainWindow *> (parent);
    solfege = parentWindow->getFramedSolfege();
    solfegeScrollBar = solfege->horizontalScrollBar();

    this->setModal(true);
    this->setWindowTitle("Stretch Keyboard");
    this->setFixedSize(400,200);

    int minKeyWidth = solfege->minimumKeyWidth();

    widthSlider = new QSlider(Qt::Horizontal, this);
    widthSlider->setRange((minKeyWidth > 16) ? minKeyWidth : 16, 64);
    widthSlider->setSliderPosition(parentWindow->solfegeKeyWidth);


    heightSlider = new QSlider(Qt::Horizontal, this);
    heightSlider->setRange(80,500);
    heightSlider->setSliderPosition(parentWindow->solfegeInternalHeight);

    widthLabel = new QLabel(this);
    widthLabel->setText("Width");

    heightLabel = new QLabel(this);
    heightLabel->setText("Height");

    layout = new QVBoxLayout();
    layout->addWidget(widthLabel);
    layout->addWidget(widthSlider);
    layout->addWidget(heightLabel);
    layout->addWidget(heightSlider);
    layout->addSpacing(20);
    this->setLayout(layout);

    connect(widthSlider, SIGNAL(valueChanged(int)), this, SLOT (widthResizeSlot(int)));
    connect(heightSlider, SIGNAL(valueChanged(int)), this, SLOT (heightResizeSlot(int)));

}

void ResizeDialog::widthResizeSlot(int newSize)
{
    float scrollOldValue = solfegeScrollBar->value();
    float scrollOldMax = solfegeScrollBar->maximum();
    parentWindow->solfegeKeyWidthResize(newSize);
    float scrollNewMax = solfegeScrollBar->maximum();
    solfegeScrollBar->setValue(scrollOldValue * scrollNewMax / scrollOldMax);
}

void ResizeDialog::heightResizeSlot(int newSize)
{
    parentWindow->solfegeHeightResize(newSize);
}
