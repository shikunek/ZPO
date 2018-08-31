#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QTimer>
#include <QMainWindow>
#include <QGraphicsView>
#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "bod.cpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class frmMain;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
class frmMain : public QMainWindow {
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    void nastaveni_site1();
    void nastaveni_site2();
    ~frmMain();


private slots:
    void on_btnOpenFile_clicked();

    void on_btnOpenFile_2_clicked();

    void zpracovat();

private:
    Ui::frmMain *ui;
//  scena pro 1. obrazek
    QGraphicsScene *scene;
//  scena pro 2. obrazek
    QGraphicsScene *scene1;
//  scena pro VYSTUPNI obrazek
    QGraphicsScene *scene3;

    QVector<CustomElipse*> body;
    QVector<CustomElipse*> body1;
    QImage frmMain::convertOpenCVMatToQtQImage(cv::Mat mat); // function prototype
};

#endif // FRMMAIN_H
