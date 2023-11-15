#ifndef SINGLEREADTHREAD_H
#define SINGLEREADTHREAD_H

#include <QObject>
#include <QThread>
#include <memory>

using QStringPtr = std::shared_ptr<QString>;

class SingleReadThread : public QThread
{
    Q_OBJECT
public:
    SingleReadThread();
    ~SingleReadThread() override;

    void set(const QStringPtr &psGrade, const QStringPtr& psLine);

protected:
    virtual void run() override final;

private:
    QStringPtr m_psGrade = nullptr;
    QStringPtr m_psLine = nullptr;
};

#endif // SINGLEREADTHREAD_H
