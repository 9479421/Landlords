#include "bgmcontrol.h"

#include <PlayHand.h>
#include <QAudioOutput>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

BGMControl::BGMControl(QObject *parent)
    : QObject{parent}
{
    for(int i = 0 ; i < 5 ; ++i)
    {
        QMediaPlayer* player = new QMediaPlayer(this);
        QAudioOutput* audioOutput = new QAudioOutput(this);
        player->setAudioOutput(audioOutput);
        audioOutput->setVolume(1);

        if(i<2 || i == 4)
        {
            player->setLoops(QMediaPlayer::Once); //只播放一次
        }
        else if(i == 2)
        {
            player->setLoops(QMediaPlayer::Infinite);
        }


        m_players.push_back(player);


        m_sourceList.push_back(QVector<QUrl>()); //置入空url集合
    }
    initPlayList();
}

void BGMControl::initPlayList()
{
    QStringList list;
    list<< "Man" << "Woman" << "BGM" << "Other" << "Ending";
    // 读json配置文件
    QFile file(":/conf/playList.json");
    file.open(QFile::ReadOnly);
    QByteArray json = file.readAll();
    file.close();
    // 解析从文件中读出的json数据
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    for(int i = 0 ; i < list.size() ; i++){
        QString prefix = list.at(i);
        QJsonArray array = obj.value(prefix).toArray();
        for( int j = 0 ; j < array.size() ; j++){
            m_sourceList[i] << QUrl(array.at(j).toString());
        }
    }
}

void BGMControl::startBGM(float volume)
{
    m_players[2]->setSource(m_sourceList[2][0]);
    m_players[2]->audioOutput()->setVolume(volume);
    m_players[2]->play();
}

void BGMControl::stopBGM()
{
    m_players[2]->stop();
}

// 玩家下注了没有
// 玩家的性别
// 什么时候播放什么样的音频文件
void BGMControl::playerRobLordMusic(int point, RoleSex sex,bool isFirst)
{
    int index = sex == Man ? 0 : 1;
    if(isFirst && point > 0)
    {
        m_players[index]->setSource(m_sourceList[index][Order]);
    }
    else if(point == 0)
    {
        if(isFirst)
        {
            m_players[index]->setSource(m_sourceList[index][NoOrder]);
        }else
        {
            m_players[index]->setSource(m_sourceList[index][NoRob]);
        }
    }
    else if(point == 2)
    {
        m_players[index]->setSource(m_sourceList[index][Rob1]);
    }
    else if(point == 3)
    {
        m_players[index]->setSource(m_sourceList[index][Rob2]);
    }

    m_players[index]->play();
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, RoleSex sex)
{
    // 得到播放列表
    int index = sex == Man ? 0 : 1;

    Card::CardPoint pt = Card::CardPoint::Card_Begin;
    // 取出牌型 然后进行判断
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();

    if(type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair ||type == PlayHand::Hand_Triple )
    {
        pt = cards.takeRandCard().point();
    }
    int number = 0;
    switch(type)
    {
        //单牌
    case PlayHand::Hand_Single:
        number = pt - 1;
        break;
        //对牌
    case PlayHand::Hand_Pair:
        number = pt - 1 + 15;
        break;
        //三张点数相同的牌
    case PlayHand::Hand_Triple:
        number = pt - 1 + 15 + 13;
        break;
        //三代一
    case PlayHand::Hand_Triple_Single:
        number = ThreeBindOne;
        break;
        //三带二
    case PlayHand::Hand_Triple_Pair:
        number = ThreeBindPair;
        break;
        //飞机
    case PlayHand::Hand_Plane:
        //飞机带两个单
    case PlayHand::Hand_Plane_Two_Single:
        //飞机带两个对
    case PlayHand::Hand_Plane_Two_Pair:
        number = Plane;
        break;
        //连对
    case PlayHand::Hand_Seq_Pair:
        number = SequencePair;
        break;
        //顺子
    case PlayHand::Hand_Seq_Single:
        number = Sequence;
        break;
        //炸弹
    case PlayHand::Hand_Bomb:
        number = Bomb;
        break;
        //王炸
    case PlayHand::Hand_Bomb_Jokers:
        number = JokerBomb;
        break;
        //炸弹带一对
    case PlayHand::Hand_Bomb_Pair:
        //炸弹带两单
    case PlayHand::Hand_Bomb_Two_Single:
        //王炸带一对
    case PlayHand::Hand_Bomb_Jokers_Pair:
        //王炸带两单
    case PlayHand::Hand_Bomb_Jokers_Two_Single:
        number = FourBindTwo;
        break;

    default:
        break;
    }

    if(!isFirst && (number >= Plane && number <= FourBindTwo))
    {
        m_players[index]->setSource(m_sourceList[index][MoreBiger1 + QRandomGenerator::global()->bounded(2)]);
    }
    else
    {
        m_players[index]->setSource(m_sourceList[index][number]);
    }
    // 播放音乐
    m_players[index]->play();
    if(number == Bomb || number == JokerBomb)
    {
        playAssistMusic(BombVoice);
    }
    if(number == Plane)
    {
        playAssistMusic(PlaneVoice);
    }

}

void BGMControl::playLastMusic(CardType type, RoleSex sex)
{
    // 玩家的性别
    int index = sex == Man ? 0: 1;
    // 找到播放列表
    if(m_players[index]->mediaStatus() == QMediaPlayer::StoppedState)
    {
        m_players[index]->setSource(m_sourceList[index][type]);
        m_players[index]->play();
    }
    else
    {
        QTimer::singleShot(1500,this,[=](){
            m_players[index]->setSource(m_sourceList[index][type]);
            m_players[index]->play();
        });
    }

}

void BGMControl::playPassMusic(RoleSex sex)
{
    // 玩家的性别
    int index = sex == Man ? 0: 1;
    // 找到要播放的音乐
    int random = QRandomGenerator::global()->bounded(4);
    m_players[index]->setSource(m_sourceList[index][Pass1 + random]);
    // 播放音乐
    m_players[index]->play();
}

void BGMControl::playAssistMusic(AssistMusic type)
{
    if(type == Dispatch){
        // 循环播放
        m_players[3]->setLoops(QMediaPlayer::Infinite);
    }
    else
    {
        // 单曲播放一次
        m_players[3]->setLoops(QMediaPlayer::Once);
    }
    // 播放音乐
    m_players[3]->setSource(m_sourceList[3][type]);
    m_players[3]->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin)
{
    if(isWin){
        m_players[4]->setSource(m_sourceList[4][0]);
    }
    else
    {
        m_players[4]->setSource(m_sourceList[4][1]);
    }
    m_players[4]->play();
}

