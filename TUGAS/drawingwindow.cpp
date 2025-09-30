#include "drawingwindow.h"
#include "drawingcanvas.h"

DrawingWindow::DrawingWindow(QWidget *parent) {
    setWindowTitle("Convex Hull Simulation");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_canvas = new DrawingCanvas(this);
    mainLayout->addWidget(m_canvas, 1);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    m_computeButton = new QPushButton("Compute Slow Hull", this);
    connect(m_computeButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::computeSlowHull);
    controlLayout->addWidget(m_computeButton);

    m_computeButton = new QPushButton("Compute Fast Hull", this);
    connect(m_computeButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::computeFastHull);
    controlLayout->addWidget(m_computeButton);

    m_clearButton = new QPushButton("Clear Canvas", this);
    connect(m_clearButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::clearCanvas);
    controlLayout->addWidget(m_clearButton);

    mainLayout->addLayout(controlLayout);
}
