#ifndef CSINGLETON_H
#define CSINGLETON_H

#include <stdio.h>
#include <memory>
#include <iostream>

template <typename T> class CSingleton
{
public:
    static T* Instance();
    static void clear();

protected:
    CSingleton() {}
    virtual ~CSingleton() {}

private:
    CSingleton(const CSingleton &) = delete;
    CSingleton &operator=(const CSingleton &) = delete;

    static std::shared_ptr<T> m_instance;
};

template <typename T>
std::shared_ptr<T>  CSingleton<T>::m_instance = std::shared_ptr<T>(new T, [](T *obj){
    if(obj)
    {
        delete obj;
        obj = nullptr;
    }
});

template<typename T>
T* CSingleton<T>::Instance()
{
    return m_instance.get();
}

template<typename T>
void CSingleton<T>::clear()
{
    m_instance.reset();
}

#endif // CSINGLETON_H
