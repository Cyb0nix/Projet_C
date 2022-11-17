#include <stdio.h>
#include "rubiks.h"
#include <windows.h>
#include <time.h>

char select_color(T_COLOR color){
    /*
     * Fonction prenant en paramètre un T_COLOR, retournant le char correspondant et en changant la couleur de
     * la console en fonction d'une T_COLOR donné argument.
     * param color T_COLOR
     * return char
     */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color) {
        case R:
            SetConsoleTextAttribute(H,12);
            return 'R';
        case B:
            SetConsoleTextAttribute(H,9);
            return 'B';
        case G:
            SetConsoleTextAttribute(H,10);
            return 'G';
        case W:
            SetConsoleTextAttribute(H,15);
            return 'W';
        case Y:
            SetConsoleTextAttribute(H,14);
            return 'Y';
        case O:
            SetConsoleTextAttribute(H,13);
            return 'O';
        case LG:
            SetConsoleTextAttribute(H,7);
            return 'X';
        default:
            return ' ';
    }
}

int side_to_index(T_SIDE side){
    /*
     * Fonction retournant l'index correspondant à la Face donné en paramètre
     * param side T_SIDE
     * return int index de la face
     */

    switch (side) {

        case LEFT:
            return 0;
        case UP:
            return 1;
        case FRONT:
            return 2;
        case DOWN:
            return 3;
        case RIGHT:
            return 4;
        case BACK:
            return 5;
        default:
            return 0;
    }
}

RUBIKS_SIDE* create_rubiks(){
    /*
     * Alloue la mémoire pour le rubikscube et définie le type de chaque face
     *
     * return RUBIKS_SIDE pointer
     */
    RUBIKS_SIDE* rubikscube = (RUBIKS_SIDE*) malloc(sizeof(RUBIKS_SIDE)*6);
    rubikscube[0].type_face = LEFT;
    rubikscube[1].type_face = UP;
    rubikscube[2].type_face = FRONT;
    rubikscube[3].type_face = DOWN;
    rubikscube[4].type_face = RIGHT;
    rubikscube[5].type_face = BACK;

    return rubikscube;
}

void init_rubiks(RUBIKS_SIDE* rubikscube){
    /*
     * Fonction Initialisant le rubiks cube en le remplissant chaque face avec la couleur adéquate
     * param rubikscube pointer RUBIKS_SIDE
     */

    T_COLOR val;
    for (int i = 0; i < 6; ++i) {
        // Définie la valeur avec laquel va être remplki la face en fonction du type de face
        switch (rubikscube[i].type_face) {
            case LEFT:
                val = O;
                break;
            case UP:
                val = W;
                break;
            case FRONT:
                val = G;
                break;
            case DOWN:
                val = Y;
                break;
            case RIGHT:
                val = R;
                break;
            case BACK:
                val = B;
                break;
            default:
                break;

        }
        // rempli chaque face avec la valeur définie plutôt
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                rubikscube[i].face[j][k] = val;
            }
        }
    }
}

void display_rubiks(RUBIKS_SIDE * rubikscube){
    /*
     * Fonction affichant sous la forme d'un patron le rubiks cube
     * param rubikscube pointer RUBIKS_SIDE
     */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    T_COLOR display[9][12]; //Création d'un tableau 2D vide affin de stocker le patron à afficher
    T_SIDE face;

    // Initialisation du tableau avec des espace affin de combler les cases que le patron n'occupera pas
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 12; ++j) {
            display[i][j] = ' ';
        }
    }

    // stockage des faces UP et DOWN dans les emplacements correspondant du tableau 2D conteant le patrons
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            display[i][3+j] = rubikscube[side_to_index(UP)].face[i][j];
        }

        for (int j = 0; j < 3; ++j) {
            display[6+i][3+j] = rubikscube[side_to_index(DOWN)].face[i][j];
        }
    }

    // stockage du reste des faces dans les emplacements correspondant du tableau 2D conteant le patrons
    for (int i = 0; i < 4; ++i) {
        switch (i) {
            case 0:
                face = LEFT;
                break;
            case 1:
                face = FRONT;
                break;
            case 2:
                face = RIGHT;
                break;
            case 3:
                face = BACK;
                break;
            default:
                break;
        }
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                display[3+j][i*3+k] = rubikscube[side_to_index(face)].face[j][k];
            }
        }
    }

    //Affichage du tableau contenant le patron du rubiks cube
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 12; ++j) {
            printf(" %c ",select_color(display[i][j]));
            SetConsoleTextAttribute(H,15);

        }
        printf("\n");
    }
    printf("\n");
}

void blank_rubiks(RUBIKS_SIDE* rubikscube){
    /*
     * Fonction vidant les case du rubiks et les remplacants par une case grise
     * param rubikscube pointer RUBIKS_SIDE
     */
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                rubikscube[i].face[j][k] = LG;
            }
        }
    }
}

