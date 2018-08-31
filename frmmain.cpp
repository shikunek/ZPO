#include "frmmain.h"
#include "ui_frmmain.h"

#include <QDebug>
#include <QFileDialog>
#include <QtCore>
#include <QVariant>
#include <QtWidgets>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <cstddef>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int rozmer = 300;                   // fixni rozmer obrazku
cv::Mat img1, img2;                 // matice pro vstupni obrazky

// Pomocna funkce pro funkci warp
// Slouzi pro vypocet miry posuvu mezi dvema body 
Point posuv(const Point bod1, const Point bod2, const float mira) 
{
    return Point(bod1.x - (bod1.x - bod2.x) * mira, bod1.y - (bod1.y - bod2.y) * mira);
}

// Vypocet warpingu
Mat warp(const Mat &obrazek, const Mat mrizka1, const Mat mrizka2, const float mira) 
{
    Mat maska(obrazek.size(), obrazek.type());
    Mat tmp(obrazek.size(), obrazek.type());
    Mat vystup(obrazek.size(), obrazek.type());
    vystup = Mat::zeros(vystup.size(), vystup.type());

    for (unsigned short y = 0; y < mrizka1.cols - 1; y++) 
	{
        for (unsigned short x = 0; x < mrizka1.rows - 1; x++) 
		{            
            Point ctverec1[4];
            ctverec1[0] = mrizka1.at<Point>(x     , y    );
            ctverec1[1] = mrizka1.at<Point>(x + 1 , y    );
            ctverec1[2] = mrizka1.at<Point>(x + 1 , y + 1);
            ctverec1[3] = mrizka1.at<Point>(x     , y + 1);
            Point2f tile1f[4] = {ctverec1[0], ctverec1[1], ctverec1[2], ctverec1[3]};

            Point ctverec2[4];
            ctverec2[0] = posuv(mrizka1.at<Point>(x     , y    ), mrizka2.at<Point>(x    , y    ), mira);
            ctverec2[1] = posuv(mrizka1.at<Point>(x + 1 , y    ), mrizka2.at<Point>(x + 1, y    ), mira);
            ctverec2[2] = posuv(mrizka1.at<Point>(x + 1 , y + 1), mrizka2.at<Point>(x + 1, y + 1), mira);
            ctverec2[3] = posuv(mrizka1.at<Point>(x     , y + 1), mrizka2.at<Point>(x    , y + 1), mira);
            Point2f tile2f[4] = {ctverec2[0], ctverec2[1], ctverec2[2], ctverec2[3]};

            // Perspektivni transformace
            warpPerspective(obrazek, tmp, getPerspectiveTransform(tile1f, tile2f), tmp.size());

            // Aplikace ctvercove masky
            maska = Mat::zeros(maska.size(), maska.type());
            fillConvexPoly(maska, ctverec2, 4, CV_RGB(255, 255, 255), CV_AA, 0);

            vystup = cv::max(maska & tmp, vystup);
        }
    }
    return vystup;
}

// Funkce obsluhujici Morphing
void morph(const Mat &obrazek1, const Mat &obrazek2, Mat &vystup, const Mat mrizka1, const Mat mrizka2, const float mira) 
{
    vystup = Mat::zeros(vystup.size(), vystup.type());

    // warping na oba obrazky
    Mat w1 = warp(obrazek1, mrizka1, mrizka2, mira);
    Mat w2 = warp(obrazek2, mrizka2, mrizka1, 1 - mira);

    // morfing pomoci vazeneho souctu
    addWeighted(w1, 1 - mira, w2, mira, 0, vystup);
}

// Funkce pro konverzi cv::Mat -> QImage
QImage Mat2QImage(cv::Mat const& src){
     cv::Mat temp;                              // pomocnam matice pro prevod
     cvtColor(src, temp, CV_BGR2RGB);           // konvertuje BGR -> RGB
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits();
     return dest;                               // vraci QImage
}

// konstruktor gui
frmMain::frmMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::frmMain) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene1 = new QGraphicsScene(this);

    connect(ui->poustec, SIGNAL (clicked()), this, SLOT (zpracovat()));
}

