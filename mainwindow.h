#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <vector>
#include <QHBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void onLeftClick();
    void onCentreClick();
    void onRightClick();
private:
    QToolButton *buttonL;
    QToolButton *buttonC;
    QToolButton *buttonR;
    QLabel *textLabel;
};

#endif
