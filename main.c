#include "rubiks.h"

int main(){
    RUBIKS_SIDE * rubikscube;
    rubikscube = create_rubiks();
    init_rubiks(rubikscube);
    display_rubiks(rubikscube);
    menu(rubikscube);


    return 0;
}