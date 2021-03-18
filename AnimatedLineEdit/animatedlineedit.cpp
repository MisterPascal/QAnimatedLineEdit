#include "animatedlineedit.h"

#include <QPropertyAnimation>
#include <QPainterPath>
#include <QToolButton>
#include <QLabel>
#include <QPainter>

#define MARGINS 5

AnimatedLineEdit::AnimatedLineEdit(QWidget *parent) :
    QLineEdit(parent),
    mPlaceholderLabel(new QLabel(this)){

    mPlaceholderLabel->setStyleSheet(QStringLiteral("QLabel{ color: gray; }"));
    mPlaceholderLabel->setAlignment(Qt::AlignCenter);
    setStyleSheet(QStringLiteral("QLineEdit{ border-radius: ") + QString::number(mBorderRadius) + QStringLiteral("; background-color: transparent; }"));
    setFrame(false);
}

void AnimatedLineEdit::resizeEvent(QResizeEvent *){

    updateTextMargins();

    //set margin to have some space at the top
    setContentsMargins(1, mPlaceholderLabel->sizeHint().height() / 2, 1, 1);

    if(leadingButtons.isEmpty())
        setTextMargins(mPlaceholderLabel->sizeHint().height() / 2, MARGINS, MARGINS, MARGINS);

    setMinimumHeight(mPlaceholderLabel->sizeHint().height()*2 + MARGINS * 2);

    mPlaceHolderRect = QRect(textLeftMargin, contentsRect().center().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height());
    mPlaceholderLabel->setGeometry(mPlaceHolderRect);

    for (int i = 0; i < leadingButtons.count(); ++i) {
        leadingButtons.at(i)->setGeometry(MARGINS + mButtonSize * i+1 /*icon spaces*/, contentsRect().center().y() - mButtonSize/2, mButtonSize, mButtonSize);
    }

    for (int i = 0; i < trailingButtons.count(); ++i) {
        trailingButtons.at(i)->setGeometry(contentsRect().right() - MARGINS*2 /*some right space*/ - mButtonSize * (i+1) /*icon spaces*/, contentsRect().center().y() - mButtonSize/2, mButtonSize, mButtonSize);
    }

}

void AnimatedLineEdit::focusInEvent(QFocusEvent *e){
    if(text().isEmpty()){
        QPropertyAnimation *animation = new QPropertyAnimation(mPlaceholderLabel, "geometry", this);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setDuration(mAnimationSpeed);
        animation->setStartValue(mPlaceholderLabel->geometry());
        animation->setEndValue(QRect(textLeftMargin, contentsRect().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height()));

        animation->start(QPropertyAnimation::DeleteWhenStopped);

        QFont f = mPlaceholderLabel->font();
        placeholderTextFontSize.first = f.pointSize();

        if(placeholderTextFontSize.first < 0){ //point size is not working on all sysmtems, eg. Android delivers wrong values sometimes
            placeholderTextFontSize.first = f.pixelSize();
            placeholderTextFontSize.second = true;
        }

        if(placeholderTextFontSize.second)
            f.setPixelSize((int)placeholderTextFontSize.first*0.9);
        else
            f.setPointSizeF((int)placeholderTextFontSize.first*0.9);

        mPlaceholderLabel->setFont(f);
    }

    QLineEdit::focusInEvent(e);
}

void AnimatedLineEdit::focusOutEvent(QFocusEvent *e){
    if(text().isEmpty()){
        QPropertyAnimation *animation = new QPropertyAnimation(mPlaceholderLabel, "geometry", this);
        animation->setEasingCurve(QEasingCurve::InCubic);
        animation->setDuration(mAnimationSpeed);
        animation->setStartValue(mPlaceholderLabel->geometry());
        animation->setEndValue(mPlaceHolderRect);
        animation->start(QPropertyAnimation::DeleteWhenStopped);

        QFont f = mPlaceholderLabel->font();

        if(placeholderTextFontSize.second)
            f.setPixelSize((int)placeholderTextFontSize.first);
        else
            f.setPointSizeF((int)placeholderTextFontSize.first);

        mPlaceholderLabel->setFont(f);
    }
    QLineEdit::focusOutEvent(e);
}

