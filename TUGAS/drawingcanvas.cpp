#include "drawingcanvas.h"
#include <QPainter>
#include <algorithm>
#include <QPolygon>
#include <QSet>

DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400, 400);
}

// Menghitung orientasi tiga titik (p, q, r): > 0 (Kiri/CCW), < 0 (Kanan/CW), = 0 (Koliner)
int DrawingCanvas::orientation(const QPoint& p, const QPoint& q, const QPoint& r) const
{
    qreal val = (q.x() - p.x()) * (r.y() - q.y()) - (q.y() - p.y()) * (r.x() - q.x());
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

// ALGORITMA SLOW CONVEX HULL (O(n^3))
QVector<QPoint> DrawingCanvas::slowConvexHull(const QVector<QPoint>& P)
{
    m_iterations = 0;
    if (P.size() < 3) return P;

    QSet<QPoint> hullPointsSet;

    for (int i = 0; i < P.size(); ++i) {
        for (int j = 0; j < P.size(); ++j) {
            if (i == j) continue;
            const QPoint& p = P[i];
            const QPoint& q = P[j];

            bool isHullEdge = true;

            for (int k = 0; k < P.size(); ++k) {
                if (k == i || k == j) continue;
                m_iterations++;
                const QPoint& r = P[k];
                if (orientation(p, q, r) > 0) {
                    isHullEdge = false;
                    break;
                }
            }

            if (isHullEdge) {
                hullPointsSet.insert(p);
                hullPointsSet.insert(q);
            }
        }
    }

    QVector<QPoint> hull = hullPointsSet.values();

    // Urutkan titik hull searah jarum jam
    if (hull.size() > 2) {
        QPoint anchor = *std::min_element(hull.begin(), hull.end(),
                                          [](const QPoint& a, const QPoint& b) {
                                              return (a.y() < b.y()) || (a.y() == b.y() && a.x() < b.x());
                                          });

        std::sort(hull.begin(), hull.end(), [&](const QPoint& a, const QPoint& b) {
            qreal cross = (a.x() - anchor.x()) * (b.y() - anchor.y()) -
                          (a.y() - anchor.y()) * (b.x() - anchor.x());
            if (qAbs(cross) < 0.0001) {
                qreal da = (a.x() - anchor.x()) * (a.x() - anchor.x()) +
                           (a.y() - anchor.y()) * (a.y() - anchor.y());
                qreal db = (b.x() - anchor.x()) * (b.x() - anchor.x()) +
                           (b.y() - anchor.y()) * (b.y() - anchor.y());
                return da < db;
            }
            return cross < 0;
        });
    }

    return hull;
}

// ALGORITMA MONOTONE CHAIN (O(n log n))
QVector<QPoint> DrawingCanvas::monotoneChainConvexHull(const QVector<QPoint>& P)
{
    m_iterations = 0;
    if (P.size() < 3) return P;

    QVector<QPoint> sortedP = P;
    std::sort(sortedP.begin(), sortedP.end(), [](const QPoint& a, const QPoint& b) {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    });

    QVector<QPoint> upper, lower;

    for (const QPoint& p : sortedP) {
        while (upper.size() >= 2) {
            m_iterations++; // hitung setiap cek orientasi
            if (orientation(upper[upper.size()-2], upper.last(), p) >= 0) {
                upper.removeLast();
            } else break;
        }
        upper.append(p);
    }

    for (int i = sortedP.size()-1; i >= 0; --i) {
        const QPoint& p = sortedP[i];
        while (lower.size() >= 2) {
            m_iterations++; // hitung setiap cek orientasi
            if (orientation(lower[lower.size()-2], lower.last(), p) >= 0) {
                lower.removeLast();
            } else break;
        }
        lower.append(p);
    }

    QVector<QPoint> hull = upper;
    for (int i = 1; i < lower.size()-1; ++i) hull.append(lower[i]);
    return hull;
}

void DrawingCanvas::clearCanvas()
{
    m_points.clear();
    m_hullPoints.clear();
    m_iterations = 0;
    m_algorithmName = "None";
    update();
}

void DrawingCanvas::computeSlowHull()
{
    m_hullPoints = slowConvexHull(m_points);
    m_algorithmName = "SLOW CONVEX HULL";
    update();
}

void DrawingCanvas::computeFastHull()
{
    m_hullPoints = monotoneChainConvexHull(m_points);
    m_algorithmName = "MONOTONE CHAIN";
    update();
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_points.append(event->pos());
        m_hullPoints.clear();
        m_iterations = 0;
        m_algorithmName = "None";
        update();
    }
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_hullPoints.size() >= 2) {
        painter.setPen(QPen(Qt::darkBlue, 3, Qt::SolidLine));
        painter.setBrush(QColor(100, 100, 255, 50));

        QPolygon hullPolygon;
        for (const QPoint& p : m_hullPoints) hullPolygon.append(p);
        painter.drawPolygon(hullPolygon);
    }

    painter.setPen(QPen(Qt::darkRed, 1));
    painter.setBrush(Qt::darkRed);
    for (const QPoint& p : m_points) painter.drawEllipse(p, 5, 5);

    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Arial", 10));
    painter.drawText(10, 20, QString("Algoritma: %1").arg(m_algorithmName));
    painter.drawText(10, 40, QString("Jumlah Titik: %1").arg(m_points.size()));
    painter.drawText(10, 60, QString("Total Iterasi: %L1").arg(m_iterations));
}
