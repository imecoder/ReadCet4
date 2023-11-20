#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "fun.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pReadThread = new ReadThread();
    connect(m_pReadThread, &ReadThread::do_read_word,
            this, &MainWindow::slot_read_word);

    m_pSingleReadThread = new SingleReadThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_read_word(const QString &sLine)
{
    if(m_slWordShow.contains(sLine))
        return;

    m_slWordShow.append(sLine);
    m_slWordShowModel.setStringList(m_slWordShow);

    ui->lineEditWord->setText(sLine.split(" ").at(0));
    ui->labelMean->setText(sLine.split(" ").at(1));
    ui->listView->setModel(&m_slWordShowModel);
    ui->listView->scrollToBottom();
}

void MainWindow::on_btnCtrl_clicked()
{
    switch (m_pReadThread->state()) {
    case ReadThread::Running: {
        m_pReadThread->pause();
        ui->btnCtrl->setText("继续");
        break;
    }
    case ReadThread::Paused: {
        m_pReadThread->resume();
        ui->btnCtrl->setText("暂停");
        break;
    }
    default: {
        if_return(!getWords());
        m_slWordShow.clear();

        m_pReadThread->config(std::make_shared<QStringList>(m_slLineRead));
        m_pReadThread->start();
        ui->btnCtrl->setText("暂停");
        ui->btnStop->setEnabled(true);
        break;
    }
    }
}

void MainWindow::on_btnStop_clicked()
{
    m_pReadThread->stop();
    ui->btnCtrl->setText("开始");
    ui->btnStop->setEnabled(false);
}

bool MainWindow::getWords()
{
    m_slLineRead.clear();

    QFile file(QCoreApplication::applicationDirPath() + "/words/" +
               ui->boxLetter->currentText() + ".txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "提示", "读打开单词文件失败");
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        m_slLineRead.append(in.readLine());
    }

    file.close();

    return true;
}

bool MainWindow::saveWords(const QStringList &slWord)
{
    QFile file(QCoreApplication::applicationDirPath() + "/words/" +
               ui->boxLetter->currentText() + ".txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "提示", "写打开单词文件失败");
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (const QString& sWord : slWord) {
        out << sWord << endl;
    }

    file.close();
    return true;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    m_pReadThread->pause();
    ui->btnCtrl->setText("继续");
    QString sLine = m_slWordShow.at(index.row());
    ui->lineEditWord->setText(sLine.split(" ").at(0));
    ui->labelMean->setText(sLine.split(" ").at(1));

    m_pSingleReadThread->set(std::make_shared<QString>(m_slWordShow.at(index.row())));

    m_pSingleReadThread->start();
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    m_pReadThread->pause();
    ui->btnCtrl->setText("继续");

    m_slWordShow.removeAt(index.row());
    saveWords(m_slWordShow);
    m_slWordShowModel.setStringList(m_slWordShow);

    ui->listView->setModel(&m_slWordShowModel);
    if(m_pReadThread->state()==ReadThread::Running)
        ui->listView->scrollToBottom();
}

void MainWindow::on_btnRestore_clicked()
{
    on_btnStop_clicked();

    QString srcName = QCoreApplication::applicationDirPath() + "/allwords.txt";
    QString destName = QCoreApplication::applicationDirPath() + "/words/" +
            ui->boxLetter->currentText() + ".txt";

    QFile srcFile(srcName);
    if (!srcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "提示", "读打开单词文件失败");
        return;
    }
    QTextStream in(&srcFile);
    in.setCodec("UTF-8");
    QStringList slLineRead;
    while (!in.atEnd()) {
        slLineRead.append(in.readLine());
    }
    srcFile.close();


    QFile destFile(destName);
    if (!destFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "提示", "写打开单词文件失败");
        srcFile.close();
        return;
    }
    QTextStream out(&destFile);
    for (const QString& sLine : slLineRead) {
        if(!sLine.startsWith(ui->boxLetter->currentText()))
            continue;
        out << sLine << endl;
    }
    destFile.close();
}
