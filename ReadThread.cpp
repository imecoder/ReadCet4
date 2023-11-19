#include "ReadThread.h"
#include <QDebug>
#include <QString>

#include "fun.h"

ReadThread::ReadThread(QObject *parent)
    : QThread(parent),
      m_bPause(false),
      m_bStop(false)
{

}

ReadThread::~ReadThread()
{
    stop();
}

ReadThread::State ReadThread::state() const
{
    State s = Stoped;
    if (!QThread::isRunning())
    {
        s = Stoped;
    }
    else if (QThread::isRunning() && m_bPause)
    {
        s = Paused;
    }
    else if (QThread::isRunning() && (!m_bPause))
    {
        s = Running;
    }
    return s;
}

void ReadThread::config(const QStringListPtr &pslLineRead)
{
    m_pslLineRead = pslLineRead;
}

void ReadThread::start(QThread::Priority pri)
{
    QThread::start(pri);
}

void ReadThread::pause()
{
    if_return (!QThread::isRunning());

    m_bPause = true;
    emit do_thread_paused();
}

void ReadThread::resume()
{
    if_return (!QThread::isRunning());

    m_bPause = false;
    m_condition.wakeAll();
    emit do_thread_resumed();
}

void ReadThread::stop()
{
    if_return (!QThread::isRunning()) ;

    m_bStop = true;
    m_condition.wakeAll();
    QThread::wait();
    QThread::quit();
}


void ReadThread::run()
{
    qDebug() << "线程进入.";
    emit do_thread_started();

    for(auto& sLine : *m_pslLineRead) {
        if(m_bStop) {
            m_bStop = false;
            break;
        }

        if (m_bPause)
        {
            m_mutex.lock();
            m_condition.wait(&m_mutex);
            m_bPause = false;
            m_mutex.unlock();

            if(m_bStop) {
                m_bStop = false;
                break;
            }
        }

        qDebug() << sLine;
        emit do_read_word(sLine);

        QString mp3file = QCoreApplication::applicationDirPath()+"/audio/"+ sLine.split(" ").at(0) +".mp3";
//        qDebug() << mp3file;
        QMediaPlayer player;
        player.setMedia(QUrl::fromLocalFile(mp3file));
        connect(&player, SIGNAL(error(QMediaPlayer::Error)),
                this, SLOT(slot_play_error(QMediaPlayer::Error)));
        player.play();

        QThread::msleep(2000);
    }

    qDebug() << "线程退出.";
    emit do_thread_finished();
}

void ReadThread::slot_play_error(QMediaPlayer::Error error)
{
    qDebug() << error << "播放失败";
}
