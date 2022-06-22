#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QTreeView>
#include <QTableView>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void on_select_comboboxOnChangedSlot(const int index);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QFileSystemModel *fileModel;
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    QTableView *tableView;
    QPushButton *btnPrint;
    QCheckBox *checkbox;
    QComboBox *combobox;
    QLabel *label, *label_path;

};
#endif // MAINWINDOW_H
