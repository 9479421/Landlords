/********************************************************************************
** Form generated from reading UI file 'buttongroup.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONGROUP_H
#define UI_BUTTONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include "mybutton.h"

QT_BEGIN_NAMESPACE

class Ui_ButtonGroup
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    MyButton *start;
    QSpacerItem *horizontalSpacer_4;
    QWidget *playCardPage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    MyButton *playCard;
    QSpacerItem *horizontalSpacer_2;
    QWidget *PassOrPlayPage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    MyButton *pass;
    QSpacerItem *horizontalSpacer_6;
    MyButton *playCard1;
    QSpacerItem *horizontalSpacer_7;
    QWidget *callLordPage;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    MyButton *giveup;
    QSpacerItem *horizontalSpacer_9;
    MyButton *oneScore;
    QSpacerItem *horizontalSpacer_10;
    MyButton *twoScore;
    QSpacerItem *horizontalSpacer_11;
    MyButton *threeScore;
    QSpacerItem *horizontalSpacer_12;
    QWidget *nullpage;

    void setupUi(QWidget *ButtonGroup)
    {
        if (ButtonGroup->objectName().isEmpty())
            ButtonGroup->setObjectName("ButtonGroup");
        ButtonGroup->resize(500, 60);
        horizontalLayout = new QHBoxLayout(ButtonGroup);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(ButtonGroup);
        stackedWidget->setObjectName("stackedWidget");
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        horizontalLayout_3 = new QHBoxLayout(startPage);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(195, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        start = new MyButton(startPage);
        start->setObjectName("start");

        horizontalLayout_3->addWidget(start);

        horizontalSpacer_4 = new QSpacerItem(194, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(startPage);
        playCardPage = new QWidget();
        playCardPage->setObjectName("playCardPage");
        horizontalLayout_2 = new QHBoxLayout(playCardPage);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(195, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        playCard = new MyButton(playCardPage);
        playCard->setObjectName("playCard");

        horizontalLayout_2->addWidget(playCard);

        horizontalSpacer_2 = new QSpacerItem(194, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(playCardPage);
        PassOrPlayPage = new QWidget();
        PassOrPlayPage->setObjectName("PassOrPlayPage");
        horizontalLayout_4 = new QHBoxLayout(PassOrPlayPage);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_5 = new QSpacerItem(100, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        pass = new MyButton(PassOrPlayPage);
        pass->setObjectName("pass");

        horizontalLayout_4->addWidget(pass);

        horizontalSpacer_6 = new QSpacerItem(40, 15, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        playCard1 = new MyButton(PassOrPlayPage);
        playCard1->setObjectName("playCard1");
        playCard1->setEnabled(true);

        horizontalLayout_4->addWidget(playCard1);

        horizontalSpacer_7 = new QSpacerItem(100, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        stackedWidget->addWidget(PassOrPlayPage);
        callLordPage = new QWidget();
        callLordPage->setObjectName("callLordPage");
        horizontalLayout_5 = new QHBoxLayout(callLordPage);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        giveup = new MyButton(callLordPage);
        giveup->setObjectName("giveup");

        horizontalLayout_5->addWidget(giveup);

        horizontalSpacer_9 = new QSpacerItem(40, 10, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        oneScore = new MyButton(callLordPage);
        oneScore->setObjectName("oneScore");

        horizontalLayout_5->addWidget(oneScore);

        horizontalSpacer_10 = new QSpacerItem(40, 10, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        twoScore = new MyButton(callLordPage);
        twoScore->setObjectName("twoScore");

        horizontalLayout_5->addWidget(twoScore);

        horizontalSpacer_11 = new QSpacerItem(40, 10, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        threeScore = new MyButton(callLordPage);
        threeScore->setObjectName("threeScore");

        horizontalLayout_5->addWidget(threeScore);

        horizontalSpacer_12 = new QSpacerItem(1, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        stackedWidget->addWidget(callLordPage);
        nullpage = new QWidget();
        nullpage->setObjectName("nullpage");
        stackedWidget->addWidget(nullpage);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ButtonGroup);

        QMetaObject::connectSlotsByName(ButtonGroup);
    } // setupUi

    void retranslateUi(QWidget *ButtonGroup)
    {
        ButtonGroup->setWindowTitle(QCoreApplication::translate("ButtonGroup", "Form", nullptr));
        start->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        playCard->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        pass->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        playCard1->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        giveup->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        oneScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        twoScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        threeScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButtonGroup: public Ui_ButtonGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONGROUP_H
