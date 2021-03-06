#ifndef BASICTOWER1_H
#define BASICTOWER1_H
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <QPainter>
#include <QTime>
class enemy1;
class Dialog;


class basictower : public QObject
{
    Q_OBJECT
public:
    basictower(QPoint pos, Dialog *game, const QPixmap &sprite = QPixmap(":/basic.png"));
    ~basictower();

    void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(enemy1 *enemy);
    void removeBullet();
    void damageEnemy();
    void lostSightOfEnemy();

private slots:
    void shootWeapon();

private:
    bool			_attacking;
    int				_attackRange;
    int				_damage;
    int				_fireRate;
    qreal			_rotationSprite;

    enemy1 *			_chooseenemy;
    Dialog *	_game;
    QTimer *		_fireRateTimer;

    const QPoint	_pos;
    const QPixmap	_sprite;

    static const QSize ms_fixedSize;
signals:

};

#endif // BASICTOWER1_H
