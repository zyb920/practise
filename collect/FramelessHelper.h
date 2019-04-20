#ifndef FRAMELESSHELPER_H
#define FRAMELESSHELPER_H

#include <QObject>

class FramelessHelperPrivate;

class FramelessHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FramelessHelper)
public:
    explicit FramelessHelper(QObject *parent = nullptr);
    ~FramelessHelper();
    void setMargin(uint margin);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    FramelessHelperPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(FramelessHelper)
};

#endif // FRAMELESSHELPER_H
