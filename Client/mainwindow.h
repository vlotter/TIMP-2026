#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onResponse(const QByteArray& res);
    
    void on_vigBtn_clicked();
    void on_shaBtn_clicked();
    void on_bisBtn_clicked();
    void on_graphBtn_clicked();
    void on_adminBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
