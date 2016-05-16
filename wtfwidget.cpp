#include "wtfwidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>

int WTFWidget::CIRCLE_RADIUS = 20;
int WTFWidget::CIRCLE_BG_COLOR = 0x000000;
int WTFWidget::CIRCLE_INNER_RADIUS = CIRCLE_RADIUS * 0.6;

int WTFWidget::CIRCLE_TRANS_DST = 20;
int WTFWidget::ARROW_ROTATE_DST = 30;
int WTFWidget::BOUNCE_DST       = 30;

qreal WTFWidget::sMagicNumber = 0.55228475;

WTFWidget::WTFWidget(QWidget *parent) : QWidget(parent)
{
    QRect parentRect = parent->contentsRect();
    setFixedSize(parentRect.width(), parentRect.height());
}


void WTFWidget::mousePressEvent(QMouseEvent* event) {
        lastDownY = event->y();
        qDebug() << "down: " << lastDownY;

}

void WTFWidget::mouseMoveEvent(QMouseEvent* event) {
       int nowY = event->y();
       int delta = nowY - lastDownY;
       if(delta < 0 )
       {
           return;
       }
       float adjustDelta = delta / 2.5f;
       if(adjustDelta <= CIRCLE_TRANS_DST) {
           float percent = adjustDelta / (float)CIRCLE_TRANS_DST;
           setTrimPercent(percent,true);
       }
       else if(adjustDelta <= (CIRCLE_TRANS_DST + ARROW_ROTATE_DST)){
            setTrimPercent(1.0f,false);
            float percent = (adjustDelta - CIRCLE_TRANS_DST) / ARROW_ROTATE_DST;
            setArrowPercent(percent,true);
       }
       else if(adjustDelta <= (CIRCLE_TRANS_DST + ARROW_ROTATE_DST + BOUNCE_DST))
       {
            setTrimPercent(1.0f,false);
            setArrowPercent(1.0f,false);
            float  percent =  (adjustDelta -  CIRCLE_TRANS_DST - ARROW_ROTATE_DST) / BOUNCE_DST;
            setBouncePercent(percent,true);
       }


//       qDebug() << "move distance " << delta << " percent: " << percent;
}

void WTFWidget::mouseReleaseEvent(QMouseEvent* event) {

}

void WTFWidget::init(){

}

void WTFWidget::setTrimPercent(float percent,bool update){
       this->_percent = percent;
        if(update)
        {
           this->update();
        }
}

void WTFWidget::setArrowPercent(float percent, bool update){
    this->_innerPercent = percent;
    if(update)
    {
        this->update();
    }
}

void WTFWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillRect(this->contentsRect(),Qt::BrushStyle::SolidPattern);
    drawCircle(painter);
    drawBounceCircle(painter);
    drawInnerArrow(painter);
    painter.end();
}


void WTFWidget::drawCircle(QPainter& painter){
    QRect rect = this->contentsRect();
    QPoint center = rect.center();
    float max = 1.0f;
    qreal alpha = qMax(0.0f,qMin(max,this->_percent)) * 0xff;
    QBrush brush(QColor::fromRgb(11,160,238,alpha));
    painter.setBrush(brush);
    QRect pieRect(center.x() - CIRCLE_RADIUS,
                  center.y() - CIRCLE_RADIUS,
                  CIRCLE_RADIUS * 2,
                  CIRCLE_RADIUS * 2);
    painter.drawPie(pieRect,1440,-this->_percent * 5760);
}

void WTFWidget::drawInnerArrow(QPainter& painter) {

    if(_innerPercent == 0) return;

    QRect rect = this->contentsRect();
    QPoint center = rect.center();
    painter.save();
    painter.translate(center);
    painter.rotate(90 * _innerPercent);

    QColor arrowColor = QColor::fromRgb(0xff,0xff,0xff,0xff * _innerPercent);
    QPen pen = QPen(arrowColor);
    pen.setWidth(2);
    painter.setPen(pen);
    int left = - CIRCLE_INNER_RADIUS;
    int top =  - CIRCLE_INNER_RADIUS;
    QRect arcRect = QRect(left,top,CIRCLE_INNER_RADIUS * 2,
                          CIRCLE_INNER_RADIUS * 2);
    painter.drawArc(arcRect,90 * 16,270 * 16);

    // start draw arrow
    qreal arrowBorderLen = 8;
    QPainterPath path;
    QPoint topPoint(0,
                    - CIRCLE_INNER_RADIUS - arrowBorderLen/2);
    path.moveTo(topPoint);
    qreal distance = (arrowBorderLen / 2) / qTan(qDegreesToRadians((double)30));
    QPoint rightPoint(distance,-CIRCLE_INNER_RADIUS);
    path.lineTo(rightPoint);
    QPoint bottomPoint(0,
                       - CIRCLE_INNER_RADIUS + arrowBorderLen/2);
    path.lineTo(bottomPoint);
    path.closeSubpath();
    painter.fillPath(path,QBrush(arrowColor));

    painter.translate(-center.x(),-center.y());
    painter.restore();
}


void WTFWidget::setBouncePercent(float percent,bool update)
{
    this->_bouncePercent = percent;
    if(update){
        this->update();
    }
}



void WTFWidget::setAlpha(int alpha){
    this->mAlpha = alpha;
}

void WTFWidget::drawBounceCircle(QPainter& painter)
{

   if(_bouncePercent == 0) return;
   QRect rect = this->contentsRect();
   QPoint center = rect.center();

   QPainterPath mPath;
   float radius = CIRCLE_RADIUS;

   float longRadius = radius * (1 + _bouncePercent) /*+ distance*/;
   float shortRadius = radius * 1.0f /*- distance * 0.1f*/;


   painter.save();
   painter.translate(center);
   painter.rotate(90);

   mPath.lineTo(0, -radius);
   mPath.cubicTo(radius * sMagicNumber, -radius
           , longRadius, -radius * sMagicNumber
           , longRadius, 0);
   mPath.lineTo(0, 0);

   mPath.lineTo(0, radius);
   mPath.cubicTo(radius * sMagicNumber, radius
           , longRadius, radius * sMagicNumber
           , longRadius, 0);
   mPath.lineTo(0, 0);

   mPath.lineTo(0, -radius);
   mPath.cubicTo(-radius * sMagicNumber, -radius
           , -shortRadius, -radius * sMagicNumber
           , -shortRadius, 0);
   mPath.lineTo(0, 0);

   mPath.lineTo(0, radius);
   mPath.cubicTo(-radius * sMagicNumber, radius
           , -shortRadius, radius * sMagicNumber
           , -shortRadius, 0);
   mPath.lineTo(0, 0);

   QBrush brush = QBrush(QColor::fromRgb(11,160,238));
   painter.fillPath(mPath,brush);

   painter.translate(-center.x(),-center.y());
   painter.restore();
}

