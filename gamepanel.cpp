#include "gamepanel.h"
#include "playhand.h"
#include "ui_gamepanel.h"

#include <QFile>
#include<QPainter>
#include<QRandomGenerator>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    // 1.背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);
    // 2.窗口的标题的大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000,650);
    // 3.实例化游戏控制类对象
    gameControlInit();
    // 4.玩家得分
    updatePlayerScore();
    // 5.切割游戏图片
    initCardMap();
    // 6.初始化游戏中的按钮组
    initButtonGroup();
    // 7.初始化玩家在窗口中的上下文环境
    initPlayerContext();
    // 8. 扑克牌初始化场景
    initGameScene();
    // 定时器实例化
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);

    m_animation = new AnimationWindow(this);

}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    m_gameCtl = new GameControl(this);
    m_gameCtl->playerInit();
    // 得到三个玩家的实例对象
    Robot* leftRobot = m_gameCtl->getLeftRobot();
    Robot* rightRobot = m_gameCtl->getRightRobot();
    UserPlayer* user = m_gameCtl->getUserPlayer();
    // 存储的顺序：左侧机器人，右侧机器人，当前玩家
    m_playerList<<leftRobot <<rightRobot<<user;


    connect(m_gameCtl,&GameControl::playerStatusChanged , this, &GamePanel::onPlayerStatusChanged);
    connect(m_gameCtl,&GameControl::notifyGrabLordBet , this, &GamePanel::onGrabLordBet);
    connect(m_gameCtl,&GameControl::gameStatusChanged , this, &GamePanel::gameStatusPrecess);
    connect(m_gameCtl,&GameControl::notifyPlayHand , this, &GamePanel::onDisposePlayHand);
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScores(m_playerList[0]->getScore(),
                              m_playerList[1]->getScore(),
                              m_playerList[2]->getScore());
}

void GamePanel::initCardMap()
{
    // 1.加载大图
    QPixmap pixmap(":/images/card.png");
    // 2.计算每张图片大小
    m_cardSize.setHeight(pixmap.height()/5);
    m_cardSize.setWidth(pixmap.width()/13);
    // 3.背景图
    m_cardBackImg = pixmap.copy(2*m_cardSize.width(), 4*m_cardSize.height(),
                                m_cardSize.width(),m_cardSize.height());

    // 正常花色
    for(int i =0, suit = Card::Suit_Begin+1;  suit<Card::Suit_End; ++suit,++i)
    {
        for(int j =0,pt = Card::Card_Begin+1;  pt<Card::Card_SJ; ++pt,++j)
        {
            Card card((Card::CardPoint)pt, (Card::CardSuit)suit);
            // 裁剪图片
            cropImage(pixmap, j*m_cardSize.width(), i*m_cardSize.height(),card);
        }
    }
    // 大小王
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap, 0, 4*m_cardSize.height(),c);

    c.setPoint(Card::Card_BJ);
    cropImage(pixmap,m_cardSize.width(), 4*m_cardSize.height(),c);
}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card& c)
{
    QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
    CardPanel* panel = new CardPanel(this);
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(c);
    panel->hide();
    m_cardMap.insert(c, panel);
}

void GamePanel::initButtonGroup()
{

    ui->btnGroup->initButtons();
    ui->btnGroup->selectPanel(ButtonGroup::Start);

    connect(ui->btnGroup, &ButtonGroup::startGame, this, [=](){
        //界面的初始化
        ui->btnGroup->selectPanel(ButtonGroup::Empty);
        m_gameCtl->clearPlayerScore();
        updatePlayerScore();
        // 修改游戏状态 -> 发牌
        gameStatusPrecess(GameControl::DispatchCard);
    });
    connect(ui->btnGroup, &ButtonGroup::playHand, this, [=](){});
    connect(ui->btnGroup, &ButtonGroup::pass, this, [=](){});
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [=](int bet){
        //这是给玩家用的，1、2、3分 机器人有线程去执行grabLordBet
        m_gameCtl->getUserPlayer()->grabLordBet(bet); //叫地主函数->gameControl（判断叫地主次数和细节）->gamePanel显示
    });


}

