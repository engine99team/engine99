#ifndef ENGINE99_GAME_H
#define ENGINE99_GAME_H

//Init stage of a game. You can register your systems and components here.
int init_game ();

//Game events
int game_event (SDL_Event* event);


#endif //ENGINE99_GAME_H
