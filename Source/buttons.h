#ifndef BUTTONS_H
#define BUTTONS_H
#include <QPushButton>
#include <QMouseEvent>

//namespace Ui {
//class Buttons;
//}

class Buttons : public QPushButton
{
    Q_OBJECT
public:
    explicit Buttons(int buttonSize,bool mine,QWidget *parent = 0);
    ~Buttons();
    void setMine(bool mine);
    bool isMine();
    void setFlag();
    bool isFlagged();
    void unClickedButton();
    void clickedButton();
    void mineButton();
    void flagButton();

signals:
    void rightClicked();

public slots:
    void mousePressEvent(QMouseEvent *e);
private:
//    Ui::Buttons *ui;
    bool bMine;
    bool bFlag;
    int iconSize;

};

#endif // BUTTONS_H
