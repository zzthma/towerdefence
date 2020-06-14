#include "dialog.h"
#include "ui_dialog.h"
#include "route.h"
#include "enemy1.h"
#include "attack.h"
#include "sound.h"
#include "listreader.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>


static const int TowerCost = 400;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
    , _waves(0)
    , _playerHp(5)
    , _playrGold(1000)
    , _gameEnded(false)
    , _gameWin(false)
{
    ui->setupUi(this);
    preLoadWavesInfo();
    loadTowerPositions();
    addWayPoints();

    _sound = new sound(this);
    _sound->startBGM();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    QTimer::singleShot(300, this, SLOT(gameStart()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::loadTowerPositions()
{
    QFile file(":/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    listreader reader;
    reader.read(&file);

    QList<QVariant> array = reader.data();
    foreach (QVariant dict, array)
    {
        QMap<QString, QVariant> point = dict.toMap();
        int x = point.value("x").toInt();
        int y = point.value("y").toInt();
        _towerPositionsList.push_back(QPoint(x, y));
    }

    file.close();
}

void Dialog::paintEvent(QPaintEvent *)
{
    if (_gameEnded || _gameWin)
    {
        QString text = _gameEnded ? "YOU LOST!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    QPixmap cachePix(":/map1.jpg");
    QPainter cachePainter(&cachePix);

    foreach (const towerposition &towerPos, _towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const basictower *tower, _towersList)
        tower->draw(&cachePainter);

    foreach (const route *route, _wayPointsList)
        route->draw(&cachePainter);

    foreach (const enemy1 *enemy, _enemyList)
        enemy->draw(&cachePainter);

    foreach (const attack *attack, _bulletList)
        attack->draw(&cachePainter);

    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);

    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    auto it = _towerPositionsList.begin();
    while (it != _towerPositionsList.end())
    {
        if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
        {
            _sound->playSound(TowerPlaceSound);
            _playrGold -= TowerCost;
            it->hasTower();

            basictower *tower = new basictower(it->centerPos(), this);
            _towersList.push_back(tower);
            update();
            break;
        }

        ++it;
    }
}

bool Dialog::canBuyTower() const
{
    if (_playrGold >= TowerCost)
        return true;
    return false;
}

void Dialog::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(_waves + 1));
}

void Dialog::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(_playerHp));
}

void Dialog::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(_playrGold));
}

void Dialog::doGameOver()
{
    if (!_gameEnded)
    {
        _gameEnded = true;

    }
}

void Dialog::awardgold(int gold)
{
    _playrGold += gold;
    update();
}

sound *Dialog::sound() const
{
    return m_audioPlayer;
}

void Dialog::addroute()
{
    route *wayPoint1 = new WayPoint(QPoint(750, 500));
    m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(70, 500));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(70, 340));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(830, 340));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(830, 180));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(70, 180));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);
}

void Dialog::getHPdamage(int damage)
{
    _sound->playSound(LifeLoseSound);
    _playerHp -= damage;
    if (_playerHp <= 0)
        doGameOver();
}

void Dialog::removedenemy(enemy1 *enemy)
{
    Q_ASSERT(enemy);

    _enemyList.removeOne(enemy);
    delete enemy;

    if (_enemyList.empty())
    {
        ++_waves;
        if (!loadWave())
        {
            _gameWin = true;
        }
    }
}

void Dialog::removedbullet(attack *attack)
{
    Q_ASSERT(attack);

    _bulletList.removeOne(attack);
    delete bullet;
}

void Dialog::addbullet(attack *attack)
{
    Q_ASSERT(attack);

    _bulletList.push_back(attack);
}

void Dialog::updateMap()
{
    foreach (enemy1 *enemy, _enemyList)
        enemy->move();
    foreach (basictower *tower, _towersList)
        tower->checkEnemyInRange();
    update();
}

void Dialog::preLoadWavesInfo()
{
    QFile file(":/Waves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);


    _wavesInfo = reader.data();

    file.close();
}

bool Dialog::loadWave()
{
    if (_waves >= _wavesInfo.size())
        return false;

    WayPoint *startWayPoint = _wayPointsList.back();
    QList<QVariant> curWavesInfo = _wavesInfo[_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        enemy1 *enemy = new enemy1(startWayPoint, this);
        _enemyList.push_back(enemy);
        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
    }

    return true;
}

QList<enemy1 *> Dialog::enemyList() const
{
    return _enemyList;
}

void Dialog::gameStart()
{
    loadWave();
}
