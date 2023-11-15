#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QStringListModel>
#include "ReadThread.h"
#include "SingleReadThread.h"

#define _Uni8(a) QString::fromUtf8(a)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void slot_read_word(const QString& sLine);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_btnCtrl_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_btnStop_clicked();


    void on_btnRestore_clicked();

private:
    bool getWords();
    bool saveWords(const QStringList& slWord);

private:
    Ui::MainWindow *ui;
    QStringList m_slLineRead;
    ReadThread* m_pReadThread = nullptr;
    QStringListModel m_slWordShowModel;
    QStringList m_slWordShow;

    SingleReadThread* m_pSingleReadThread = nullptr;
};
#endif // MAINWINDOW_H