void fill_rubiks(RUBIKS_SIDE* rubikscube){
    /*
     * est la fonction qui vous permet de remplir un rubik’s cube,
     * vidé préalablement par la fonction « blank_rubiks »,
     * case par case avec le choix de la case a remplir sur la face et
     * avec la couleur que vous voulez. Vos placements de case devront respecter
     * les règles de résolution du rubik’s cube 3x3 pour être appliqué
     */
    int move = 1;
    int face, row, col;
    int cpt_r = 0, cpt_b = 0, cpt_g = 0, cpt_w = 0, cpt_y = 0, cpt_o = 0;
    int cpt_centre_r = 0, cpt_centre_b = 0, cpt_centre_g = 0, cpt_centre_w = 0, cpt_centre_y = 0, cpt_centre_o = 0;
    int cpt_angle_r = 0, cpt_angle_b = 0, cpt_angle_g = 0, cpt_angle_w = 0, cpt_angle_y = 0, cpt_angle_o = 0;
    int cpt_pole_r = 0, cpt_pole_b = 0, cpt_pole_g = 0, cpt_pole_w = 0, cpt_pole_y = 0, cpt_pole_o = 0;
    int val;
    //Vide le rubiks cube(enleve les couleurs du cube)
    blank_rubiks(rubikscube);
    // Boucle pour demander a l'utilisateur de remplir chaque case du tableau
    for(int cpt = 0; cpt < 54; ++cpt){
        for (int i = 0; i < 76; ++i) {
            printf("-");
        }
        printf("\n        Choisissez une face: \n");

        printf("\n");
        //Demande de choisir la face a modifier
        printf("0: Left    1: Up     2: Front    3: Down    4: Right     5: Back     6: Exit\n");
        for (int i = 0; i < 76; ++i) {
            printf("-");
        }
        printf("\nAction: ");
        scanf("%d",&face);
        //Si l'utilisateur choisi de sortir il revient au menu
        if (face == 6){
                display_rubiks(rubikscube);
                move = 0;
                break;
        }
        printf("\n");

        for (int i = 0; i < 51; ++i) {
            printf("-");
        }
        printf("\n        Quelle case voulez vous modifier ?\n");

        printf("\n");
        //Demande de choisir la ligne a modifier sur la face choisi
        printf("0: Ligne 1    1: Ligne 2     2: Ligne 3     3: Exit\n");
        for (int i = 0; i < 51; ++i) {
            printf("-");
        }
        printf("\nAction: ");
        scanf("%d",&row);
        //Si l'utilisateur choisi de sortir il revient au menu
        if (row == 3){
            display_rubiks(rubikscube);
            move = 0;
            break;
        }
        printf("\n");
        //Demande la colonne a modifier sur la face et la ligne choisi
        printf("0: Colonne 1    1: Colonne 2     2: Colonne 3     3: Exit\n");
        for (int i = 0; i < 57; ++i) {
            printf("-");
        }
        printf("\nAction: ");
        scanf("%d",&col);
        //Si l'utilisateur choisi de sortir il revient au menu
        if (col == 3){
            display_rubiks(rubikscube);
            move = 0;
            break;
        }
        for (int i = 0; i < 80; ++i) {
            printf("-");
        }
        printf("\n     Choisissez votre couleur :\n");

        printf("\n");
        //Demande la couleur a modifier sur la case selectionner par les actions precedente
        printf("1: Rouge   2: Bleu     3: Vert     4: Blanc     5: Jaune     6: Rose     7: Exit \n");
        for (int i = 0; i < 80; ++i) {
            printf("-");
        }
        printf("\nAction: ");
        scanf("%d",&val);
        //Si l'utilisateur choisi de sortir il revient au menu
        if (val == 7){
            display_rubiks(rubikscube);
            move = 0;
            break;
        }
        printf("\n");
        //En fonction de la couleur selectionner la "case" correspondante s'active
        switch (val) {
            //Si l'utilisateur choisi 1 les conditions suivantes sont tester
            case 1:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_r >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_r >= 1 && cpt_pole_r < 4){
                        if (rubikscube[face].face[row][col] == R){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_r >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = R;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_r += 1;
                        cpt_r += 1;
                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_r >= 1 && cpt_angle_r < 4) && (rubikscube[l].face[m][n] == R)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_r >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = R;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_r += 1;
                                    cpt_r += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                            //Et si le compteur >= 1, ne place pas la couleur
                            if (cpt_centre_r >= 1){
                                printf("Vous ne pouvez placer cette couleur ici");
                                break;
                            }
                            else {
                                //affiche la couleur a la case correspondante
                                rubikscube[face].face[row][col] = R;
                                //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                cpt_centre_r += 1;
                                cpt_r += 1;
                                break;
                            }
                    }
                }

                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == R)&&(rubikscube[1].face[0][0] == R))||((rubikscube[0].face[0][1] == R)&&(rubikscube[1].face[1][0] == R))||((rubikscube[0].face[0][2] == R)&&(rubikscube[1].face[2][0] == R))||((rubikscube[5].face[0][2] == R)&&(rubikscube[1].face[0][0] == R))||((rubikscube[5].face[0][1] == R)&&(rubikscube[1].face[0][1] == R))||((rubikscube[5].face[0][0] == R)&&(rubikscube[1].face[0][2] == R))||((rubikscube[4].face[0][0] == R)&&(rubikscube[1].face[2][2] == R))||((rubikscube[4].face[0][1] == R)&&(rubikscube[1].face[1][2] == R))||((rubikscube[4].face[0][2] == R)&&(rubikscube[1].face[0][2] == R))||((rubikscube[2].face[0][0] == R)&&(rubikscube[1].face[2][0] == R))||((rubikscube[2].face[0][1] == R)&&(rubikscube[1].face[2][1] == R))||((rubikscube[2].face[0][2] == R)&&(rubikscube[1].face[2][2] == R))||((rubikscube[2].face[0][0] == R)&&(rubikscube[0].face[0][2] == R))||((rubikscube[2].face[1][0] == R)&&(rubikscube[0].face[1][2] == R))||((rubikscube[2].face[2][0] == R)&&(rubikscube[0].face[2][2] == R))||((rubikscube[2].face[2][0] == R)&&(rubikscube[3].face[0][0] == R))||((rubikscube[2].face[2][1] == R)&&(rubikscube[3].face[0][1] == R))||((rubikscube[2].face[2][2] == R)&&(rubikscube[3].face[0][2] == R))||((rubikscube[2].face[0][2] == R)&&(rubikscube[4].face[0][0] == R))||((rubikscube[2].face[1][2] == R)&&(rubikscube[4].face[1][0] == R))||((rubikscube[2].face[2][2] == R)&&(rubikscube[4].face[2][0] == R))||((rubikscube[3].face[0][0] == R)&&(rubikscube[0].face[2][2] == R))||((rubikscube[3].face[1][0] == R)&&(rubikscube[0].face[2][1] == R))||((rubikscube[3].face[2][0] == R)&&(rubikscube[0].face[2][0] == R))||((rubikscube[3].face[0][2] == R)&&(rubikscube[4].face[2][0] == R))||((rubikscube[3].face[1][2] == R)&&(rubikscube[4].face[1][0] == R))||((rubikscube[3].face[2][2] == R)&&(rubikscube[4].face[2][0] == R))||((rubikscube[3].face[2][0] == R)&&(rubikscube[5].face[2][2] == R))||((rubikscube[3].face[2][1] == R)&&(rubikscube[5].face[2][1] == R))||((rubikscube[3].face[2][2] == R)&&(rubikscube[5].face[2][0] == R))||((rubikscube[5].face[0][0] == R)&&(rubikscube[4].face[0][2] == R))||((rubikscube[5].face[1][0] == R)&&(rubikscube[4].face[1][2] == R))||((rubikscube[5].face[2][0] == R)&&(rubikscube[4].face[2][2] == R))||((rubikscube[5].face[0][2] == R)&&(rubikscube[0].face[0][0] == R))||((rubikscube[5].face[1][2] == R)&&(rubikscube[0].face[1][0] == R))||((rubikscube[5].face[2][2] == R)&&(rubikscube[0].face[2][0] == R))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
            //Si l'utilisateur choisi 2 les conditions suivantes sont tester
            case 2:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_b >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_b >= 1 && cpt_pole_b < 4){
                        if (rubikscube[face].face[row][col] == B){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_b >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = B;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_b += 1;
                        cpt_b += 1;


                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_b >= 1 && cpt_angle_b < 4) && (rubikscube[l].face[m][n] == B)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_b >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = B;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_b += 1;
                                    cpt_b += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                        //Et si le compteur >= 1, ne place pas la couleur
                        if (cpt_centre_b >= 1){
                            printf("Vous ne pouvez placer cette couleur ici");
                            break;
                        }
                        else {
                            //affiche la couleur a la case correspondante
                            rubikscube[face].face[row][col] = B;
                            //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                            cpt_centre_b += 1;
                            cpt_b += 1;
                            break;
                        }
                    }
                }
                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == B)&&(rubikscube[1].face[0][0] == B))||((rubikscube[0].face[0][1] == B)&&(rubikscube[1].face[1][0] == B))||((rubikscube[0].face[0][2] == B)&&(rubikscube[1].face[2][0] == B))||((rubikscube[5].face[0][2] == B)&&(rubikscube[1].face[0][0] == B))||((rubikscube[5].face[0][1] == B)&&(rubikscube[1].face[0][1] == B))||((rubikscube[5].face[0][0] == B)&&(rubikscube[1].face[0][2] == B))||((rubikscube[4].face[0][0] == B)&&(rubikscube[1].face[2][2] == B))||((rubikscube[4].face[0][1] == B)&&(rubikscube[1].face[1][2] == B))||((rubikscube[4].face[0][2] == B)&&(rubikscube[1].face[0][2] == B))||((rubikscube[2].face[0][0] == B)&&(rubikscube[1].face[2][0] == B))||((rubikscube[2].face[0][1] == B)&&(rubikscube[1].face[2][1] == B))||((rubikscube[2].face[0][2] == B)&&(rubikscube[1].face[2][2] == B))||((rubikscube[2].face[0][0] == B)&&(rubikscube[0].face[0][2] == B))||((rubikscube[2].face[1][0] == B)&&(rubikscube[0].face[1][2] == B))||((rubikscube[2].face[2][0] == B)&&(rubikscube[0].face[2][2] == B))||((rubikscube[2].face[2][0] == B)&&(rubikscube[3].face[0][0] == B))||((rubikscube[2].face[2][1] == B)&&(rubikscube[3].face[0][1] == B))||((rubikscube[2].face[2][2] == B)&&(rubikscube[3].face[0][2] == B))||((rubikscube[2].face[0][2] == B)&&(rubikscube[4].face[0][0] == B))||((rubikscube[2].face[1][2] == B)&&(rubikscube[4].face[1][0] == B))||((rubikscube[2].face[2][2] == B)&&(rubikscube[4].face[2][0] == B))||((rubikscube[3].face[0][0] == B)&&(rubikscube[0].face[2][2] == B))||((rubikscube[3].face[1][0] == B)&&(rubikscube[0].face[2][1] == B))||((rubikscube[3].face[2][0] == B)&&(rubikscube[0].face[2][0] == B))||((rubikscube[3].face[0][2] == B)&&(rubikscube[4].face[2][0] == B))||((rubikscube[3].face[1][2] == B)&&(rubikscube[4].face[1][0] == B))||((rubikscube[3].face[2][2] == B)&&(rubikscube[4].face[2][0] == B))||((rubikscube[3].face[2][0] == B)&&(rubikscube[5].face[2][2] == B))||((rubikscube[3].face[2][1] == B)&&(rubikscube[5].face[2][1] == B))||((rubikscube[3].face[2][2] == B)&&(rubikscube[5].face[2][0] == B))||((rubikscube[5].face[0][0] == B)&&(rubikscube[4].face[0][2] == B))||((rubikscube[5].face[1][0] == B)&&(rubikscube[4].face[1][2] == B))||((rubikscube[5].face[2][0] == B)&&(rubikscube[4].face[2][2] == B))||((rubikscube[5].face[0][2] == B)&&(rubikscube[0].face[0][0] == B))||((rubikscube[5].face[1][2] == B)&&(rubikscube[0].face[1][0] == B))||((rubikscube[5].face[2][2] == B)&&(rubikscube[0].face[2][0] == B))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
            case 3:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_g >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_g >= 1 && cpt_pole_g < 4){
                        if (rubikscube[face].face[row][col] == G){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_g >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = G;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_g += 1;
                        cpt_g += 1;


                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_g >= 1 && cpt_angle_g < 4) && (rubikscube[l].face[m][n] == G)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_g >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = G;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_g += 1;
                                    cpt_g += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                        //Et si le compteur >= 1, ne place pas la couleur
                        if (cpt_centre_g >= 1){
                            printf("Vous ne pouvez placer cette couleur ici");
                            break;
                        }
                        else {
                            //affiche la couleur a la case correspondante
                            rubikscube[face].face[row][col] = G;
                            //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                            cpt_centre_g += 1;
                            cpt_g += 1;
                            break;

                        }
                    }
                }
                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == G)&&(rubikscube[1].face[0][0] == G))||((rubikscube[0].face[0][1] == G)&&(rubikscube[1].face[1][0] == G))||((rubikscube[0].face[0][2] == G)&&(rubikscube[1].face[2][0] == G))||((rubikscube[5].face[0][2] == G)&&(rubikscube[1].face[0][0] == G))||((rubikscube[5].face[0][1] == G)&&(rubikscube[1].face[0][1] == G))||((rubikscube[5].face[0][0] == G)&&(rubikscube[1].face[0][2] == G))||((rubikscube[4].face[0][0] == G)&&(rubikscube[1].face[2][2] == G))||((rubikscube[4].face[0][1] == G)&&(rubikscube[1].face[1][2] == G))||((rubikscube[4].face[0][2] == G)&&(rubikscube[1].face[0][2] == G))||((rubikscube[2].face[0][0] == G)&&(rubikscube[1].face[2][0] == G))||((rubikscube[2].face[0][1] == G)&&(rubikscube[1].face[2][1] == G))||((rubikscube[2].face[0][2] == G)&&(rubikscube[1].face[2][2] == G))||((rubikscube[2].face[0][0] == G)&&(rubikscube[0].face[0][2] == G))||((rubikscube[2].face[1][0] == G)&&(rubikscube[0].face[1][2] == G))||((rubikscube[2].face[2][0] == G)&&(rubikscube[0].face[2][2] == G))||((rubikscube[2].face[2][0] == G)&&(rubikscube[3].face[0][0] == G))||((rubikscube[2].face[2][1] == G)&&(rubikscube[3].face[0][1] == G))||((rubikscube[2].face[2][2] == G)&&(rubikscube[3].face[0][2] == G))||((rubikscube[2].face[0][2] == G)&&(rubikscube[4].face[0][0] == G))||((rubikscube[2].face[1][2] == G)&&(rubikscube[4].face[1][0] == G))||((rubikscube[2].face[2][2] == G)&&(rubikscube[4].face[2][0] == G))||((rubikscube[3].face[0][0] == G)&&(rubikscube[0].face[2][2] == G))||((rubikscube[3].face[1][0] == G)&&(rubikscube[0].face[2][1] == G))||((rubikscube[3].face[2][0] == G)&&(rubikscube[0].face[2][0] == G))||((rubikscube[3].face[0][2] == G)&&(rubikscube[4].face[2][0] == G))||((rubikscube[3].face[1][2] == G)&&(rubikscube[4].face[1][0] == G))||((rubikscube[3].face[2][2] == G)&&(rubikscube[4].face[2][0] == G))||((rubikscube[3].face[2][0] == G)&&(rubikscube[5].face[2][2] == G))||((rubikscube[3].face[2][1] == G)&&(rubikscube[5].face[2][1] == G))||((rubikscube[3].face[2][2] == G)&&(rubikscube[5].face[2][0] == G))||((rubikscube[5].face[0][0] == G)&&(rubikscube[4].face[0][2] == G))||((rubikscube[5].face[1][0] == G)&&(rubikscube[4].face[1][2] == G))||((rubikscube[5].face[2][0] == G)&&(rubikscube[4].face[2][2] == G))||((rubikscube[5].face[0][2] == G)&&(rubikscube[0].face[0][0] == G))||((rubikscube[5].face[1][2] == G)&&(rubikscube[0].face[1][0] == G))||((rubikscube[5].face[2][2] == G)&&(rubikscube[0].face[2][0] == G))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
            case 4:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_w >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_w >= 1 && cpt_pole_w < 4){
                        if (rubikscube[face].face[row][col] == W){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_w >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = W;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_w += 1;
                        cpt_w += 1;


                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_w >= 1 && cpt_angle_w < 4) && (rubikscube[l].face[m][n] == W)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_w >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = W;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_w += 1;
                                    cpt_w += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                        //Et si le compteur >= 1, ne place pas la couleur
                        if (cpt_centre_w >= 1){
                            printf("Vous ne pouvez placer cette couleur ici");
                            break;
                        }
                        else {
                            //affiche la couleur a la case correspondante
                            rubikscube[face].face[row][col] = W;
                            //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                            cpt_centre_w += 1;
                            cpt_w += 1;
                            break;

                        }
                    }
                }
                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == W)&&(rubikscube[1].face[0][0] == W))||((rubikscube[0].face[0][1] == W)&&(rubikscube[1].face[1][0] == W))||((rubikscube[0].face[0][2] == W)&&(rubikscube[1].face[2][0] == W))||((rubikscube[5].face[0][2] == W)&&(rubikscube[1].face[0][0] == W))||((rubikscube[5].face[0][1] == W)&&(rubikscube[1].face[0][1] == W))||((rubikscube[5].face[0][0] == W)&&(rubikscube[1].face[0][2] == W))||((rubikscube[4].face[0][0] == W)&&(rubikscube[1].face[2][2] == W))||((rubikscube[4].face[0][1] == W)&&(rubikscube[1].face[1][2] == W))||((rubikscube[4].face[0][2] == W)&&(rubikscube[1].face[0][2] == W))||((rubikscube[2].face[0][0] == W)&&(rubikscube[1].face[2][0] == W))||((rubikscube[2].face[0][1] == W)&&(rubikscube[1].face[2][1] == W))||((rubikscube[2].face[0][2] == W)&&(rubikscube[1].face[2][2] == W))||((rubikscube[2].face[0][0] == W)&&(rubikscube[0].face[0][2] == W))||((rubikscube[2].face[1][0] == W)&&(rubikscube[0].face[1][2] == W))||((rubikscube[2].face[2][0] == W)&&(rubikscube[0].face[2][2] == W))||((rubikscube[2].face[2][0] == W)&&(rubikscube[3].face[0][0] == W))||((rubikscube[2].face[2][1] == W)&&(rubikscube[3].face[0][1] == W))||((rubikscube[2].face[2][2] == W)&&(rubikscube[3].face[0][2] == W))||((rubikscube[2].face[0][2] == W)&&(rubikscube[4].face[0][0] == W))||((rubikscube[2].face[1][2] == W)&&(rubikscube[4].face[1][0] == W))||((rubikscube[2].face[2][2] == W)&&(rubikscube[4].face[2][0] == W))||((rubikscube[3].face[0][0] == W)&&(rubikscube[0].face[2][2] == W))||((rubikscube[3].face[1][0] == W)&&(rubikscube[0].face[2][1] == W))||((rubikscube[3].face[2][0] == W)&&(rubikscube[0].face[2][0] == W))||((rubikscube[3].face[0][2] == W)&&(rubikscube[4].face[2][0] == W))||((rubikscube[3].face[1][2] == W)&&(rubikscube[4].face[1][0] == W))||((rubikscube[3].face[2][2] == W)&&(rubikscube[4].face[2][0] == W))||((rubikscube[3].face[2][0] == W)&&(rubikscube[5].face[2][2] == W))||((rubikscube[3].face[2][1] == W)&&(rubikscube[5].face[2][1] == W))||((rubikscube[3].face[2][2] == W)&&(rubikscube[5].face[2][0] == W))||((rubikscube[5].face[0][0] == W)&&(rubikscube[4].face[0][2] == W))||((rubikscube[5].face[1][0] == W)&&(rubikscube[4].face[1][2] == W))||((rubikscube[5].face[2][0] == W)&&(rubikscube[4].face[2][2] == W))||((rubikscube[5].face[0][2] == W)&&(rubikscube[0].face[0][0] == W))||((rubikscube[5].face[1][2] == W)&&(rubikscube[0].face[1][0] == W))||((rubikscube[5].face[2][2] == W)&&(rubikscube[0].face[2][0] == W))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
            case 5:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_y >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_y >= 1 && cpt_pole_y < 4){
                        if (rubikscube[face].face[row][col] == Y){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_y >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = Y;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_y += 1;
                        cpt_y += 1;


                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_y >= 1 && cpt_angle_y < 4) && (rubikscube[l].face[m][n] == Y)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_y >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = Y;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_y += 1;
                                    cpt_y += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                        //Et si le compteur >= 1, ne place pas la couleur
                        if (cpt_centre_y >= 1){
                            printf("Vous ne pouvez placer cette couleur ici");
                            break;
                        }
                        else {
                            //affiche la couleur a la case correspondante
                            rubikscube[face].face[row][col] = Y;
                            //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                            cpt_centre_y += 1;
                            cpt_y += 1;
                            break;

                        }
                    }
                }
                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == Y)&&(rubikscube[1].face[0][0] == Y))||((rubikscube[0].face[0][1] == Y)&&(rubikscube[1].face[1][0] == Y))||((rubikscube[0].face[0][2] == Y)&&(rubikscube[1].face[2][0] == Y))||((rubikscube[5].face[0][2] == Y)&&(rubikscube[1].face[0][0] == Y))||((rubikscube[5].face[0][1] == Y)&&(rubikscube[1].face[0][1] == Y))||((rubikscube[5].face[0][0] == Y)&&(rubikscube[1].face[0][2] == Y))||((rubikscube[4].face[0][0] == Y)&&(rubikscube[1].face[2][2] == Y))||((rubikscube[4].face[0][1] == Y)&&(rubikscube[1].face[1][2] == Y))||((rubikscube[4].face[0][2] == Y)&&(rubikscube[1].face[0][2] == Y))||((rubikscube[2].face[0][0] == Y)&&(rubikscube[1].face[2][0] == Y))||((rubikscube[2].face[0][1] == Y)&&(rubikscube[1].face[2][1] == Y))||((rubikscube[2].face[0][2] == Y)&&(rubikscube[1].face[2][2] == Y))||((rubikscube[2].face[0][0] == Y)&&(rubikscube[0].face[0][2] == Y))||((rubikscube[2].face[1][0] == Y)&&(rubikscube[0].face[1][2] == Y))||((rubikscube[2].face[2][0] == Y)&&(rubikscube[0].face[2][2] == Y))||((rubikscube[2].face[2][0] == Y)&&(rubikscube[3].face[0][0] == Y))||((rubikscube[2].face[2][1] == Y)&&(rubikscube[3].face[0][1] == Y))||((rubikscube[2].face[2][2] == Y)&&(rubikscube[3].face[0][2] == Y))||((rubikscube[2].face[0][2] == Y)&&(rubikscube[4].face[0][0] == Y))||((rubikscube[2].face[1][2] == Y)&&(rubikscube[4].face[1][0] == Y))||((rubikscube[2].face[2][2] == Y)&&(rubikscube[4].face[2][0] == Y))||((rubikscube[3].face[0][0] == Y)&&(rubikscube[0].face[2][2] == Y))||((rubikscube[3].face[1][0] == Y)&&(rubikscube[0].face[2][1] == Y))||((rubikscube[3].face[2][0] == Y)&&(rubikscube[0].face[2][0] == Y))||((rubikscube[3].face[0][2] == Y)&&(rubikscube[4].face[2][0] == Y))||((rubikscube[3].face[1][2] == Y)&&(rubikscube[4].face[1][0] == Y))||((rubikscube[3].face[2][2] == Y)&&(rubikscube[4].face[2][0] == Y))||((rubikscube[3].face[2][0] == Y)&&(rubikscube[5].face[2][2] == Y))||((rubikscube[3].face[2][1] == Y)&&(rubikscube[5].face[2][1] == Y))||((rubikscube[3].face[2][2] == Y)&&(rubikscube[5].face[2][0] == Y))||((rubikscube[5].face[0][0] == Y)&&(rubikscube[4].face[0][2] == Y))||((rubikscube[5].face[1][0] == Y)&&(rubikscube[4].face[1][2] == Y))||((rubikscube[5].face[2][0] == Y)&&(rubikscube[4].face[2][2] == Y))||((rubikscube[5].face[0][2] == Y)&&(rubikscube[0].face[0][0] == Y))||((rubikscube[5].face[1][2] == Y)&&(rubikscube[0].face[1][0] == Y))||((rubikscube[5].face[2][2] == Y)&&(rubikscube[0].face[2][0] == Y))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
            case 6:
                //Si le compteur du nombre de face de cette couleur arrive a 9, la couleur ne sera pas appliquer
                if (cpt_o >= 9){
                    printf("Vous ne pouvez pas placer plus de 9 fois cette couleur !\n");
                    break;
                }
                //Si la case choisi ce trouve au pole d'une des faces (cad les cases du centre de chaque arrete) alors ...
                if (((0 == face) && (0 == row) && (1 == col)) || ((0 == face) && (1 == row) && (0 == col)) || ((0 == face) && (1 == row) && (2 == col)) || ((0 == face) && (2 == row) && (1 == col)) || ((1 == face) && (0 == row) && (1 == col)) || ((1 == face) && (1 == row) && (0 == col)) || ((1 == face) && (1 == row) && (2 == col)) || ((1 == face) && (2 == row) && (1 == col)) || ((2 == face) && (0 == row) && (1 == col)) || ((2 == face) && (1 == row) && (0 == col)) || ((2 == face) && (1 == row) && (2 == col)) || ((2 == face) && (2 == row) && (1 == col)) || ((3 == face) && (0 == row) && (1 == col)) || ((3 == face) && (1 == row) && (0 == col)) || ((3 == face) && (1 == row) && (2 == col)) || ((3 == face) && (2 == row) && (1 == col)) || ((4 == face) && (0 == row) && (1 == col)) || ((4 == face) && (1 == row) && (0 == col)) || ((4 == face) && (1 == row) && (2 == col)) || ((4 == face) && (2 == row) && (1 == col)) || ((5 == face) && (0 == row) && (1 == col)) || ((5 == face) && (1 == row) && (0 == col)) || ((5 == face) && (1 == row) && (2 == col)) || ((5 == face) && (2 == row) && (1 == col))){
                    //Si le compteur du pole est entre 1 et 3 et que la case choisi est deja de la couleur choisi alors un message indique que la couleur est deja presente et quitte le switch
                    if (cpt_pole_o >= 1 && cpt_pole_o < 4){
                        if (rubikscube[face].face[row][col] == O){
                            printf("Tu as deja place cette couleur a cette case !\n");
                            break;
                        }
                    }
                    //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                    if (cpt_pole_o >= 4) {
                        printf("Il ne peut y avoir cette couleur sur les poles que 4 fois !\n");
                        break;
                    }
                    else {
                        //Applique la couleur au coordonner choisi
                        rubikscube[face].face[row][col] = O;
                        //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                        cpt_pole_o += 1;
                        cpt_o += 1;


                    }
                }
                //Pour les cases ce trouvant sur les cotes de chaque face
                for (int l=0; l<6; ++l) {
                    for (int m = 0; m < 3; m += 2) {
                        for (int n = 0; n < 3; n += 2) {
                            //Si la case choisi ce trouve sur un coté d'une face
                            if ((l == face) && (m == row) && (col == n)){
                                //Et si le cpt est entre 1 et 3 et que la case choisi est deja de cette couleur alors affiche le message et sors du switch
                                if ((cpt_angle_y >= 1 && cpt_angle_y < 4) && (rubikscube[l].face[m][n] == O)){
                                    printf("Tu as deja place cette couleur a cette case !\n");
                                    break;
                                }
                                //Si compteur plus grand ou egale a 4, supprime la couleur appliquer et affiche le message d'alerte et sors du switch
                                if (cpt_angle_y >= 4){
                                    printf("Il ne peut y avoir cette couleur sur les cotes que 4 fois !\n");
                                    break;
                                }
                                else{
                                    //Applique la couleur au coordonner choisi
                                    rubikscube[face].face[row][col] = O;
                                    //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                                    cpt_angle_o += 1;
                                    cpt_o += 1;

                                }

                            }
                        }
                    }
                }
                //Pour chaque face
                for (int l=0; l<6; ++l){
                    //Si la ligne et la colonne valent toutes les deux 1 (donc que c'est au centre d'une face)
                    if ((row == 1)&&(col == 1)) {
                        //Et si le compteur >= 1, ne place pas la couleur
                        if (cpt_centre_y >= 1){
                            printf("Vous ne pouvez placer cette couleur ici");
                            break;
                        }
                        else {
                            //affiche la couleur a la case correspondante
                            rubikscube[face].face[row][col] = O;
                            //Ajoute 1 aux compteurs si les conditions au dessus ne sont pas effectuer
                            cpt_centre_o += 1;
                            cpt_o += 1;
                            break;

                        }
                    }
                }
                //Si la case choisi a une case adjacente sur une autre face de la meme couleur alors, supprime la couleur de la case choisi et affiche le message d'alerte
                if (((rubikscube[0].face[0][0] == O)&&(rubikscube[1].face[0][0] == O))||((rubikscube[0].face[0][1] == O)&&(rubikscube[1].face[1][0] == O))||((rubikscube[0].face[0][2] == O)&&(rubikscube[1].face[2][0] == O))||((rubikscube[5].face[0][2] == O)&&(rubikscube[1].face[0][0] == O))||((rubikscube[5].face[0][1] == O)&&(rubikscube[1].face[0][1] == O))||((rubikscube[5].face[0][0] == O)&&(rubikscube[1].face[0][2] == O))||((rubikscube[4].face[0][0] == O)&&(rubikscube[1].face[2][2] == O))||((rubikscube[4].face[0][1] == O)&&(rubikscube[1].face[1][2] == O))||((rubikscube[4].face[0][2] == O)&&(rubikscube[1].face[0][2] == O))||((rubikscube[2].face[0][0] == O)&&(rubikscube[1].face[2][0] == O))||((rubikscube[2].face[0][1] == O)&&(rubikscube[1].face[2][1] == O))||((rubikscube[2].face[0][2] == O)&&(rubikscube[1].face[2][2] == O))||((rubikscube[2].face[0][0] == O)&&(rubikscube[0].face[0][2] == O))||((rubikscube[2].face[1][0] == O)&&(rubikscube[0].face[1][2] == O))||((rubikscube[2].face[2][0] == O)&&(rubikscube[0].face[2][2] == O))||((rubikscube[2].face[2][0] == O)&&(rubikscube[3].face[0][0] == O))||((rubikscube[2].face[2][1] == O)&&(rubikscube[3].face[0][1] == O))||((rubikscube[2].face[2][2] == O)&&(rubikscube[3].face[0][2] == O))||((rubikscube[2].face[0][2] == O)&&(rubikscube[4].face[0][0] == O))||((rubikscube[2].face[1][2] == O)&&(rubikscube[4].face[1][0] == O))||((rubikscube[2].face[2][2] == O)&&(rubikscube[4].face[2][0] == O))||((rubikscube[3].face[0][0] == O)&&(rubikscube[0].face[2][2] == O))||((rubikscube[3].face[1][0] == O)&&(rubikscube[0].face[2][1] == O))||((rubikscube[3].face[2][0] == O)&&(rubikscube[0].face[2][0] == O))||((rubikscube[3].face[0][2] == O)&&(rubikscube[4].face[2][0] == O))||((rubikscube[3].face[1][2] == O)&&(rubikscube[4].face[1][0] == O))||((rubikscube[3].face[2][2] == O)&&(rubikscube[4].face[2][0] == O))||((rubikscube[3].face[2][0] == O)&&(rubikscube[5].face[2][2] == O))||((rubikscube[3].face[2][1] == O)&&(rubikscube[5].face[2][1] == O))||((rubikscube[3].face[2][2] == O)&&(rubikscube[5].face[2][0] == O))||((rubikscube[5].face[0][0] == O)&&(rubikscube[4].face[0][2] == O))||((rubikscube[5].face[1][0] == O)&&(rubikscube[4].face[1][2] == O))||((rubikscube[5].face[2][0] == O)&&(rubikscube[4].face[2][2] == O))||((rubikscube[5].face[0][2] == O)&&(rubikscube[0].face[0][0] == O))||((rubikscube[5].face[1][2] == O)&&(rubikscube[0].face[1][0] == O))||((rubikscube[5].face[2][2] == O)&&(rubikscube[0].face[2][0] == O))){
                    rubikscube[face].face[row][col] = LG;
                    printf("Deux cases adjacentes ne doivent pas avoir la meme couleur !\n");
                    break;
                }
                //sors du switch
                break;
        }
        //Met un espace et affiche la modification apporter ou non sur le cube
        printf("\n");
        display_rubiks(rubikscube);
    }
}

