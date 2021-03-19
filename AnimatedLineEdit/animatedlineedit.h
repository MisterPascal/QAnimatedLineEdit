#ifndef ANIMATEDLINEEDIT_H
#define ANIMATEDLINEEDIT_H

#include <QLineEdit>

class QLabel;

class AnimatedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    enum Style{
        Boxed,
        Underlined
    };

    explicit AnimatedLineEdit(QWidget *parent = nullptr);

    virtual void setPlaceholderText(const QString &);
    virtual void setFont(const QFont &);

    //we cant use the addAction function cause the actions wouldnt be centered cause of the contentMargin
    QToolButton *addActionButton(const QIcon &icon, const QString &text = QString(), ActionPosition pos = LeadingPosition);

    void setBorderRadius(int value);
    void setButtonSize(int value);
    void setAnimationSpeed(int animationSpeed);
    void setBorderColorFocus(const QColor &borderColorFocus);
    void setBorderColorNoFocus(const QColor &borderColorNoFocus);
    void setStyle(const AnimatedLineEdit::Style &style);

protected:
    void resizeEvent(QResizeEvent *);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void paintEvent(QPaintEvent*);

private:
    AnimatedLineEdit::Style mStyle = Style::Boxed;
    QLabel *mPlaceholderLabel; //label which replaces the placeholder and moves to the top when focus in
    QRect mPlaceHolderRectTop; //position of the placeholderLabel if at the top
    QRect mPlaceHolderRectCenter; //position of the placeholderLabel if at the center

    QColor mBorderColorFocus = QColor("#F26026");
    QColor mBorderColorNoFocus = QColor("gray");

    int mBorderRadius = 5;
    int mButtonSize = 25;

    int mAnimationSpeed = 200;

    QPair<int, bool> placeholderTextFontSize = QPair<int, bool>(12, false); //pixelSize = false -> use to store old font size and type

    int textLeftMargin = 0;
    int textRightMargin = 0;

    QList<QToolButton*> leadingButtons;
    QList<QToolButton*> trailingButtons;

    void updateTextMargins();
    void updatePlaceHolderLabelFontSize(bool placeholderAtTop);
};

#endif // ANIMATEDLINEEDIT_H
