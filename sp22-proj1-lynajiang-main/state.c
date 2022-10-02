#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.

  game_state_t *defaultboard;

  /** Allocate memory for the heap of the struct */
  defaultboard = (game_state_t *) calloc(1, sizeof(game_state_t));

  defaultboard->x_size = 14;
  defaultboard->y_size = 10;
  defaultboard->num_snakes = 1;
  defaultboard->snakes = (snake_t *) calloc(defaultboard->num_snakes, sizeof(snake_t));
  defaultboard->snakes[0].tail_x = 4;
  defaultboard->snakes[0].tail_y = 4;
  defaultboard->snakes[0].head_x = 5;
  defaultboard->snakes[0].head_y = 4;
  defaultboard->snakes[0].live = true;

  int x = 0;
  int y = 0;
  char row[defaultboard->x_size];
  row[0] = 0;

  int fruit_x = 9;
  int fruit_y = 2;
  defaultboard->board = (char **) calloc((defaultboard->y_size) + 1, sizeof(char *));
  for (int i = 0; i < defaultboard->y_size; i++) {
    defaultboard->board[i] = (char *) calloc((defaultboard->x_size) + 1, sizeof(char));
  }
  for (; y < defaultboard->y_size; y++) {
    for (; x < defaultboard->x_size; x++) {
      if ((y == 0) || (y == defaultboard->y_size - 1)) {
        strcat(row, "#");
      }
      else if ((defaultboard->snakes->tail_x == x) && (defaultboard->snakes->tail_y == y)) {
        strcat(row, "d");
      }

      else if ((defaultboard->snakes->head_x == x) && (defaultboard->snakes->head_y == y)) {
        strcat(row, ">");
      }
      else if (x == fruit_x && y == fruit_y) {
        strcat(row, "*");
      }

      else if(x == 0 || x == defaultboard->x_size - 1) {
        strcat(row, "#");
      }
      else {
        strcat(row, " ");
      }
    }
    strcpy(defaultboard->board[y], row);
    row[0] = 0;
    x = 0;

  }
  return defaultboard; //return pointer to newly struct
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  free(state->snakes);
  for (int i = 0; i < state->y_size; i++) {
    free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for (int i = 0; i < state->y_size; i++) {
    fprintf(fp, state->board[i]);
    fprintf(fp, "\n");
    
  }
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    return true;
  }
  return false;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  if (is_tail(c)) {
    return true;
  }
  if (c == '^' || c == 'v' || c == '<' || c == '>') {
    return true;
  }
  return false;
}

static char body_to_tail(char c) {
  if (c == '^') {
    return 'w';
  }
  if (c == '<') {
    return 'a';
  }
  if (c == 'v') {
    return 's';
  }
  if (c == '>') {
    return 'd';
  }
  return '?';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd') {
    return 1;
  }
  else if (c == '<' || c == 'a') {
    return -1;
  }
  return 0;
}

