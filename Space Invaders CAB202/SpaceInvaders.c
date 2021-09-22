#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <curses.h>

//Configuration
#define DELAY (10) /* Millisecond delay between game updates */

#define S_W (5)
#define S_H (3)
#define L_D_W (5)
#define L_D_H (5)
#define S_D_W (3)
#define S_D_H (3)
#define M_W (1)
#define M_H (1)
#define MAX_D (10)
#define MAX_M (100)
#define MAX_SD (20)

void setup();

//Game state.
bool game_started = false;
bool game_over = false; /* Set this to true when game is over */
bool update_screen = true; /* Set this to false to prevent screen update */


// Global Variables
int lives = 5;
int score = 0;

//Timer
int current_ingame_time = 0;
int timer_ingame = 0;
int seconds = 0;
int minutes = 0;

void my_timer(void){
  int real_time = get_current_time();
  int current_ingame_time = 0;

  if (current_ingame_time != real_time){
    current_ingame_time = real_time;
    timer_ingame++;
  }

  if (timer_ingame == 60){
    timer_ingame = 0;
    seconds++;
  }

  if (seconds == 60){
    seconds = 0;
    minutes++;
  }
}



char * introhelp_screen_image =
/**/
/**/ "########################################"
/**/ "#         CAB202 Assignment 1          #"
/**/ "#        The Diamonds of Doom          #"
/**/ "#            Nathan Thai               #"
/**/ "#              n9823361                #"
/**/ "########################################"
/**/ "#              Controls                #"
/**/ "#         q      : quit                #"
/**/ "#         h      : help                #"
/**/ "#    Arrow keys  : move left/right     #"
/**/ "# Space, z, x, c : shoot bullet        #"
/**/ "########################################"
/**/ "#        Press a key to play...        #"
/**/ "########################################";

char * gameover_image =
/**/ "           Game Over!             "
/**/ "Would you like to play again? y/n ";








// Set spaceship
char * spaceship_image =
/**/ "  |  "
/**/ " [@] "
/**/ "|_M_|";

char * large_diamond_image =
/**/ "  ^  "
/**/ " | | "
/**/ "< X >"
/**/ " | | "
/**/ "  v  ";

char * small_diamond_image =
/**/ " ^ "
/**/ "<x>"
/**/ " v ";

char * missile_image = 
/**/ "*";

sprite_id introhelp_screen;
sprite_id gameover_screen;
sprite_id spaceship;
sprite_id missiles[MAX_M];
sprite_id diamonds[MAX_D];
sprite_id small_diamonds[MAX_SD];


void reset_arrays(){
    for (int i=0; i < MAX_M; i++){
        if (i < MAX_D){
            diamonds[i] = NULL;
        }
        if (i < MAX_SD){
            small_diamonds[i] = NULL;
        }
        missiles[i] = NULL;
    }
}

///// SCREENS /////

void draw_border(void){
  int left = 0;
  int top = 0;
  int right = screen_width() - 1;
  int bottom = screen_height() - 1;

  // Border
  draw_line(left, top, right, top, '#');
  draw_line(right, top, right, bottom, '#');
  draw_line(left, bottom, right, bottom, '#');
  draw_line(left, top, left, bottom, '#');


  draw_line(left, (top + 2), right, (top + 2), '#');


}

void status_display(void){
    my_timer();
    draw_formatted(2, 1,  "Lives: %d", lives);
    draw_formatted(screen_width()/3, 1, "Score: %d", score);

    draw_formatted((screen_width() + screen_width())/3, 1, "Time %d", minutes);
    draw_formatted((screen_width() + screen_width())/3 + 6, 1, ":%02d", seconds);

}

void display_introhelp_screen(){
  int middlex = (screen_width()/2);
  int middley = (screen_height()/2);
  int intro_w = 40;
  int intro_h = 14;

  clear_screen();
  introhelp_screen = sprite_create((middlex - (intro_w/2)), (middley - (intro_h/2)),
   40, 14, introhelp_screen_image);
  sprite_draw(introhelp_screen);
  while (get_char() >= 0){}
  show_screen();
  while (get_char() >= 0){}
  wait_char();
  clear_screen();
}

void display_gameover_screen(){
    int w = screen_width(), h = screen_height();
    clear_screen();
    int message_width = strlen(gameover_image) / 2;
    gameover_screen = sprite_create((w - message_width) / 2, (h - 2) / 2, message_width, 2, gameover_image);
    sprite_draw(gameover_screen);
    while (get_char() >= 0){}
    show_screen();
    while (get_char() >= 0){}
}

