#include "coloreditor.h"
#include <QColorDialog>

ColorEditor::ColorEditor(QWidget *parent) : QWidget(parent)
{
}

ColorEditor::ColorEditor(QColor color, QWidget *parent) : ColorEditor(parent){
    this->color = color;
}

ColorEditor::~ColorEditor(){
}

QColor ColorEditor::getColor(){
    return color;
}

void ColorEditor::setColor(QColor color){
    this->color = color;
}

void ColorEditor::display(){
    QColor newColor = QColorDialog::getColor(color);
            if(newColor.isValid())
                color = newColor;
}
