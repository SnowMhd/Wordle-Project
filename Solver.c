
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ATTEMPTS 6
#define WORD_LEN 5

/*
 * Lit un fichier de mots et retourne un tableau de chaînes
 * Paramètres :
 *   - filename : nom du fichier contenant les mots
 *   - count : pointeur pour retourner le nombre de mots lus
 * Retourne : tableau de pointeurs vers les mots lus
 */
char **read_words(const char *filename, int *count)
{
    FILE *fp;
    char **words = NULL;
    char line[16];
    int n = 0;
    int i;

    fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Cannot open file\n");
        exit(1);
    }

    // Lecture ligne par ligne du fichier
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0'; // Supprime le saut de ligne
        if (strlen(line) != WORD_LEN)
            continue; // Ignore les mots de mauvaise longueur

        // Réallocation dynamique du tableau
        words = realloc(words, (n + 1) * sizeof(char *));
        words[n] = malloc(WORD_LEN + 1);
        for (i = 0; i < WORD_LEN + 1; i++)
            words[n][i] = 0; // Initialisation à 0
        strcpy(words[n], line);
        n++;
    }

    fclose(fp);
    *count = n;
    return words;
}

/*
 * Calcule le feedback (g=vert, y=jaune, b=noir) pour une tentative
 * Paramètres :
 *   - guess : le mot proposé
 *   - target : le mot secret
 *   - fb : buffer pour stocker le feedback (doit avoir taille WORD_LEN+1)
 */
void compute_feedback(const char *guess, const char *target, char fb[WORD_LEN + 1])
{
    int i, j;
    char temp_target[WORD_LEN];
    int used[WORD_LEN]; // Marque les lettres déjà utilisées du mot secret

    // Initialisation
    for (i = 0; i < WORD_LEN; i++)
    {
        fb[i] = 'b'; // Par défaut, toutes les lettres sont noires
        temp_target[i] = target[i];
        used[i] = 0;
    }
    fb[WORD_LEN] = '\0';

    /* Première passe : lettres vertes (bonne lettre, bonne position) */
    for (i = 0; i < WORD_LEN; i++)
    {
        if (guess[i] == temp_target[i])
        {
            fb[i] = 'g';
            used[i] = 1; // Marque la lettre comme utilisée dans le mot secret
        }
    }

    /* Deuxième passe : lettres jaunes (bonne lettre, mauvaise position) */
    for (i = 0; i < WORD_LEN; i++)
    {
        if (fb[i] == 'g')
            continue; // On ignore les lettres déjà vertes
        for (j = 0; j < WORD_LEN; j++)
        {
            if (!used[j] && guess[i] == temp_target[j])
            {
                fb[i] = 'y';
                used[j] = 1; // Marque la lettre comme utilisée
                break;
            }
        }
    }
}

/*
 * Vérifie si un mot est compatible avec le feedback d'une tentative précédente
 * Paramètres :
 *   - word : le mot à tester
 *   - guess : le mot précédemment proposé
 *   - fb : le feedback reçu pour 'guess'
 * Retourne : 1 si le mot est compatible, 0 sinon
 */
int keep_word(const char *word, const char *guess, const char *fb)
{
    int i, j;
    char temp_word[WORD_LEN + 1];
    int used[WORD_LEN]; // Marque les lettres déjà appariées

    strcpy(temp_word, word);
    for (i = 0; i < WORD_LEN; i++)
        used[i] = 0;

    /* Vérification des lettres vertes */
    for (i = 0; i < WORD_LEN; i++)
    {
        if (fb[i] == 'g')
        {
            if (word[i] != guess[i])
                return 0; // Doit avoir la même lettre à la même position
            used[i] = 1;  // Marque cette position comme utilisée
        }
    }

    /* Vérification des lettres jaunes */
    for (i = 0; i < WORD_LEN; i++)
    {
        if (fb[i] == 'y')
        {
            if (word[i] == guess[i])
                return 0; // Ne doit pas être à la même position
            int found = 0;
            // Cherche la lettre ailleurs dans le mot
            for (j = 0; j < WORD_LEN; j++)
            {
                if (!used[j] && word[j] == guess[i])
                {
                    used[j] = 1;
                    found = 1;
                    break;
                }
            }
            if (!found)
                return 0; // La lettre doit être présente ailleurs
        }
    }

    /* Vérification des lettres noires */
    for (i = 0; i < WORD_LEN; i++)
    {
        if (fb[i] == 'b')
        { // La lettre ne doit pas apparaître dans les positions non utilisées
            for (j = 0; j < WORD_LEN; j++)
            {
                if (!used[j] && word[j] == guess[i])
                    return 0;
            }
        }
    }

    return 1; // Le mot passe tous les tests
}

