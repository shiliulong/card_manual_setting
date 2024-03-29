#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    {// 初始化卡牌信息
        for (int i = 0; i < CARDCOLOR::MAX; ++i)
        ui->cardRegionLayout->addLayout(InitializeCard(CARDCOLOR(i)));        
    }
    
    {
        m_btnPlayer1 = new QPushButton("玩家1");
        m_btnPlayer2 = new QPushButton("玩家2");
        m_btnPlayer3 = new QPushButton("玩家3");
        m_btnPlayer1->setStyleSheet("background-color:rgb(119, 136, 153)");
        m_btnPlayer2->setStyleSheet("background-color:rgb(119, 136, 153)");
        m_btnPlayer3->setStyleSheet("background-color:rgb(119, 136, 153)");
        
        m_btnReset4Player1 = new QPushButton("重置玩家1");
        m_btnReset4Player2 = new QPushButton("重置玩家2");
        m_btnReset4Player3 = new QPushButton("重置玩家3");
    
        //将信号与槽函数（UserSlots）链接起来
        connect(m_btnPlayer1, SIGNAL(clicked()), this, SLOT(UserSlots()));
        connect(m_btnPlayer2, SIGNAL(clicked()), this, SLOT(UserSlots()));
        connect(m_btnPlayer3, SIGNAL(clicked()), this, SLOT(UserSlots()));
    
        //将信号与槽函数（UserResetSlots）链接起来
        connect(m_btnReset4Player1, SIGNAL(clicked()), this, SLOT(UserResetSlots()));
        connect(m_btnReset4Player2, SIGNAL(clicked()), this, SLOT(UserResetSlots()));
        connect(m_btnReset4Player3, SIGNAL(clicked()), this, SLOT(UserResetSlots()));
    }
    
    {// 初始化玩家牌
        for (int i = 0; i < 3; ++i)
            ui->playerRegionLayout->addLayout(InitializePlayer(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QHBoxLayout* MainWindow::InitializeCard(CARDCOLOR cardType)
{
    QString prefix;//prefix空字符串对象
    switch(cardType)
    {
    case RED:
        prefix = "红桃\n";
        break;
    case BLACK:
        prefix = "黑桃\n";
        break;
    case PLUM:
        prefix = "梅花\n";
        break;
    case BLOCK:
        prefix = "方块\n";
        break;
    case KING:
        prefix = "";//默认情况的代码块
        break;
    }
    
    QHBoxLayout *cardLayout = new QHBoxLayout();
    //for循环遍历整数变量i的取值范围从1到13（包括1和13）
    for (int i = 1; i <= 13; ++i)
    {
        QString cardName = prefix;
        
        if (CARDCOLOR::KING == cardType)
        {
            if (1 == i)
            {
                cardName += "大王";
            }
            else if(2 == i)
            {
                cardName += "小王";
            }
            else if (3 == i)
            {
                
            }
            else
            {
                break;
            }
            
        }
        else
        {
            if (1 == i)
                cardName += "A";
            else if(11 == i)
                cardName += "J";
            else if (12 == i)
                cardName += "Q";
            else if(13 == i)
                cardName += "K";
            else 
            {
                cardName += QString::number(i);//静态函数
            }
        }
        
        if (CARDCOLOR::KING == cardType && 3 == i)
        {
            auto horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
            cardLayout->addItem(horizontalSpacer);
        }
        else
        {
            auto btn = new QPushButton(cardName);
            btn->setFixedSize(50,50);
            cardLayout->addWidget(btn); //添加到水平布局中
            
            int id = GenCardUID();
            m_mapAllCards[id] = btn;
            m_mapBtnAddress2ID[btn] = id;
            
            //信号与槽函数（BtnSlots）进行关联
            connect(btn,SIGNAL(clicked(bool)),this,SLOT(BtnSlots()));
        }

    }
    
    return cardLayout;
}

QHBoxLayout *MainWindow::InitializePlayer(int playerIndex)//初始化玩家布局
{
    QPushButton* btn4Player = nullptr;
    QPushButton* btnReset4Player = nullptr;
    setPlayerCardInfo* ownedCards4Player = nullptr;
    if (0 == playerIndex)
    {
        btn4Player = m_btnPlayer1;
        btnReset4Player = m_btnReset4Player1;
        ownedCards4Player = m_ownedCardsId4Player1;
    }
    else if (1 == playerIndex)
    {
        btn4Player = m_btnPlayer2;
        btnReset4Player = m_btnReset4Player2;
        ownedCards4Player = m_ownedCardsId4Player2;
    }
    else
    {
        btn4Player = m_btnPlayer3;
        btnReset4Player = m_btnReset4Player3;
        ownedCards4Player = m_ownedCardsId4Player3;
    }

    QHBoxLayout *layout4Player = new QHBoxLayout();
    layout4Player->addWidget(btn4Player);

    for (int i = 0; i < MAXCARDNUM4PLAYER; ++i)
    {
        auto btn = new QPushButton();
        //btn->setEnabled(false);
        btn->setFixedSize(70, 26);
        layout4Player->addWidget(btn);
        
        ownedCards4Player[i].m_rawCardID = -1;//表示该卡牌并未被赋值
        ownedCards4Player[i].m_ownedCardBtn = btn;
        
        //信号与槽函数进行关联
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(RemoveCardFromPlayer()));
    }
    
    layout4Player->addWidget(btnReset4Player);

    return layout4Player;
}

int MainWindow::GenCardUID()//生成唯一的卡牌标识符
{
    static int s_lastGenID = 0;//在函数内部定义了一个静态局部变量
    return ++s_lastGenID;
}

setPlayerCardInfo* MainWindow::GetPlayerCardInfo(QPushButton* btn)
{
    if (btn == nullptr)//如果为空指针   这直接返回空指针
        return nullptr;
    
    setPlayerCardInfo* ownedCardsId4Player = nullptr;
    switch(m_userFlag)
    {
    case 1:
        ownedCardsId4Player = m_ownedCardsId4Player1;
        break;
    case 2:
        ownedCardsId4Player = m_ownedCardsId4Player2;
        break;
    case 3:
        ownedCardsId4Player = m_ownedCardsId4Player3;
        break;
    default:
        return nullptr;
    }
    
    for(int i = 0; i < MAXCARDNUM4PLAYER; ++i)
    {
        if (ownedCardsId4Player[i].m_ownedCardBtn == btn)
            return &(ownedCardsId4Player[i]);
    }
    
    return nullptr;
    
}

void MainWindow::UserSlots()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender()); 
    //qobject_cast对直接使用sender()返回的指针可能无法直接访问特定的对象成员或方法进行类型转换
    if (btn)
    {
        QString text = btn->text();
        if("玩家1" == text)
        {
            m_userFlag = 1;
            
            m_btnPlayer1->setStyleSheet("background-color:rgb(255, 0, 0)");
            m_btnPlayer2->setStyleSheet("background-color:rgb(119, 136, 153)");
            m_btnPlayer3->setStyleSheet("background-color:rgb(119, 136, 153)");
        }
        else if ("玩家2" == text) 
        {
            m_userFlag = 2;
            
            m_btnPlayer2->setStyleSheet("background-color:rgb(255, 0, 0)");
            m_btnPlayer1->setStyleSheet("background-color:rgb(119, 136, 153)");
            m_btnPlayer3->setStyleSheet("background-color:rgb(119, 136, 153)");
        }
        else if("玩家3" == text)
        {
            m_userFlag = 3;
            
            m_btnPlayer3->setStyleSheet("background-color:rgb(255, 0, 0)");
            m_btnPlayer2->setStyleSheet("background-color:rgb(119, 136, 153)");
            m_btnPlayer1->setStyleSheet("background-color:rgb(119, 136, 153)");
        }

    }
}

