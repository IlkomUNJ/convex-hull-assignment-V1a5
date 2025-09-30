#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include "drawingcanvas.h"

class DrawingWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DrawingWindow(QWidget *parent = nullptr);

private:
    DrawingCanvas *m_canvas;
    QPushButton *m_computeButton;
    QPushButton *m_clearButton;
};

#endif // DRAWINGWINDOW_H
