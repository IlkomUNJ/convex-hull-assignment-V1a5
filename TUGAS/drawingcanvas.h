#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>

class DrawingCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void clearCanvas();
    void computeSlowHull();
    void computeFastHull();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> m_points;
    QVector<QPoint> m_hullPoints;

    qulonglong m_iterations = 0;
    QString m_algorithmName = "None";

    int orientation(const QPoint& p,const QPoint& q,const QPoint& r) const;

    QVector<QPoint> slowConvexHull(const QVector<QPoint>& P);
    QVector<QPoint> monotoneChainConvexHull(const QVector<QPoint>& P);
};

#endif // DRAWINGCANVAS_H
