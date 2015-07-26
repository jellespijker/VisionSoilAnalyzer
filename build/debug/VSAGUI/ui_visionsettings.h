/********************************************************************************
** Form generated from reading UI file 'visionsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISIONSETTINGS_H
#define UI_VISIONSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisionSettings
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *cb_use_adaptContrast;
    QSpinBox *sb_adaptContrKernel;
    QDoubleSpinBox *sb_adaptContrastFactor;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cb_useBlur;
    QSpacerItem *horizontalSpacer;
    QSpinBox *sb_blurMask;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *rb_useLight;
    QRadioButton *rb_useDark;
    QCheckBox *cb_ignoreBorder;
    QCheckBox *cb_fillHoles;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *sb_sigmaFactor;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QRadioButton *rb_useOpen;
    QRadioButton *rb_useClose;
    QRadioButton *rb_useDilate;
    QRadioButton *rb_useErode;
    QLabel *label_2;
    QSpinBox *sb_morphMask;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QDialog *VisionSettings)
    {
        if (VisionSettings->objectName().isEmpty())
            VisionSettings->setObjectName(QStringLiteral("VisionSettings"));
        VisionSettings->resize(350, 400);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/ICH"), QSize(), QIcon::Normal, QIcon::Off);
        VisionSettings->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(VisionSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 360, 301, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        buttonBox->setCenterButtons(true);
        groupBox = new QGroupBox(VisionSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 311, 101));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 30, 315, 72));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        cb_use_adaptContrast = new QCheckBox(layoutWidget);
        cb_use_adaptContrast->setObjectName(QStringLiteral("cb_use_adaptContrast"));

        horizontalLayout->addWidget(cb_use_adaptContrast);

        sb_adaptContrKernel = new QSpinBox(layoutWidget);
        sb_adaptContrKernel->setObjectName(QStringLiteral("sb_adaptContrKernel"));

        horizontalLayout->addWidget(sb_adaptContrKernel);

        sb_adaptContrastFactor = new QDoubleSpinBox(layoutWidget);
        sb_adaptContrastFactor->setObjectName(QStringLiteral("sb_adaptContrastFactor"));

        horizontalLayout->addWidget(sb_adaptContrastFactor);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        cb_useBlur = new QCheckBox(layoutWidget);
        cb_useBlur->setObjectName(QStringLiteral("cb_useBlur"));

        horizontalLayout_2->addWidget(cb_useBlur);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        sb_blurMask = new QSpinBox(layoutWidget);
        sb_blurMask->setObjectName(QStringLiteral("sb_blurMask"));

        horizontalLayout_2->addWidget(sb_blurMask);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox_2 = new QGroupBox(VisionSettings);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 100, 321, 151));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 20, 311, 123));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        rb_useLight = new QRadioButton(layoutWidget1);
        rb_useLight->setObjectName(QStringLiteral("rb_useLight"));

        horizontalLayout_3->addWidget(rb_useLight);

        rb_useDark = new QRadioButton(layoutWidget1);
        rb_useDark->setObjectName(QStringLiteral("rb_useDark"));

        horizontalLayout_3->addWidget(rb_useDark);


        verticalLayout_2->addLayout(horizontalLayout_3);

        cb_ignoreBorder = new QCheckBox(layoutWidget1);
        cb_ignoreBorder->setObjectName(QStringLiteral("cb_ignoreBorder"));

        verticalLayout_2->addWidget(cb_ignoreBorder);

        cb_fillHoles = new QCheckBox(layoutWidget1);
        cb_fillHoles->setObjectName(QStringLiteral("cb_fillHoles"));

        verticalLayout_2->addWidget(cb_fillHoles);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        sb_sigmaFactor = new QDoubleSpinBox(layoutWidget1);
        sb_sigmaFactor->setObjectName(QStringLiteral("sb_sigmaFactor"));
        sb_sigmaFactor->setMaximum(3);

        horizontalLayout_4->addWidget(sb_sigmaFactor);


        verticalLayout_2->addLayout(horizontalLayout_4);

        groupBox_3 = new QGroupBox(VisionSettings);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 240, 311, 121));
        layoutWidget2 = new QWidget(groupBox_3);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 20, 311, 90));
        gridLayout = new QGridLayout(layoutWidget2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        rb_useOpen = new QRadioButton(layoutWidget2);
        rb_useOpen->setObjectName(QStringLiteral("rb_useOpen"));

        gridLayout->addWidget(rb_useOpen, 0, 0, 1, 1);

        rb_useClose = new QRadioButton(layoutWidget2);
        rb_useClose->setObjectName(QStringLiteral("rb_useClose"));

        gridLayout->addWidget(rb_useClose, 0, 1, 1, 1);

        rb_useDilate = new QRadioButton(layoutWidget2);
        rb_useDilate->setObjectName(QStringLiteral("rb_useDilate"));

        gridLayout->addWidget(rb_useDilate, 1, 1, 1, 1);

        rb_useErode = new QRadioButton(layoutWidget2);
        rb_useErode->setObjectName(QStringLiteral("rb_useErode"));

        gridLayout->addWidget(rb_useErode, 1, 0, 1, 1);

        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        sb_morphMask = new QSpinBox(layoutWidget2);
        sb_morphMask->setObjectName(QStringLiteral("sb_morphMask"));

        gridLayout->addWidget(sb_morphMask, 2, 1, 1, 1);

        layoutWidget3 = new QWidget(VisionSettings);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(0, 0, 2, 2));
        verticalLayout_3 = new QVBoxLayout(layoutWidget3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        retranslateUi(VisionSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), VisionSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VisionSettings, SLOT(reject()));
        QObject::connect(cb_use_adaptContrast, SIGNAL(toggled(bool)), sb_adaptContrKernel, SLOT(setEnabled(bool)));
        QObject::connect(cb_use_adaptContrast, SIGNAL(toggled(bool)), sb_adaptContrastFactor, SLOT(setEnabled(bool)));
        QObject::connect(cb_useBlur, SIGNAL(toggled(bool)), sb_blurMask, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(VisionSettings);
    } // setupUi

    void retranslateUi(QDialog *VisionSettings)
    {
        VisionSettings->setWindowTitle(QApplication::translate("VisionSettings", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("VisionSettings", "Intensity Transformations", 0));
        cb_use_adaptContrast->setText(QApplication::translate("VisionSettings", "Use Adaptive Contrast", 0));
        cb_useBlur->setText(QApplication::translate("VisionSettings", "Use Gaussian Blur", 0));
        groupBox_2->setTitle(QApplication::translate("VisionSettings", "Segmentation settings", 0));
        rb_useLight->setText(QApplication::translate("VisionSettings", "Light Ob&jects", 0));
        rb_useDark->setText(QApplication::translate("VisionSettings", "Dar&k Object", 0));
        cb_ignoreBorder->setText(QApplication::translate("VisionSettings", "Ignore partial border particles", 0));
        cb_fillHoles->setText(QApplication::translate("VisionSettings", "Fill holes", 0));
        label->setText(QApplication::translate("VisionSettings", "Sigma factor", 0));
        groupBox_3->setTitle(QApplication::translate("VisionSettings", "Morphological filter", 0));
        rb_useOpen->setText(QApplication::translate("VisionSettings", "Open", 0));
        rb_useClose->setText(QApplication::translate("VisionSettings", "Close", 0));
        rb_useDilate->setText(QApplication::translate("VisionSettings", "Dilate", 0));
        rb_useErode->setText(QApplication::translate("VisionSettings", "Erode", 0));
        label_2->setText(QApplication::translate("VisionSettings", "Mask Size", 0));
    } // retranslateUi

};

namespace Ui {
    class VisionSettings: public Ui_VisionSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISIONSETTINGS_H