static int incr_y(char c) {
  if (c == 'w' || c == '^') {
    return -1;
  }
  else if (c == 's' || c == 'v') {
    return 1;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].head_x;
  int y = state->snakes[snum].head_y;
  int next_x;
  int next_y;
  next_x = x + incr_x(get_board_at(state, x, y));
  next_y = y + incr_y(get_board_at(state, x, y));
  return get_board_at(state, next_x, next_y);
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int head_x = state->snakes[snum].head_x;
  int head_y = state->snakes[snum].head_y;
  int next_x = head_x + incr_x(get_board_at(state, head_x, head_y));
  int next_y = head_y + incr_y(get_board_at(state, head_x, head_y));
  state->snakes[snum].head_x = next_x;
  state->snakes[snum].head_y = next_y;
  set_board_at(state, next_x, next_y, get_board_at(state, head_x, head_y));
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int tail_x = state->snakes[snum].tail_x;
  int tail_y = state->snakes[snum].tail_y;
  int next_x = tail_x + incr_x(get_board_at(state, tail_x, tail_y));
  int next_y = tail_y + incr_y(get_board_at(state, tail_x, tail_y));
  state->snakes[snum].tail_x = next_x;
  state->snakes[snum].tail_y = next_y;
  set_board_at(state, next_x, next_y, body_to_tail(get_board_at(state, next_x, next_y)));
  set_board_at(state, tail_x, tail_y, ' ');
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; i++) {
    int head_x = state->snakes[i].head_x;
    int head_y = state->snakes[i].head_y;

    if (is_snake(next_square(state, i)) || next_square(state, i) == '#') {
        set_board_at(state, head_x, head_y, 'x');
        state->snakes[i].live = false;
    }

    else if (next_square(state, i) == ' ') {
      update_head(state, i);
      update_tail(state, i);
    }
    else if (next_square(state, i) == '*') {
      update_head(state, i);
      add_food(state);
    }
    
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE* file;
  char c;
  file = fopen(filename, "r");
  int num_cols = 0;
  int num_rows = 0;
  game_state_t *read_board;
  read_board = (game_state_t *) calloc(1, sizeof(game_state_t));
  // read_board->snakes = (snake_t *) calloc(1, sizeof(snake_t));
  // read_board->num_snakes = 1;
  int total_c = 1;
  read_board->x_size = 0;
  while (1) {
    c = fgetc(file);
    if (c == '\n') {
      total_c--;
      num_rows++;
      if (read_board->x_size == 0) {
        read_board->x_size = total_c;
      }
    }
    else if (feof(file)) {
      break;
    }
    total_c++;

  }
  // rewind(file);
  read_board->y_size = num_rows;
  read_board->board = (char **) malloc(((read_board->y_size)) *sizeof(char *));
  for (int i = 0; i < read_board->y_size; i++) {
    read_board->board[i] = (char *) malloc(((read_board->x_size) + 1) *sizeof(char));
  }
  char row[read_board->x_size];
  row[0] = 0;
  file = fopen(filename, "r");
  for (int y = 0; y < read_board->y_size; y++) {
    for (int x = 0; x < read_board->x_size + 1; x++) {
      if (feof(file)) {
        set_board_at(read_board, x, y, '\0');
        break;
      }
      c = fgetc(file);
      if (c != '\n') {
        printf("%c", c);
        set_board_at(read_board, x, y, c);
      
      }
      if (c == '\n') {
        printf("%c", c);
        set_board_at(read_board, x, y, '\0');
      }
    }
    
  }

  fclose(file);
  
  return read_board;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int tail_x = state->snakes[snum].tail_x;
  int tail_y = state->snakes[snum].tail_y;
  int next_x = tail_x + incr_x(get_board_at(state, tail_x, tail_y));
  int next_y = tail_y + incr_y(get_board_at(state, tail_x, tail_y));
  while (is_snake(get_board_at(state, next_x, next_y))) {
    tail_x = next_x;
    tail_y = next_y;
    next_x = tail_x + incr_x(get_board_at(state, tail_x, tail_y));
    next_y = tail_y + incr_y(get_board_at(state, tail_x, tail_y));
    
  }
  state->snakes[snum].head_x = tail_x;
  state->snakes[snum].head_y = tail_y;
  return;
}

int num_tails(game_state_t* state) {
  int num_tails = 0;
  for (int i = 0; i < state->y_size; i++) {
    for (int j = 0; j < state->x_size; j++) {
      if (is_tail(get_board_at(state, j, i))) {
        num_tails++;
      }
    }
  }
  return num_tails;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int num_snakes = 0;
  state->num_snakes = 1;
  state->snakes = (snake_t *) calloc(num_tails(state), sizeof(snake_t));
  for (int i = 0; i < state->y_size; i++) {
    for (int j = 0; j < state->x_size; j++) {
      if (is_tail(get_board_at(state, j, i))) {
        // if (num_snakes > state->num_snakes) {
        //   state->snakes = (snake_t *) realloc(state->snakes, num_snakes * sizeof(snake_t));
        // }
        state->snakes[num_snakes].tail_x = j;
        state->snakes[num_snakes].tail_y = i;
        state->snakes[num_snakes].live = true;
        num_snakes += 1;
        state->num_snakes = num_snakes;
      }
    }
  }

  for (int i = 0; i < num_snakes; i++) {
    find_head(state, i);
  }
  return state;
}
