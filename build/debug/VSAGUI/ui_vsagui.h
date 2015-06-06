/********************************************************************************
** Form generated from reading UI file 'vsagui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSAGUI_H
#define UI_VSAGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_VSAGUI
{
public:
    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionSegmentation;
    QAction *actionMicroscope;
    QAction *actionImport;
    QAction *actionExport;
    QAction *actionLearn;
    QAction *actionIntensity;
    QAction *actionCIE_La_b;
    QAction *actionRedness_Index;
    QAction *actionRGB;
    QAction *actionSettings;
    QAction *actionCheese;
    QAction *actionSettings_2;
    QAction *actionSoilCape;
    QAction *actionSegmentation_Settings;
    QAction *actionSave_Settings;
    QAction *actionLoad_Settings;
    QAction *actionRestore_Default;
    QAction *actionHardware_Settings;
    QAction *actionNeural_Net_Settings;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_psd_desc;
    QCustomPlot *plot_psd;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_shape_desc;
    QCustomPlot *plot_shape;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_color_desc;
    QCustomPlot *plot_lab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_sample;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *AnalyzeButton;
    QPushButton *SegmentButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *SnapshotButton;
    QLabel *MainImg;
    QSlider *OffsetSlider;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_soil_desc;
    QTableView *tableView_sample;
    QLabel *label_grain_desc;
    QTableView *tableView_grain;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuColorspace;
    QMenu *menuNeuralNet;
    QMenu *menuSettings;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VSAGUI)
    {
        if (VSAGUI->objectName().isEmpty())
            VSAGUI->setObjectName(QStringLiteral("VSAGUI"));
        VSAGUI->resize(1024, 768);
        VSAGUI->setMinimumSize(QSize(1024, 768));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/ICH"), QSize(), QIcon::Normal, QIcon::Off);
        VSAGUI->setWindowIcon(icon);
        actionNew = new QAction(VSAGUI);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionLoad = new QAction(VSAGUI);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionSave = new QAction(VSAGUI);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionExit = new QAction(VSAGUI);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionExit->setCheckable(false);
        actionSegmentation = new QAction(VSAGUI);
        actionSegmentation->setObjectName(QStringLiteral("actionSegmentation"));
        actionMicroscope = new QAction(VSAGUI);
        actionMicroscope->setObjectName(QStringLiteral("actionMicroscope"));
        actionImport = new QAction(VSAGUI);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionExport = new QAction(VSAGUI);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        actionLearn = new QAction(VSAGUI);
        actionLearn->setObjectName(QStringLiteral("actionLearn"));
        actionIntensity = new QAction(VSAGUI);
        actionIntensity->setObjectName(QStringLiteral("actionIntensity"));
        actionCIE_La_b = new QAction(VSAGUI);
        actionCIE_La_b->setObjectName(QStringLiteral("actionCIE_La_b"));
        actionRedness_Index = new QAction(VSAGUI);
        actionRedness_Index->setObjectName(QStringLiteral("actionRedness_Index"));
        actionRGB = new QAction(VSAGUI);
        actionRGB->setObjectName(QStringLiteral("actionRGB"));
        actionSettings = new QAction(VSAGUI);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        actionCheese = new QAction(VSAGUI);
        actionCheese->setObjectName(QStringLiteral("actionCheese"));
        actionSettings_2 = new QAction(VSAGUI);
        actionSettings_2->setObjectName(QStringLiteral("actionSettings_2"));
        actionSoilCape = new QAction(VSAGUI);
        actionSoilCape->setObjectName(QStringLiteral("actionSoilCape"));
        actionSegmentation_Settings = new QAction(VSAGUI);
        actionSegmentation_Settings->setObjectName(QStringLiteral("actionSegmentation_Settings"));
        actionSave_Settings = new QAction(VSAGUI);
        actionSave_Settings->setObjectName(QStringLiteral("actionSave_Settings"));
        actionLoad_Settings = new QAction(VSAGUI);
        actionLoad_Settings->setObjectName(QStringLiteral("actionLoad_Settings"));
        actionRestore_Default = new QAction(VSAGUI);
        actionRestore_Default->setObjectName(QStringLiteral("actionRestore_Default"));
        actionHardware_Settings = new QAction(VSAGUI);
        actionHardware_Settings->setObjectName(QStringLiteral("actionHardware_Settings"));
        actionNeural_Net_Settings = new QAction(VSAGUI);
        actionNeural_Net_Settings->setObjectName(QStringLiteral("actionNeural_Net_Settings"));
        centralWidget = new QWidget(VSAGUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_psd_desc = new QLabel(centralWidget);
        label_psd_desc->setObjectName(QStringLiteral("label_psd_desc"));

        verticalLayout_5->addWidget(label_psd_desc);

        plot_psd = new QCustomPlot(centralWidget);
        plot_psd->setObjectName(QStringLiteral("plot_psd"));
        plot_psd->setMinimumSize(QSize(200, 200));

        verticalLayout_5->addWidget(plot_psd);


        horizontalLayout->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_shape_desc = new QLabel(centralWidget);
        label_shape_desc->setObjectName(QStringLiteral("label_shape_desc"));

        verticalLayout_4->addWidget(label_shape_desc);

        plot_shape = new QCustomPlot(centralWidget);
        plot_shape->setObjectName(QStringLiteral("plot_shape"));
        plot_shape->setMinimumSize(QSize(200, 200));

        verticalLayout_4->addWidget(plot_shape);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_color_desc = new QLabel(centralWidget);
        label_color_desc->setObjectName(QStringLiteral("label_color_desc"));

        verticalLayout_3->addWidget(label_color_desc);

        plot_lab = new QCustomPlot(centralWidget);
        plot_lab->setObjectName(QStringLiteral("plot_lab"));
        plot_lab->setMinimumSize(QSize(200, 200));

        verticalLayout_3->addWidget(plot_lab);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_sample = new QLabel(centralWidget);
        label_sample->setObjectName(QStringLiteral("label_sample"));

        horizontalLayout_3->addWidget(label_sample);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        AnalyzeButton = new QPushButton(centralWidget);
        AnalyzeButton->setObjectName(QStringLiteral("AnalyzeButton"));

        horizontalLayout_3->addWidget(AnalyzeButton);

        SegmentButton = new QPushButton(centralWidget);
        SegmentButton->setObjectName(QStringLiteral("SegmentButton"));

        horizontalLayout_3->addWidget(SegmentButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        SnapshotButton = new QPushButton(centralWidget);
        SnapshotButton->setObjectName(QStringLiteral("SnapshotButton"));

        horizontalLayout_3->addWidget(SnapshotButton);


        verticalLayout_7->addLayout(horizontalLayout_3);

        MainImg = new QLabel(centralWidget);
        MainImg->setObjectName(QStringLiteral("MainImg"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainImg->sizePolicy().hasHeightForWidth());
        MainImg->setSizePolicy(sizePolicy);
        MainImg->setMinimumSize(QSize(640, 480));
        MainImg->setBaseSize(QSize(640, 480));
        MainImg->setScaledContents(true);

        verticalLayout_7->addWidget(MainImg);


        horizontalLayout_2->addLayout(verticalLayout_7);

        OffsetSlider = new QSlider(centralWidget);
        OffsetSlider->setObjectName(QStringLiteral("OffsetSlider"));
        OffsetSlider->setMinimum(-10);
        OffsetSlider->setMaximum(10);
        OffsetSlider->setOrientation(Qt::Vertical);
        OffsetSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_2->addWidget(OffsetSlider);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_soil_desc = new QLabel(centralWidget);
        label_soil_desc->setObjectName(QStringLiteral("label_soil_desc"));

        verticalLayout_6->addWidget(label_soil_desc);

        tableView_sample = new QTableView(centralWidget);
        tableView_sample->setObjectName(QStringLiteral("tableView_sample"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView_sample->sizePolicy().hasHeightForWidth());
        tableView_sample->setSizePolicy(sizePolicy1);
        tableView_sample->setMinimumSize(QSize(300, 150));

        verticalLayout_6->addWidget(tableView_sample);

        label_grain_desc = new QLabel(centralWidget);
        label_grain_desc->setObjectName(QStringLiteral("label_grain_desc"));

        verticalLayout_6->addWidget(label_grain_desc);

        tableView_grain = new QTableView(centralWidget);
        tableView_grain->setObjectName(QStringLiteral("tableView_grain"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tableView_grain->sizePolicy().hasHeightForWidth());
        tableView_grain->setSizePolicy(sizePolicy2);
        tableView_grain->setMinimumSize(QSize(300, 150));

        verticalLayout_6->addWidget(tableView_grain);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(verticalLayout_2);

        VSAGUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VSAGUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 28));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuColorspace = new QMenu(menuAbout);
        menuColorspace->setObjectName(QStringLiteral("menuColorspace"));
        menuNeuralNet = new QMenu(menuBar);
        menuNeuralNet->setObjectName(QStringLiteral("menuNeuralNet"));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        VSAGUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VSAGUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VSAGUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VSAGUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VSAGUI->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuNeuralNet->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionLoad);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuAbout->addAction(menuColorspace->menuAction());
        menuAbout->addSeparator();
        menuColorspace->addAction(actionIntensity);
        menuColorspace->addAction(actionCIE_La_b);
        menuColorspace->addAction(actionRedness_Index);
        menuColorspace->addAction(actionRGB);
        menuColorspace->addSeparator();
        menuColorspace->addAction(actionSettings);
        menuNeuralNet->addAction(actionImport);
        menuNeuralNet->addAction(actionExport);
        menuNeuralNet->addSeparator();
        menuNeuralNet->addAction(actionLearn);
        menuSettings->addAction(actionSave_Settings);
        menuSettings->addAction(actionLoad_Settings);
        menuSettings->addSeparator();
        menuSettings->addAction(actionRestore_Default);
        menuSettings->addSeparator();
        menuSettings->addAction(actionSegmentation_Settings);
        menuSettings->addAction(actionHardware_Settings);
        menuSettings->addAction(actionNeural_Net_Settings);

        retranslateUi(VSAGUI);
        QObject::connect(actionExit, SIGNAL(triggered()), VSAGUI, SLOT(close()));

        QMetaObject::connectSlotsByName(VSAGUI);
    } // setupUi

    void retranslateUi(QMainWindow *VSAGUI)
    {
        VSAGUI->setWindowTitle(QApplication::translate("VSAGUI", "Vision Soil Analyzer", 0));
        actionNew->setText(QApplication::translate("VSAGUI", "&New", 0));
        actionLoad->setText(QApplication::translate("VSAGUI", "&Load", 0));
        actionSave->setText(QApplication::translate("VSAGUI", "&Save", 0));
        actionExit->setText(QApplication::translate("VSAGUI", "&Exit", 0));
        actionSegmentation->setText(QApplication::translate("VSAGUI", "Segmentation", 0));
        actionMicroscope->setText(QApplication::translate("VSAGUI", "Microscope", 0));
        actionImport->setText(QApplication::translate("VSAGUI", "&Import", 0));
        actionExport->setText(QApplication::translate("VSAGUI", "&Export", 0));
        actionLearn->setText(QApplication::translate("VSAGUI", "&Learn", 0));
        actionIntensity->setText(QApplication::translate("VSAGUI", "&Intensity", 0));
        actionCIE_La_b->setText(QApplication::translate("VSAGUI", "&CIE La*b*", 0));
        actionRedness_Index->setText(QApplication::translate("VSAGUI", "&Redness Index", 0));
        actionRGB->setText(QApplication::translate("VSAGUI", "R&GB", 0));
        actionSettings->setText(QApplication::translate("VSAGUI", "&Settings", 0));
        actionCheese->setText(QApplication::translate("VSAGUI", "&Cheese", 0));
        actionSettings_2->setText(QApplication::translate("VSAGUI", "&Settings", 0));
        actionSoilCape->setText(QApplication::translate("VSAGUI", "&SoilCape", 0));
        actionSegmentation_Settings->setText(QApplication::translate("VSAGUI", "S&egmentation Settings", 0));
        actionSave_Settings->setText(QApplication::translate("VSAGUI", "&Save Settings", 0));
        actionLoad_Settings->setText(QApplication::translate("VSAGUI", "&Load Settings", 0));
        actionRestore_Default->setText(QApplication::translate("VSAGUI", "&Restore Default", 0));
        actionHardware_Settings->setText(QApplication::translate("VSAGUI", "&Hardware Settings", 0));
        actionNeural_Net_Settings->setText(QApplication::translate("VSAGUI", "&Neural Net Settings", 0));
        label_psd_desc->setText(QApplication::translate("VSAGUI", "Particle Size Distribution", 0));
        label_shape_desc->setText(QApplication::translate("VSAGUI", "Shape Classifaction", 0));
        label_color_desc->setText(QApplication::translate("VSAGUI", "CIE La*b* Color Description", 0));
        label_sample->setText(QApplication::translate("VSAGUI", "Soil Sample", 0));
        AnalyzeButton->setText(QApplication::translate("VSAGUI", "Analyze", 0));
        SegmentButton->setText(QApplication::translate("VSAGUI", "Segment", 0));
        SnapshotButton->setText(QApplication::translate("VSAGUI", "SnapeShot", 0));
        MainImg->setText(QString());
        label_soil_desc->setText(QApplication::translate("VSAGUI", "Soil Sample Description", 0));
        label_grain_desc->setText(QApplication::translate("VSAGUI", "Individual Grain Description", 0));
        menuFile->setTitle(QApplication::translate("VSAGUI", "Fi&le", 0));
        menuAbout->setTitle(QApplication::translate("VSAGUI", "Soil Sa&mple", 0));
        menuColorspace->setTitle(QApplication::translate("VSAGUI", "&Colorspace", 0));
        menuNeuralNet->setTitle(QApplication::translate("VSAGUI", "&NeuralNet", 0));
        menuSettings->setTitle(QApplication::translate("VSAGUI", "Settin&gs", 0));
    } // retranslateUi

};

namespace Ui {
    class VSAGUI: public Ui_VSAGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSAGUI_H