void scramble_rubiks(RUBIKS_SIDE* rubikscube){
    /*
     * Fonction effectuant des mouvements aléatoires sur le rubiks cube afin de le mélanger
     * param rubikscube pointer RUBIKS_SIDE
     */
    srand(time(NULL)); //Choisi alétoirement un nombre de mouvement allant être effectué
    int loop = rand();

    //Effectue un nombre d'action choisi aléatoirement
    for (int i = 0; i < loop; ++i) {

        // choisie un nombre aléatoire afin de choisir une action a effectuer de manière aléatoire

        int move = rand();
        do {
            move = move/10;
        }while (move > 11);
        switch (move) {
            case 0:
                front_clockwise(rubikscube,1);
                break;
            case 1:
                front_anticlockwise(rubikscube,1);
                break;
            case 2:
                back_clockwise(rubikscube,1);
                break;
            case 3:
                back_anticlockwise(rubikscube,1);
                break;
            case 4:
                left_clockwise(rubikscube,1);
                break;
            case 5:
                left_anticlockwise(rubikscube,1);
                break;
            case 6:
                right_clockwise(rubikscube,1);
                break;
            case 7:
                right_anticlockwise(rubikscube,1);
                break;
            case 8:
                up_clockwise(rubikscube,1);
                break;
            case 9:
                up_anticlockwise(rubikscube,1);
                break;
            case 10:
                down_clockwise(rubikscube,1);
                break;
            case 11:
                down_anticlockwise(rubikscube,1);
                break;
        }
    }
}

