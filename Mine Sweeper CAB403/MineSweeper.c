#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define NUM_MINES 10
#define RANDOM_NUMBER_SEED 42

bool game_over = false;
bool won_game = false;
bool mine_hit = false;

int welcomeChoice;
int mines = NUM_MINES;

typedef struct Tile
{
  int adjacent_mines;
  bool revealed;
  bool is_mine;
  bool flagged;
  int tile;
} indivTile;

int *ptr;

void welcome_screen(indivTile tile1[9][9]);
void start_game(indivTile tile1[9][9]);
void initialise_board();
void setup_board(indivTile tile1[9][9]);
void print_board(indivTile tile1[9][9]);
void print_leaderboard();
void game_control(indivTile tile1[9][9]);
void reveal_tile(indivTile tile1[9][9]);
void place_flag(indivTile tile1[9][9]);
int check_surrounding_mines(indivTile tile1[9][9], int r, int c);
void check_surrounding_tiles(indivTile tile1[9][9], int r, int c);
void set_tile_values(indivTile tile1[9][9]);
void place_mines(indivTile tile1[9][9]);
bool tile_contains_mine(indivTile tile1[9][9], int x, int y);
void game_over_screen(indivTile tile1[9][9]);
void game_win(indivTile tile1[9][9]);
void reveal_all_mines(indivTile tile1[9][9], int r, int c);
void check_move_result(indivTile tile1[9][9]);


int main()
{
  srand(RANDOM_NUMBER_SEED);
  indivTile tile1[9][9];
  system("clear");
  welcome_screen(tile1);
  return 0;
}

// display welcome screen
void welcome_screen(indivTile tile1[9][9])
{
  int input = 0;

  printf("==================================================\n");
  printf("Welcome to the Minesweeper gaming system.\n");
  printf("==================================================\n\n");
  printf("Please enter a selection:\n");
  printf("<1> Play Minesweeper\n");
  printf("<2> Show Leaderboard\n");
  printf("<3> Quit\n\n");
  printf("Selection option (1-3): ");
  scanf(" %d", &input);

  if (input == 1)
  {
    printf("Option 1 Chosen\n\n");
    start_game(tile1);
  }
  else if (input == 2)
  {
    printf("Option 2 Chosen\n\n");
    print_leaderboard();
  }
  else if (input == 3)
  {
    printf("Option 3 Chosen\n");
    exit(0);
  }
  else
  {
    system("clear");
    welcome_screen(tile1);
  }
}

void setup_board(indivTile tile1[9][9])
{
  for (int i = 0; i < NUM_TILES_X; i++)
  {
    for (int j = 0; j < NUM_TILES_Y; j++)
    {
      tile1[i][j].revealed = false;
      tile1[i][j].is_mine = false;
      tile1[i][j].flagged = false;
    }
  }
  mines = NUM_MINES;
  place_mines(tile1);
}

void place_mines(indivTile tile1[9][9])
{
  for(int i = 0; i < NUM_MINES; i++)
  {
    int x, y;
    do
    {
      x = rand() % NUM_TILES_X;
      y = rand() % NUM_TILES_Y;
    } while (tile_contains_mine(tile1, x, y));
    // place mine at (x, y)
  }
}

bool tile_contains_mine(indivTile tile1[9][9], int x, int y)
{
  if (tile1[x][y].is_mine == true)
  {
    return true;
  }
  else
  {
    tile1[x][y].is_mine = true;
    return false;
  }
}

