#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ATTEMPTS 6    // le joueur a 6 essais
#define WORDLEN 5     // mot de 5 lettres obligatoire
#define MAXWORDS 2500 // max mots dans words.txt est 2500

// code couleurs
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define GRAY "\033[1;37m"
#define RESET "\033[0m"

// fonction qui compare ton mot proposer(guess)avec le mot secret
void evaluate_guess(const char *guess, const char *secret, int colors[WORDLEN])
{
    int used[WORDLEN] = {0};

    // on marque les verts(si lettre bon endroit et juste alors vert (2)Else gris(0))
    for (int i = 0; i < WORDLEN; i++)
    {
        if (guess[i] == secret[i])
        {
            colors[i] = 2; // vert
            used[i] = 1;
        }
        else
        {
            colors[i] = 0; // par defaut gris
        }
    }

    // on marque les jaunes(1)
    for (int i = 0; i < WORDLEN; i++)
    {
        if (colors[i] == 0)
        { // seulement si c'est pas vert
            for (int j = 0; j < WORDLEN; j++)
            {
                if (!used[j] && guess[i] == secret[j])
                {
                    colors[i] = 1; // jaune
                    used[j] = 1;   // marquer cette lettre comme utilisee
                    break;
                }
            }
        }
    }
}

// affiche toutes les tentatives du joueur avec les couleurs
void print_board(char board[ATTEMPTS][WORDLEN], int colorBoard[ATTEMPTS][WORDLEN])
{
    for (int i = 0; i < ATTEMPTS; i++)
    {
        for (int j = 0; j < WORDLEN; j++)
        {

            char c = (board[i][j] == '\0') ? '.' : board[i][j];

            switch (colorBoard[i][j])
            {
            case 2:
                printf(GREEN "%c " RESET, c);
                break;
            case 1:
                printf(YELLOW "%c " RESET, c);
                break;
            case 0:
                printf(GRAY "%c " RESET, c);
                break; // les printf reset c'est pour reinitialiser la couleur entre chaque lettre
            default:
                printf("%c ", c);
                break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

// ouvre le fichier words.txt et lit mot par mot
int load_words(const char *filename, char words[MAXWORDS][WORDLEN + 1])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening word file"); // erreur (le fichier ne s'ouvre pas)
        return 0;
    }

    int count = 0;
    // lecture des mots
    while (count < MAXWORDS && fscanf(file, "%5s", words[count]) == 1)
    {
        if (strlen(words[count]) == WORDLEN)
        {
            count++; // count=nombre total de mots
        }
    }

    fclose(file);
    return count;
}

int main()
{ // charger les mots
    char words[MAXWORDS][WORDLEN + 1];
    int wordCount = load_words("words.txt", words);

    if (wordCount == 0)
    {
        printf("No words loaded. Please check words.txt.\n");
        return 1; // si 0 alors fichier vide (aucun mot de 5 lettres)
    }

    // choisis un mot secret au hasard
    srand(time(NULL));
    const char *secret = words[rand() % wordCount];

    // Initialiser le tableau du jeu (6 tentatives 5 lettres)
    char board[ATTEMPTS][WORDLEN] = {{0}};
    int colorBoard[ATTEMPTS][WORDLEN] = {{0}};

    char guess[100];

    printf("Welcome to C Wordle!\n");
    printf("Guess the %d-letter word. You have %d attempts.\n\n", WORDLEN, ATTEMPTS);

    // a chaque tentative: 1)le joueur tape le mot 2)verifie que 5lettres 3)remplit lz tableau board 4) appelle la fuction (evaluate_guess) 5)affiche le board coloree 6)verifie si le mot est juste
    for (int attempt = 0; attempt < ATTEMPTS; attempt++)
    {

        printf("Attempt %d/%d - Enter a %d-letter word: ",
               attempt + 1, ATTEMPTS, WORDLEN);

        scanf("%s", guess);

        // Check guess length
        if (strlen(guess) != WORDLEN)
        {
            printf("Invalid length! Please enter exactly %d letters.\n", WORDLEN);
            attempt--;
            continue;
        }

        // Store letters in board
        for (int i = 0; i < WORDLEN; i++)
        {
            board[attempt][i] = guess[i];
        }

        // Evaluate guess and print board
        evaluate_guess(guess, secret, colorBoard[attempt]);
        print_board(board, colorBoard);

        // Check win
        if (strcmp(guess, secret) == 0)
        {
            printf("🎉 You guessed it!\n");
            return 0;
        }
    }

    // Out of attempts
    printf("Out of tries! The word was: %s\n", secret);
    return 0;
}