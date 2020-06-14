#ifndef SOUND_H
#define SOUND_H

#include <QObject>

class QMediaPlayer;

enum SoundType
{
    TowerPlaceSound,
    LifeLoseSound,
    LaserShootSound,
    EnemyDestorySound
};

class sound : public QObject
{
public:
    explicit sound(QObject *parent = 0);

    void startBGM();
    void playSound(SoundType soundType);

private:
    QMediaPlayer *_backgroundMusic;
};


#endif // SOUND_H