void print_board(indivTile tile1[9][9])
{
  printf("==================================================\n");
  printf("Minesweeper\n");
  printf("==================================================\n\n");
  printf("GAME START\n\n");
  printf("Remaining Mines: %d\n\n", mines);
  printf("      1 2 3 4 5 6 7 8 9 \n");
  printf("  ---------------------- ");

  char row = 'A';
  for (int i = 0; i < NUM_TILES_X; i++)
  {
    for (int j = 0; j < NUM_TILES_Y; j++)
    {
      if (j%9 == 0)
      {
        printf("\n");
        printf("  %c | ", row);
        row++;
      }
      if (tile1[i][j].revealed == false && tile1[i][j].flagged == false)
      {
        printf("  "); //  1 = unrevealed
      }
      else if (tile1[i][j].revealed == true)
      {
        //printf("%d ", tile1[i][j].adjacent_mines); // 2 = revealed (adjacent mines)
        if (tile1[i][j].is_mine == true)
        {
          printf("* ");
        }
        else
        {
          printf("%d ", tile1[i][j].adjacent_mines); // 2 = revealed (adjacent mines)
        }
      }
      else if (tile1[i][j].flagged == true)
      {
        printf("f "); // 3 = flagged
      }
    }
  }
  check_move_result(tile1);
}

void check_move_result(indivTile tile1[9][9]) // print board stuff
{
  if (won_game == true)
  {
    game_win(tile1);
  }
  else if (mine_hit == true)
  {
    while (mine_hit == true)
    {
      game_over_screen(tile1);
      system("clear");
      print_board(tile1);
    }
  }
  else
  {
    printf("\n\n\n");
    printf("Choose an option:\n");
    printf("<R> Reveal Tile\n");
    printf("<F> Flag Tile\n");
    printf("<Q> Quit Game\n\n");
  }
}

void print_leaderboard()
{
  system("clear");
  printf("==================================================\n");
  printf("Leaderboard:\n");
  printf("==================================================\n\n");
  printf("  N8     8 seconds     4 games won, 4 games played\n\n");
}

void game_over_screen(indivTile tile1[9][9])
{
  char option;
  char option2;
  game_over = true;

  printf("\n\n\n");
  printf("Game Over!\n");
  printf("Play Again? (Y/N)\n");
  scanf(" %c", &option);

  if (option == 'y' || option == 'Y')
  {
    mine_hit = false;
    start_game(tile1);
  }
  else if (option == 'n' || option == 'N')
  {
    printf("Return to Main Menu? (Y/N)\n");
    scanf(" %c", &option2);
    if (option2 == 'y' || option2 == 'Y')
    {
      system("clear");
      mine_hit = false;
      welcome_screen(tile1);
    }
    else if (option2 == 'n' || option2 == 'N')
    {
      exit(0);
    }
  }
  else
  {
    printf("Invalid option. Please select a valid option.\n");
  }
}

void game_win(indivTile tile1[9][9])
{
  char option;
  char option2;

  printf("\n\n\n\nYOU WIN!");
  printf("\n\nCongratulations! You found every mine!\n\n");
  printf("Play Again? (Y/N)\n");
  scanf(" %c", &option);
  if (option == 'y' || option == 'Y')
  {
    won_game = false;
    start_game(tile1);
  }
  else if (option == 'n' || option == 'N')
  {
    printf("Return to Main Menu? (Y/N)\n");
    scanf(" %c", &option2);
    if (option2 == 'y' || option2 == 'Y')
    {
      system("clear");
      welcome_screen(tile1);
    }
    else if (option2 == 'n' || option2 == 'N')
    {
      exit(0);
    }
  }
}

void game_control(indivTile tile1[9][9])
{
  char option;
  char option2;

  do
  {
    printf("Options (R, F, Q): ");
    scanf(" %c", &option);
  } while(!(option == 'r' || option == 'R' || option == 'f' || option == 'F'
  || option == 'q' || option == 'Q'));

  // printf("Options (R, F, Q): ");
  // scanf(" %c", &option);

  if (option == 'r' || option == 'R')
  {
    reveal_tile(tile1);
    printf("debug 5\n");
  }
  else if (option == 'f' || option == 'F')
  {
    place_flag(tile1);
    printf("debug pf 3\n");
  }
  else if (option == 'q' || option == 'Q')
  {
    do
    {
      printf("\nReturn to Main Menu? (Y/N)\n");
      scanf(" %c", &option2);
    } while (option2 != 'y' || option2 != 'Y' || option2 != 'n' || option2 != 'N');

    if (option2 == 'y' || option2 == 'Y')
    {
      system("clear");
      welcome_screen(tile1);
    }
    else if (option2 == 'n' || option2 == 'N')
    {
      exit(0);
    }
  }
  else
  {
    printf("Invalid option. Please select a valid option.\n");
  }
}

