#include "qanimatedlineedit.h"

#include <QPropertyAnimation>
#include <QPainterPath>
#include <QToolButton>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>

#define MARGINS 5

QAnimatedLineEdit::QAnimatedLineEdit(QWidget *parent) :
    QLineEdit(parent),
    mPlaceholderLabel(new QLabel(this)){

    mPlaceholderLabel->setStyleSheet(QStringLiteral("QLabel{ color: gray; }"));
    mPlaceholderLabel->setAlignment(Qt::AlignCenter);
    setStyleSheet(QStringLiteral("QLineEdit{ border-radius: ") + QString::number(mBorderRadius) + QStringLiteral("; background-color: transparent; } QLineEdit::disabled{ color: gray; }"));
    setFrame(false);
    setFont(font()); //reaply same font to store current values

    mClearButton = addActionButton(QIcon(QStringLiteral(":/icons/backspace.svg")), QString(), QLineEdit::TrailingPosition);
    mClearButton->setVisible(false);
    connect(mClearButton, &QToolButton::clicked, this, &QAnimatedLineEdit::clear);
    setClearButtonEnabled(true);

    connect(this, &QAnimatedLineEdit::textChanged, this, &QAnimatedLineEdit::updateAfterTextChange);
}

void QAnimatedLineEdit::resizeEvent(QResizeEvent *){

    updateTextMargins();

    //set top-margin to have some space at the top
    setContentsMargins(1, mPlaceholderLabel->sizeHint().height() / 2, 1, mPlaceholderLabel->sizeHint().height() / 2);

    updatePlaceHolderLabelFontSize(!text().isEmpty() || hasFocus());

    updatePlaceHolderRects();

    mPlaceholderLabel->setGeometry((text().isEmpty() && !hasFocus()) ? mPlaceHolderRectCenter : mPlaceHolderRectTop);

    updateButtonsPosition();
}

void QAnimatedLineEdit::focusInEvent(QFocusEvent *e){
    if(text().isEmpty()){
        updatePlaceHolderRects();
        QPropertyAnimation *animation = new QPropertyAnimation(mPlaceholderLabel, "geometry", this);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setDuration(mAnimationSpeed);
        animation->setStartValue(mPlaceholderLabel->geometry());
        animation->setEndValue(mPlaceHolderRectTop);
        animation->start(QPropertyAnimation::DeleteWhenStopped);

        connect(animation, &QPropertyAnimation::finished, this, QOverload<>::of(&QAnimatedLineEdit::update));
        updatePlaceHolderLabelFontSize(true);
    }

    QLineEdit::focusInEvent(e);
}

void QAnimatedLineEdit::focusOutEvent(QFocusEvent *e){
    if(text().isEmpty()){
        QPropertyAnimation *animation = new QPropertyAnimation(mPlaceholderLabel, "geometry", this);
        animation->setEasingCurve(QEasingCurve::InCubic);
        animation->setDuration(mAnimationSpeed);
        animation->setStartValue(mPlaceholderLabel->geometry());
        animation->setEndValue(mPlaceHolderRectCenter);
        animation->start(QPropertyAnimation::DeleteWhenStopped);

        connect(animation, &QPropertyAnimation::finished, this, QOverload<>::of(&QAnimatedLineEdit::update));
        connect(animation, &QPropertyAnimation::finished, this, &QAnimatedLineEdit::updateAfterTextChange); //ensure the label is at the top if you use setText while youre in focus or animation

        updatePlaceHolderLabelFontSize(false);
    }
    QLineEdit::focusOutEvent(e);
}

void QAnimatedLineEdit::paintEvent(QPaintEvent *event){
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
        bool isOutsideContentRect = (mPlaceholderLabel->y() <= contentsRect().y());
        if((hasFocus() || isOutsideContentRect) && !mPlaceholderLabel->text().isEmpty()){
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

        if((hasFocus() || isOutsideContentRect) && !mPlaceholderLabel->text().isEmpty()){
            int left = mPlaceholderLabel->geometry().left() - mBorderRadius*2;
            if(left < mBorderRadius)
                left = mBorderRadius;
            path.lineTo(left, contentsRect().top()); //to the label
        }
    }

    p.drawPath(path);
}

void QAnimatedLineEdit::setPlaceholderText(const QString &placeholderTextOverride){
    mPlaceholderLabel->setText(placeholderTextOverride);
}

void QAnimatedLineEdit::setFont(const QFont & f){
    mPlaceholderLabel->setFont(f);
    mPlaceholderFontHeight = mPlaceholderLabel->fontMetrics().height();
//    placeholderTextFontSize.first = f.pointSize();

//    if(placeholderTextFontSize.first < 0){ //point size is not working on all sysmtems, eg. Android delivers wrong values sometimes
//        placeholderTextFontSize.first = f.pixelSize();
//        placeholderTextFontSize.second = true; //set second to true if we use a pixelSize instead of a pointSize
//    }

    QLineEdit::setFont(f);
}

