#ifndef QSINGLETON_H
#define QSINGLETON_H

#include <QSharedPointer>

template <typename T> class QSingleton
{
public:
    static T* Instance();
    static void clear();

protected:
    QSingleton() {}
    virtual ~QSingleton() {}

private:
    Q_DISABLE_COPY(QSingleton)
    static QSharedPointer<T> m_instance;
};

template <typename T>
QSharedPointer<T>  QSingleton<T>::m_instance = QSharedPointer<T>(new T, [](T *obj){
            if(obj)
            {
                obj->disconnect();
                obj->deleteLater();
                obj = nullptr;
            }
        });

template<typename T>
T* QSingleton<T>::Instance()
{
    return m_instance.data();
}

template<typename T>
void QSingleton<T>::clear()
{
    m_instance.clear();
}

#endif // QSINGLETON_H
