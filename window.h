/****************************************************************************
 **
 ** Copyright (C) 2012 Mathieu Moriceau
 ** All rights reserved.
 ** Contact: Watcheur (watcheur@orange.fr)
 **
 ****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QDialog>
#include <QDir>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <deque>

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

private slots:
    // First step - Seek logs
    void browse();
    void find();
    void openFileOfItem(int row, int column);

    // Second Step - Load logs
    void load_logs(std::string filename);

private:
    QStringList findFiles(const QStringList &files, const QString &text);
    void showFiles(const QStringList &files);
    QComboBox *createComboBox(const QString &text = QString());
    void createFilesTable();
    QPushButton *createButton(const QString &text, const char *member);

    QComboBox *fileComboBox;
    QComboBox *textComboBox;
    QComboBox *directoryComboBox;
    QLabel *fileLabel;
    QLabel *textLabel;
    QLabel *directoryLabel;
    QLabel *filesFoundLabel;
    QPushButton *browseButton;
    QPushButton *findButton;
    QTableWidget *filesTable;
    QGridLayout *mainLayout;
    QHBoxLayout *buttonsLayout;
    void clear_layout();

    QDir currentDir;
};

#endif