/*
 * Choisit un mot secret aléatoire dans la liste
 * Paramètres :
 *   - words : tableau de mots
 *   - n : nombre de mots dans le tableau
 * Retourne : pointeur vers le mot secret
 */
char *pick_secret(char **words, int n)
{
    return words[rand() % n];
}

/*
 * Affiche le feedback avec codes couleurs ANSI
 * Paramètres :
 *   - guess : le mot proposé
 *   - fb : le feedback à afficher
 */
void print_feedback(const char *guess, const char *fb)
{
    int i;
    for (i = 0; i < WORD_LEN; i++)
    {
        if (fb[i] == 'g')
            printf("\x1b[42m %c \x1b[0m", guess[i]); // Fond vert
        else if (fb[i] == 'y')
            printf("\x1b[43m %c \x1b[0m", guess[i]); // Fond jaune
        else
            printf("\x1b[40m %c \x1b[0m", guess[i]); // Fond noir
    }
    printf("\n");
}

/*
 * Fonction principale - Implémente le solver automatique de Wordle
 * Algorithme : utilise "soare" comme premier mot, puis filtre progressivement
 * le dictionnaire basé sur le feedback reçu
 */
int main()
{
    char **words;
    int n, i, attempt, new_n;
    char secret[WORD_LEN + 1];
    char guess[WORD_LEN + 1];
    char fb[WORD_LEN + 1];
    char grid[MAX_ATTEMPTS][WORD_LEN + 1];    // Historique des tentatives
    char fb_grid[MAX_ATTEMPTS][WORD_LEN + 1]; // Historique des feedbacks

    srand((unsigned int)time(NULL)); // Initialisation du générateur aléatoire

    // Chargement du dictionnaire
    words = read_words("words.txt", &n);
    strcpy(secret, pick_secret(words, n));

    printf("=== WordSolver automatique fiable ===\n");
    printf("Mot secret choisi !\n\n");

    strcpy(guess, "soare"); // Mot de départ optimal statistiquement

    // Boucle principale des tentatives
    for (attempt = 0; attempt < MAX_ATTEMPTS; attempt++)
    {
        compute_feedback(guess, secret, fb);

        // Sauvegarde de l'historique
        strcpy(grid[attempt], guess);
        strcpy(fb_grid[attempt], fb);

        // Affichage de l'historique
        for (i = 0; i <= attempt; i++)
            print_feedback(grid[i], fb_grid[i]);
        printf("\n");

        if (strcmp(fb, "ggggg") == 0)
            break; // Mot trouvé !

        /* Filtrage du dictionnaire : ne garde que les mots compatibles */
        new_n = 0;
        for (i = 0; i < n; i++)
        {
            if (keep_word(words[i], guess, fb))
            {
                words[new_n++] = words[i];
            }
        }
        n = new_n;

        if (n == 0)
        {
            printf("Aucun mot restant !\n");
            break;
        }

        strcpy(guess, words[0]); // Stratégie simple : premier mot restant
    }

    // Affichage du résultat final
    if (strcmp(fb, "ggggg") == 0)
        printf("Mot trouve : %s\n", guess);
    else
        printf("Mot non trouve apres %d tentatives !\n", MAX_ATTEMPTS);

    // Nettoyage mémoire
    for (i = 0; i < n; i++)
        free(words[i]);
    free(words);

    return 0;
}