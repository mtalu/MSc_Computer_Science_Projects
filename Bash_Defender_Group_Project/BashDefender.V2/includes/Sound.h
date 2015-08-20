//
//  Sound.h
//  Bash_Defender_V2
//


#ifndef Bash_Defender_V2_Sound_h
#define Bash_Defender_V2_Sound_h


enum soundTypes {
    bulletSound,
    missileSound,
    laserSound,
    start_gameSound = 0,
    new_waveSound,
    playerTakesDamageSound,
    game_wonSound,
    game_lostSound
};

typedef enum soundTypes SoundTypes;

void init_sound();
void playBackgroundSound();
void towerSound(int type);
void enemyDeathSound();
void generalSounds(int type);
void shutSound();

#endif
