#include "FramelessHelper.h"
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QtDebug>
#include <QGuiApplication>
#include <QCursor>
#include <QPoint>
#include <QRect>

int defaultWH = 18;
#define Region_Count 9

class FramelessHelperPrivate
{
public:
    //九宫格
    enum WidgetRegion
    {
        TopLeft = 0,
        Top,
        TopRight,
        Left,
        Inner,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Unknown
    };

    FramelessHelperPrivate(FramelessHelper *p) : q_ptr(p)
    {
        i_margin = 6;
        m_b_pressed = false;
        m_p_parentWidget = qobject_cast<QWidget *>(p->parent());
        if(m_p_parentWidget)
        {
            m_p_parentWidget->setMouseTracking(true);
            m_p_parentWidget->setWindowFlags(Qt::FramelessWindowHint);
            m_p_parentWidget->installEventFilter(p);
        }
    }

    void updateCursor(const QPoint &point);
    FramelessHelperPrivate::WidgetRegion hitTest(const QPoint &point);
    void positionChange(const QPoint &newPosition, bool directX, bool directY);
    void reinitRegions(int w, int h);
    void resetMargins(uint margin);
    void move(const QPoint &point);

    void mousePressed(const QPoint &point);
    void mouseMoved(const QPoint &point);
    void mouseReleased();

private:
    bool m_b_pressed;

    uint i_margin;

    //按下坐标  大小
    QSize  m_pressedSize;
    QPoint m_pressedPoint;
    FramelessHelperPrivate::WidgetRegion m_region;

    // 九宫格，对应9个区域
    QRect m_regions[Region_Count];

    QWidget *m_p_parentWidget;

private:
    FramelessHelper * const q_ptr;
    Q_DECLARE_PUBLIC(FramelessHelper)
};

void FramelessHelperPrivate::updateCursor(const QPoint &point)
{
    FramelessHelperPrivate::WidgetRegion region = hitTest(point);
    switch (region)
    {
    case Top:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        break;
    case TopRight:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case Right:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        break;
    case BottomRight:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case Bottom:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        break;
    case BottomLeft:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case Left:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        break;
    case TopLeft:
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    default:
        QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        break;
    }
}


FramelessHelperPrivate::WidgetRegion FramelessHelperPrivate::hitTest(const QPoint &point)
{
    for(int i=0; i<Region_Count; ++i)
    {
        if( m_regions[i].contains(point) )
            return WidgetRegion(i);
    }
    return WidgetRegion::Unknown;
}


void FramelessHelperPrivate::resetMargins(uint margin)
{
    i_margin = margin;

    reinitRegions(m_p_parentWidget->width(), m_p_parentWidget->height());
}


void FramelessHelperPrivate::reinitRegions(int width, int height)
{
    m_regions[Top]         = QRect(i_margin, 0, width - i_margin - i_margin, i_margin);
    m_regions[TopRight]    = QRect(width - i_margin, 0, i_margin, i_margin);
    m_regions[Right]       = QRect(width - i_margin, i_margin, i_margin, height - i_margin*2);
    m_regions[BottomRight] = QRect(width - i_margin, height - i_margin, i_margin, i_margin);
    m_regions[Bottom]      = QRect(i_margin, height - i_margin, width - i_margin*2, i_margin);
    m_regions[BottomLeft]  = QRect(0, height - i_margin, i_margin, i_margin);
    m_regions[Left]        = QRect(0, i_margin, i_margin, height - i_margin*2);
    m_regions[TopLeft]     = QRect(0, 0, i_margin, i_margin);
    m_regions[Inner]       = QRect(i_margin, i_margin, width - i_margin*2, height - i_margin*2);
}


