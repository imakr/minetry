#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "buttons.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void buttonLeftClicked(int id);
    void buttonRightClicked(int id);

private:
    Ui::MainWindow *ui;
    int buttonSize = 40;
    int mainWidth;
    int mainHeight;
    int firstClick;
    //Buttons *pButtons[10][10];
    QVector <Buttons *> pButtons;
    QVector <int> randNum;
    int b_Size;
    int totalMines;
    int totalFlags;
    QString level;
    void createBoard();
    void initialize(QString level);
    void generateMines(int id);
    void revealAllMines();
    int mineCount(int row,int column);
    void recursiveClick(int row, int column);
    void winCheck();

};

#endif // MAINWINDOW_H
