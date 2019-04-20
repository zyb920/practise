#include "d_p_point_test.h"

class D_P_Point_TestPrivate
{
public:
    D_P_Point_TestPrivate(D_P_Point_Test *parent)
        : q_ptr(parent)
    {
    }
    void foobar()
    {
        Q_Q(D_P_Point_Test);
        emit q->dummySignal();
    }

private:
    D_P_Point_Test * const q_ptr;
    Q_DECLARE_PUBLIC(D_P_Point_Test)
};

D_P_Point_Test::D_P_Point_Test(QObject *parent) :
    QObject(parent) , d_ptr(new D_P_Point_TestPrivate(this))
{
}

D_P_Point_Test::~D_P_Point_Test()
{
    Q_D(D_P_Point_Test);
    delete d;
}

void D_P_Point_Test::dummyFunc()
{
    Q_D(D_P_Point_Test);
    d->foobar();
}
