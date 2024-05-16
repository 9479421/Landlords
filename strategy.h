#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playhand.h"


class Strategy
{
public:
    Strategy(Player* player, const Cards& cards);
    // 找出指定数量（count）的相同点数的牌（point），找出count张点数为point的牌
    Cards findSamePointCards(Card::CardPoint point , int count);
    // 找出所有点数数量为count的牌 ==> 得到一个多张扑克牌数组
    QVector<Cards> findCardsByCount(int count);
    // 根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);
    // 按牌型找牌，并且指定要找的牌是否要大过指定的牌型
    QVector<Cards> findCardType(PlayHand hand, bool beat);
private:
    using function = Cards (Strategy::*)(Card::CardPoint point);
    struct CardInfo
    {
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra; // 顺子或者连对的数量
        bool beat;
        int number; // 指定点数的牌的数量
        int base; // 最基础的顺子或者连对的数量
        function getSeq;
    };
    QVector<Cards> getCards(Card::CardPoint point ,int number);
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin ,PlayHand::HandType type);
    QVector<Cards> getPlane(Card::CardPoint begin);
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getSeqPairOrSeqSingle(CardInfo& info);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);
    QVector<Cards> getBomb(Card::CardPoint begin);

private:
    Player* m_player;
    Cards m_cards;
};

#endif // STRATEGY_H