void FramelessHelperPrivate::positionChange(const QPoint &newPosition, bool directX, bool directY)
{
    const QPoint delta = QPoint(newPosition.x() - m_pressedPoint.x(), newPosition.y() - m_pressedPoint.y());

    QWidget * const pw = m_p_parentWidget;

    int x = pw->x();
    int y = pw->y();
    int w = pw->width();
    int h = pw->height();

    //防止没有设置最小值导致为0出问题
    int minimumWidth  = pw->minimumWidth();
    int minimumHeight = pw->minimumHeight();
    if(minimumWidth < defaultWH)
        minimumWidth = defaultWH;
    if(minimumHeight < defaultWH)
        minimumHeight = defaultWH;

    int tmpW=0, tmpH=0;
    if(directX)
        tmpW = m_pressedSize.width() + delta.x();
    else
        tmpW = w - delta.x();

    if(directY)
        tmpH = m_pressedSize.height() + delta.y();
    else
        tmpH = h - delta.y();


    if(tmpW <= minimumWidth)
    {
        if(!directX)
            x += (w - minimumWidth);
        w = minimumWidth;
    }
    else if(tmpW <= pw->maximumWidth())
    {
        w = tmpW;
        if(!directX)
        {
            x += delta.x();
        }
    }

    if(tmpH <= minimumHeight)
    {
        if(!directY)
            y += (h - minimumHeight);
        h = minimumHeight;
    }
    else if(tmpH <= pw->maximumHeight() )
    {
        h = tmpH;
        if(!directY)
        {
            y += delta.y();
        }
    }

    pw->setGeometry(x,y,w,h);
}


void FramelessHelperPrivate::move(const QPoint &point)
{
    QPoint delta = QPoint(point.x() - m_pressedPoint.x(), point.y() - m_pressedPoint.y() );
    const int x = m_p_parentWidget->x() + delta.x();
    const int y = m_p_parentWidget->y() + delta.y();
    m_p_parentWidget->move(x, y);
}

void FramelessHelperPrivate::mousePressed(const QPoint &point)
{
    m_b_pressed = true;

    m_pressedPoint = point;
    m_region      = hitTest(point);
    m_pressedSize  = m_p_parentWidget->size();
}

void FramelessHelperPrivate::mouseMoved(const QPoint &point)
{
    if(!m_b_pressed)
    {
        updateCursor(point);
    }
    else
    {
        //检查按下所在区域
        switch(m_region)
        {
        case FramelessHelperPrivate::TopLeft:
            positionChange(point, false, false);
            break;
        case FramelessHelperPrivate::Top:
            positionChange(QPoint(m_pressedPoint.x(), point.y()), true, false);
            break;
        case FramelessHelperPrivate::TopRight:
            positionChange(point, true, false);
            break;
        case FramelessHelperPrivate::Left:
            positionChange(QPoint(point.x(), m_pressedPoint.y()), false, true);
            break;
        case FramelessHelperPrivate::Right:
            positionChange(QPoint(point.x(), m_pressedPoint.y()), true, true);
            break;
        case FramelessHelperPrivate::BottomLeft:
            positionChange(point, false,true);
            break;
        case FramelessHelperPrivate::Bottom:
            positionChange(QPoint(m_pressedPoint.x(), point.y()), true, true);
            break;
        case FramelessHelperPrivate::BottomRight:
            positionChange(point, true, true);
            break;
        case FramelessHelperPrivate::Inner:
        {
            move(point);
            break;
        }
        default: break;
        }
    }
}

void FramelessHelperPrivate::mouseReleased()
{
    m_b_pressed = false;
}


//----
FramelessHelper::FramelessHelper(QObject *p) : QObject(p),
    d_ptr(new FramelessHelperPrivate(this)) {}


FramelessHelper::~FramelessHelper()
{
    Q_D(FramelessHelper);
    delete d;
}

void FramelessHelper::setMargin(uint margin)
{
    Q_D(FramelessHelper);
    d->resetMargins(margin);
}

bool FramelessHelper::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(FramelessHelper);

    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    {
        const QPoint point = static_cast<QMouseEvent *>(event)->pos();
        d->mousePressed(point);

        return true;
    }
    case QEvent::MouseMove:
    {
        //更新鼠标样式
        const QPoint point = static_cast<QMouseEvent *>(event)->pos();
        d->mouseMoved(point);

        return true;
    }
    case QEvent::MouseButtonRelease:
    {
        d->mouseReleased();
        return true;
    }
    case QEvent::Resize:
    {
        QResizeEvent *re = static_cast<QResizeEvent *>(event);

        const int width  = re->size().width();
        const int height = re->size().height();

        d->reinitRegions(width, height);

        return true;
    }
    default:
        break;
    }
    return QObject::eventFilter(watched, event);
}