void GamePanel::initPlayerContext()
{
    // 1.放置玩家扑克牌的区域
    QRect cardsRect[] =
    {
        // x, y, width, height
        QRect(90,130,100,height() - 200), //左侧机器人
        QRect(rect().right() - 190 , 130, 100, height() - 200), // 右侧机器人
        QRect(250, rect().bottom()- 120, width() - 500 , 100) // 当前玩家
    };
    // 2.玩家出牌的区域
    QRect playHandRect[] =
    {
        // x, y, width, height
        QRect(260,150,100,100), //左侧机器人
        QRect(rect().right() - 360 , 150, 100, 100), // 右侧机器人
        QRect(150, rect().bottom()- 290, width() - 300 , 100) // 当前玩家
    };
    // 3.玩家头像显示的位置
    QPoint roleImgPos[] =
    {
        // x, y, width, height
        QPoint(cardsRect[0].left() - 80 , cardsRect[0].height()/2+20), //左侧机器人
        QPoint(cardsRect[1].right() + 10 , cardsRect[1].height()/2 + 20), // 右侧机器人
        QPoint(cardsRect[2].right() - 10 , cardsRect[2].top()- 10) // 当前玩家
    };

    // 循环
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i = 0 ; i < m_playerList.size() ; ++i){
        PlayerContext context;
        context.align = i==index? Horizontal : Vertical;
        context.isFrontSide = i==index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        // 提示信息
        context.info = new QLabel(this);
        context.info->resize(160,98);
        context.info->hide();
        // 显示到出牌区域的中心位置
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width()- context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(pt);
        //玩家的头像
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84,120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerList.at(i), context);
    }


}

void GamePanel::initGameScene()
{
    // 1. 发牌区的扑克牌
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(m_cardBackImg,m_cardBackImg);
    // 2. 发牌过程中移动的扑克牌
    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(m_cardBackImg,m_cardBackImg);
    // 3. 最后的三张底牌(用于窗口的显示)
    for (int i = 0; i < 3; ++i) {
        CardPanel* panel = new CardPanel(this);

        panel->setImage(m_cardBackImg,m_cardBackImg);

        m_last3Card.push_back(panel);
        panel->hide();
    }
    // 4. 扑克牌的位置
    m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
                           height() / 2 - 100);

    m_baseCard->move(m_baseCardPos);
    m_moveCard->move(m_baseCardPos);

    int base = (width() - 3*m_cardSize.width() - 2*10) /2;
    for (int i = 0; i < 3; ++i) {
        m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i , 20);
    }


}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    //记录游戏状态
    m_gameStatus = status;
    //处理游戏状态
    switch(status){
    case GameControl::DispatchCard:
        startDispatchCard();
        break;
    case GameControl::CallingLord:
    {
        // 取出底牌数据
        CardList last3Card = m_gameCtl->getSurplusCards().toCardList();
        // 给底牌窗口设置图片
        for (int i = 0; i < last3Card.size(); ++i) {
            //设置底牌的front图片
            QPixmap front = m_cardMap[last3Card.at(i)]->getImage();
            m_last3Card[i]->setImage(front, m_cardBackImg);
            m_last3Card[i]->hide();

            qDebug("输出：");
        }
        // 开始叫地主
        m_gameCtl->startLordCard();
        break;
    }
    case GameControl::PlayingHand:
        // 隐藏发牌区的底牌和移动的牌
        m_baseCard->hide();
        m_moveCard->hide(); //其实已经被隐藏了，可写可不写
        // 显示留给地主的三张底牌
        //
        for (int i = 0; i < m_last3Card.size(); ++i) {

            m_last3Card.at(i)->show();
        }
        // 隐藏各个玩家抢地主过程中的提示信息
        for (int i = 0; i < m_playerList.size(); ++i) {
            //隐藏各个玩家抢地主过程中的提示信息
            PlayerContext &context  = m_contextMap[m_playerList.at(i)];
            context.info->hide();
            //显示各个玩家的头像
            Player* player = m_playerList.at(i);
            QPixmap pixmap = loadRoleImage(player->getSex(),player->getDirection(),player->getRole());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
        //
        break;
    default:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    // 重置每张卡牌的属性
    for (auto it = m_cardMap.begin(); it!=m_cardMap.end(); ++it) {
        it.value()->setSelected(false);
        it.value()->setFrontSide(true); //把所有牌都设置成正面
        it.value()->hide();
    }
    // 隐藏三张底牌
    for (int i = 0; i < m_last3Card.size(); ++i) {
        m_last3Card.at(i)->hide();
    }
    // 重置玩家的窗口上下文信息
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for (int i = 0; i < m_playerList.size() ; ++i) {
        m_contextMap[m_playerList.at(i)].lastCards.clear();
        m_contextMap[m_playerList.at(i)].info->hide();
        m_contextMap[m_playerList.at(i)].roleImg->hide();
        m_contextMap[m_playerList.at(i)].isFrontSide = i == index ? true : false;
    }
    // 重置所有玩家的卡牌数据
    m_gameCtl->resetCardData();
    // 显示底牌
    m_baseCard->show();
    // 隐藏按钮面板
    ui->btnGroup->selectPanel(ButtonGroup::Empty);
    // 启动定时器
    m_timer->start(10);
    // 播放背景音乐

}

void GamePanel::cardMoveStep(Player *player , int curPos)
{
    // 得到每个玩家的扑克牌展示区域
    QRect cardRect = m_contextMap[player].cardRect;
    // 每个玩家的单元步长
    int unit[] = {
        (m_baseCardPos.x() - cardRect.right()) /100,
        (cardRect.left() - m_baseCardPos.x()) / 100,
        (cardRect.top() - m_baseCardPos.y()) / 100,
    };
    // 每次窗口移动的时候每个玩家对应的牌的实时坐标位置
    QPoint pos[] =
    {
        QPoint(m_baseCardPos.x() - curPos*unit[0], m_baseCardPos.y()),
        QPoint(m_baseCardPos.x() + curPos*unit[1], m_baseCardPos.y()),
        QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos* unit[2]),
    };

    // 移动扑克牌窗口
    int index = m_playerList.indexOf(player);
    m_moveCard->move(pos[index]);

    // 临界状态处理
    if(curPos == 0){
        m_moveCard->show();
    }
    if(curPos== 100){
        m_moveCard->hide();
    }
}