// Funkce pro ziskani jednotlivych ctvercu v obraze a volani veskere obsluhy morfingu
void frmMain::zpracovat(){
    int pocet_radku = 3;
    Mat mat1(pocet_radku+2, pocet_radku+2, DataType<Point>::type);
    Mat mat2(pocet_radku+2, pocet_radku+2, DataType<Point>::type);

    float prirustek = (rozmer/(pocet_radku+1))-1;
    QPointF soura[5][5];
    QPointF soura1[5][5];
    int pocet = 0;
    int pocet1 = 0;
    int pozice_y = 0;

    for (int radek = 0; radek < pocet_radku+2; ++radek){
        int pozice_x = 0;

        for (int sloupec = 0; sloupec < pocet_radku+2; ++sloupec){
            if (radek == 0 || sloupec == 0 || radek == (pocet_radku+1) || sloupec == (pocet_radku+1)){
                  soura[radek][sloupec] = QPointF(pozice_x,pozice_y);
            }
            else{
                soura[radek][sloupec].setX(body[pocet]->rect().x() + body[pocet]->pos().x());
                soura[radek][sloupec].setY(body[pocet]->rect().y() + body[pocet]->pos().y());
                pocet++;
            }
            mat1.at<Point>(radek, sloupec) = Point(soura[radek][sloupec].x(), soura[radek][sloupec].y());
            //printf("Bod %d,%d pozice %f, %f\n",radek,sloupec,soura[radek][sloupec].x(),soura[radek][sloupec].y());
            pozice_x += prirustek;
        }
        pozice_y += prirustek;
    }

    pozice_y = 0;
    for (int radek = 0; radek < pocet_radku+2; ++radek){
        int pozice_x = 0;

        for (int sloupec = 0; sloupec < pocet_radku+2; ++sloupec){
            if (radek == 0 || sloupec == 0 || radek == (pocet_radku+1) || sloupec == (pocet_radku+1)){
                soura1[radek][sloupec] = QPointF(pozice_x,pozice_y);
            }
            else{
                soura1[radek][sloupec].setX(body1[pocet1]->rect().x() + body1[pocet1]->pos().x());
                soura1[radek][sloupec].setY(body1[pocet1]->rect().y() + body1[pocet1]->pos().y());
                pocet1++;
            }
            mat2.at<Point>(radek, sloupec) = Point(soura1[radek][sloupec].x(),soura1[radek][sloupec].y());
            //printf("Bod %d,%d pozice %f, %f\n",radek,sloupec,soura1[radek][sloupec].x(),soura1[radek][sloupec].y());
            pozice_x += prirustek;
        }
        pozice_y += prirustek;
    }

    ui->labelVyp->setText("counting...");
    ui->labelVyp->show();
    repaint();
    QVector<QPixmap> destination;
    float krok = 0.1f;
    for(float kroky = 0.0; kroky < 1.1; kroky += krok){
        Mat mat_out(img1.size(), img1.type());
        morph(img1, img2, mat_out, mat1, mat2, kroky);

        QImage tmp  = Mat2QImage(mat_out);
        QPixmap dstImg = QPixmap::fromImage(tmp);

        destination.push_back(dstImg);
    }
    ui->labelVyp->setText("");
    ui->labelVyp->show();
    repaint();

    // ukaze prvni obrazek
    QLabel * label = new QLabel();
    QImage firstImage = Mat2QImage(img1);
    label->setPixmap(QPixmap::fromImage(firstImage));
    ui->gridLayout->addWidget(label, 0, 0, Qt::AlignVCenter|Qt::AlignHCenter);
    label->show();

    for(int kroky = 0; kroky < destination.size(); kroky++){
       QLabel * label_2 = new QLabel();
       QPixmap result_2(destination[kroky].size());
       result_2.fill(Qt::transparent);
       QPainter painter_2(&result_2);

       label_2->setPixmap(result_2);
       ui->gridLayout->addWidget(label_2, 0, 0, Qt::AlignVCenter|Qt::AlignHCenter);

       // animace mezi 2 obrazky, 10 kroku
       for(int i = 0; i <= 10; i++){
           result_2.fill(Qt::transparent);
           painter_2.setOpacity(i/10.0);
           painter_2.drawPixmap(0, 0, destination[kroky]);

           label_2->setPixmap(result_2);
           label_2->show();
           repaint();

           QThread::msleep(10);
       }
       painter_2.end();
       QThread::msleep(20);
    }
}