// INITIALISE
void initialise_spaceship(){
    int w = screen_width(), h = screen_height();
    int ssw = S_W, ssh = S_H;
    int x = ((w-ssw)/2), y = ((h-ssh)-1);

    spaceship = sprite_create(x, y, ssw, ssh, spaceship_image);
}

void initialise_diamond(double x, double y){
    for ( int i = 0; i < MAX_D; i++){
        if (!diamonds[i]){
            diamonds[i] = sprite_create(x, y, L_D_W, L_D_H, large_diamond_image);
            int angle = rand() % 360;
            sprite_turn_to(diamonds[i], 0.1, 0.1);
            sprite_turn(diamonds[i], angle);
            break;
        }
    }
}

void initialise_small_diamond(double x, double y, double dx, double dy){
    for (int i=0; i < MAX_SD; i++){
        if (!small_diamonds[i]){
            small_diamonds[i] = sprite_create(x, y, S_D_W, S_D_H, small_diamond_image);
            int angle = rand() % 360;
            sprite_turn_to(small_diamonds[i], 0.1, 0.1);
            sprite_turn(small_diamonds[i], angle);
            break;
        }
    }
}

void initialise_missiles(double x, double y){
    for ( int i = 0; i < MAX_M; i++){
        if (missiles[i]){
            if (round(sprite_x(missiles[i])) == round(x) && round(sprite_y(missiles[i])) == round(y)){
                break;
            }
        }
        else if (!missiles[i]){
            missiles[i] = sprite_create(x, y, 1, 1, missile_image);
            sprite_turn_to(missiles[i], 0, -0.1);
            break;
        }
    }
}

void create_diamonds(){
    int w = screen_width(), h = screen_height();
    int zx = 1 + rand() % (w - L_D_W - 2);
    for (int i = 0; i < MAX_D; i++){
        initialise_diamond(zx, 3);
    }
}

void draw_sprites(){
    for ( int i = 0; i < MAX_M; i++){
        if (missiles[i]){
            sprite_draw(missiles[i]);
        }
        if (i < MAX_D){
            if (diamonds[i] != NULL){
                sprite_draw(diamonds[i]);
            }
        }
        if (i < MAX_SD){
            if (small_diamonds[i]){
                sprite_draw(small_diamonds[i]);
            }
        }
    }
}

void diamond_move(sprite_id diamond, int width, int height){
    int w = screen_width(), h = screen_height();
    sprite_step(diamond);

    //diamond border collision
    int ldx = round(sprite_x(diamond));
    int ldy = round(sprite_y(diamond));
    double lddx = sprite_dx(diamond);
    double lddy = sprite_dy(diamond);

    // diamond hits left or right border
    if (ldx <= 0){
        lddx = fabs(lddx);
    }
    else if (ldx >= w - width){
        lddx = -fabs(lddx);
    }
    // diamond hits top or bottom border
    if (ldy <= 3){
        lddy = fabs(lddy);
    }
    else if (ldy >= h - height){
        lddy = -fabs(lddy);
    }

    // large diamond direction change
    if (lddx != sprite_dx(diamond) || lddy != sprite_dy(diamond)){
        sprite_back(diamond);
        sprite_turn_to(diamond, lddx, lddy);
    }


}

void shoot_missiles(int side){
    if (side == 1){
        initialise_missiles(sprite_x(spaceship), sprite_y(spaceship));
    }
    else if (side == 2){
        initialise_missiles(sprite_x(spaceship)+2, sprite_y(spaceship)-1);
    }
    else if (side == 3){
        initialise_missiles(sprite_x(spaceship)+4, sprite_y(spaceship));
    }
}

bool collision_coords(sprite_id sprite, int xs, int ys){
    int x = (int) round(sprite->x);
    int y = (int) round(sprite->y);
    int offset = 0;

    for (int row=0; row < sprite->height; row++){
        for (int col=0; col < sprite->width; col++){
            char ch = sprite->bitmap[offset++] & 0xff;
            if (ch != ' '){
                if (x + col == xs & y + row == ys){
                    return true;
                }
            }
        }
    }
    return false;
}

