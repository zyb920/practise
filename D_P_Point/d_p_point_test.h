#ifndef D_P_POINT_TEST_H
#define D_P_POINT_TEST_H

#include <QObject>

class D_P_Point_TestPrivate;

class D_P_Point_Test : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(D_P_Point_Test)
    Q_DECLARE_PRIVATE(D_P_Point_Test)
public:
    explicit D_P_Point_Test(QObject *parent = 0);
    virtual ~D_P_Point_Test();

    void dummyFunc();

signals:
    void dummySignal();

private:
    D_P_Point_TestPrivate * const d_ptr;
};

#endif // D_P_POINT_TEST_H
