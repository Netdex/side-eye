#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QPixmap>
#include <QIcon>
#include <QToolButton>
#include <QSize>
#include <QLabel>
#include <QObject>
#include <iostream>
#include <QMainWindow>
#include <QKeyEvent>
#include <QtMultimedia/QCameraInfo>
#include <QColor>
#include <vector>
#include "winsw.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    const int width = 1000;
    const int height = 500;

    this->setWindowTitle("Side-eye");
    this->setGeometry(10, 10, width, height);

    QSize picSize(width, height);

    QVBoxLayout *topLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QHBoxLayout *textLayout = new QHBoxLayout;

    //std::cout << QCameraInfo::availableCameras().count();

    textLabel = new QLabel("Select a screen to begin the calibration\n\n\n");
    /*if (QCameraInfo::availableCameras().count() > 0) {
        textLabel->setText("greater than 0");
    } else {
        textLabel->setText("le 0");
    }*/

    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setFont(QFont("Segoe UI", 30));
    textLabel->setUpdatesEnabled(true);
    textLabel->setWordWrap(true);

    textLayout->addWidget(textLabel);

    QPixmap pixmapL("C:\\nellemaple\\Coding\\htn2018\\pics\\Lgrey.png");
    QIcon iconL(pixmapL);
    buttonL = new QToolButton;
    buttonL->setIcon(iconL);
    buttonL->setIconSize(picSize);
    buttonL->setFixedSize(width, height);
    buttonL->setCursor(Qt::PointingHandCursor);
    buttonL->setAutoFillBackground(true);
    buttonL->setStyleSheet("border: none");
    connect(buttonL, SIGNAL(released()), this, SLOT(onLeftClick()));

    QPixmap pixmapR("C:\\nellemaple\\Coding\\htn2018\\pics\\Rgrey.png");
    QIcon iconR(pixmapR);
    buttonR = new QToolButton;
    buttonR->setFixedSize(width, height);
    buttonR->setIcon(iconR);
    buttonR->setIconSize(picSize);
    buttonR->setCursor(Qt::PointingHandCursor);
    buttonR->setStyleSheet("border: none");
    connect(buttonR, SIGNAL(released()), this, SLOT(onRightClick()));

    buttonLayout->addWidget(buttonL);
    buttonLayout->addWidget(buttonR);

    int displays = query_display_ids().size();
    std::cout << "displays " << displays;
    //int displays = 2;

    if (displays == 3) {
        QPixmap pixmapC("C:\\nellemaple\\Coding\\htn2018\\pics\\Cgrey.png");
        QIcon iconC(pixmapC);
        buttonC = new QToolButton;
        buttonC->setFixedSize(width, height);
        buttonC->setIcon(iconC);
        buttonC->setIconSize(picSize);
        buttonC->setCursor(Qt::PointingHandCursor);
        buttonC->setStyleSheet("border: none");
        connect(buttonC, SIGNAL(released()), this, SLOT(onCentreClick()));

        buttonLayout->addWidget(buttonC);
    }

    topLayout->addStretch();
    topLayout->addLayout(buttonLayout);
    topLayout->addStretch();
    topLayout->addLayout(textLayout);
    topLayout->addStretch();
    topLayout->addStretch();

    QWidget *window = new QWidget();
    window->setLayout(topLayout);

    setCentralWidget(window);
}

void MainWindow::onLeftClick() {
    textLabel->setText("To calibrate the left screen, turn your head toward the left screen.\n Look at the top right corner, and press and hold 0 for five seconds.\n Repeat this process for the remaining three corners and the centre of the screen.");
    QPixmap pixmapLblue("C:\\nellemaple\\Coding\\htn2018\\pics\\Lblue.png");
    QIcon iconLblue(pixmapLblue);
    buttonL->setIcon(iconLblue);
}

void MainWindow::onCentreClick() {
    textLabel->setText("To calibrate the centre screen, turn your head toward the centre screen.\n Look at the top right corner, and press and hold 0 for five seconds.\n Repeat this process for the remaining three corners and the centre of the screen.");
    QPixmap pixmapCblue("C:\\nellemaple\\Coding\\htn2018\\pics\\Cblue.png");
    QIcon iconCblue(pixmapCblue);
    buttonC->setIcon(iconCblue);
}

void MainWindow::onRightClick() {
    textLabel->setText("To calibrate the right screen, turn your head toward the right screen.\n Look at the top right corner, and press and hold 0 for five seconds.\n Repeat this process for the remaining three corners and the centre of the screen.");
    QPixmap pixmapRblue("C:\\nellemaple\\Coding\\htn2018\\pics\\Rblue.png");
    QIcon iconRblue(pixmapRblue);
    buttonR->setIcon(iconRblue);
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_0) {
        textLabel->setText("Key pressed...\n\n\n");
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_0) {
        textLabel->setText("Key released!\n\n\n");
    }
}