bool collision_sprites(sprite_id sprite_one, sprite_id sprite_two){
    if (sprite_one && sprite_two){
        int x = (int) round(sprite_one->x);
        int y = (int) round(sprite_one->y);
        int offset = 0;
        
        for (int row=0; row < sprite_one->height; row++){
            for (int col=0; col < sprite_one->width; col++){
                char ch = sprite_one->bitmap[offset++] & 0xff;
                if (ch != ' '){
                    if (collision_coords(sprite_two, x + col, y + row)){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void collision(){
    for (int i=0; i < MAX_M; i++){
        for (int j=0; j < MAX_D; j++){
            if (spaceship && diamonds[j] && collision_sprites(diamonds[i], spaceship)){
                reset_arrays();
                setup();
                lives--;
            }
            if (collision_sprites(diamonds[j], missiles[i])){
                missiles[i] = NULL;
                
                initialise_small_diamond(sprite_x(diamonds[j]), sprite_y(diamonds[j]), sprite_dx(diamonds[j]), sprite_dy(diamonds[j]));
                initialise_small_diamond(sprite_x(diamonds[j]), sprite_y(diamonds[j]), sprite_dx(diamonds[j]), sprite_dy(diamonds[j]));
                diamonds[j] = NULL;
                score++;
                i = 0;
                j = 0;
            }

        }
        for (int j=0; j < MAX_SD; j++){
            if (spaceship && small_diamonds[j] && collision_sprites(small_diamonds[j], spaceship)){
                reset_arrays();
                setup();
                lives--;
            }
            if (collision_sprites(small_diamonds[j], missiles[i])){
                missiles[i] = NULL;
                small_diamonds[j] = NULL;
                i = 0;
                j = 0;
            }
        }
    }
}




///// GAME SETUP /////

void update_sprites(){
    for ( int i = 0; i < MAX_M; i++){
        
        if (missiles[i]){
            double mdy = sprite_dy(missiles[i]);
            if (round(sprite_y(missiles[i])) <= 3){
                mdy = fabs(mdy);
                missiles[i] = NULL;
            } else {
                sprite_step(missiles[i]);


                // if (mdy != sprite_dy(missiles[i])){
                //     sprite_destroy(missiles[i]);
                // }

            }
        }

        if (i < MAX_D){
            if (diamonds[i] != NULL){
                diamond_move(diamonds[i], L_D_W, L_D_H);
            }
        }
        if (i < MAX_SD){
            if (small_diamonds[i]){
                diamond_move(small_diamonds[i], S_D_W, S_D_H);
            }
        }
    }
}
///// RUN GAME /////


void setup(void){
    if (game_started == false){
        game_started = true;
        display_introhelp_screen();
    }
        

    initialise_spaceship();
    // initialise_diamond((1 + rand() % (screen_width() - L_D_W - 2)), 3);
    // initialise_large_diamond();
    create_diamonds();
    // show_screen();

}

void process(void){
    int w = screen_width();
    int key = get_char();
    int sx = round(sprite_x(spaceship));

    //if (!help_dialog && !game_over_dialog)
    draw_border();
    status_display();
    update_sprites();
    collision();
    sprite_draw(spaceship);
    // initialise_diamond()
    draw_sprites();

    if ( key == 'h'){
        display_introhelp_screen();
    }

    // If q is pressed, prompt game over, play again screen
    if (key == 'q' || lives == 0){
        display_gameover_screen();
        if (wait_char() == 'y'){
        current_ingame_time = 0;
        timer_ingame = 0;
        seconds = 0;
        minutes = 0;
        lives = 5;
        score = 0;
        clear_screen();
        setup_screen();
        setup();
        show_screen();
        return;
        }
        else if (wait_char() == 'n'){
        clear_screen();
        game_over = true;
        }
    }

    if (key == KEY_LEFT && sx > 1 ){
        sprite_move(spaceship, -1, 0);
    }
    if (key == KEY_RIGHT && sx < w - sprite_width(spaceship) - 1){
        sprite_move(spaceship, +1, 0);
    }
    if (key ==  'z'){
        shoot_missiles(1);
    }
    if (key ==  'x'){
        shoot_missiles(2);
    }
    if (key ==  'c'){
        shoot_missiles(3);
    }

}

void cleanup(void){}

int main(void){
    setup_screen();
    setup();
    show_screen();
    while (!game_over)
    {   
        clear_screen();
        process();
        if (update_screen)
        {
            show_screen();
        }
        timer_pause(DELAY);
    }
    cleanup();
    return 0;
}