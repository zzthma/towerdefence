#include "basictower.h"
#include "enemy1.h"
#include"attack.h"
#include "dialog.h"
#include "effect.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>


const QSize basictower::ms_fixedSize(64,64);

basictower::basictower(QPoint pos, Dialog *game, const QPixmap &sprite)
    : _attacking(false)
    , _attackRange(70)
    , _damage(15)
    , _fireRate(1500)
    , _rotationSprite(0.0)
    , _chooseEnemy(NULL)
    , _game(game)
    , _pos(pos)
    , _sprite(sprite)
{
    _fireRateTimer = new QTimer(this);
    connect(_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}
basictower::~basictower()
{
    delete _fireRateTimer;
    _fireRateTimer = NULL;
}

void basictower::checkEnemyInRange()
{
    if (_chooseenemy)
    {
        QVector2D normalized(_chooseenemy->pos() - _pos);
        normalized.normalize();
        _rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        if (!collision(_pos, _attackRange, _chooseenemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        QList<enemy1 *> enemyList = _game->enemyList();
        foreach (enemy1 *enemy, enemyList)
        {
            if (collision(_pos, _attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void basictower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawEllipse(_pos, _attackRange, _attackRange);

    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);

    painter->translate(_pos);
    painter->rotate(_rotationSprite);
    painter->drawPixmap(offsetPoint, _sprite);
    painter->restore();
}

void basictower::attackEnemy()
{
    _fireRateTimer->start(_fireRate);
}

void basictower::chooseEnemyForAttack(enemy1 *enemy)
{
    _chooseenemy = enemy;
    attackEnemy();
    _chooseenemy->getAttacked(this);
}




void basictower::targetKilled()
{
    if (_chooseenemy)
        _chooseenemy = NULL;

    _fireRateTimer->stop();
    _rotationSprite = 0.0;
}

void basictower::lostSightOfEnemy()
{
    _chooseenemy->gotLostSight(this);
    if (_chooseenemy)
        _chooseenemy = NULL;

    _fireRateTimer->stop();
    _rotationSprite = 0.0;
}
