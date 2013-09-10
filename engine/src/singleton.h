#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutexLocker>
#include <QMutex>
/**
 * @brief Template for a singleton on the heap.
 * @attention The instance, if any, will not be deleted automatically.
 */
template<class Derived>
class Singleton
{
public:
    static Derived* instance()
    {
        if (!m_pInstance)
        {
            QMutexLocker locker(&m_mutex);
            if (!m_pInstance)
            {
                m_pInstance = new Derived;
            }
        }

        return m_pInstance;
    }

    static void deleteIfExists()
    {
        if (m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = 0;
        }
    }

    virtual ~Singleton()
    {
    }

protected:
    Singleton()
    {
    }

private:
    static Derived* m_pInstance;

    static QMutex m_mutex;
};

template<class Derived>
Derived* Singleton<Derived>::m_pInstance = 0;

template<class Derived>
QMutex Singleton<Derived>::m_mutex;

#endif // SINGLETON_H
