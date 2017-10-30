#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    filesForLines = QStringList() << "feher" << "fekete" << "szurke-lo" << "szurke-marha" << "vilagos-szurke-muanyag";
    artificial_zebra = "mu-zebra";
    real_zebra = "valodi-zebra";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToList(QString str)
{
    if(ui->listWidget->count() > 100)
        ui->listWidget->takeItem(0);
    ui->listWidget->addItem(str);
}

QStringList MainWindow::readDirectories(QString dirName)
{
    QDir dir;
    dir.setPath(dirName);
    QStringList directoryList = dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList sortedDirectoryList;

    if(!directoryList.isEmpty()){
        QMap<int, QString> hourMap;
        foreach(QString current, directoryList){
            int time = QString(current.split("-").first()).toInt();
            hourMap[time] = current;
        }
        QList<int> timeIndexList = hourMap.keys();
        qSort(timeIndexList.begin(), timeIndexList.end());
        foreach(int currentIndex, timeIndexList)
            sortedDirectoryList.append(dir.path() + "/" + hourMap[currentIndex]);
    }
    return sortedDirectoryList;
}

void MainWindow::SelectAndSaveHeatData(QStringList folderList)
{
    if(folderList.isEmpty()){
        addToList("Directory list empty");
    }
    else{
        QDir currentFolder;
        foreach(QString currentDirPath, folderList){
            currentFolder.setPath(currentDirPath);
            QStringList currentFileList = currentFolder.entryList(QStringList("*table.csv"), QDir::Files | QDir::NoDotAndDotDot);
            foreach(QString currentFile, currentFileList){
                QFile file(currentFolder.path() + "/" + currentFile);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    addToList(currentFile + "could not be opened.");
                QTextStream stream(&file);
                foreach(QString keyword, filesForLines){
                    if(currentFile.contains(keyword) && currentFile.contains("1_table")){
                        QPair<QString, QPair<double, double>> heatData;
                        heatData.first = currentFile;
                        while(!stream.atEnd()){
                            QString line = stream.readLine();
                            if(line.startsWith("Line average")){
                                heatData.second.first = QString(line.split("\t").last()).toDouble();
                            }
                            if(line.startsWith("Line StD")){
                                heatData.second.second = QString(line.split("\t").last()).toDouble();
                            }
                        }
                        heatDataMap[keyword + "_" + "1_table"].append(heatData);
                    }
                    if(currentFile.contains(keyword) && currentFile.contains("2_table")){
                        QPair<QString, QPair<double, double>> heatData;
                        heatData.first = currentFile;
                        while(!stream.atEnd()){
                            QString line = stream.readLine();
                            if(line.startsWith("Line average")){
                                heatData.second.first = QString(line.split("\t").last()).toDouble();
                            }
                            if(line.startsWith("Line StD")){
                                heatData.second.second = QString(line.split("\t").last()).toDouble();
                            }
                        }
                        heatDataMap[keyword + "_" + "2_table"].append(heatData);
                    }
                }
                if(currentFile.contains(artificial_zebra)){
                    QPair<QString, QPair<double, double>> heatData;
                    heatData.first = currentFile;
                    while(!stream.atEnd()){
                        QString line = stream.readLine();
                        if(line.startsWith("Average")){
                            heatData.second.first = QString(line.split("\t").last()).toDouble();
                        }
                        if(line.startsWith("StD")){
                            heatData.second.second = QString(line.split("\t").last()).toDouble();
                        }
                    }
                    heatDataMap[artificial_zebra].append(heatData);
                }
                if(currentFile.contains(real_zebra) && currentFile.contains("h_table")){
                    QPair<QString, QPair<double, double>> heatData;
                    heatData.first = currentFile;
                    while(!stream.atEnd()){
                        QString line = stream.readLine();
                        if(line.startsWith("Average")){
                            heatData.second.first = QString(line.split("\t").last()).toDouble();
                        }
                        if(line.startsWith("StD")){
                            heatData.second.second = QString(line.split("\t").last()).toDouble();
                        }
                    }
                    heatDataMap[real_zebra + "_" + "h_table"].append(heatData);
                }
                if(currentFile.contains(real_zebra) && currentFile.contains("v_table")){
                    QPair<QString, QPair<double, double>> heatData;
                    heatData.first = currentFile;
                    while(!stream.atEnd()){
                        QString line = stream.readLine();

                        if(line.startsWith("Average")){
                            heatData.second.first = QString(line.split("\t").last()).toDouble();
                        }
                        if(line.startsWith("StD")){
                            heatData.second.second = QString(line.split("\t").last()).toDouble();
                        }
                    }
                    heatDataMap[real_zebra + "_" + "v_table"].append(heatData);
                }

                file.close();
            }
        }

        foreach(QString currentKey, heatDataMap.keys()){
            QFile outFile(currentFolder.path().remove(currentFolder.path().split("/").last()) + currentKey + ".csv");
            if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
                addToList(outFile.fileName() + "could not be opened.");
            QTextStream outStream(&outFile);
            for(int i = 0; i < heatDataMap[currentKey].size(); i++){
                outStream << heatDataMap[currentKey].at(i).first + "\t"
                             + QString::number(heatDataMap[currentKey].at(i).second.first) + "\t"
                             + QString::number(heatDataMap[currentKey].at(i).second.second) + "\n";
            }
            outFile.close();
            addToList(outFile.fileName() + " writing finished.");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    directoryList.clear();
    heatDataMap.clear();

    QString dirName = QFileDialog::getExistingDirectory(this, "Open directory", "../../");
    directoryList = readDirectories(dirName);
    SelectAndSaveHeatData(directoryList);
}
