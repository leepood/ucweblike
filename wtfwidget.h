#ifndef WTFWIDGET_H
#define WTFWIDGET_H

#include <QWidget>
#include <QPainter>

class WTFWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WTFWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;

signals:

private:
    static int CIRCLE_RADIUS;
    static int CIRCLE_BG_COLOR;
    static int CIRCLE_INNER_RADIUS;
    static qreal sMagicNumber;

    static int CIRCLE_TRANS_DST;  // 圆圈闭合距离
    static int ARROW_ROTATE_DST;  // 箭头旋转+alpha距离
    static int BOUNCE_DST;        // 绘制水滴距离

    int mAlpha;
    int lastDownY;
    float _percent = 0;
    float _innerPercent = 0;
    float _bouncePercent = 0;

    void init();
    void drawCircle(QPainter& painter);
    void drawInnerArrow(QPainter& painter);
    void drawBounceCircle(QPainter& painter);

    void setAlpha(int alpha);
    void setTrimPercent(float percent,bool update);
    void setArrowPercent(float percent,bool update);
    void setBouncePercent(float percent,bool update);

public slots:
};

#endif // WTFWIDGET_H