void free_rubiks(RUBIKS_SIDE* rubikscube){
    //Libère la mémoire alloué au rubiks
    free(rubikscube);
}

void front_clockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face front dans le sens horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(LEFT)].face[i][2];
        rubikscube[side_to_index(LEFT)].face[i][2] = rubikscube[side_to_index(DOWN)].face[0][i];
        rubikscube[side_to_index(DOWN)].face[0][i]= rubikscube[side_to_index(RIGHT)].face[2-i][0];
        rubikscube[side_to_index(RIGHT)].face[2-i][0] = rubikscube[side_to_index(UP)].face[2][2-i];
        rubikscube[side_to_index(UP)].face[2][2-i] = tmp;
    }

    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(FRONT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(FRONT)].face[j][2-i] = tmp_face[i][j];
        }
    }

    if (type == 2){
        type = 1;
        front_clockwise(rubikscube,type);
    }
}

void back_clockwise(RUBIKS_SIDE* rubikscube,int type){
    // Effectue une rotation de la face back dans le sens anti horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(LEFT)].face[2-i][0];
        rubikscube[side_to_index(LEFT)].face[2-i][0] = rubikscube[side_to_index(UP)].face[0][i];
        rubikscube[side_to_index(UP)].face[0][i]= rubikscube[side_to_index(RIGHT)].face[i][2];
        rubikscube[side_to_index(RIGHT)].face[i][2] = rubikscube[side_to_index(DOWN)].face[2][2-i];
        rubikscube[side_to_index(DOWN)].face[2][2-i] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(BACK)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(BACK)].face[j][2-i] = tmp_face[i][j];
        }
    }
    if (type == 2){
        type = 1;
        back_clockwise(rubikscube,type);
    }

}

