#include "SingleReadThread.h"

SingleReadThread::SingleReadThread()
{

}

SingleReadThread::~SingleReadThread()
{

}

void SingleReadThread::set(const QStringPtr &psLine)
{
    m_psLine = psLine;
}


#include <QCoreApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
void SingleReadThread::run()
{
    QString mp3file = QCoreApplication::applicationDirPath()+"/audio/"+ m_psLine->split(" ").at(0) +".mp3";
    qDebug() << mp3file;
    QMediaPlayer player;
    player.setMedia(QUrl::fromLocalFile(mp3file));
    player.play();
    QThread::msleep(2000);
}