void MainWindow::BtnSlots()
{
    //sender()是一个QObject类的成员函数，用于返回发出信号的对象的指针
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    if (btn)
    {
        QString text = btn->text();
        text.remove("\n");

        if(m_userFlag == 1)
        {
            bool bFind = false;
            for (int i = 0; i < MAXCARDNUM4PLAYER; ++i)
            {
                if (m_ownedCardsId4Player1[i].m_rawCardID <= 0)
                {
                    bFind = true;
                    auto text = btn->text();
                    text.remove("\n");
                    m_ownedCardsId4Player1[i].m_ownedCardBtn->setText(text);
                    m_ownedCardsId4Player1[i].m_rawCardID = m_mapBtnAddress2ID[btn];
                    break;
                }
            }
            if(!bFind)
            {
                QMessageBox::information(nullptr,"提示","玩家手牌满");
                return;
            }
            
            btn->setEnabled(false);//将按钮设为不可用状态
        }
        else if(m_userFlag == 2)
        {
            bool bFind = false;
            for (int i = 0; i < MAXCARDNUM4PLAYER; ++i)
            {
                if (m_ownedCardsId4Player2[i].m_rawCardID <= 0)
                {
                    bFind = true;
                    auto text = btn->text();
                    text.remove("\n");
                    m_ownedCardsId4Player2[i].m_ownedCardBtn->setText(text);
                    m_ownedCardsId4Player2[i].m_rawCardID = m_mapBtnAddress2ID[btn];
                    break;
                }
            }
            if (!bFind)
            {
                QMessageBox::information(nullptr,"提示","玩家手牌满");
                return;
            }

            btn->setEnabled(false);
        }
        else if(m_userFlag == 3)
        {
            bool bFind = false;
            for (int i = 0; i < MAXCARDNUM4PLAYER; ++i)
            {
                if (m_ownedCardsId4Player3[i].m_rawCardID <= 0)
                {
                    bFind = true;
                    auto text = btn->text();
                    text.remove("\n");
                    m_ownedCardsId4Player3[i].m_ownedCardBtn->setText(text);
                    m_ownedCardsId4Player3[i].m_rawCardID = m_mapBtnAddress2ID[btn];
                    break;
                }
            }
            if (!bFind)
            {
                QMessageBox::information(nullptr,"提示","玩家手牌满");
                return;
            }
            
            btn->setEnabled(false);
        }
        else
        {
            QMessageBox::information(nullptr,"提示","请先选择玩家");
        }
    }
}