void right_clockwise(RUBIKS_SIDE* rubikscube,int type){
    //Effectue une rotation de la face right dans le sens horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(FRONT)].face[i][2];
        rubikscube[side_to_index(FRONT)].face[i][2] = rubikscube[side_to_index(DOWN)].face[i][2];
        rubikscube[side_to_index(DOWN)].face[i][2]= rubikscube[side_to_index(BACK)].face[2-i][0];
        rubikscube[side_to_index(BACK)].face[2-i][0] = rubikscube[side_to_index(UP)].face[i][2];
        rubikscube[side_to_index(UP)].face[i][2] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(RIGHT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(RIGHT)].face[j][2-i] = tmp_face[i][j];
        }
    }
    if (type == 2){
        type = 1;
        right_clockwise(rubikscube,type);
    }
}

void left_clockwise(RUBIKS_SIDE* rubikscube,int type){
    //Effectue une rotation de la face left dans le sens horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[i][2];
        rubikscube[side_to_index(BACK)].face[i][2] = rubikscube[side_to_index(DOWN)].face[2-i][0];
        rubikscube[side_to_index(DOWN)].face[2-i][0]= rubikscube[side_to_index(FRONT)].face[2-i][0];
        rubikscube[side_to_index(FRONT)].face[2-i][0] = rubikscube[side_to_index(UP)].face[2-i][0];
        rubikscube[side_to_index(UP)].face[2-i][0] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(LEFT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(LEFT)].face[j][2-i] = tmp_face[i][j];
        }
    }
    if (type == 2){
        type = 1;
        left_clockwise(rubikscube,type);
    }
}

