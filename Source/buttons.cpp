#include "buttons.h"
#include <QDebug>

Buttons::Buttons(int buttonSize,bool mine,QWidget *parent): QPushButton(parent)
{
    iconSize = buttonSize;
    setMinimumSize(buttonSize,buttonSize);
    setMaximumSize(buttonSize,buttonSize);
    unClickedButton();
    QFont font = this->font();
    font.setPointSize(15);
    setFont(font);
    setMine(mine);
    bFlag = false;
}

void Buttons::setMine(bool mine){
    bMine = mine;
}

bool Buttons::isMine(){
    return bMine;
}

void Buttons::setFlag(){
    bFlag = !bFlag;
//    setText(bFlag? "?" : "");
}

bool Buttons::isFlagged(){
    return bFlag;
}

void Buttons::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton && (!isFlagged()))
        emit clicked();
    if(e->button()==Qt::RightButton)
        emit rightClicked();
    
}

void Buttons::unClickedButton()
{
    setStyleSheet("QPushButton{background-color: rgb(224, 255, 252); border-style: none;border:1px solid #666;}");
}

void Buttons::clickedButton()
{
    setStyleSheet("QPushButton{background-color: rgb(255, 230, 254); border-style: inset;border:1px solid #666;}");
    setEnabled(false);
}

void Buttons::mineButton()
{
    setIcon(QIcon(":/image/Mine_Image.png"));
    setIconSize(QSize(iconSize,iconSize));
}
void Buttons::flagButton()
{
    if(bFlag){
        setIcon(QIcon(":/image/Flag_Image.png"));
        setIconSize(QSize(iconSize,iconSize));
    }
    else
       setIcon(QIcon(""));
}

Buttons::~Buttons()
{
//    qDebug()<<"Button Destr";
}