void GamePanel::disposCard(Player *player, Cards &cards)
{
    CardList list = cards.toCardList();
    for (int i = 0; i < list.size(); ++i) {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->setOwner(player);
    }
    // 更新扑克牌在窗口中的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player* player)
{
    Cards cards = player->getCards();
    CardList list = cards.toCardList();
    // 取出展示扑克牌的区域
    int cardSpace = 20;
    QRect cardsRect = m_contextMap[player].cardRect;
    for(int i = 0 ; i < list.size() ; ++i){
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->show();
        panel->raise();
        panel->setFrontSide(m_contextMap[player].isFrontSide);

        // 水平 or 垂直显示
        if(m_contextMap[player].align == Horizontal){
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) /2;
            int topY = cardsRect.top() + (cardsRect.height() - m_cardSize.height()) / 2;
            if(panel->isSelected()){
                topY -= 10;
            }
            panel->move(leftX+cardSpace*i , topY);
        }else{
            int leftX = cardsRect.left() + (cardsRect.width() - m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX,topY + cardSpace*i);
        }
    }

    //显示玩家打出的牌

    //得到当前玩家的出牌区域以及本轮打出的牌
    QRect playCardRect = m_contextMap[player].playHandRect;
    Cards lastCards = m_contextMap[player].lastCards;
    if(!lastCards.isEmpty()){

        int playSpacing = 24;

        CardList lastCardList = lastCards.toCardList();
        CardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i = 0; itplayed!=lastCardList.constEnd(); itplayed++,i++){
            CardPanel* panel = m_cardMap[*itplayed];
            panel->setFrontSide(true);
            panel->raise();

            //将打出的牌移动到出牌区域
            if(m_contextMap[player].align == Horizontal)
            {
                int leftBase = playCardRect.left() +
                               (playCardRect.width() - (lastCardList.size() - 1)*playSpacing - panel->width()) /2;

                int top = playCardRect.top() + (playCardRect.height() - panel->height()) / 2;
                panel->move(leftBase + i * playSpacing, top);
            }
            else
            {
                int left = playCardRect.left() +
                           (playCardRect.width() - panel->width()) /2;

                int top = playCardRect.top();
                panel->move(left, top + i * playSpacing);
            }
            panel->show();
        }
    }
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> famerMan;
    QVector<QString> famerWoman;
    lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png" ;
    lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png" ;
    famerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png" ;
    famerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png" ;

    //加载图片QPixmap QImage
    QImage image;
    int random = QRandomGenerator::global()->bounded(2);
    if(sex == Player::Man && role == Player::Lord)
    {
        image.load(lordMan.at(random));
    }
    else if(sex == Player::Man && role == Player::Farmer){
        image.load(famerMan.at(random));
    }
    else if(sex == Player::Woman && role == Player::Lord){
        image.load(lordWoman.at(random));
    }
    else if(sex == Player::Woman && role == Player::Farmer){
        image.load(famerWoman.at(random));
    }

    QPixmap pixmap;
    if(direct == Player::Left){
        pixmap = QPixmap::fromImage(image);
    }
    else
    {
        pixmap = QPixmap::fromImage(image.mirrored(true,false));
    }
    return pixmap;
}


