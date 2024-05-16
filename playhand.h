#ifndef PLAYHAND_H
#define PLAYHAND_H

#include "cards.h"


class PlayHand
{
public:
    //出牌组合或者方式
    enum HandType
    {
        Hand_Unknown,
        Hand_Pass,

        Hand_Single,
        Hand_Pair,

        Hand_Triple,
        Hand_Triple_Single,
        Hand_Triple_Pair,

        Hand_Plane,
        Hand_Plane_Two_Single, // 555_666_3_4
        Hand_Plane_Two_Pair, // 555_666_33_44

        Hand_Seq_Pair,  // 连对 33_44_55(_66...)
        Hand_Seq_Single, //顺子,34567(8...)

        Hand_Bomb, //炸弹
        Hand_Bomb_Single, //炸弹带一个
        Hand_Bomb_Pair, //炸弹带一对
        Hand_Bomb_Two_Single, //炸弹带两个单

        Hand_Bomb_Jokers, //王炸
        Hand_Bomb_Jokers_Single, //王炸带一个
        Hand_Bomb_Jokers_Pair, //王炸带一对
        Hand_Bomb_Jokers_Two_Single //王炸带两单
    };

    PlayHand();
    // 传递给类一组牌，通过类分析出牌型，点数，以及相关的附属信息（比如顺子：记录牌的数量）
    PlayHand(Cards& cards);
    PlayHand(HandType type, Card::CardPoint pt, int extra);

    // 得到牌的属性信息
    HandType getHandType();
    Card::CardPoint getCardPoint();
    int getExtra();

    // 比较自己的牌和其他人的牌的牌型大小
    bool canBeat(const PlayHand& other);

private:
    // 1.对扑克牌进行分类，1张的，2张，3张，4张有多少种
    void classify(Cards &cards);

    // 2.对牌型进行分类
    void judgeCardType();


    // 判断牌的类型

    bool isPass(); //放弃出牌
    bool isSingle(); //单
    bool isPair(); //对
    bool isTriple(); //三个相同
    bool isTripleSingle(); //三带一
    bool isTriplePair(); //三带二
    bool isPlane(); //飞机
    bool isPlaneTwoSingle();
    bool isPlaneTwoPair();
    bool isSeqPair();
    bool isSeqSingle();
    bool isBomb();
    bool isBombSingle();
    bool isBombPair();
    bool isBombTwoSingle();
    bool isBombJokers();
    bool isBombJokersSingle();
    bool isBombJokersPair();
    bool isBombJokersTwoSingle();

private:
    HandType m_type;
    Card::CardPoint m_pt;
    int m_extra;
    QVector<Card::CardPoint> m_oneCard;
    QVector<Card::CardPoint> m_twoCard;
    QVector<Card::CardPoint> m_threeCard;
    QVector<Card::CardPoint> m_fourCard;

};

#endif // PLAYHAND_H
