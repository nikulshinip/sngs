#ifndef SENSORCOLOREDITOR_H
#define SENSORCOLOREDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

//! диалог выбора света графиков
class ColorEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColorEditor(QWidget *parent = nullptr);
    ColorEditor(QColor color, QWidget *parent = nullptr);
    ~ColorEditor();

    QColor getColor();
    void setColor(QColor color);
    void display();

private:
    QColor color;

};

#endif // SENSORCOLOREDITOR_H
