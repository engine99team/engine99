#ifndef ENGINE99_GAME_H
#define ENGINE99_GAME_H

//Init stage of a game. You can register your systems and components here.
int init_game (void);

//Game events
int game_event (SDL_Event* event);

//Finish game and clear memory
int destroy_game (void);


#endif //ENGINE99_GAME_H