void reveal_tile(indivTile tile1[9][9])
{
  char row;
  int row2;
  int col;
  int col2;

  do
  {
    printf("Enter a coordinate to reveal: ");
    scanf(" %c%d", &row, &col);
  } while(row < 'a' || row > 'i' || col < 1 || col > 9);

    ////////////////needs index error checking
  col2 = col - 1;
  printf("debug 1\n");

  if (row == 'a' || row == 'A')
  {
    row2 = 0;
  }
  else if (row == 'b' || row == 'B')
  {
    row2 = 1;
  }
  else if (row == 'c' || row == 'C')
  {
    row2 = 2;
  }
  else if (row == 'd' || row == 'D')
  {
    row2 = 3;
  }
  else if (row == 'e' || row == 'E')
  {
    row2 = 4;
  }
  else if (row == 'f' || row == 'F')
  {
    row2 = 5;
  }
  else if (row == 'g' || row == 'G')
  {
    row2 = 6;
  }
  else if (row == 'h' || row == 'H')
  {
    row2 = 7;
  }
  else if (row == 'i' || row == 'I')
  {
    row2 = 8;
  }

  tile1[row2][col2].revealed = true;
  printf("debug 2\n");
  reveal_all_mines(tile1, row2, col2);
}

void reveal_all_mines(indivTile tile1[9][9], int r, int c) //reveal tile stuff
{
  int row2 = r, col2 = c;
  if (tile1[row2][col2].revealed == true)
  {
    printf("debug 2.2\n");
    if (tile1[row2][col2].is_mine == true)
    {
      printf("debug 2.3\n");
      for (int i = 0; i < NUM_TILES_X; i++)
      {
        printf("debug 2.4\n");
        for (int j = 0; j < NUM_TILES_Y; j++)
        {
          printf("debug 2.5\n");
          if (tile1[i][j].is_mine == true)
          {
            printf("debug 2.6\n");
            tile1[i][j].revealed = true;
            mine_hit = true;
            printf("debug 2.7\n");
          }
        }
      }
    }
    else
    {
      printf("debug 2.8\n");
      set_tile_values(tile1);
      check_surrounding_tiles(tile1, row2, col2);
      printf("debug 3\n");
    }
  }
}

void set_tile_values(indivTile tile1[9][9])
{ printf("debug 2.9\n");
  for (int i = 0; i < NUM_TILES_X; i++)
  {
    for (int j = 0; j < NUM_TILES_Y; j++)
    {
      printf("debug 2.9.1\n");
      tile1[i][j].adjacent_mines = check_surrounding_mines(tile1, i, j);
    }
  }
}