//funkce ktera nastavi vyberovou sit pres PRVNI obrazek
void frmMain::nastaveni_site1(){
    QVector<QGraphicsLineItem*> usecky;

    int pocet_radku = 3;
    float prirustek = (rozmer/(pocet_radku+1));
    float y = prirustek;

    for(int k = 0; k < pocet_radku; k++){
        float x = prirustek;
        for( int j = 0; j < pocet_radku; j++){
            CustomElipse *elipse = new CustomElipse(QRectF(x-5, y-5, 10, 10));
            elipse->setBrush(Qt::blue);
            body.push_back(elipse);
            scene->addItem(elipse);
            x+= prirustek;
        }
        y+= prirustek;
    }

    for(int i = 0; i < body.size() ; i++){
        int xOffset = body[i]->rect().x() + body[i]->rect().width()/2;
        int yOffset = body[i]->rect().y() + body[i]->rect().height()/2;

        // DOPOMOCNA PRO PROCHAZENI SEZNAMEM
        QGraphicsLineItem* jedna;

        if (i == 0){
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset-prirustek, yOffset)));

            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset, yOffset-prirustek)));

            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));

            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body[i]->addLine(usecky[i*4+0], true, usecky[i*4+1], true, usecky[i*4+2], true, usecky[i*4+3], true);
        }
        else if(i < pocet_radku && i != 0){             // treba upravit pro vice jak 2 sloupce
            // HORNI RADEK
            // VEZME HORNI, DOLNI A PRAVOU
            // NA LEVOU SE MUSI PRIPOJIT
            usecky.push_back(jedna);
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset, yOffset-prirustek)));
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body[i]->addLine(usecky[i*4-1], false, usecky[i*4+1],true,usecky[i*4+2],true,usecky[i*4+3], true);
        }

        else if (i %pocet_radku == 0 && i != 0){
            // LEVY SLOUPEC
            // VEZME LEVOU, DOLNI A PRAVOU
            // NA HORNI SE MUSI PRIPOJIT
            usecky.push_back(jedna);
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset-prirustek, yOffset)));
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body[i]->addLine(usecky[(i*4)-(4*(pocet_radku-1))-2], false, usecky[i*4+1],true,usecky[i*4+2],true,usecky[i*4+3], true);
        }
        else{
            // POSLEDNI SLOUPEC
            // VEZME PRAVOU A DOLNI
            // NA HORNI A LEVOU SE MUSI PRIPOJIT
            usecky.push_back(jedna);
            usecky.push_back(jedna);
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky.push_back(scene->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));
            body[i]->addLine(usecky[(i*4)-(4*(pocet_radku-1))-2], false, usecky[(i*4)-1],false,usecky[i*4+2],true,usecky[i*4+3], true);
        }
    }
}

