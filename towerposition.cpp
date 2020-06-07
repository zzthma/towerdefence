#include "towerposition.h"
#include <QPainter>

const QSize towerposition::ms_fixedSize(64, 64);

towerposition::towerposition(QPoint pos, const QPixmap &sprite)
    : _ht(false)
    , _pos(pos)
    , _sprite(sprite)
{
}

const QPoint towerposition::centerPos() const
{
    QPoint offsetPoint(ms_fixedSize.width() / 2, ms_fixedSize.height() / 2);
    return _pos + offsetPoint;
}

bool towerposition::containPoint(const QPoint &pos) const
{
    bool isXInHere = _pos.x() < pos.x() && pos.x() < (_pos.x() + ms_fixedSize.width());
    bool isYInHere = _pos.y() < pos.y() && pos.y() < (_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

bool towerposition::hasTower() const
{
    return _ht;
}

void towerposition::setHasTower(bool ht)
{
    _ht = ht;
}

void towerposition::draw(QPainter *painter) const
{
    painter->drawPixmap(_pos.x(), _pos.y(), _sprite);
}
