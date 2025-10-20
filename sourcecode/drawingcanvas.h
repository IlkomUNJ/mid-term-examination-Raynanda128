#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <array>

using Array3x3 = std::array<std::array<bool, 3>, 3>;

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void clearPoints();
    void paintLines();
    void detectAndMarkSegments();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    const int WINDOW_WIDTH=600;
    const int WINDOW_HEIGHT=400;
    QVector<QPoint> m_points;
    QVector<QPoint> m_candidatePoints;
    QVector<Array3x3> m_idealPatterns;
    bool isPaintLinesClicked = false;

    bool compareWindows(const Array3x3& w1, const Array3x3& w2);
};
#endif // DRAWINGCANVAS_H
