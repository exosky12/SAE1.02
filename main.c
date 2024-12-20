#include "main.h"

Heap heapEmpty(void)
{
    return NULL;
}

Heap add(Heap f, int x)
{
    Node *m;
    m=(Node*)malloc(sizeof(Node));

    if(m==NULL)
    {
        printf("Pb malloc!");
        exit(1);
    } 

    m->v=x;

    if(f==NULL)
    {
        m->suiv=m;
        return m;
    }

    m->suiv = f->suiv;
    f->suiv=m;
    return m;
}

Heap deleteHead(Heap f)
{
    Node *aux;
    if(f==NULL)
    {
        printf("pb opération interdite !");
        exit(1);
    }
    if(f==f->suiv)
    {
        free(f);
        return heapEmpty();
    }

    aux = f->suiv;
    f->suiv = f-> suiv->suiv;
    free(aux);
    return f;
}

Boolean isEmpty(Heap f)
{
    return f==NULL;
}

int head(Heap f)
{
    if(isEmpty(f))
    {
        printf("pbm Heap vide");
        exit(1);
    }
    return f->suiv->v;
}

void display(Heap f)
{
    Node *m;

    if(f==NULL)
        return;
    m=f->suiv;

    while(m != f)
    {
        printf("%d",m->v);
        m=m->suiv;
    } 
    printf("%d\n",m->v);
}

int length(Heap f)
{
    int i = 0;
    Node *m;
    m = f->suiv;
    while(m != f)
    {
        i++;
        m = m->suiv;
    }
    i++;
    return i;
}

void existingGameDisplay(void)
{
  FILE *gameHeap;
  char gameName[100], playerName[100];
  clearScreen();
  printf("▁ ▂ ▄ ▅ ▆ ▇ █ Jouer une partie prédéfinie █ ▇ ▆ ▅ ▄ ▂ ▁\n\n");
  printf("Entrer le nom du fichier correspondant à la partie > ");
  scanf("%s", gameName);
  gameHeap = fopen(gameName, "rb");
  if (gameHeap == NULL){
    printf("[ERREUR] Fichier introuvable\n");
    return;
  }
  printf("Enter le pseudo du joueur > ");
  scanf("%s", playerName);
  printf("\n[CONTEXTE] Vous arrivez dans un corridor, bordé par deux falaises des monstres arrivent les uns après les autres.\n");
  existingGame();
}

// Ajouter type de retour et paramètres
void existingGame(void)
{
    // Ajouter code
}

void createNewGameDisplay()
{
  char nickname[50];
  clearScreen();
  printf("▁ ▂ ▄ ▅ ▆ ▇ █ Créer une nouvelle partie █ ▇ ▆ ▅ ▄ ▂ ▁\n\n");
  printf("Entrer votre pseudo > ");
  scanf("%s", nickname);
  //createNewGame(nickname, playersTab);
}

// Ajouter type de retour et paramètres
void createNewGame(char nickname[50], Player playersTab[100])
{
  Player player;
  strcpy(player.nickname, nickname);
  player.nbPv = 20;
  player.nbDamages = 0;
  

  // Ajouter code
}

int loadData(void)
{
  Player *playersTab;
  int nbPlayers = 0;

  //charger nbPlayers

  playersTab = (Player*)malloc(nbPlayers*sizeof(Player));
}

char determineWinner(char weaponPlayer, char weaponMonster) {
    if (weaponPlayer == weaponMonster) {
        return 'D';
    }
    if (weaponPlayer == '#') {
        return 'P';
    }
    if (weaponMonster == '#') {
        return 'M';
    }
    if (weaponPlayer == 'O') {
        return 'M';
    }
    if (weaponMonster == 'O') {
        return 'P';
    }
    if (weaponPlayer == 'P' && weaponMonster == 'C') {
        return 'P';
    }
    if (weaponPlayer == 'C' && weaponMonster == 'F') {
        return 'P';
    }
    if (weaponPlayer == 'F' && weaponMonster == 'P') {
        return 'P';
    }
    if (weaponMonster == 'P' && weaponPlayer == 'C') {
        return 'M';
    }
    if (weaponMonster == 'C' && weaponPlayer == 'F') {
        return 'M';
    }
    if (weaponMonster == 'F' && weaponPlayer == 'P') {
        return 'M';
    }
    return 'D';
}

void global(void){
  Boolean quit = False;
  int choice;
  
  
  while(!quit){
    clearScreen();
    printf("\n\t▁ ▂ ▄ ▅ ▆ ▇ █  MENU  █ ▇ ▆ ▅ ▄ ▂ ▁\n\n\n");
    printf("[1] Jouer une partie prédéfinie\n");
    printf("[2] Créer une nouvelle partie\n");
    printf("[3] Afficher la liste des joueurs triée par nom\n");
    printf("[4] Afficher la liste des joueurs triée par meilleur score\n");
    printf("[9] Quitter\n\n");
    printf("Votre choix > ");
    scanf("%d", &choice);
    switch(choice){
      case 1:
        existingGameDisplay();
        break;
      case 2:
        createNewGameDisplay();
        break;
      case 3:
        printf("Afficher la liste des joueurs triée par nom\n");
        break;
      case 4:
        printf("Afficher la liste des joueurs triée par meilleur score\n");
        break;
      case 9:
        printf("Au revoir...\n");
        quit = True;
        break;
      default:
        printf("Choix invalide\n");
        break;
    }
  }
}

void clearScreen(void)
{
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

