#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList filesForLines;
    QStringList directoryList;
    QString artificial_zebra, real_zebra;
    QMap<QString, QList<QPair<QString, QPair<double, double>>>> heatDataMap;

    void addToList(QString str);
    QStringList readDirectories(QString dirName);
    void SelectAndSaveHeatData(QStringList folderList);
};

#endif // MAINWINDOW_H