void up_clockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face up dans le sens horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[0][i];
        rubikscube[side_to_index(BACK)].face[0][i] = rubikscube[side_to_index(LEFT)].face[0][i];
        rubikscube[side_to_index(LEFT)].face[0][i]= rubikscube[side_to_index(FRONT)].face[0][i];
        rubikscube[side_to_index(FRONT)].face[0][i] = rubikscube[side_to_index(RIGHT)].face[0][i];
        rubikscube[side_to_index(RIGHT)].face[0][i] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(UP)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(UP)].face[j][2-i] = tmp_face[i][j];
        }
    }
    if (type == 2){
        type = 1;
        up_clockwise(rubikscube,type);
    }
}

void down_clockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face Down dans le sens horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[2][2-i];
        rubikscube[side_to_index(BACK)].face[2][2-i] = rubikscube[side_to_index(RIGHT)].face[2][2-i];
        rubikscube[side_to_index(RIGHT)].face[2][2-i]= rubikscube[side_to_index(FRONT)].face[2][2-i];
        rubikscube[side_to_index(FRONT)].face[2][2-i] = rubikscube[side_to_index(LEFT)].face[2][2-i];
        rubikscube[side_to_index(LEFT)].face[2][2-i] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(DOWN)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(DOWN)].face[j][2-i] = tmp_face[i][j];
        }
    }
    if (type == 2){
        type = 1;
        down_clockwise(rubikscube,type);
    }
}


