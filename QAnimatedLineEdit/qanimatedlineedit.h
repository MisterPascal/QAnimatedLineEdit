#ifndef QANIMATEDLINEEDIT_H
#define QANIMATEDLINEEDIT_H

#include <QLineEdit>

class QLabel;

class QAnimatedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    enum Style{
        Boxed,
        Underlined
    };

    explicit QAnimatedLineEdit(QWidget *parent = nullptr);

    virtual void setPlaceholderText(const QString &);
    virtual void setFont(const QFont &);
    virtual void setClearButtonEnabled(bool);

    //we cant use the addAction function cause the actions wouldnt be centered cause of the contentMargin
    QToolButton *addActionButton(const QIcon &icon, const QString &text = QString(), ActionPosition pos = LeadingPosition);

    void setBorderRadius(int value);
    void setButtonSize(int value);
    void setAnimationSpeed(int animationSpeed);
    void setBorderColorFocus(const QColor &borderColorFocus);
    void setBorderColorNoFocus(const QColor &borderColorNoFocus);
    void setStyle(const QAnimatedLineEdit::Style &style);

public slots:
    void setText(const QString &);

protected:
    void resizeEvent(QResizeEvent *);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void paintEvent(QPaintEvent*);

private slots:
    void updateAfterTextChange();
    void updateButtonsPosition();
    void updatePlaceHolderRects();
    void updatePlaceHolderLabelFontSize(bool placeholderAtTop);
    void updateTextMargins();

private:
    QAnimatedLineEdit::Style mStyle = Style::Boxed;
    QLabel *mPlaceholderLabel; //label which replaces the placeholder and moves to the top when focus in
    QRect mPlaceHolderRectTop; //position of the placeholderLabel if at the top
    QRect mPlaceHolderRectCenter; //position of the placeholderLabel if at the center

    QColor mBorderColorFocus = QColor("#F26026");
    QColor mBorderColorNoFocus = QColor("gray");

    int mBorderRadius = 5;
    int mButtonSize = 25;

    int mAnimationSpeed = 200;

    QPair<int, bool> placeholderTextFontSize = QPair<int, bool>(12, false); //pixelSize = false -> use to store old font size and type
    int mPlaceholderFontHeight = 12;

    int textLeftMargin = 0;
    int textRightMargin = 0;

    QList<QToolButton*> leadingButtons;
    QList<QToolButton*> trailingButtons;

    QToolButton *mClearButton;
};

#endif // QANIMATEDLINEEDIT_H
