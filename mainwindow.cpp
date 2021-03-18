#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QDebug>

#include "AnimatedLineEdit/animatedlineedit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AnimatedLineEdit Examples");

    //add some normal lineEdits
    QLineEdit *lineEdit_normal = new QLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_normal);

    lineEdit_normal->setPlaceholderText("Lorem ipsum");

    QLineEdit *lineEdit_normal_action = new QLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_normal_action);

    lineEdit_normal_action->setPlaceholderText("Lorem ipsum");
    lineEdit_normal_action->addAction(QIcon(QStringLiteral(":/icons/search.svg")), QLineEdit::LeadingPosition);


    //add some example lineEdits
    AnimatedLineEdit *lineEdit_animated = new AnimatedLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_animated);

    lineEdit_animated->setPlaceholderText("Lorem ipsum");

    AnimatedLineEdit *lineEdit_animated_trailing = new AnimatedLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_animated_trailing);

    lineEdit_animated_trailing->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);
    lineEdit_animated_trailing->setPlaceholderText("Lorem ipsum");

    AnimatedLineEdit *lineEdit_animated_leading = new AnimatedLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_animated_leading);

    lineEdit_animated_leading->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_leading->setBorderColorNoFocus("lightgreen");
    lineEdit_animated_leading->setBorderColorFocus("darkgreen");
    QToolButton *b = lineEdit_animated_leading->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    connect(b, &QToolButton::clicked, this, [=]{
        qDebug() << "search btn";
    });

    AnimatedLineEdit *lineEdit_animated_multipleButtons = new AnimatedLineEdit(this);
    ui->centralwidget->layout()->addWidget(lineEdit_animated_multipleButtons);

    lineEdit_animated_multipleButtons->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}