void MainWindow::UserResetSlots()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn)
    {
        setPlayerCardInfo* ownedCardsId4Player = nullptr;
        QString text = btn->text();
        if("重置玩家1" == text)
        {
            ownedCardsId4Player = m_ownedCardsId4Player1;
            m_btnPlayer1->setStyleSheet("background-color:rgb(119, 136, 153)");
        }
        else if ("重置玩家2" == text) 
        {
            ownedCardsId4Player = m_ownedCardsId4Player2;
            m_btnPlayer2->setStyleSheet("background-color:rgb(119, 136, 153)");
        }
        else if("重置玩家3" == text)
        {
            ownedCardsId4Player = m_ownedCardsId4Player3;
            m_btnPlayer3->setStyleSheet("background-color:rgb(119, 136, 153)");
        }
        
        for(int i = 0; i < MAXCARDNUM4PLAYER; ++i)
        {
            if(ownedCardsId4Player[i].m_rawCardID <= 0)
                continue;
            
            m_mapAllCards[ownedCardsId4Player[i].m_rawCardID]->setEnabled(true);
            
            ownedCardsId4Player[i].m_ownedCardBtn->setText("");//将当前元素的m_ownedCardBtn的文本设置为空字符串
            ownedCardsId4Player[i].m_rawCardID = -1;//卡牌重置
        }   
    }    
    
    m_userFlag = 0;
}

void MainWindow::RemoveCardFromPlayer()//移除玩家牌信息
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    
    setPlayerCardInfo* playerCardInfo = GetPlayerCardInfo(btn);
    if(playerCardInfo == nullptr)
    {
        QMessageBox::information(nullptr,"提示","未知错误!!");
        return;
    }
    if(m_mapAllCards.find(playerCardInfo->m_rawCardID) == m_mapAllCards.end())
    {
        QMessageBox::information(nullptr,"提示","未知错误!!");
        return;
    }
    m_mapAllCards[playerCardInfo->m_rawCardID]->setEnabled(true);

    playerCardInfo->m_rawCardID = -1;
    playerCardInfo->m_ownedCardBtn->setText("");
    
}








