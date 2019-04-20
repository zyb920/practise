#ifndef AUDIOVISUAL_H
#define AUDIOVISUAL_H

#include <QWidget>

class AudioVisual : public QWidget
{
    Q_OBJECT
public:
    explicit AudioVisual(uint count, QWidget *parent = nullptr);

    virtual QSize sizeHint() const;

public slots:
    void setCurValue(uint value);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    uint m_i_value;
    int m_i_width;//宽度
    int m_i_height;//高度
    int m_i_margin;//间距
    uint m_i_count;//个数
};

#endif // AUDIOVISUAL_H
