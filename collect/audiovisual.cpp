#include "audiovisual.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtDebug>

AudioVisual::AudioVisual(uint count, QWidget *parent) : QWidget(parent)
{
    m_i_value  = 0;
    m_i_width  = 3;
    m_i_height = 20;
    m_i_margin = 4;
    m_i_count  = count;
}

void AudioVisual::setCurValue(uint value)
{
    if(value <= m_i_count && value != m_i_value)
    {
        m_i_value = value;
#ifdef Q_OS_WIN
        update();
#else //MAC下使用
        repaint();
#endif
    }
}

QSize AudioVisual::sizeHint() const
{
    return QSize(2*(m_i_count*(m_i_width+m_i_margin)+m_i_margin)+m_i_margin, m_i_height+m_i_margin*2);
}

void AudioVisual::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    for(uint i=0;i<m_i_count;++i)
    {
        p.fillRect(QRect((m_i_count-1-i)*(m_i_width+m_i_margin)+m_i_margin, m_i_margin, m_i_width, m_i_height) ,
                   ((i<m_i_value) ? (Qt::green):(Qt::gray)));
    }

    const int x = m_i_count*(m_i_width+m_i_margin)+m_i_margin*2;

    for(uint i=0;i<m_i_count;++i)
    {
        p.fillRect(QRect(x+i*(m_i_width+m_i_margin)+m_i_margin, m_i_margin, m_i_width, m_i_height) ,
                   ((i<m_i_value) ? (Qt::green):(Qt::gray)) );
    }

}
