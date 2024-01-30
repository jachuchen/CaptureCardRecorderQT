#include <QApplication>
#include <QTimer>

#include "ScreenRecorder.h"
#include "mainwindow.h"
#include "playwidget_yuy2.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QMessageBox errorDialog;
    try {
        MainWindow w;

        //CPlayWidget_yuy2 w(nullptr);
        w.show();



//        QTimer *timer = new QTimer(&w);
//        char* videobuffer  = new  char[1920*1080*2] ;
//        unsigned long int len=1920*1080*2;

//        QObject::connect(timer, &QTimer::timeout, [&] (){
//            ScreenRecorder::captureVideo(videobuffer,&len);
//            w.PlayOneFrame(videobuffer,len);
//        } );
//        timer->start(60);



        return a.exec();
    } catch (const std::exception& e) {
        // Call to open the error dialog
        std::string message = e.what();
        message += "\nPlease close and restart the application";
        errorDialog.critical(0, "Error", QString::fromStdString(message));
    }
}
