/********************************************************************************
** Form generated from reading UI file 'frmmain.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMMAIN_H
#define UI_FRMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmMain
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOpenFile;
    QLabel *lblChosenFile;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnOpenFile_2;
    QLabel *lblChosenFile_2;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView2;
    QPushButton *poustec;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *labelVyp;

    void setupUi(QMainWindow *frmMain)
    {
        if (frmMain->objectName().isEmpty())
            frmMain->setObjectName(QStringLiteral("frmMain"));
        frmMain->resize(1004, 725);
        centralWidget = new QWidget(frmMain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 11, 981, 31));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnOpenFile = new QPushButton(layoutWidget);
        btnOpenFile->setObjectName(QStringLiteral("btnOpenFile"));
        btnOpenFile->setMinimumSize(QSize(100, 0));
        QFont font;
        font.setPointSize(12);
        btnOpenFile->setFont(font);

        horizontalLayout->addWidget(btnOpenFile);

        lblChosenFile = new QLabel(layoutWidget);
        lblChosenFile->setObjectName(QStringLiteral("lblChosenFile"));
        lblChosenFile->setFont(font);
        lblChosenFile->setAutoFillBackground(true);

        horizontalLayout->addWidget(lblChosenFile);

        horizontalLayout->setStretch(1, 1);

        horizontalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnOpenFile_2 = new QPushButton(layoutWidget);
        btnOpenFile_2->setObjectName(QStringLiteral("btnOpenFile_2"));
        btnOpenFile_2->setMinimumSize(QSize(100, 0));
        btnOpenFile_2->setFont(font);

        horizontalLayout_2->addWidget(btnOpenFile_2);

        lblChosenFile_2 = new QLabel(layoutWidget);
        lblChosenFile_2->setObjectName(QStringLiteral("lblChosenFile_2"));
        lblChosenFile_2->setFont(font);
        lblChosenFile_2->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(lblChosenFile_2);

        horizontalLayout_2->setStretch(1, 1);

        horizontalLayout_3->addLayout(horizontalLayout_2);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 50, 300, 300));
        graphicsView->setMinimumSize(QSize(300, 300));
        graphicsView2 = new QGraphicsView(centralWidget);
        graphicsView2->setObjectName(QStringLiteral("graphicsView2"));
        graphicsView2->setGeometry(QRect(680, 50, 300, 300));
        graphicsView2->setMinimumSize(QSize(300, 300));
        poustec = new QPushButton(centralWidget);
        poustec->setObjectName(QStringLiteral("poustec"));
        poustec->setGeometry(QRect(410, 190, 161, 51));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 380, 971, 331));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelVyp = new QLabel(centralWidget);
        labelVyp->setObjectName(QStringLiteral("labelVyp"));
        labelVyp->setGeometry(QRect(410, 250, 151, 20));
        frmMain->setCentralWidget(centralWidget);

        retranslateUi(frmMain);

        QMetaObject::connectSlotsByName(frmMain);
    } // setupUi

    void retranslateUi(QMainWindow *frmMain)
    {
        frmMain->setWindowTitle(QApplication::translate("frmMain", "frmMain", 0));
        btnOpenFile->setText(QApplication::translate("frmMain", "Open File", 0));
        lblChosenFile->setText(QString());
        btnOpenFile_2->setText(QApplication::translate("frmMain", "Open File", 0));
        lblChosenFile_2->setText(QString());
        poustec->setText(QApplication::translate("frmMain", "Spustit", 0));
        labelVyp->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class frmMain: public Ui_frmMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMMAIN_H
