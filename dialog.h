#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
class towerposition;
class basictower;
class route;
class enemy1;
class attack;
class sound;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getHPdamage(int damage = 1);
    void removedenemy(enemy1 *enemy);
    void removedbullet(attack *attack);
    void addbullet(attct *attct);
    void awardgold(int gold);

    sound* sound() const;
    QList<enemy1 *> enemyList() const;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void updateMap();
    void gameStart();

private:
    void loadTowerPositions();
    void addroute();
    bool loadWave();
    bool canBuyTower() const;
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
    void preLoadWavesInfo();

private:
    Ui::Dialog *		ui;
    int						_waves;
    int						_playerHp;
    int						_playrGold;
    bool					_gameEnded;
    bool					_gameWin;
    sound *			_sound;
    QList<QVariant>			_wavesInfo;
    QList<towerposition>	_towerPositionsList;
    QList<basictower *>			_towersList;
    QList<route *>		_wayPointsList;
    QList<enemy1 *>			_enemyList;
    QList<attack *>			_bulletList;
};



#endif // DIALOG_H
