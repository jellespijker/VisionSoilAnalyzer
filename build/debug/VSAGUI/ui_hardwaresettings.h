/********************************************************************************
** Form generated from reading UI file 'hardwaresettings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HARDWARESETTINGS_H
#define UI_HARDWARESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HardwareSettings
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_systemname;
    QLabel *label_3;
    QLabel *label_nodename;
    QLabel *label_5;
    QLabel *label_releasename;
    QLabel *label_7;
    QLabel *label_versioname;
    QLabel *label_9;
    QLabel *label_machinename;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_11;
    QSpinBox *sb_HDRframes;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_ll;
    QDoubleSpinBox *sb_lightLevel;
    QCheckBox *cb_encoderInv;
    QCheckBox *cb_enableRainbow;

    void setupUi(QDialog *HardwareSettings)
    {
        if (HardwareSettings->objectName().isEmpty())
            HardwareSettings->setObjectName(QStringLiteral("HardwareSettings"));
        HardwareSettings->resize(600, 200);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/ICH"), QSize(), QIcon::Normal, QIcon::Off);
        HardwareSettings->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(HardwareSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(490, 160, 91, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        groupBox = new QGroupBox(HardwareSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(170, 10, 421, 141));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 401, 121));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_systemname = new QLabel(layoutWidget);
        label_systemname->setObjectName(QStringLiteral("label_systemname"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_systemname->sizePolicy().hasHeightForWidth());
        label_systemname->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, label_systemname);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        label_nodename = new QLabel(layoutWidget);
        label_nodename->setObjectName(QStringLiteral("label_nodename"));

        formLayout->setWidget(1, QFormLayout::FieldRole, label_nodename);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        label_releasename = new QLabel(layoutWidget);
        label_releasename->setObjectName(QStringLiteral("label_releasename"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label_releasename);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_7);

        label_versioname = new QLabel(layoutWidget);
        label_versioname->setObjectName(QStringLiteral("label_versioname"));

        formLayout->setWidget(3, QFormLayout::FieldRole, label_versioname);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_9);

        label_machinename = new QLabel(layoutWidget);
        label_machinename->setObjectName(QStringLiteral("label_machinename"));

        formLayout->setWidget(4, QFormLayout::FieldRole, label_machinename);

        groupBox_2 = new QGroupBox(HardwareSettings);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 161, 171));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 141, 139));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_2->addWidget(label_11);

        sb_HDRframes = new QSpinBox(layoutWidget1);
        sb_HDRframes->setObjectName(QStringLiteral("sb_HDRframes"));
        sb_HDRframes->setMaximum(20);

        horizontalLayout_2->addWidget(sb_HDRframes);


        verticalLayout->addLayout(horizontalLayout_2);

        line_2 = new QFrame(layoutWidget1);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_ll = new QLabel(layoutWidget1);
        label_ll->setObjectName(QStringLiteral("label_ll"));

        horizontalLayout_3->addWidget(label_ll);

        sb_lightLevel = new QDoubleSpinBox(layoutWidget1);
        sb_lightLevel->setObjectName(QStringLiteral("sb_lightLevel"));
        sb_lightLevel->setMaximum(1);
        sb_lightLevel->setSingleStep(0.1);

        horizontalLayout_3->addWidget(sb_lightLevel);


        verticalLayout->addLayout(horizontalLayout_3);

        cb_encoderInv = new QCheckBox(layoutWidget1);
        cb_encoderInv->setObjectName(QStringLiteral("cb_encoderInv"));
        cb_encoderInv->setCheckable(true);

        verticalLayout->addWidget(cb_encoderInv);

        cb_enableRainbow = new QCheckBox(layoutWidget1);
        cb_enableRainbow->setObjectName(QStringLiteral("cb_enableRainbow"));

        verticalLayout->addWidget(cb_enableRainbow);


        retranslateUi(HardwareSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), HardwareSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), HardwareSettings, SLOT(reject()));

        QMetaObject::connectSlotsByName(HardwareSettings);
    } // setupUi

    void retranslateUi(QDialog *HardwareSettings)
    {
        HardwareSettings->setWindowTitle(QApplication::translate("HardwareSettings", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("HardwareSettings", "System info", 0));
        label->setText(QApplication::translate("HardwareSettings", "System:", 0));
        label_systemname->setText(QApplication::translate("HardwareSettings", "Sytem name", 0));
        label_3->setText(QApplication::translate("HardwareSettings", "Nodename:", 0));
        label_nodename->setText(QApplication::translate("HardwareSettings", "node name", 0));
        label_5->setText(QApplication::translate("HardwareSettings", "Release", 0));
        label_releasename->setText(QApplication::translate("HardwareSettings", "Release name", 0));
        label_7->setText(QApplication::translate("HardwareSettings", "Version", 0));
        label_versioname->setText(QApplication::translate("HardwareSettings", "Version name", 0));
        label_9->setText(QApplication::translate("HardwareSettings", "Machine", 0));
        label_machinename->setText(QApplication::translate("HardwareSettings", "Machine name", 0));
        groupBox_2->setTitle(QApplication::translate("HardwareSettings", "Hardware Settings", 0));
        label_11->setText(QApplication::translate("HardwareSettings", "HDR frames", 0));
        label_ll->setText(QApplication::translate("HardwareSettings", "Light level", 0));
        cb_encoderInv->setText(QApplication::translate("HardwareSettings", "Encoder invert", 0));
        cb_enableRainbow->setText(QApplication::translate("HardwareSettings", "Enable rainbow", 0));
    } // retranslateUi

};

namespace Ui {
    class HardwareSettings: public Ui_HardwareSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HARDWARESETTINGS_H
