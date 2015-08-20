//
//  Sound.c
//  Bash_Defender_V2



//#include "../includes/parser.h"
//#include "../includes/tower.h"
#include <stdbool.h>
#include <time.h>
#include "../includes/Sound.h"

#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2/SDL.h>

struct sound{
    Mix_Music *background_music;
    Mix_Chunk *tower_sound[3];
    Mix_Chunk *enemy_sound;
    Mix_Chunk *generalSounds[5];
};


typedef struct sound *Sound;

/* 
 *internal Sound functions 
 */
Sound getSoundPointer();
void check_load_sound(void *music, char *sound_name);
void load_sound_effect (Mix_Chunk **effect, char *effect_name);

/*
 * initialize the sound
 */
void init_sound() {
    
    Sound s = getSoundPointer();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Error openning audio mixer");
        SDL_Quit();
    }
    
    s->background_music = Mix_LoadMUS("Sounds/sound.wav");
    check_load_sound((Mix_Music*)s->background_music, "Sounds/sound.mp3");
    
    load_sound_effect(&s->enemy_sound, "Sounds/dead_enemy.wav");
    load_sound_effect(&s->tower_sound[missileSound], "Sounds/projectile.wav");
    load_sound_effect(&s->tower_sound[laserSound], "Sounds/laser_sound.wav");
    load_sound_effect(&s->tower_sound[bulletSound], "Sounds/bullet.wav");
    load_sound_effect(&s->generalSounds[start_gameSound], "Sounds/start_game.wav");
    load_sound_effect(&s->generalSounds[new_waveSound], "Sounds/next_wave.wav");
    load_sound_effect(&s->generalSounds[playerTakesDamageSound], "Sounds/player_damage.wav");
    load_sound_effect(&s->generalSounds[game_wonSound], "Sounds/game_win.wav");
    
    load_sound_effect(&s->generalSounds[game_lostSound], "Sounds/game_lost.wav");



    Mix_VolumeMusic(60);
}

/** Start game sound */
void generalSounds(int type){
    Sound s = getSoundPointer();
    //printf(" from sound %d", type);
    Mix_PlayChannel(-1, s->generalSounds[type], 0);
    Mix_VolumeChunk(s->generalSounds[type], 50);
}

/*
 *Load sound effect
 */
void load_sound_effect (Mix_Chunk **effect, char *effect_name) {
    *effect = Mix_LoadWAV(effect_name);
    check_load_sound((Mix_Chunk *)*effect, effect_name);
}

/*
 *play tower sound by types (i.e laser or bullet...) 
 */
void towerSound(int type){
    Sound s = getSoundPointer();
    Mix_PlayChannel(-1, s->tower_sound[type], 0);
    Mix_VolumeChunk(s->tower_sound[type], 50);
}


/*
 *play enemy death sound 
 */
void enemyDeathSound(){
    Sound s = getSoundPointer();
    Mix_PlayChannel(-1, s->enemy_sound, 0);
    Mix_VolumeChunk(s->enemy_sound, 50);
    Mix_VolumeMusic(120);
}


/*
 *play background sound
 */
void playBackgroundSound(){
    SDL_Delay(20);
    Sound s = getSoundPointer();
    Mix_PlayMusic(s->background_music, -1);
}

/*
*check that sound was loaded correctly
*/
void check_load_sound(void *music, char *sound_name){
    if(music == NULL){
        printf("Cannot find %s\n", sound_name);
        SDL_Quit();
        exit(1);
    }
}

/*
 *singleton..
*/
Sound getSoundPointer() {
    static Sound sound;
    if(!sound)	{
        sound = (Sound)malloc(sizeof(struct sound));
    }
    return sound;
}

/*
 *destroy everything
 */
void shutSound(){
    Sound s = getSoundPointer();
    Mix_FreeMusic(s->background_music);
    Mix_FreeChunk(s->enemy_sound);
    Mix_FreeChunk(s->tower_sound[0]);
    Mix_FreeChunk(s->tower_sound[1]);
    Mix_FreeChunk(s->generalSounds[0]);

    Mix_CloseAudio();
    Mix_Quit();
}
