#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <ctime>
#include <QSignalMapper>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Minesweeper");
    srand(time(NULL));
    level= ui->comboBox->currentText();
    b_Size = 10;
    totalMines = 10;
    firstClick = 1;
    mainWidth = buttonSize * b_Size;
    mainHeight = (buttonSize * b_Size)+60;
    this->setMinimumSize(mainWidth,mainHeight);
    this->setMaximumSize(mainWidth,mainHeight);
    randNum.resize(10);
    pButtons.resize(100);
    createBoard();

    connect(ui->comboBox,&QComboBox::currentTextChanged,this,&MainWindow::initialize);


}
/****************************************************************************************************/
/* A Function for initial settings on three different levels such as Easy,Medium,Hard.              */
/****************************************************************************************************/
void MainWindow::initialize(QString level)
{
    ui->comboBox->setCurrentText(level);
    for (Buttons *obj : pButtons)
        delete obj;

    if(level == "Easy")
    {
        b_Size = 10;
        totalMines = 10;
        firstClick = 1;
        totalFlags = 10;
        buttonSize = 40;
        mainWidth = buttonSize * b_Size;
        mainHeight = (buttonSize * b_Size)+60;
        randNum.resize(10);
        pButtons.clear();
        pButtons.resize(100);

    }
    else if(level == "Medium")
    {
        b_Size = 16;
        totalMines = 40;
        firstClick = 1;
        totalFlags = 40;
        buttonSize = 30;
        mainWidth = buttonSize * b_Size;
        mainHeight = (buttonSize * b_Size)+60;
        randNum.resize(40);
        pButtons.clear();
        pButtons.resize(256);
    }
    else if(level == "Hard")
    {
        b_Size = 20;
        totalMines = 99;
        firstClick = 1;
        totalFlags = 99;
        buttonSize = 25;
        mainWidth = buttonSize * b_Size;
        mainHeight = (buttonSize * b_Size)+60;
        randNum.resize(99);
        pButtons.clear();
        pButtons.resize(400);
    }

    ui->flagCountLabel->setText(QString::number(totalMines));
    createBoard();
}
/****************************************************************************************************/
/* A Common Function for creating the game board based on the choosen level.                        */
/****************************************************************************************************/
void MainWindow::createBoard()
{
    QSignalMapper *mouseLeftMapper;
    QSignalMapper *mouseRightMapper;
    mouseLeftMapper = new QSignalMapper(this);
    mouseRightMapper = new QSignalMapper(this);

    //    for(int i=0; i<10; i++){
    //        for(int j=0; j<10; j++){
    //            pButtons[i][j] = new Buttons;
    //            ui->gridLayout->addWidget(pButtons[i][j],i,j);
    //        }
    //    }

    for(auto &ii: pButtons){
        ii= new Buttons(buttonSize,false);

    }
    int index = 0;
    for(int i=0; i<b_Size; i++)
    {
        for(int j=0; j<b_Size; j++)
        {
            index = (i*b_Size)+j;
            ui->gridLayout->addWidget(pButtons.at(index),i,j);
            //event binding
            mouseLeftMapper->setMapping(pButtons.at(index),index);
            mouseRightMapper->setMapping(pButtons.at(index),index);
            connect(pButtons.at(index),SIGNAL(clicked(bool)),mouseLeftMapper,SLOT(map()));
            connect(pButtons.at(index),SIGNAL(rightClicked()),mouseRightMapper,SLOT(map()));
        }
    }
    this->setMinimumSize(mainWidth,mainHeight);
    this->setMaximumSize(mainWidth,mainHeight);
    connect(mouseLeftMapper, SIGNAL(mapped(int)),this, SLOT(buttonLeftClicked(int)));
    connect(mouseRightMapper, SIGNAL(mapped(int)),this, SLOT(buttonRightClicked(int)));
}
/****************************************************************************************************/
/* A Common Function for creating mines randomly without duplicates based on the choosen level.     */
/****************************************************************************************************/
void MainWindow::generateMines(int id)
{
    int upperLimit = (b_Size * b_Size);
    int randMine;
    for(int i=0; i<totalMines; i++){
        do{
            randMine = rand() % upperLimit;
            randNum.replace(i, randMine);
        }while((randNum[i] == id) || (pButtons.at(randNum[i])->isMine()));
        pButtons.at(randNum[i])->setMine(true);

//        pButtons.at(randNum[i])->setText("*");
//        qDebug()<<pButtons.at(randNum[i])->isMine()<<randNum[i];
    }
}
/****************************************************************************************************/
/* A slot for mouse left press event.                                                               */
/****************************************************************************************************/
void MainWindow::buttonLeftClicked(int id)
{
    if(firstClick == 1)
    {
        firstClick++;
        generateMines(id);
    }
    int row, column;
    row = id/b_Size;
    column = id%b_Size;

    if(pButtons.at(id)->isMine()){
        revealAllMines();
    }
    else{
        recursiveClick(row, column);
        winCheck();
    }

}
/****************************************************************************************************/
/* A slot for mouse right press event. It handles flagging and flag count.                          */
/****************************************************************************************************/
void MainWindow::buttonRightClicked(int id)
{
    if(pButtons.at(id)->isEnabled())
    {
        pButtons.at(id)->setFlag();
        if(pButtons.at(id)->isFlagged())
        {
            pButtons.at(id)->flagButton();
            totalFlags--;
        }
        else
        {
                pButtons.at(id)->flagButton();
                totalFlags++;
        }

    ui->flagCountLabel->setText(QString::number(totalMines));
    winCheck();

    }
}
/****************************************************************************************************/
/* A function that reveals all the mines on the game board and displays the game result.            */
/****************************************************************************************************/
void MainWindow::revealAllMines()
{
    for(int i=0; i<totalMines; i++)
    {
        pButtons.at(randNum[i])->mineButton();
    }
    QMessageBox::information(this, "Game Result", "Oops! You Lost.");
    emit ui->comboBox->currentTextChanged("Easy");
}
/****************************************************************************************************/
/* A function for counting the mines that are present around the clicked buttons.                   */
/****************************************************************************************************/
int MainWindow::mineCount(int row, int column)
{
    int index =0, count = 0;
    for(int i=row-1; i<=row+1; i++)
    {
        for(int j=column-1; j<=column+1; j++)
        {
            if(i>=0 && j>=0 && i<b_Size && j<b_Size)
            {
                index = (i*b_Size)+j;
                if(pButtons.at(index)->isMine())
                {
                    count++;
                }
            }
        }
    }
    return count;
}
/****************************************************************************************************/
/* A recursive function to reveal the empty buttons which are not close to the mines.               */
/* It also sets the mine count number on the UI button.                                             */
/****************************************************************************************************/
void MainWindow::recursiveClick(int row, int column)
{
    int count = 0,index = 0;
    index = (row*b_Size)+column;
    if(pButtons.at(index)->isFlagged()) return;

    pButtons.at(index)->clickedButton();
    count = mineCount(row, column);

    if(count != 0)
    {
        pButtons.at(index)->setText(QString::number(count));
    }
    else if((count == 0))
    {
        for(int i=row-1; i<=row+1; i++)
        {
            for(int j=column-1; j<=column+1; j++)
            {
                if(i>=0 && j>=0 && i<b_Size && j<b_Size)
                {
                    if(pButtons.at((i*b_Size)+j)->isEnabled())
                    {
                        recursiveClick(i,j);
                    }
                }
            }
        }
    }

}
/****************************************************************************************************/
/* A function which performs win check on every button click to find end of the game and winn       */
/* winning status.                                                                                  */
/****************************************************************************************************/
void MainWindow::winCheck()
{
    int count = 0;
    int index;
    for(int i=0; i<b_Size; i++)
    {
        for(int j=0; j<b_Size; j++)
        {
            index = (i*b_Size)+j;
            if(pButtons.at(index)->isMine() && pButtons.at(index)->isEnabled()){
                count++;
            }
            if(!pButtons.at(index)->isMine() && pButtons.at(index)->isEnabled())
                return;
        }
    }
    if(count == totalMines)
    {
        QMessageBox::information(this, "Game Result", "Congratulations! You Won.");
        emit ui->comboBox->currentTextChanged("Easy");
    }

}

MainWindow::~MainWindow()
{
    delete ui;

}