//funkce ktera nastavi vyberovou sit pres DRUHY obrazek
void frmMain::nastaveni_site2(){
    QVector<QGraphicsLineItem*> usecky1;

    int pocet_radku = 3;
    float prirustek = (rozmer/(pocet_radku+1));
    float y = prirustek;

    for(int k = 0; k < pocet_radku; k++){
        float x = prirustek;
        for( int j = 0; j < pocet_radku; j++){
            CustomElipse *elipse = new CustomElipse(QRectF(x-5, y-5, 10, 10));
            elipse->setBrush(Qt::blue);
            body1.push_back(elipse);
            scene1->addItem(elipse);
            x+= prirustek;
        }

        y+= prirustek;
    }


    for(int i = 0; i < body1.size() ; i++){
        int xOffset = body1[i]->rect().x() + body1[i]->rect().width()/2;
        int yOffset = body1[i]->rect().y() + body1[i]->rect().height()/2;

        // DOPOMOCNA PRO PROCHAZENI SEZNAMEM
        QGraphicsLineItem* jedna;

        if (i == 0){
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset-prirustek, yOffset)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset, yOffset-prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body1[i]->addLine(usecky1[i*4+0], true, usecky1[i*4+1], true, usecky1[i*4+2], true, usecky1[i*4+3], true);
        }
        else if(i < pocet_radku && i != 0){             // treba upravit pro vice jak 2 sloupce
            // HORNI RADEK
            // VEZME HORNI, DOLNI A PRAVOU
            // NA LEVOU SE MUSI PRIPOJIT
            usecky1.push_back(jedna);
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset, yOffset-prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body1[i]->addLine(usecky1[i*4-1], false, usecky1[i*4+1],true,usecky1[i*4+2],true,usecky1[i*4+3], true);
        }

        else if (i %pocet_radku == 0 && i != 0){
            // LEVY SLOUPEC
            // VEZME LEVOU, DOLNI A PRAVOU
            // NA HORNI SE MUSI PRIPOJIT
            usecky1.push_back(jedna);
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset-prirustek, yOffset)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body1[i]->addLine(usecky1[(i*4)-(4*(pocet_radku-1))-2], false, usecky1[i*4+1],true,usecky1[i*4+2],true,usecky1[i*4+3], true);
        }
        else{
            // POSLEDNI SLOUPEC
            // VEZME PRAVOU A DOLNI
            // NA HORNI A LEVOU SE MUSI PRIPOJIT
            usecky1.push_back(jedna);
            usecky1.push_back(jedna);
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset,yOffset+prirustek)));
            usecky1.push_back(scene1->addLine(QLineF(xOffset, yOffset, xOffset+prirustek, yOffset)));

            body1[i]->addLine(usecky1[(i*4)-(4*(pocet_radku-1))-2], false, usecky1[(i*4)-1],false,usecky1[i*4+2],true,usecky1[i*4+3], true);
        }
    }
}

// destruktor gui
frmMain::~frmMain() {
    delete ui;
}

// kod ktery se provede po stisknuti tlacitka "Open file".
// provede se tim nahrani prvniho obrazku
void frmMain::on_btnOpenFile_clicked() {
    QString strFileName = QFileDialog::getOpenFileName();       // otevre dialog k otevreni souboru

    if(strFileName == "") {                                     // pokud soubor nebyl vybran
        ui->lblChosenFile->setText("file not chosen");          // nastavi hlasku do labelu
        return;                                                 // konec
    }

    img1 = cv::imread(strFileName.toStdString());        // otevre dany obrazek

    if (img1.empty()) {                                  // pokud se nepodarilo otevrit obrazek
        ui->lblChosenFile->setText("error: image not read from file");      // nastavi chybovou hlasku labelu
        return;                                                             // konec
    }

    ui->lblChosenFile->setText(strFileName);                // nastavi labelu nazev souboru

    QPixmap image;                      // nachystame pro nahrani obrazku do Qt
    QImage  *imageObject;
    imageObject = new QImage();
    imageObject->load(strFileName);
    image = QPixmap::fromImage(*imageObject);

    scene->addPixmap(image);            // nastaveni obrazku do sceny

    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setScene(scene);
    nastaveni_site1();      // po nahrani obrazku nastavime vyberovou sit
}

// kod ktery se provede po stisknuti tlacitka "Open file".
// provede se tim nahrani druheho obrazku
void frmMain::on_btnOpenFile_2_clicked(){
    QString strFileName = QFileDialog::getOpenFileName();       // otevre dialog k otevreni souboru

    if(strFileName == "") {                                     // pokud soubor nebyl vybran
        ui->lblChosenFile_2->setText("file not chosen");        // nastavi hlasku do labelu
        return;                                                 // konec
    }

    img2 = cv::imread(strFileName.toStdString());       // otevre dany obrazek

    if (img2.empty()) {									// pokud se nepodarilo otevrit obrazek
        ui->lblChosenFile_2->setText("error: image not read from file");    // nastavi chybovou hlasku labelu
        return;                                                             // konec
    }

    ui->lblChosenFile_2->setText(strFileName);          // nastavi labelu nazev souboru

    QPixmap image;                      // nachystame pro nahrani obrazku do Qt
    QImage  *imageObject;
    imageObject = new QImage();
    imageObject->load(strFileName);
    image = QPixmap::fromImage(*imageObject);

    scene1->addPixmap(image);           // nastaveni obrazku do sceny

    // vypneme scrollbary
    ui->graphicsView2->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView2->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView2->setScene(scene1);
    nastaveni_site2();      // po nahrani obrazku nastavime vyberovou sit
}