void check_surrounding_tiles(indivTile tile1[9][9], int r, int c)
{
  printf("WHAT2");
  //return;

  // int topLeft = 0, top = 0, topRight = 0, right = 0,
  // bottomRight = 0, bottom = 0, bottomLeft = 0, left = 0;

  // for (int i = 0; i < NUM_TILES_X; i++)
  // {
  //   for (int j = 0; j < NUM_TILES_Y; j++)
  //   {
  //     tile1[i][j].adjacent_mines = check_surrounding_mines(tile1, i, j);
  //   }
  // }
  //printf("debugg 1");

  // if (tile1[r][c].adjacent_mines == 0)
  // {
  //   if (r == 0)
  //   {
  //     //printf("debugg 2");
  //     if (c == 0)
  //     {
  //       //printf("debugg 3");
  //
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //       if (tile1[r+1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c+1);
  //       }
  //     }
  //     else if (c == 8)
  //     {
  //       //printf("debugg 7");
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //       if (tile1[r+1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c-1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //     }
  //     else if (c > 0 && c < 8)
  //     {
  //       //printf("debugg 11");
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //       if (tile1[r+1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c-1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //       if (tile1[r+1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c+1);
  //       }
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //     }
  //   }
  //   else if (r == 8)
  //   {
  //     //printf("debugg 17");
  //     if (c == 0)
  //     {
  //       //printf("debugg 18");
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //       if (tile1[r-1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c-1);
  //       }
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //     }
  //     else if (c == 8)
  //     {
  //       //printf("debugg 22");
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //       if (tile1[r-1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c-1);
  //       }
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //     }
  //     else if (c > 0 && c < 8)
  //     {
  //       //printf("debugg 26");
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //       if (tile1[r-1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c-1);
  //       }
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //       if (tile1[r-1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c+1);
  //       }
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //     }
  //   }
  //   else if (r > 0 && r < 8)
  //   {
  //     //printf("debugg 32");
  //     if (c == 0)
  //     {
  //       //printf("debugg 33");
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //       if (tile1[r-1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c+1);
  //       }
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //       if (tile1[r+1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c+1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //     }
  //     else if (c == 8)
  //     {
  //       //printf("debugg 39");
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //       if (tile1[r-1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c-1);
  //       }
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //       if (tile1[r+1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c-1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //     }
  //     else if (c > 0 && c < 8)
  //     {
  //       //printf("debugg 45");
  //       if (tile1[r-1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c-1);
  //       }
  //       if (tile1[r-1][c].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c);
  //       }
  //       if (tile1[r-1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r-1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r-1, c+1);
  //       }
  //       if (tile1[r][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c+1);
  //       }
  //       if (tile1[r+1][c+1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c+1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c+1);
  //       }
  //       if (tile1[r+1][c].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c);
  //       }
  //       if (tile1[r+1][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r+1][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r+1, c-1);
  //       }
  //       if (tile1[r][c-1].adjacent_mines == 0)
  //       {
  //         tile1[r][c-1].revealed = true;
  //         //check_surrounding_tiles(tile1, r, c-1);
  //       }
  //     }
  //   }
  //   else
  //   {
  //     return;
  //   }// end if
  // }

  if (tile1[r][c].adjacent_mines == 0)
  {
    if (r < 0 || r > 8 || c < 0 || c > 8)
    {
      printf("WHAT3");
      return;
    }
    printf("WHAT4");

    if (r > 0 & r < 8 && c > 0 && c < 8)
    {
      printf("WEE\n");
      if (tile1[r-1][c-1].adjacent_mines == 0 && tile1[r-1][c-1].revealed == false)
      {
        printf("HAI\n");
        tile1[r-1][c-1].revealed = true;
        check_surrounding_tiles(tile1, r-1, c-1);
      }
      if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
      {
        printf("HAI2\n");
        tile1[r-1][c].revealed = true;
        printf("HAI2.1\n");
        check_surrounding_tiles(tile1, r-1, c); //ENTERED
      }
      if (tile1[r-1][c+1].adjacent_mines == 0 && tile1[r-1][c+1].revealed == false)
      {
        printf("HAI3\n");
        tile1[r-1][c+1].revealed = true;
        check_surrounding_tiles(tile1, r-1, c+1);
      }
      if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
      {
        printf("HAI4\n");
        tile1[r][c+1].revealed = true;
        check_surrounding_tiles(tile1, r, c+1);
      }
      if (tile1[r+1][c+1].adjacent_mines == 0 && tile1[r+1][c+1].revealed == false)
      {
        printf("HAI5\n");
        tile1[r+1][c+1].revealed = true;
        check_surrounding_tiles(tile1, r+1, c+1);
      }
      if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
      {
        printf("HAI6\n");
        tile1[r+1][c].revealed = true;
        check_surrounding_tiles(tile1, r+1, c);
      }
      if (tile1[r+1][c-1].adjacent_mines == 0 && tile1[r+1][c-1].revealed == false)
      {
        printf("HAI7\n");
        tile1[r+1][c-1].revealed = true;
        check_surrounding_tiles(tile1, r+1, c-1);
      }
      if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
      {
        printf("HAI8\n");
        tile1[r][c-1].revealed = true;
        check_surrounding_tiles(tile1, r, c-1);
      }
    }
    else if (r == 0)
    {
      printf("WEE2\n");
      if (c == 0)
      {
        printf("WOO\n");
        if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
        {
          printf("HO\n");
          tile1[r][c+1].revealed = true;
          check_surrounding_tiles(tile1, r, c+1);
        }
        if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
        {
          printf("HO2\n");
          tile1[r+1][c].revealed = true;
          check_surrounding_tiles(tile1, r+1, c);
        }
        if (tile1[r+1][c+1].adjacent_mines == 0 && tile1[r+1][c+1].revealed == false)
        {
          printf("HO3\n");
          tile1[r+1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c+1);
        }
      }
      else if (c == 8)
      {
        printf("WOO2\n");
        if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
        {
          printf("HO5\n");
          tile1[r][c-1].revealed = true;
          check_surrounding_tiles(tile1, r, c-1);
        }
        if (tile1[r+1][c-1].adjacent_mines == 0 && tile1[r+1][c-1].revealed == false)
        {
          printf("HO6\n");
          tile1[r+1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c-1);
        }
        if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
        {
          printf("HO7\n");
          tile1[r+1][c].revealed = true;
          check_surrounding_tiles(tile1, r+1, c);
        }
      }
      else
      {
        printf("WEE3\n");
        //printf("debugg 11");
        if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
        {
          printf("HO8\n");
          tile1[r][c-1].revealed = true;
          check_surrounding_tiles(tile1, r, c-1);
        }
        if (tile1[r+1][c-1].adjacent_mines == 0 && tile1[r+1][c-1].revealed == false)
        {
          printf("HO9\n");
          tile1[r+1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c-1);
        }
        if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
        {
          printf("HO10\n");
          tile1[r+1][c].revealed = true;
          check_surrounding_tiles(tile1, r+1, c); //ENTERED
        }
        if (tile1[r+1][c+1].adjacent_mines == 0 && tile1[r+1][c+1].revealed == false)
        {
          printf("HO11\n");
          tile1[r+1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c+1);
        }
        if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
        {
          printf("HO12\n");
          tile1[r][c+1].revealed = true;
          check_surrounding_tiles(tile1, r, c+1);
        }
      }
    }
    else if (r == 8)
    {
      printf("WEE4\n");
      if (c == 0)
      {
        if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
        {
          tile1[r-1][c].revealed = true;
          check_surrounding_tiles(tile1, r-1, c);
        }
        if (tile1[r-1][c+1].adjacent_mines == 0 && tile1[r-1][c+1].revealed == false)
        {
          tile1[r-1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c-1);
        }
        if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
        {
          tile1[r][c+1].revealed = true;
          check_surrounding_tiles(tile1, r, c+1);
        }
      }
      else if (c == 8)
      {
        if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
        {
          tile1[r][c-1].revealed = true;
          check_surrounding_tiles(tile1, r, c-1);
        }
        if (tile1[r-1][c-1].adjacent_mines == 0 && tile1[r-1][c-1].revealed == false)
        {
          tile1[r-1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c-1);
        }
        if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
        {
          tile1[r-1][c].revealed = true;
          check_surrounding_tiles(tile1, r-1, c);
        }
      }
      else
      {
        //printf("debugg 26");
        if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
        {
          tile1[r][c-1].revealed = true;
          check_surrounding_tiles(tile1, r, c-1);
        }
        if (tile1[r-1][c-1].adjacent_mines == 0 && tile1[r-1][c-1].revealed == false)
        {
          tile1[r-1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c-1);
        }
        if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
        {
          tile1[r-1][c].revealed = true;
          check_surrounding_tiles(tile1, r-1, c);
        }
        if (tile1[r-1][c+1].adjacent_mines == 0 && tile1[r-1][c+1].revealed == false)
        {
          tile1[r-1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c+1);
        }
        if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
        {
          tile1[r][c+1].revealed = true;
          check_surrounding_tiles(tile1, r, c+1);
        }
      }
    }
    else
    {
      printf("WEE5\n");
      if (c == 0)
      {
        printf("NO\n");
        if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
        {
          printf("NO1\n");
          tile1[r-1][c].revealed = true;
          check_surrounding_tiles(tile1, r-1, c);
        }
        if (tile1[r-1][c+1].adjacent_mines == 0 && tile1[r-1][c+1].revealed == false)
        {
          printf("NO2\n");
          tile1[r-1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c+1);
        }
        if (tile1[r][c+1].adjacent_mines == 0 && tile1[r][c+1].revealed == false)
        {
          printf("NO3\n");
          tile1[r][c+1].revealed = true;
          check_surrounding_tiles(tile1, r, c+1);
        }
        if (tile1[r+1][c+1].adjacent_mines == 0 && tile1[r+1][c+1].revealed == false)
        {
          printf("NO4\n");
          tile1[r+1][c+1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c+1);
        }
        if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
        {
          printf("NO5\n");
          tile1[r+1][c].revealed = true;
          check_surrounding_tiles(tile1, r+1, c);
        }
      }
      else if (c == 8)
      {
        if (tile1[r-1][c].adjacent_mines == 0 && tile1[r-1][c].revealed == false)
        {
          tile1[r-1][c].revealed = true;
          check_surrounding_tiles(tile1, r-1, c);
        }
        if (tile1[r-1][c-1].adjacent_mines == 0 && tile1[r-1][c-1].revealed == false)
        {
          tile1[r-1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r-1, c-1);
        }
        if (tile1[r][c-1].adjacent_mines == 0 && tile1[r][c-1].revealed == false)
        {
          tile1[r][c-1].revealed = true;
          check_surrounding_tiles(tile1, r, c-1);
        }
        if (tile1[r+1][c-1].adjacent_mines == 0 && tile1[r+1][c-1].revealed == false)
        {
          tile1[r+1][c-1].revealed = true;
          check_surrounding_tiles(tile1, r+1, c-1);
        }
        if (tile1[r+1][c].adjacent_mines == 0 && tile1[r+1][c].revealed == false)
        {
          tile1[r+1][c].revealed = true;
          check_surrounding_tiles(tile1, r+1, c);
        }
      }
    }
  }
}