void front_anticlockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face Front dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(RIGHT)].face[2-i][0];
        rubikscube[side_to_index(RIGHT)].face[2-i][0] = rubikscube[side_to_index(DOWN)].face[0][i];
        rubikscube[side_to_index(DOWN)].face[0][i]= rubikscube[side_to_index(LEFT)].face[i][2];
        rubikscube[side_to_index(LEFT)].face[i][2] = rubikscube[side_to_index(UP)].face[2][2-i];
        rubikscube[side_to_index(UP)].face[2][2-i] = tmp;
    }

    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(FRONT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(FRONT)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        front_anticlockwise(rubikscube,type);
    }
}

void back_anticlockwise(RUBIKS_SIDE* rubikscube,int type){
    //Effectue une rotation de la face BACK dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(LEFT)].face[i][0];
        rubikscube[side_to_index(LEFT)].face[i][0] = rubikscube[side_to_index(DOWN)].face[2][i];
        rubikscube[side_to_index(DOWN)].face[2][i]= rubikscube[side_to_index(RIGHT)].face[2-i][2];
        rubikscube[side_to_index(RIGHT)].face[2-i][2] = rubikscube[side_to_index(UP)].face[0][2-i];
        rubikscube[side_to_index(UP)].face[0][2-i] = tmp;
    }

    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(BACK)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(BACK)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        back_anticlockwise(rubikscube,type);
    }

}

void left_anticlockwise(RUBIKS_SIDE* rubikscube,int type){
    //Effectue une rotation de la face LEFT dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[2-i][2];
        rubikscube[side_to_index(BACK)].face[2-i][2] = rubikscube[side_to_index(UP)].face[i][0];
        rubikscube[side_to_index(UP)].face[i][0]= rubikscube[side_to_index(FRONT)].face[i][0];
        rubikscube[side_to_index(FRONT)].face[i][0] = rubikscube[side_to_index(DOWN)].face[i][0];
        rubikscube[side_to_index(DOWN)].face[i][0] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(LEFT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(LEFT)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        left_anticlockwise(rubikscube,type);
    }
}

void right_anticlockwise(RUBIKS_SIDE* rubikscube,int type){
    //Effectue une rotation de la face Right dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(FRONT)].face[2-i][2];
        rubikscube[side_to_index(FRONT)].face[2-i][2] = rubikscube[side_to_index(UP)].face[2-i][2];
        rubikscube[side_to_index(UP)].face[2-i][2]= rubikscube[side_to_index(BACK)].face[i][0];
        rubikscube[side_to_index(BACK)].face[i][0] = rubikscube[side_to_index(DOWN)].face[2-i][2];
        rubikscube[side_to_index(DOWN)].face[2-i][2] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(RIGHT)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(RIGHT)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        right_anticlockwise(rubikscube,type);
    }
}

void up_anticlockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face up dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[0][2-i];
        rubikscube[side_to_index(BACK)].face[0][2-i] = rubikscube[side_to_index(RIGHT)].face[0][2-i];
        rubikscube[side_to_index(RIGHT)].face[0][2-i]= rubikscube[side_to_index(FRONT)].face[0][2-i];
        rubikscube[side_to_index(FRONT)].face[0][2-i] = rubikscube[side_to_index(LEFT)].face[0][2-i];
        rubikscube[side_to_index(LEFT)].face[0][2-i] = tmp;
    }
    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(UP)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(UP)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        up_anticlockwise(rubikscube,type);
    }
}

void down_anticlockwise(RUBIKS_SIDE* rubikscube, int type){
    //Effectue une rotation de la face Down dans le sens anti-horaire
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[2][i];
        rubikscube[side_to_index(BACK)].face[2][i] = rubikscube[side_to_index(LEFT)].face[2][i];
        rubikscube[side_to_index(LEFT)].face[2][i]= rubikscube[side_to_index(FRONT)].face[2][i];
        rubikscube[side_to_index(FRONT)].face[2][i] = rubikscube[side_to_index(RIGHT)].face[2][i];
        rubikscube[side_to_index(RIGHT)].face[2][i] = tmp;
    }

    T_COLOR tmp_face[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp_face[i][j] = rubikscube[side_to_index(DOWN)].face[i][j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rubikscube[side_to_index(DOWN)].face[i][j] = tmp_face[j][2-i];
        }
    }
    if (type == 2){
        type = 1;
        down_anticlockwise(rubikscube,type);
    }
}

void vertical_rotation(RUBIKS_SIDE* rubikscube){
    //Effectue une rotation verticale du cube
    right_clockwise(rubikscube, 2);
    left_anticlockwise(rubikscube,2);
    T_COLOR tmp;
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 3; ++i) {
            tmp = rubikscube[side_to_index(BACK)].face[2-i][1];
            rubikscube[side_to_index(BACK)].face[2-i][1] = rubikscube[side_to_index(UP)].face[i][1];
            rubikscube[side_to_index(UP)].face[i][1] = rubikscube[side_to_index(FRONT)].face[i][1];
            rubikscube[side_to_index(FRONT)].face[i][1] = rubikscube[side_to_index(DOWN)].face[i][1];
            rubikscube[side_to_index(DOWN)].face[i][1] = tmp;

        }
    }
}

void quarter_vertical_rotation(RUBIKS_SIDE* rubikscube){
    //Effectue un quart de rotation verticale du cube
    left_anticlockwise(rubikscube,1);
    right_clockwise(rubikscube,1);
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[2-i][1];
        rubikscube[side_to_index(BACK)].face[2-i][1] = rubikscube[side_to_index(UP)].face[i][1];
        rubikscube[side_to_index(UP)].face[i][1] = rubikscube[side_to_index(FRONT)].face[i][1];
        rubikscube[side_to_index(FRONT)].face[i][1] = rubikscube[side_to_index(DOWN)].face[i][1];
        rubikscube[side_to_index(DOWN)].face[i][1] = tmp;

    }

}


void horizontal_rotation(RUBIKS_SIDE* rubikscube){
    //Effectue une rotation horizontale du cube
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp = rubikscube[side_to_index(FRONT)].face[i][j];
            rubikscube[side_to_index(FRONT)].face[i][j] = rubikscube[side_to_index(BACK)].face[i][j];
            rubikscube[side_to_index(BACK)].face[i][j] = tmp;
            tmp = rubikscube[side_to_index(LEFT)].face[i][j];
            rubikscube[side_to_index(LEFT)].face[i][j] = rubikscube[side_to_index(RIGHT)].face[i][j];
            rubikscube[side_to_index(RIGHT)].face[i][j] = tmp;
        }
    }
}

void quarter_horizontal_rotation(RUBIKS_SIDE* rubikscube){
    //Effectue un quart de rotation horizontale du cube
    down_clockwise(rubikscube,1);
    up_anticlockwise(rubikscube,1);
    T_COLOR tmp;
    for (int i = 0; i < 3; ++i) {
        tmp = rubikscube[side_to_index(BACK)].face[1][2-i];
        rubikscube[side_to_index(BACK)].face[1][2-i] = rubikscube[side_to_index(RIGHT)].face[1][2-i];
        rubikscube[side_to_index(RIGHT)].face[1][2-i] = rubikscube[side_to_index(FRONT)].face[1][2-i];
        rubikscube[side_to_index(FRONT)].face[1][2-i] = rubikscube[side_to_index(LEFT)].face[1][2-i];
        rubikscube[side_to_index(LEFT)].face[1][2-i] = tmp;
    }
}

