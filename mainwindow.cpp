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

    //boxed lineEdits

    //add some normal lineEdits
    QLineEdit *lineEdit_normal = new QLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_normal);

    lineEdit_normal->setPlaceholderText("Lorem ipsum");
    lineEdit_normal->setAlignment(Qt::AlignHCenter);

    QLineEdit *lineEdit_normal_action = new QLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_normal_action);

    lineEdit_normal_action->setPlaceholderText("Lorem ipsum");
    lineEdit_normal_action->addAction(QIcon(QStringLiteral(":/icons/search.svg")), QLineEdit::LeadingPosition);


    //add some example lineEdits
    AnimatedLineEdit *lineEdit_animated = new AnimatedLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_animated);

    lineEdit_animated->setPlaceholderText("Lorem ipsum");
    lineEdit_animated->setText("testtext");
    lineEdit_animated->setAlignment(Qt::AlignHCenter);

    AnimatedLineEdit *lineEdit_animated_trailing = new AnimatedLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_animated_trailing);

    lineEdit_animated_trailing->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);
    lineEdit_animated_trailing->setPlaceholderText("Lorem ipsum");

    connect(ui->pushButton, &QPushButton::clicked, this, [=]{
        lineEdit_animated_trailing->setText("TextText");
    });

    AnimatedLineEdit *lineEdit_animated_leading = new AnimatedLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_animated_leading);

    lineEdit_animated_leading->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_leading->setBorderColorNoFocus("lightgreen");
    lineEdit_animated_leading->setBorderColorFocus("darkgreen");
    QToolButton *b = lineEdit_animated_leading->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    connect(b, &QToolButton::clicked, this, [=]{
        qDebug() << "search btn";
    });

    AnimatedLineEdit *lineEdit_animated_multipleButtons = new AnimatedLineEdit(this);
    ui->verticalLayout_boxedEdits->addWidget(lineEdit_animated_multipleButtons);

    lineEdit_animated_multipleButtons->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);





    //underlined lineEdits

    //add some normal lineEdits
    QLineEdit *lineEdit_normal_underlined = new QLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_normal_underlined);

    lineEdit_normal_underlined->setPlaceholderText("Lorem ipsum");

    QLineEdit *lineEdit_normal_action_underlined = new QLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_normal_action_underlined);

    lineEdit_normal_action_underlined->setPlaceholderText("Lorem ipsum");
    lineEdit_normal_action_underlined->addAction(QIcon(QStringLiteral(":/icons/search.svg")), QLineEdit::LeadingPosition);


    //add some example lineEdits
    AnimatedLineEdit *lineEdit_animated_underlined = new AnimatedLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_animated_underlined);

    lineEdit_animated_underlined->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_underlined->setStyle(AnimatedLineEdit::Underlined);

    AnimatedLineEdit *lineEdit_animated_trailing_underlined = new AnimatedLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_animated_trailing_underlined);

    lineEdit_animated_trailing_underlined->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);
    lineEdit_animated_trailing_underlined->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_trailing_underlined->setStyle(AnimatedLineEdit::Underlined);

    AnimatedLineEdit *lineEdit_animated_leading_underlined = new AnimatedLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_animated_leading_underlined);

    lineEdit_animated_leading_underlined->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_leading_underlined->setStyle(AnimatedLineEdit::Underlined);
    lineEdit_animated_leading_underlined->setBorderColorNoFocus("lightgreen");
    lineEdit_animated_leading_underlined->setBorderColorFocus("darkgreen");
    QToolButton *b2 = lineEdit_animated_leading_underlined->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    connect(b2, &QToolButton::clicked, this, [=]{
        qDebug() << "search btn";
    });

    AnimatedLineEdit *lineEdit_animated_multipleButtons_underlined = new AnimatedLineEdit(this);
    ui->verticalLayout_underlinedEdits->addWidget(lineEdit_animated_multipleButtons_underlined);

    lineEdit_animated_multipleButtons_underlined->setPlaceholderText("Lorem ipsum");
    lineEdit_animated_multipleButtons_underlined->setStyle(AnimatedLineEdit::Underlined);
    lineEdit_animated_multipleButtons_underlined->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons_underlined->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::LeadingPosition);
    lineEdit_animated_multipleButtons_underlined->addActionButton(QIcon(QStringLiteral(":/icons/search.svg")), QString(), QLineEdit::TrailingPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}
