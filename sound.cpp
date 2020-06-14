#include "sound.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>


static const QString s_curDir = QDir::currentPath() + "/";

sound::sound(QObject *parent)
    : QObject(parent)
    , _backgroundMusic(NULL)
{
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/8bitDungeonLevel.mp3");
    if (QFile::exists(backgroundMusicUrl.toLocalFile()))
    {
        _backgroundMusic = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backgroundMusicUrl);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        _backgroundMusic->setPlaylist(backgroundMusicList);
    }
}

void sound::startBGM()
{
    if (_backgroundMusic)
        _backgroundMusic->play();
}

void sound::playSound(SoundType soundType)
{
    static const QUrl mediasUrls[] =
    {
        QUrl::fromLocalFile(s_curDir + "/settle.wav"),
        QUrl::fromLocalFile(s_curDir + "/lose.wav"),
        QUrl::fromLocalFile(s_curDir + "/shoot.wav"),
        QUrl::fromLocalFile(s_curDir + "/destroy.wav")
    };
    static QMediaPlayer player;

    if (QFile::exists(mediasUrls[soundType].toLocalFile()))
    {
        player.setMedia(mediasUrls[soundType]);
        player.play();
    }
}
