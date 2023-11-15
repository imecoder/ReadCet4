#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QObject>
#include <atomic>
#include <QMutex>
#include <QWaitCondition>

using QStringListPtr = std::shared_ptr<QStringList>;

class ReadThread : public QThread
{
    Q_OBJECT
public:
    ReadThread(QObject *parent = nullptr);
    ~ReadThread() override;

    enum State
    {
        Stoped,     //<停止状态
        Running,    //<运行状态
        Paused      //<暂停状态
    };

public:
    State state() const;
    void config(const QString& sGrade,
                const QStringListPtr& pslLineRead);

    void start(Priority pri = InheritPriority);
    void pause();
    void resume();
    void stop();

protected:
    virtual void run() override final;

signals:
    void do_thread_started();
    void do_thread_paused();
    void do_thread_resumed();
    void do_thread_finished();
    void do_read_word(const QString& sLine);

private:
    std::atomic_bool    m_bPause;
    std::atomic_bool    m_bStop;
    QMutex              m_mutex;
    QWaitCondition      m_condition;

    QString m_sGrade  = "cet4";
    QStringListPtr m_pslLineRead = nullptr;
};

#endif // READTHREAD_H
