#include <stdio.h>
#include <stdlib.h>

#define PIECE_EMPTY '.'
#define PIECE_YELLOW 'O'
#define PIECE_RED 'X'

typedef struct {
    char** grid;
    int player_turn;
    int grid_height;
    int grid_width;
    int win_condition;
} Game;

Game* init_game(int grid_height, int grid_width, int win_condition) {
    Game* game = malloc(sizeof(Game));
    game->grid = malloc(grid_height * sizeof(char*));
    for (int i = 0; i < grid_height; i++) {
        game->grid[i] = malloc(grid_width * sizeof(char));
        for (int j = 0; j < grid_width; j++) {
            game->grid[i][j] = PIECE_EMPTY;
        }
    }
    game->player_turn = 0;
    game->grid_height = grid_height;
    game->grid_width = grid_width;
    game->win_condition = win_condition;
    return game;
}

void free_game(Game* game) {
    for (int i = 0; i < game->grid_height; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
    free(game);
}

void print_grid(Game* game) {
    for (int i = 0; i < game->grid_height; i++) {
        for (int j = 0; j < game->grid_width; j++) {
            printf("%c ", game->grid[i][j]);
        }
        printf("\n");
    }
    for (int j = 0; j < game->grid_width; j++) {
        printf("%d ", j + 1);
    }
    printf("\n");
}

void move_piece(Game* game, int col) {
    if (col < 0 || col >= game->grid_width) {
        printf("Colonne invalide !\n");
        return;
    }

    if (game->grid[0][col] != PIECE_EMPTY) {
        printf("Colonne déjà pleine !\n");
        return;
    }

    int row = game->grid_height - 1;
    while (row >= 0 && game->grid[row][col] != PIECE_EMPTY) {
        row--;
    }
    game->grid[row][col] = (game->player_turn == 0) ? PIECE_YELLOW : PIECE_RED;
    game->player_turn = 1 - game->player_turn;
}

int check_win(Game* game) {
    int count;

    for (int i = 0; i < game->grid_height; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->grid[i][j] == PIECE_YELLOW ? 0 : 1;
            }
        }
    }

    for (int i = 0; i <= game->grid_height - game->win_condition; i++) {
        for (int j = 0; j < game->grid_width; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i + k][j]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->grid[i][j] == PIECE_YELLOW ? 0 : 1;
            }
        }
    }


    for (int i = game->win_condition - 1; i < game->grid_height; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i - k][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->grid[i][j] == PIECE_YELLOW ? 0 : 1;
            }
        }
    }


    for (int i = 0; i <= game->grid_height - game->win_condition; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i + k][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->grid[i][j] == PIECE_YELLOW ? 0 : 1;
            }
        }
    }

    return -1;
}

void game_loop(Game* game) {
    while (1) {
        print_grid(game);
        int col;
        printf("Joueur %d, choisissez la colonne (1-%d) : ", game->player_turn + 1, game->grid_width);
        scanf("%d", &col);
        move_piece(game, col - 1);
        int winner = check_win(game);
        if (winner != -1) {
            print_grid(game);
            printf("Joueur %d a gagné !\n", winner == 0 ? 1 : 2);
            break;
        }
    }
}

int main() {
    int grid_height, grid_width, win_condition;
    printf("Entrez la hauteur de la grille : ");
    scanf("%d", &grid_height);
    printf("Entrez la largeur de la grille : ");
    scanf("%d", &grid_width);

    if (grid_height < 2 || grid_width < 2) {
        printf("La taille de la grille est invalide !\n");
        return 1;
    }

    printf("Entrez le nombre de pions à aligner pour gagner (minimum 2) : ");
    scanf("%d", &win_condition);

    int min_dimension = (grid_height < grid_width) ? grid_height : grid_width;
    if (win_condition < 2 || win_condition > min_dimension) {
        printf("Le nombre de pions à aligner est invalide !\n");
        return 1;
    }

    Game* game = init_game(grid_height, grid_width, win_condition);
    game_loop(game);
    free_game(game);
    return 0;
}