void place_flag(indivTile tile1[9][9])
{
  char row;
  int row2;
  int col;
  int col2;

  printf("Enter a coordinate to flag: ");
  scanf(" %c%d", &row, &col);

  col2 = col - 1;

  if (row == 'a' || row == 'A')
  {
    row2 = 0;
  }
  else if (row == 'b' || row == 'B')
  {
    row2 = 1;
  }
  else if (row == 'c' || row == 'C')
  {
    row2 = 2;
  }
  else if (row == 'd' || row == 'D')
  {
    row2 = 3;
  }
  else if (row == 'e' || row == 'E')
  {
    row2 = 4;
  }
  else if (row == 'f' || row == 'F')
  {
    row2 = 5;
  }
  else if (row == 'g' || row == 'G')
  {
    row2 = 6;
  }
  else if (row == 'h' || row == 'H')
  {
    row2 = 7;
  }
  else if (row == 'i' || row == 'I')
  {
    row2 = 8;
  }

  tile1[row2][col2].flagged = true;
  if (tile1[row2][col2].is_mine == true)
  {
    mines = mines-1;
    if (mines == 0)
    {
      won_game = true;
    }
  }
  printf("debug pf 2.5");
}

int check_surrounding_mines(indivTile tile1[9][9], int r, int c)
{
  int topLeft = 0, top = 0, topRight = 0, right = 0,
  bottomRight = 0, bottom = 0, bottomLeft = 0, left = 0;
  int sum = 0;
  printf("debug 2.9.2\n");
  //printf("debugg 1");
  if (r == 0)
  {
    //printf("debugg 2");
    if (c == 0)
    {
      //printf("debugg 3");

      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 4");

      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 5");
      }
      if (tile1[r+1][c+1].is_mine == true)
      {
        bottomRight = 1;
        //printf("debugg 6");
      }
    }
    else if (c == 8)
    {
      //printf("debugg 7");
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 8");
      }
      if (tile1[r+1][c-1].is_mine == true)
      {
        bottomLeft = 1;
        //printf("debugg 9");
      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 10");
      }
    }
    else if (c > 0 && c < 8)
    {
      //printf("debugg 11");
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 12");
      }
      if (tile1[r+1][c-1].is_mine == true)
      {
        bottomLeft = 1;
        //printf("debugg 13");
      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 14");
      }
      if (tile1[r+1][c+1].is_mine == true)
      {
        bottomRight = 1;
        //printf("debugg 15");
      }
      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 16");
      }
    }
  }
  else if (r == 8)
  {
    //printf("debugg 17");
    if (c == 0)
    {
      //printf("debugg 18");
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 19");
      }
      if (tile1[r-1][c+1].is_mine == true)
      {
        topRight = 1;
        //printf("debugg 20");
      }
      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 21");
      }
    }
    else if (c == 8)
    {
      //printf("debugg 22");
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 23");
      }
      if (tile1[r-1][c-1].is_mine == true)
      {
        topLeft = 1;
        //printf("debugg 24");
      }
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 25");
      }
    }
    else if (c > 0 && c < 8)
    {
      //printf("debugg 26");
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 27");
      }
      if (tile1[r-1][c-1].is_mine == true)
      {
        topLeft = 1;
        //printf("debugg 28");
      }
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 29");
      }
      if (tile1[r-1][c+1].is_mine == true)
      {
        topRight = 1;
        //printf("debugg 30");
      }
      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 31");
      }
    }
  }
  else if (r > 0 && r < 8)
  {
    //printf("debugg 32");
    if (c == 0)
    {
      //printf("debugg 33");
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 34");
      }
      if (tile1[r-1][c+1].is_mine == true)
      {
        topRight = 1;
        //printf("debugg 35");
      }
      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 36");
      }
      if (tile1[r+1][c+1].is_mine == true)
      {
        bottomRight = 1;
        //printf("debugg 37");
      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 38");
      }
    }
    else if (c == 8)
    {
      //printf("debugg 39");
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 40");
      }
      if (tile1[r-1][c-1].is_mine == true)
      {
        topLeft = 1;
        //printf("debugg 41");
      }
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 42");
      }
      if (tile1[r+1][c-1].is_mine == true)
      {
        bottomLeft = 1;
        //printf("debugg 43");
      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 44");
      }
    }
    else if (c > 0 && c < 8)
    {
      //printf("debugg 45");
      if (tile1[r-1][c-1].is_mine == true)
      {
        topLeft = 1;
        //printf("debugg 46");
      }
      if (tile1[r-1][c].is_mine == true)
      {
        top = 1;
        //printf("debugg 47");
      }
      if (tile1[r-1][c+1].is_mine == true)
      {
        topRight = 1;
        //printf("debugg 48");
      }
      if (tile1[r][c+1].is_mine == true)
      {
        right = 1;
        //printf("debugg 49");
      }
      if (tile1[r+1][c+1].is_mine == true)
      {
        bottomRight = 1;
        //printf("debugg 5");
      }
      if (tile1[r+1][c].is_mine == true)
      {
        bottom = 1;
        //printf("debugg 51");
      }
      if (tile1[r+1][c-1].is_mine == true)
      {
        bottomLeft = 1;
        //printf("debugg 52");
      }
      if (tile1[r][c-1].is_mine == true)
      {
        left = 1;
        //printf("debugg 53");
      }
    }
  }
  //printf("debugg 54");
  sum = topLeft + top + topRight + right + bottomRight + bottom + bottomLeft + left;
  //printf("debugg 55\n");
  //printf("%d \n", sum);
  printf("\n WHAT");
  return sum;
}

void start_game(indivTile tile1[9][9])
{
  game_over = false;

  setup_board(tile1);
  while (game_over == false)
  {
    system("clear");
    print_board(tile1);
    game_control(tile1);
  }
}
