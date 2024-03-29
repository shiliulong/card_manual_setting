#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QHBoxLayout; // 前置声明 
class QPushButton;
class QLineEdit;
struct setPlayerCardInfo
{
    int m_rawCardID; // 对应到卡牌区的Button的ID
    QPushButton* m_ownedCardBtn;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    enum CARDCOLOR//枚举
    {
        RED,
        BLACK,
        PLUM,
        BLOCK,
        KING,
        MAX
    };
    
    enum
    {
        MAXCARDNUM4PLAYER = 13
    };

    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void BtnSlots();
    void UserSlots();
    void UserResetSlots();
    
    void RemoveCardFromPlayer();
private:
    QHBoxLayout* InitializeCard(CARDCOLOR cardType);
    QHBoxLayout* InitializePlayer(int playerIndex);
    int          GenCardUID();
    setPlayerCardInfo* GetPlayerCardInfo(QPushButton* btn);
    
private:
    QPushButton* m_btnPlayer1;
    QPushButton* m_btnPlayer2;
    QPushButton* m_btnPlayer3;
    QPushButton* m_btnReset4Player1;
    QPushButton* m_btnReset4Player2;
    QPushButton* m_btnReset4Player3;
    
    setPlayerCardInfo m_ownedCardsId4Player1[MAXCARDNUM4PLAYER];
    setPlayerCardInfo m_ownedCardsId4Player2[MAXCARDNUM4PLAYER];
    setPlayerCardInfo m_ownedCardsId4Player3[MAXCARDNUM4PLAYER];
    std::map<int, QPushButton*> m_mapAllCards;//存储所有卡牌的ID和对应的按钮指针的映射
    std::map<QPushButton*, int> m_mapBtnAddress2ID;
private:
    Ui::MainWindow *ui;
    
    int m_userFlag = 0;
};

#endif // MAINWINDOW_H