void QAnimatedLineEdit::setClearButtonEnabled(bool enable){
    if(isReadOnly() || property("noClearButton").toBool() || !enable){
        mClearButton->setEnabled(false); //set disabled to use after text change
        mClearButton->setVisible(false);
        return;
    }
    mClearButton->setEnabled(true); //set enabled to use after text change
}

QToolButton *QAnimatedLineEdit::addActionButton(const QIcon &icon, const QString &text, ActionPosition pos){
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


void QAnimatedLineEdit::setStyle(const QAnimatedLineEdit::Style &style){
    mStyle = style;
}

void QAnimatedLineEdit::setBorderColorFocus(const QColor &borderColorFocus){
    mBorderColorFocus = borderColorFocus;
    update();
}

void QAnimatedLineEdit::setBorderColorNoFocus(const QColor &borderColorNoFocus){
    mBorderColorNoFocus = borderColorNoFocus;
    update();
}

void QAnimatedLineEdit::setAnimationSpeed(int animationSpeed){
    mAnimationSpeed = animationSpeed;
}

void QAnimatedLineEdit::setButtonSize(int value){
    mButtonSize = value;
    update();
}

void QAnimatedLineEdit::setBorderRadius(int value){
    mBorderRadius = value;
    update();
}

void QAnimatedLineEdit::updateAfterTextChange(){
    if(mPlaceholderLabel->y() > contentsRect().y()){ //check if placeholderLabel is still in the center
        resizeEvent(new QResizeEvent(size(), size())); //reuse the resizeEvent
    }
    mClearButton->setVisible(!text().isEmpty() && mClearButton->isEnabled());
    updateButtonsPosition();
}

void QAnimatedLineEdit::setText(const QString & t){
    QLineEdit::setText(t);
    updateAfterTextChange();
}

void QAnimatedLineEdit::updateButtonsPosition(){
    int visibleBtnCount = 0; //dont use i cause we need a separate counter to ignore unvisible buttons
    for (int i = 0; i < leadingButtons.count(); ++i) {
        if(!leadingButtons.at(i)->isVisible()){ //ignore unvisible btns eg unvisible clearbtn
            continue;
        }
        leadingButtons.at(i)->setGeometry(MARGINS + mButtonSize * visibleBtnCount /*icon spaces*/, contentsRect().center().y() - mButtonSize/2, mButtonSize, mButtonSize);
        visibleBtnCount++;
    }

    visibleBtnCount = 1;
    for (int i = 0; i < trailingButtons.count(); ++i) {
        if(!trailingButtons.at(i)->isVisible()){ //ignore unvisible btns eg unvisible clearbtn
            continue;
        }
        trailingButtons.at(i)->setGeometry(contentsRect().right() - MARGINS*2 /*some right space*/ - mButtonSize * visibleBtnCount /*icon spaces*/, contentsRect().center().y() - mButtonSize/2, mButtonSize, mButtonSize);
        visibleBtnCount++;
    }
}

void QAnimatedLineEdit::updatePlaceHolderRects(){
    if(alignment().testFlag(Qt::AlignCenter) || alignment().testFlag(Qt::AlignHCenter)){
        mPlaceHolderRectCenter = QRect(contentsRect().center().x() - mPlaceholderLabel->sizeHint().width()/2, contentsRect().center().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height());
        mPlaceHolderRectTop = QRect(contentsRect().center().x() - mPlaceholderLabel->sizeHint().width()/2, contentsRect().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height());
    }
    else{
        mPlaceHolderRectCenter = QRect(textLeftMargin, contentsRect().center().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height());
        mPlaceHolderRectTop = QRect(textLeftMargin, contentsRect().y() - mPlaceholderLabel->sizeHint().height()/2, mPlaceholderLabel->sizeHint().width(), mPlaceholderLabel->sizeHint().height());
    }
}

void QAnimatedLineEdit::updatePlaceHolderLabelFontSize(bool placeholderAtTop){
    if(placeholderAtTop){
        QFont f = mPlaceholderLabel->font();

        if(placeholderTextFontSize.second)
            f.setPixelSize((int)placeholderTextFontSize.first*0.9);
        else
            f.setPointSizeF((int)placeholderTextFontSize.first*0.9);

        mPlaceholderLabel->setFont(f);
    }
    else{
        QFont f = mPlaceholderLabel->font();

        if(placeholderTextFontSize.second)
            f.setPixelSize((int)placeholderTextFontSize.first);
        else
            f.setPointSizeF((int)placeholderTextFontSize.first);

        mPlaceholderLabel->setFont(f);
    }


}

void QAnimatedLineEdit::updateTextMargins(){
    textLeftMargin = MARGINS*2 /*some left space*/ + (mButtonSize) * leadingButtons.count() /*icon spaces*/;
    textRightMargin = MARGINS*2 /*some right space*/ + (contentsRect().height() / 2) * trailingButtons.count() /*icon spaces*/;
    setTextMargins(textLeftMargin, MARGINS, textRightMargin, MARGINS);
}
