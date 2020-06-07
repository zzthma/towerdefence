#ifndef ENEMY1_H
#define ENEMY1_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <QPainter>
#include "mainwindow.h"
#include "route.h"
#include "basictower.h"

class enemy1 : public QObject
{
    Q_OBJECT
public:
    explicit enemy1(QObject *parent = nullptr);
    enemy1(route *startroute, MainWindow *game, const QPixmap &sprite = QPixmap(":/enemy1.png"));
    ~enemy1();

    void draw(QPainter *painter) const;
    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(basictower *attacker);
    void gotLostSight(basictower *attacker);
    QPoint pos() const;

public slots:
    void doActivate();

private:
    bool			_active;
    int				_maxHp;
    int				_currentHp;
    qreal			_walkSpeed;
    qreal			_rotationSprite;

    QPoint			_pos;
    route *		_destination;
    MainWindow *	_game;
    QList<basictower *>	_attackedTowersList;

    const QPixmap	_sprite;
    static const QSize ms_fixedSize;

signals:

};

#endif // ENEMY1_H