void AnimatedLineEdit::paintEvent(QPaintEvent *event){
    QLineEdit::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if(hasFocus())
        p.setPen(QPen(QBrush(mBorderColorFocus), 1));
    else
        p.setPen(QPen(QBrush(mBorderColorNoFocus), 1));

    QPainterPath path;

    if(mStyle == Style::Underlined){
        path.moveTo(contentsRect().left() + mBorderRadius, contentsRect().bottom()); //bottom left
        path.lineTo(contentsRect().right() + mBorderRadius, contentsRect().bottom()); //bottom right
    }
    else{
        if(hasFocus() && !mPlaceholderLabel->text().isEmpty()){
            path.moveTo(mPlaceholderLabel->geometry().right() + mBorderRadius * 2, contentsRect().top()); //top left
        }
        else{
            path.moveTo(contentsRect().left() + mBorderRadius, contentsRect().top()); //top left
        }

        path.lineTo(contentsRect().right() - mBorderRadius, contentsRect().top()); //top right
        path.arcTo(QRect(contentsRect().right() - mBorderRadius*2, contentsRect().top(), mBorderRadius*2, mBorderRadius*2), 90, -90); //top right corner
        path.lineTo(contentsRect().right(), contentsRect().bottom() - mBorderRadius); //bottom right
        path.arcTo(QRect(contentsRect().right() - mBorderRadius*2, contentsRect().bottom() - mBorderRadius*2, mBorderRadius*2, mBorderRadius*2), 0, -90); //bottom right corner
        path.lineTo(contentsRect().left() + mBorderRadius, contentsRect().bottom()); //bottom left
        path.arcTo(QRect(contentsRect().left(), contentsRect().bottom() - mBorderRadius*2, mBorderRadius*2, mBorderRadius*2), -90, -90); //bottom left corner
        path.lineTo(contentsRect().left(), contentsRect().top() + mBorderRadius); //top left
        path.arcTo(QRect(contentsRect().left(), contentsRect().top(), mBorderRadius*2, mBorderRadius*2), 180, -90); //top left corner

        if(hasFocus() && !mPlaceholderLabel->text().isEmpty()){
            int left = mPlaceholderLabel->geometry().left() - mBorderRadius*2;
            if(left < mBorderRadius)
                left = mBorderRadius;
            path.lineTo(left, contentsRect().top()); //to the label
        }
    }

    p.drawPath(path);
}

void AnimatedLineEdit::setStyle(const AnimatedLineEdit::Style &style){
    mStyle = style;
}

void AnimatedLineEdit::setBorderColorFocus(const QColor &borderColorFocus){
    mBorderColorFocus = borderColorFocus;
    update();
}

void AnimatedLineEdit::setBorderColorNoFocus(const QColor &borderColorNoFocus){
    mBorderColorNoFocus = borderColorNoFocus;
    update();
}

void AnimatedLineEdit::setAnimationSpeed(int animationSpeed){
    mAnimationSpeed = animationSpeed;
}

void AnimatedLineEdit::setButtonSize(int value){
    mButtonSize = value;
    update();
}

void AnimatedLineEdit::setBorderRadius(int value){
    mBorderRadius = value;
    update();
}

void AnimatedLineEdit::setPlaceholderText(const QString &placeholderTextOverride){
    mPlaceholderLabel->setText(placeholderTextOverride);
}

void AnimatedLineEdit::setFont(const QFont & f){
    mPlaceholderLabel->setFont(f);
    QLineEdit::setFont(f);
}

QToolButton *AnimatedLineEdit::addActionButton(const QIcon &icon, const QString &text, ActionPosition pos){
    QToolButton *b = new QToolButton(this);
    b->setIcon(icon);
    b->setText(text);
    b->setStyleSheet("QToolButton{background: transparent; border: 1px solid transparent;}"); //without border we got a black button an Android
    b->setCursor(Qt::ArrowCursor);

    if(pos == LeadingPosition)
        leadingButtons.append(b);
    if(pos == TrailingPosition)
        trailingButtons.append(b);

    updateTextMargins();
    return b;
}

void AnimatedLineEdit::updateTextMargins(){
    textLeftMargin = MARGINS*2 /*some left space*/ + (mButtonSize) * leadingButtons.count() /*icon spaces*/;
    textRightMargin = MARGINS*2 /*some right space*/ + (contentsRect().height() / 2) * trailingButtons.count() /*icon spaces*/;
    setTextMargins(textLeftMargin, MARGINS, textRightMargin, MARGINS);
}
