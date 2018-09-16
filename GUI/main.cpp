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
#include <QRunnable>
#include <QThreadPool>
#include "mainwindow.h"

class Work : public QRunnable {
public:
    void run() {
        qDebug() << "Hello from thread " << QThread::currentThread();
    }
};

int main(int argc, char** argv) {
 QApplication app(argc, argv);

 Work work;
 work.setAutoDelete(false);
 QThreadPool *threadPool = QThreadPool::globalInstance();
 threadPool->start(&work);
 qDebug() << "hello from GUI thread " << QThread::currentThread();
 threadPool->waitForDone();

 MainWindow mainWindow;
 mainWindow.showMaximized();

 return app.exec();
}