void GamePanel::onDispatchCard()
{
    // 记录扑克牌的位置
    static int curMovePos = 0;
    // 当前玩家
    Player* curPlayer = m_gameCtl->getCurrentPlayer();
    if(curMovePos >= 100){
        // 给玩家发一张牌
        Card card = m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cs(card);
        disposCard(curPlayer, cs);
        // 切换玩家
        m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
        curMovePos = 0;
        // 发牌动画
        cardMoveStep(curPlayer, curMovePos);
        // 判断牌发完了
        if(m_gameCtl->getSurplusCards().cardCount() == 3){
            // 终止定时器
            m_timer->stop();
            // 切换游戏状态
            gameStatusPrecess(GameControl::CallingLord);
            return;
        }
    }
    // 移动扑克牌
    cardMoveStep(curPlayer,curMovePos);
    curMovePos += 15;

}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status)
{
    switch(status){
    case GameControl::ThinkingForCallLord:
        if(player == m_gameCtl->getUserPlayer()){ //只针对人类玩家才会显示选择分数的界面
            ui->btnGroup->selectPanel(ButtonGroup::CallLord, m_gameCtl->getPlayerMaxBet());
        }
        break;
    case GameControl::ThinkingForPlayHand:
        //1.隐藏上一轮打出的牌
        hidePlayerDropCards(player);
        if(player == m_gameCtl->getUserPlayer()){
            // 取出出牌玩家的对象
            Player* pendPlayer = m_gameCtl->getPendPlayer();
            if(pendPlayer == m_gameCtl->getUserPlayer() || pendPlayer == nullptr)
            {
                ui->btnGroup->selectPanel(ButtonGroup::PlayCard);
            }
            else
            {
                ui->btnGroup->selectPanel(ButtonGroup::PassOrPlay);
            }
        }
        else{
            ui->btnGroup->selectPanel(ButtonGroup::Empty);
        }
        break;
    case GameControl::Winning:
        m_contextMap[m_gameCtl->getLeftRobot()].isFrontSide = true;
        m_contextMap[m_gameCtl->getRightRobot()].isFrontSide = true;
        updatePlayerCards(m_gameCtl->getLeftRobot());
        updatePlayerCards(m_gameCtl->getRightRobot());
        // 更新玩家的得分
        updatePlayerScore();
        m_gameCtl->setCurrentPlayer(player); //更新谁先出牌
        break;
    default:
        break;
    }
}

void GamePanel::onGrabLordBet(Player *player, int bet, bool flag)
{
    //处理叫地主的显示
    PlayerContext context = m_contextMap[player];
    if(bet == 0){
        context.info->setPixmap(QPixmap(":/images/buqiang.png"));
    }else{
        if(flag){
            //第一次抢地主
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }else{
            //第二次抢地主
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
    }
    context.info->show();

    // 显示叫地主的分数
    showAnimation(AnimationType::Bet,bet);
    // 播放分数的背景音乐

}

void GamePanel::onDisposePlayHand(Player *player, Cards &cards)
{

    // 存储玩家打出的牌
    auto it = m_contextMap.find(player);
    it->lastCards = cards;
    //2.根据牌型播放游戏特效
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Plane ||
        type == PlayHand::Hand_Plane_Two_Pair ||
        type == PlayHand::Hand_Plane_Two_Single)
    {
        showAnimation(Plane);
    }
    else if(type == PlayHand::Hand_Seq_Pair){
        showAnimation(LianDui);
    }
    else if(type == PlayHand::Hand_Seq_Single){
        showAnimation(ShunZi);
    }
    else if(type == PlayHand::Hand_Bomb){
        showAnimation(Bomb);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        showAnimation(JokerBomb);
    }
    // 如果玩家打出的牌是空牌（不出牌），显示提示信息
    if(cards.isEmpty()){
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
    }
    //3.更新玩家剩余的牌
    updatePlayerCards(player);
    //4.播放提示音乐

}

void GamePanel::showAnimation(AnimationType type, int bet)
{
    switch(type){
    case AnimationType::LianDui:
        break;
    case AnimationType::ShunZi:
        break;
    case AnimationType::Plane:
        break;
    case AnimationType::Bomb:
        break;
    case AnimationType::JokerBomb:
        break;
    case AnimationType::Bet:
        m_animation->setFixedSize(160,98);
        m_animation->move( (width() - m_animation->width()) / 2,
                           (height() - m_animation->height()) /2 - 140);
        m_animation->showBetScore(bet);
        break;
    }
    m_animation->show();
}

void GamePanel::hidePlayerDropCards(Player *player)
{
    auto it = m_contextMap.find(player);
    if(it != m_contextMap.end()){
        if(it->lastCards.isEmpty()){
            // 牌全部出完了
            it->info->hide();
        }
        else
        {
            // Cards --> Card
            CardList list = it->lastCards.toCardList();
            for(auto last = list.begin() ; last!=list.end() ; ++last){
                m_cardMap[*last]->hide();
            }
        }
        it->lastCards.clear();
    }
}

void GamePanel::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_bkImage);
}
