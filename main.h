#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUGE "\033[31m"
#define RESET "\033[0m"
#define JAUNE "\033[33m"
#define BLEU "\033[36m"
#define VERT "\033[32m"
#define GRAS "\033[1m"
#define MAGENTA "\033[35m"

typedef enum
{
    False,
    True
} Boolean;

typedef struct Maillon
{
    int v;
    struct Maillon *suiv;
} Maillon, *File;

typedef struct
{
    char pseudo[50]; // Surnom du joueur (maximum 50 caractères + caractère de fin de chaîne)
    int nbPv;        // Nombre de points de vie du joueur
    int nbDegats;    // Nombre total de dégâts infligés par le joueur
    int nbParties;   // Nombre de jeux/joues auxquels le joueur a participé
    int *scores;     // Scores
    char armes[4];   // Liste des armes disponibles pour le joueur ('P', 'F', 'C')
} Joueur;

typedef struct
{
    char nom[50]; // Nom du monstre
    int niveau;   // Niveau du monstre (1, 2 ou 3)
    int pv;       // Points de vie du monstre
    int degats;   // Points de dégâts infligés par attaque
    int type;     /// Type du groupe : 1 pour séquentiel, 2 pour simultané
    int nbArmes;  // Nombre d'armes disponibles
    char *armes;  // Liste des armes disponibles ('P', 'F', 'C', 'O', '#')
} Monstre;

File fileVide(void);
File ajouter(File f, int x);
File supprimerTete(File f);
Boolean estVide(File f);
int tete(File f);
void afficher(File f);
int longueur(File f);

void global(void);
void clearScreen(void);
int CreerPartie(Joueur **joueursTab, int *nbJoueurs);
int gameGroupe1(Joueur *joueur, Joueur joueursTab[], Monstre monstresTab[], int indexMonstresTab[], int nbMonstres, int pointsJoueur);
int gameGroupe2(Joueur *joueur, Joueur joueursTab[], Monstre monstresTab[], int indexMonstresTab[], int nbMonstres, int pointsJoueur);
Monstre creationMonstre(int groupe);

void sauvegarderScoreJoueur(Joueur *joueursTab, int indexJoueur, int pointsGagnes);

int PartiePredefinie(int *nbJoueurs, Joueur **joueursTab, Monstre monstresTab[], int indexMonstresTabGroupe1[], int indexMonstresTabGroupe2[], int nbMonstresGroupe1, int nbMonstresGroupe2);
char determinerGagnant(char weaponJoueur, char weaponMonstre);

int rechercheDicoJoueur(char pseudoJoueur[50], Joueur joueursTab[], int nbJoueurs, int *trouve);
int rechercheArme(char arme, char armes[], int nbArmes);

Monstre *loadMonstres(char *filenom, int *nbMonstres, int **indexMonstresGroup1, int **indexMonstresGroup2, int *nbMonstresGroup1, int *nbMonstresGroup2);

Joueur *loadJoueursFromBinary(char *filenom, int *nbJoueurs);
void saveJoueursToBinary(char *filenom, Joueur *joueurs, int nbJoueurs);

void remplirIndexJoueursTriesParScore(Joueur *joueursTab, int nbJoueurs, int **indexJoueursTriesParScore);

void afficherJoueursTriesParNom(Joueur *joueursTab, int nbJoueurs);
void afficherJoueursTriesParScore(Joueur *joueursTab, int nbJoueurs, int indexJoueursTriesParScore[]);

float calculerScoreMoyen(Joueur joueur);
void afficherStatsJoueur(Joueur *joueursTab, int nbJoueurs);

void showAllPlayers(Joueur *joueursTab, int nbJoueurs);

void insererJoueurOrdreAlphabetique(Joueur **joueursTab, int *nbJoueurs, int indexTemp);
