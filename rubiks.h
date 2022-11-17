#ifndef PROJET_C_RUBIKS_H
#define PROJET_C_RUBIKS_H


typedef enum { R, B, G, W, Y, O, LG } T_COLOR ;
typedef enum { FRONT, BACK, UP, DOWN, RIGHT, LEFT } T_SIDE ;
typedef struct {
    T_SIDE type_face;
    T_COLOR face[3][3];
    } RUBIKS_SIDE;

char select_color(T_COLOR color);
int side_to_index(T_SIDE side);
char color_to_char(T_COLOR color);

RUBIKS_SIDE* create_rubiks();

void init_rubiks(RUBIKS_SIDE*);
void display_rubiks(RUBIKS_SIDE*);
void blank_rubiks(RUBIKS_SIDE*);
void fill_rubiks(RUBIKS_SIDE*);
void scramble_rubiks(RUBIKS_SIDE*);
void free_rubiks(RUBIKS_SIDE*);

void front_clockwise(RUBIKS_SIDE* rubiscube,int type);
void back_clockwise(RUBIKS_SIDE* rubiscube,int type);
void up_clockwise(RUBIKS_SIDE* rubiscube,int type);
void down_clockwise(RUBIKS_SIDE* rubiscube,int type);
void right_clockwise(RUBIKS_SIDE* rubiscube,int type);
void left_clockwise(RUBIKS_SIDE* rubiscube,int type);

void front_anticlockwise(RUBIKS_SIDE* rubiscube,int type);
void back_anticlockwise(RUBIKS_SIDE* rubiscube,int type);
void up_anticlockwise(RUBIKS_SIDE* rubiscube,int type);
void down_anticlockwise(RUBIKS_SIDE* rubiscube,int type);
void right_anticlockwise(RUBIKS_SIDE* rubiscube,int type);
void left_anticlockwise(RUBIKS_SIDE* rubiscube,int type);

void horizontal_rotation(RUBIKS_SIDE* rubiscube);
void quarter_horizontal_rotation(RUBIKS_SIDE* rubikscube);
void vertical_rotation(RUBIKS_SIDE* rubiscube);
void quarter_vertical_rotation(RUBIKS_SIDE* rubikscube);

void move_rubiks(RUBIKS_SIDE* rubikscube);

void solve_rubiks(RUBIKS_SIDE* rubikscube);
void make_cross(RUBIKS_SIDE* rubikscube);
void corner(RUBIKS_SIDE* rubikscube);
void semi_cross(RUBIKS_SIDE* rubikscube);

void menu();


#endif //PROJET_C_RUBIKS_H
