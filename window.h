#ifndef WINDOW_H
#define WINDOW_H

#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QDir>
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

protected:
    QStringList findFiles(const QStringList &files, const QString &text);
    void showFiles(const QStringList &files);
    QPushButton *createButton(const QString &text, const char *member);
    QComboBox *createComboBox(const QString &text = QString());
    void createFilesTable();

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