void move_rubiks(RUBIKS_SIDE* rubikscube){
    //Menu permettant d'effectuer les mouvements sur le cube
    int move = 1;
    for (int i = 0; i < 84; ++i) {
        printf("-");
    }
    printf("\n");
    printf("1: Choisir une face     2: Rotation horizontal     3: Rotation vertical     4 : Exit\n");
    for (int i = 0; i < 84; ++i) {
        printf("-");
    }
    int choix;
    printf("\nAction: ");
    scanf("%d",&choix);
    printf("\n");
    switch (choix) {
        case 1:
            for (int i = 0; i < 76; ++i) {
                printf("-");
            }
            printf("\n");
            printf("1: Left    2: Front     3: Up    4: Down    5: Right     6: Back     7: Exit\n");
            for (int i = 0; i < 76; ++i) {
                printf("-");
            }
            printf("\nAction: ");
            scanf("%d",&choix);
            printf("\n");
            switch (choix) {
                case 1:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            left_clockwise(rubikscube,1);
                            break;
                        case 2:
                            left_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            left_clockwise(rubikscube,2);
                            break;
                        case 4:
                            left_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;
                case 2:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            front_clockwise(rubikscube,1);
                            break;
                        case 2:
                            front_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            front_clockwise(rubikscube,2);
                            break;
                        case 4:
                            front_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;

                case 3:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            up_clockwise(rubikscube,1);
                            break;
                        case 2:
                            up_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            up_clockwise(rubikscube,2);
                            break;
                        case 4:
                            up_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;


                case 4:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            down_clockwise(rubikscube,1);
                            break;
                        case 2:
                            down_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            down_clockwise(rubikscube,2);
                            break;
                        case 4:
                            down_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;

                case 5:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            right_clockwise(rubikscube,1);
                            break;
                        case 2:
                            right_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            right_clockwise(rubikscube,2);
                            break;
                        case 4:
                            right_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;
                case 6:
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\n");
                    printf("1: quart clockwise    2: quart anticlockwise    3: demi clockwise    4: demi anticlockwise    5: Exit\n");
                    for (int i = 0; i < 101; ++i) {
                        printf("-");
                    }
                    printf("\nAction: ");
                    scanf("%d",&choix);
                    printf("\n");
                    switch (choix) {
                        case 1:
                            back_clockwise(rubikscube,1);
                            break;
                        case 2:
                            back_anticlockwise(rubikscube,1);
                            break;
                        case 3:
                            back_clockwise(rubikscube,2);
                            break;
                        case 4:
                            back_anticlockwise(rubikscube,2);
                            break;
                        case 5:
                            move = 0;
                            break;
                    }
                    break;
            }
            break;
        case 2:
            horizontal_rotation(rubikscube);
            break;
        case 3:
            vertical_rotation(rubikscube);
            break;
        case 4:
            move = 0;
            break;
    }

}

void solve_rubiks(RUBIKS_SIDE* rubikscube){
    // Fonction permettant de résoundre
    make_cross(rubikscube);

}

void make_cross(RUBIKS_SIDE* rubikscube){
    //Fonction réalisant une croix jaune sur la face down
    semi_cross(rubikscube);
    display_rubiks(rubikscube);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if((rubikscube[side_to_index(FRONT)].face[0][1] == rubikscube[side_to_index(FRONT)].face[1][1]) && (rubikscube[side_to_index(UP)].face[2][1] == rubikscube[side_to_index(DOWN)].face[1][1])){
                break;
            }
            else{
                up_clockwise(rubikscube, 1);
            }
        }
        front_clockwise(rubikscube, 2);
        quarter_horizontal_rotation(rubikscube);
    }
    display_rubiks(rubikscube);

}

void semi_cross(RUBIKS_SIDE* rubikscube){
    //Réalise une croix sur la face up de la couleur de la face down
    int check = 0;
    for (int i = 0; i < 8; i++) {
        if((rubikscube[side_to_index(FRONT)].face[0][1] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[1][0] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[2][1] == rubikscube[side_to_index(DOWN)].face[1][1])){
            check = 1;
        }
        while (check == 1) {
            if((rubikscube[side_to_index(FRONT)].face[0][1] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[1][0] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(FRONT)].face[2][1] == rubikscube[side_to_index(DOWN)].face[1][1])){
                check = 1;
            }
            while(check == 1){

                if((rubikscube[side_to_index(FRONT)].face[0][1] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(FRONT)].face[1][0] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(FRONT)].face[1][2] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(FRONT)].face[2][1] != rubikscube[side_to_index(DOWN)].face[1][1])){
                    check = 0;
                }
                if(rubikscube[side_to_index(FRONT)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]){
                    if(rubikscube[side_to_index(UP)].face[1][2] != rubikscube[side_to_index(DOWN)].face[1][1]){
                        right_clockwise(rubikscube,1);
                    }
                    else{
                        while(rubikscube[side_to_index(UP)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]){
                            up_clockwise(rubikscube,1);
                        }
                        right_clockwise(rubikscube,1);
                    }
                }
                front_clockwise(rubikscube,1);
            }
        }
        quarter_horizontal_rotation(rubikscube);
    }
    for (int k = 0; k < 8; k++) {
        if((rubikscube[side_to_index(DOWN)].face[0][1] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(DOWN)].face[1][0] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(DOWN)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]) || (rubikscube[side_to_index(DOWN)].face[2][1] == rubikscube[side_to_index(DOWN)].face[1][1])){
            check = 1;
        }
        while(check == 1){
            if((rubikscube[side_to_index(DOWN)].face[0][1] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(DOWN)].face[1][0] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(DOWN)].face[1][2] != rubikscube[side_to_index(DOWN)].face[1][1]) && (rubikscube[side_to_index(DOWN)].face[2][1] != rubikscube[side_to_index(DOWN)].face[1][1])){
                check = 0;
            }
            if(rubikscube[side_to_index(DOWN)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]){
                if(rubikscube[side_to_index(UP)].face[1][2] != rubikscube[side_to_index(DOWN)].face[1][1]){
                    right_clockwise(rubikscube,2);
                }
                else{
                    while(rubikscube[side_to_index(UP)].face[1][2] == rubikscube[side_to_index(DOWN)].face[1][1]){
                        up_clockwise(rubikscube,1);
                    }
                    right_clockwise(rubikscube,2);
                }
            }
            down_clockwise(rubikscube,1);
        }
    }
}

void menu(RUBIKS_SIDE* rubikscube){
    //Affichage du Menu
    int menu = 1;
    while (menu){
        for (int i = 0; i < 86; ++i) {
            printf("-");
        }
        printf("\n");
        printf("1: Scramble     2: Reset     3: Blank    4 : Play    5 : Fill    6 : Solve    7 : Exit\n");
        for (int i = 0; i < 86; ++i) {
            printf("-");
        }
        int choix;
        printf("\nAction: ");
        scanf("%d",&choix);
        printf("\n");
        switch (choix) {
            case 1:
                scramble_rubiks(rubikscube);
                display_rubiks(rubikscube);
                break;
            case 2:
                free_rubiks(rubikscube);
                rubikscube = create_rubiks();
                break;
            case 3:
                blank_rubiks(rubikscube);
                display_rubiks(rubikscube);
                break;
            case 4:
                move_rubiks(rubikscube);
                display_rubiks(rubikscube);
                break;
            case 5:
                fill_rubiks(rubikscube);
                break;
            case 6:
                solve_rubiks(rubikscube);
                display_rubiks(rubikscube);
                break;
            case 7 :
                menu = 0;
                break;
        }
    }
}