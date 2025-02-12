#include "main.h"

File fileVide(void)
{
    return NULL;
}

File ajouter(File f, int x)
{
    Maillon *m;
    m = (Maillon *)malloc(sizeof(Maillon));

    if (m == NULL)
    {
        printf("Pb malloc!");
        exit(1);
    }

    m->v = x;

    if (f == NULL)
    {
        m->suiv = m;
        return m;
    }

    m->suiv = f->suiv;
    f->suiv = m;
    return m;
}

File supprimerTete(File f)
{
    Maillon *aux;
    if (f == NULL)
    {
        printf("pb opération interdite !");
        exit(1);
    }
    if (f == f->suiv)
    {
        free(f);
        return fileVide();
    }

    aux = f->suiv;
    f->suiv = f->suiv->suiv;
    free(aux);
    return f;
}

Boolean estVide(File f)
{
    return f == NULL;
}

int tete(File f)
{
    if (estVide(f))
    {
        printf("pbm File vide");
        exit(1);
    }
    return f->suiv->v;
}

void afficher(File f)
{
    Maillon *m;

    if (f == NULL)
        return;
    m = f->suiv;

    while (m != f)
    {
        printf("%d", m->v);
        m = m->suiv;
    }
    printf("%d\n", m->v);
}

int longueur(File f)
{
    if (f == NULL)
        return 0;
    int i = 0;
    Maillon *m;
    m = f->suiv;
    while (m != f)
    {
        i++;
        m = m->suiv;
    }
    i++;
    return i;
}

int CreerPartie(Joueur **joueursTab, int *nbJoueurs)
{
    int *indexMonstresGroupe1 = NULL;
    int *indexMonstresGroupe2 = NULL;
    int nbMonstresGroupe1 = 0;
    int nbMonstresGroupe2 = 0;

    Monstre *monstresTab = NULL;

    clearScreen();
    printf(GRAS VERT "▁ ▂ ▄ ▅ ▆ ▇ █ Jouer une partie prédéfinie █ ▇ ▆ ▅ ▄ ▂ ▁\n\n" RESET);

    // Demander le pseudo du joueur
    char pseudoJoueur[50];
    printf("Entrez le pseudo du joueur > ");
    scanf("%s", pseudoJoueur);

    // Recherche du joueur dans le tableau
    int trouve;
    int index = rechercheDicoJoueur(pseudoJoueur, *joueursTab, *nbJoueurs, &trouve);

    if (trouve == 0)
    {
        Joueur newJoueur;
        strcpy(newJoueur.pseudo, pseudoJoueur);
        newJoueur.nbPv = 20;
        newJoueur.nbDegats = 1;
        newJoueur.nbParties = 0;
        newJoueur.scores = NULL;
        strcpy(newJoueur.armes, "PFC");

        *nbJoueurs = *nbJoueurs + 1;
        *joueursTab = (Joueur *)realloc(*joueursTab, (*nbJoueurs) * sizeof(Joueur));
        if (*joueursTab == NULL)
        {
            printf("Erreur d'allocation mémoire\n");
            exit(1);
        }

        (*joueursTab)[*nbJoueurs - 1] = newJoueur;
        index = *nbJoueurs - 1;
    }

    Joueur joueur = (*joueursTab)[index];

    // choix de l'ordre de jeu
    int choixOrdreGroupe;
    printf(JAUNE "[ACTION] Par quel groupe de monstre souhaitez-vous démarrer ? (1 Pour séquentiel, 2 pour simultané)\n" RESET);
    printf("> ");
    scanf("%d", &choixOrdreGroupe);
    while (choixOrdreGroupe != 1 && choixOrdreGroupe != 2)
    {
        printf(ROUGE "[ERREUR] Choix incorrect\n");
        printf(JAUNE "[ACTION] Par quel groupe de monstre souhaitez-vous démarrer ? (1 Pour séquentiel, 2 pour simultané)\n" RESET);
        printf("> ");
        scanf("%d", &choixOrdreGroupe);
    }

    // Premier groupe de monstres
    int nbMonstresPremierGrp;
    printf(JAUNE "[ACTION] Combien voulez-vous de monstres dans le groupe 1\n" RESET);
    printf("> ");
    scanf("%d", &nbMonstresPremierGrp);
    while (nbMonstresPremierGrp < 1)
    {
        printf(ROUGE "[ERREUR] Nombre de monstres incorrect\n" RESET);
        printf(JAUNE "[ACTION] Combien voulez-vous de monstres dans le groupe 1\n" RESET);
        printf("> ");
        scanf("%d", &nbMonstresPremierGrp);
    }

    // Allocation dynamique pour le tableau des monstres
    monstresTab = (Monstre *)malloc(nbMonstresPremierGrp * sizeof(Monstre));
    indexMonstresGroupe1 = (int *)malloc(nbMonstresPremierGrp * sizeof(int));
    if (monstresTab == NULL)
    {
        printf("Erreur allocation\n");
        return -1;
    }

    for (int i = 0; i < nbMonstresPremierGrp; i++)
    {
        monstresTab[i] = creationMonstre(1);
        indexMonstresGroupe1[i] = i;
        nbMonstresGroupe1++;
    }

    // Deuxième groupe de monstres
    int nbMonstresDeuxiemeGrp;
    printf(JAUNE "[ACTION] Combien voulez-vous de monstres du groupe 2\n" RESET);
    printf("> ");
    scanf("%d", &nbMonstresDeuxiemeGrp);
    while (nbMonstresDeuxiemeGrp < 1)
    {
        printf(ROUGE "[ERREUR] Nombre de monstres incorrect\n" RESET);
        printf(JAUNE "[ACTION] Combien voulez-vous de monstres du groupe 2\n" RESET);
        printf("> ");
        scanf("%d", &nbMonstresDeuxiemeGrp);
    }

    // realloc monstresTab
    monstresTab = (Monstre *)realloc(monstresTab, (nbMonstresDeuxiemeGrp + nbMonstresPremierGrp) * sizeof(Monstre));
    indexMonstresGroupe2 = (int *)malloc(nbMonstresDeuxiemeGrp * sizeof(int));

    if (indexMonstresGroupe2 == NULL)
    {
        printf("Erreur allocation\n");
        return -1;
    }

    // Création des monstres du deuxième groupe
    for (int i = 0; i < nbMonstresDeuxiemeGrp; i++)
    {
        // appeler la fonction creationGroupeMonstres
        monstresTab[i + nbMonstresPremierGrp] = creationMonstre(2);
        indexMonstresGroupe2[i] = i + nbMonstresPremierGrp;
        nbMonstresGroupe2++;
    }

    // Jouer la partie
    (*joueursTab)[index].nbParties++;

    if (choixOrdreGroupe == 1)
    {
        int pointsGagnes = 0;
        // Jouer le premier groupe
        pointsGagnes = gameGroupe1(&joueur, *joueursTab, monstresTab, indexMonstresGroupe1, nbMonstresGroupe1, pointsGagnes);
        sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);

        // Jouer le deuxième groupe
        if (joueur.nbPv > 0)
        {
            printf(BLEU "[INFO] Tous les monstres sont morts...\n\n" RESET);
            pointsGagnes = gameGroupe2(&joueur, *joueursTab, monstresTab, indexMonstresGroupe2, nbMonstresGroupe2, pointsGagnes);
            sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);
        }
        else
        {
            printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);
            printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
            getchar();
            getchar();
        }
    }
    else
    {
        int pointsGagnes = 0;
        // Jouer le deuxième groupe
        pointsGagnes = gameGroupe2(&joueur, *joueursTab, monstresTab, indexMonstresGroupe2, nbMonstresGroupe2, pointsGagnes);
        sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);

        // Jouer le premier groupe
        if (joueur.nbPv > 0)
        {
            printf(BLEU "[INFO] Tous les monstres sont morts...\n\n" RESET);
            pointsGagnes = gameGroupe1(&joueur, *joueursTab, monstresTab, indexMonstresGroupe1, nbMonstresGroupe1, pointsGagnes);
            sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);
        }
        else
        {
            printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);
            printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
            getchar();
            getchar();
        }
    }

    if (joueur.nbPv > 0)
    {
        printf(VERT "[PARTIE GAGNEE] Bravo ! Vous avez survécu aux terribles monstres !!!\n" RESET);
        printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
        getchar();
        getchar();
    }

    else
    {
        printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);
        printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
        getchar();
        getchar();
    }

    free(monstresTab);
    free(indexMonstresGroupe1);
    free(indexMonstresGroupe2);

    return index;
}

Monstre creationMonstre(int groupe)
{
    Monstre monstre;

    // Nom du monstre
    char nomMonstre[50];
    printf(JAUNE "[ACTION] Quel nom souhaitez vous pour votre monstre\n" RESET);
    printf("> ");
    getchar();
    fgets(nomMonstre, 50, stdin);

    nomMonstre[strlen(nomMonstre) - 1] = '\0';

    // Niveau du monstre
    int niveauMonstre;
    printf(JAUNE "[ACTION] Quel niveau souhaitez vous pour votre monstre (Niveau 1, 2 ou 3)\n" RESET);
    printf("> ");
    scanf("%d", &niveauMonstre);

    // Vérification du niveau du monstre
    while (niveauMonstre != 1 && niveauMonstre != 2 && niveauMonstre != 3)
    {
        printf(ROUGE "[ERREUR] Niveau de monstre incorrect\n" RESET);
        printf(JAUNE "[ACTION] Quel niveau souhaitez vous pour votre monstre (Niveau 1, 2 ou 3)\n" RESET);
        printf("> ");
        scanf("%d", &niveauMonstre);
    }

    // Initialisation du monstre
    strcpy(monstre.nom, nomMonstre);
    monstre.niveau = niveauMonstre;
    monstre.pv = 4;
    monstre.degats = 1;
    monstre.type = groupe;

    // Initialisation des armes du monstre
    switch (monstre.niveau)
    {
    case 1:
        monstre.nbArmes = 4;
        monstre.armes = (char *)malloc(monstre.nbArmes * sizeof(char));

        strcpy(monstre.armes, "PFCO");
        break;
    case 2:
        monstre.nbArmes = 3;
        monstre.armes = (char *)malloc(monstre.nbArmes * sizeof(char));

        strcpy(monstre.armes, "PFC");
        break;
    case 3:
        monstre.nbArmes = 5;
        monstre.armes = (char *)malloc(monstre.nbArmes * sizeof(char));

        strcpy(monstre.armes, "PFCO#");
        break;
    }

    return monstre;
}

int rechercheDicoJoueur(char pseudoJoueur[50], Joueur joueursTab[], int nbJoueurs, int *trouve)
{
    int inf = 0, sup = nbJoueurs - 1;
    int m;
    while (inf <= sup)
    {
        m = (inf + sup) / 2;
        if (strcmp(pseudoJoueur, joueursTab[m].pseudo) == 0)
        {
            *trouve = 1;
            return m;
        }
        if (strcmp(pseudoJoueur, joueursTab[m].pseudo) < 0)
        {
            inf = m + 1;
        }
        else
        {
            sup = m - 1;
        }
    }
    *trouve = 0;
    return inf;
}

int rechercheArme(char arme, char armes[], int nbArmes)
{
    // Cas de base : si la taille du tableau est 0, l'élément n'est pas trouvé
    if (nbArmes == 0)
    {
        return 0;
    }

    // Vérifie si l'élément courant est l'arme recherchée
    if (armes[nbArmes - 1] == arme)
    {
        return 1;
    }

    // Appel récursif en réduisant la taille du tableau
    return rechercheArme(arme, armes, nbArmes - 1);
}

int gameGroupe1(Joueur *joueur, Joueur joueursTab[], Monstre monstresTab[], int indexMonstresTab[], int nbMonstres, int pointsJoueur)
{
    printf("\n[CONTEXTE] Vous arrivez dans un corridor, bordé par deux falaises des monstres arrivent les uns après les autres.\n\n");

    // index qui permet de parcourir les monstres
    int index = 0;

    // int indexJoueur = rechercheDicoJoueur(joueur.pseudo, joueursTab, 1, 0);

    // Tant qu'il y a des monstres dans le groupe 1
    while (nbMonstres > 0)
    {

        // Monstre actuel
        Monstre monstreActuel = monstresTab[indexMonstresTab[index]];

        printf(BLEU "[INFO] Le monstre %s(%dptV, %dAtt) accoure et se prépare à t'attaquer %s(%dptV, %dAtt)\n" RESET,
               monstreActuel.nom, monstreActuel.pv, monstreActuel.degats, joueur->pseudo, joueur->nbPv, joueur->nbDegats);

        while (monstreActuel.pv > 0)
        {
            printf(JAUNE "[ACTION] %s(%dPts) choisis ton arme parmi " RESET, joueur->pseudo, pointsJoueur);

            // Affichage des armes disponibles
            for (int j = 0; j < 4; j++)
            {
                printf(JAUNE " %c" RESET, joueur->armes[j]);
            }

            printf("> ");

            // Arme choisie par le joueur
            char armeChoisie;
            scanf(" %c", &armeChoisie);

            // Recherche de l'arme choisie dans les armes disponibles
            int trouve = 0;

            while (trouve == 0)
            {
                trouve = rechercheArme(armeChoisie, joueur->armes, 4);

                if (trouve == 0)
                {
                    printf(ROUGE "[ERREUR] Arme invalide, veuillez choisir une arme parmi " RESET);

                    // Affichage des armes disponibles
                    for (int j = 0; j < 4; j++)
                    {
                        printf(ROUGE " %c" RESET, joueur->armes[j]);
                    }

                    printf("> ");
                    scanf(" %c", &armeChoisie);
                }
            }
            // Index de l'arm du monstre choisie aléatoirement
            int indexArmeMonstre = rand() % monstreActuel.nbArmes;

            // Arme du monstre
            char armeMonstre = monstreActuel.armes[indexArmeMonstre];

            printf(BLEU "[INFO] %s(%c) attaque %s(%c)\n\n" RESET, joueur->pseudo, armeChoisie, monstreActuel.nom, armeMonstre);

            // Détermination du gagnant
            char gagnant = determinerGagnant(armeChoisie, armeMonstre);

            switch (gagnant)
            {
            // Si le joueur gagne
            case 'P':
            {
                // Réduction des points de vie du monstre
                monstreActuel.pv -= joueur->nbDegats;

                // Points gagnés par le joueur
                int pointsGagnes = 10;
                printf(VERT "\t[VICTOIRE] %s(%dptV) gagne contre %s(%dptV)\t\t+%dpts\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv, pointsGagnes);

                // Si le monstre est mort
                if (monstreActuel.pv <= 0)

                {
                    // On met les points du monstre mort vie à 0
                    monstreActuel.pv = 0;

                    // Points gagnés par le joueur
                    pointsGagnes = 50 * monstreActuel.niveau;

                    printf(VERT "\t[VICTOIRE] %s(%dptV) gagne contre %s(%dptV)\t\t+%dpts\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv, pointsGagnes);
                    pointsGagnes = pointsGagnes + 10;
                    printf(BLEU "\t[INFO] %s meurt sous le coup de l'attaque\t\t+%dpts\n\n\n" RESET, monstreActuel.nom, pointsGagnes);
                }
                // Ajout des points gagnés par le joueur
                pointsJoueur = pointsJoueur + pointsGagnes;
                break;
            }

            // Si le monstre gagne
            case 'M':
            {
                // Réduction des points de vie du joueur
                joueur->nbPv -= monstreActuel.degats;

                // Si le joueur est mort
                if (joueur->nbPv <= 0)
                {
                    // On met les points de vie du joueur à 0
                    joueur->nbPv = 0;

                    printf(ROUGE "\t[DEFAITE] %s(%dptV) perd contre %s(%dptV)\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);

                    printf(BLEU "[INFO] PERDU... Nombre de points acquis : %d\n\n" RESET, pointsJoueur);

                    
                    return pointsJoueur;
                }

                printf(ROUGE "\t[DEFAITE] %s(%dptV) perd contre %s(%dptV)\n\n" RESET,
                       joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);
                
                break;
            }

            // Si égalité
            case 'E':
            {
                printf(MAGENTA "\t[EGALITE] Aucun de %s(%dptV) et %s(%dptV) ne gagne l'attaque\n\n" RESET,
                       joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);
                break;
            }
            }
        }

        // Passage au monstre suivant
        nbMonstres--;
        index++;
    }
    
    return pointsJoueur;
}

int gameGroupe2(Joueur *joueur, Joueur joueursTab[], Monstre monstresTab[], int indexMonstresTab[], int nbMonstres, int pointsJoueur)
{
    printf("[CONTEXTE] Vous arrivez au bout du corridor, une plaine herbeuse apparaît. Malheureusement des monstres sortent de partout pour tous vous attaquer en même temps ou presque...\n\n");

    // Création de la file des monstres du groupe 2
    File file = fileVide();
    // Ajout des monstres du groupe 2 dans la file
    for (int i = 0; i < nbMonstres; i++)
    {
        file = ajouter(file, i);
    }

    // Tant qu'il reste des monstres dans la file
    while (longueur(file) > 0)
    {
        // Index du monstre actuel
        int monstreActuelIndex = tete(file);

        Monstre monstreActuel = monstresTab[indexMonstresTab[monstreActuelIndex]];

        printf(BLEU "[INFO] Le monstre %s(%dptV, %dAtt) s'avance pour vous attaquer %s(%dptV, %dAtt)\n\n" RESET,
               monstreActuel.nom, monstreActuel.pv, monstreActuel.degats, joueur->pseudo, joueur->nbPv, joueur->nbDegats);

        // Nombre d'arme du joueur
        int nbArmes = strlen(joueur->armes);

        printf(JAUNE "[ACTION] %s(%dPts) choisis ton arme parmi" RESET, joueur->pseudo, pointsJoueur);

        // Affichage des armes disponibles
        for (int j = 0; j < nbArmes; j++)
        {
            printf(" %c", joueur->armes[j]);
        }

        printf(" > ");

        // Arme choisie par le joueur
        char armeChoisie;
        scanf(" %c", &armeChoisie);

        int trouve = 0;

        while (trouve == 0)
        {
            trouve = rechercheArme(armeChoisie, joueur->armes, 4);

            if (trouve == 0)
            {
                printf(ROUGE "[ERREUR] Arme invalide, veuillez choisir une arme parmi " RESET);

                // Affichage des armes disponibles
                for (int j = 0; j < 4; j++)
                {
                    printf(ROUGE " %c" RESET, joueur->armes[j]);
                }

                printf(" > ");
                scanf(" %c", &armeChoisie);
            }
        }

        // Index de l'arme du monstre choisie aléatoirement
        int indexArmeMonstre = rand() % monstreActuel.nbArmes;

        // Arme du monstre
        char armeMonstre = monstreActuel.armes[indexArmeMonstre];
        printf(BLEU "[INFO] %s(%c) attaque %s(%c)\n\n" RESET, joueur->pseudo, armeChoisie, monstreActuel.nom, armeMonstre);

        // Détermination du gagnant
        char gagnant = determinerGagnant(armeChoisie, armeMonstre);

        switch (gagnant)
        {

        // Si le joueur gagne
        case 'P':
        {

            // Réduction des points de vie du monstre
            monstreActuel.pv = monstreActuel.pv - joueur->nbDegats;
            monstresTab[indexMonstresTab[monstreActuelIndex]].pv = monstreActuel.pv;

            // Points gagnés par le joueur
            int pointsGagnes = 10;
            printf(VERT "\t[VICTOIRE] %s(%dptV) gagne contre %s(%dptV)\t\t+%dpts\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv, pointsGagnes);

            // Si le monstre est mort
            if (monstreActuel.pv <= 0)
            {
                // On met les points du monstre mort vie à 0
                monstreActuel.pv = 0;

                // Points gagnés par le joueur, 100 fois le niveau du monstre
                pointsGagnes = 100 * monstreActuel.niveau;

                // Si le monstre est mort, on le retire de la file
                file = supprimerTete(file);
                printf(VERT "\t[VICTOIRE] %s(%dptV) gagne contre %s(%dptV)\t\t+%dpts\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv, pointsGagnes);
                pointsGagnes = pointsGagnes + 10;
                printf(BLEU "\t[INFO] %s meurt sous le coup de l'attaque\t\t+%dpts\n\n" RESET, monstreActuel.nom, pointsGagnes);
            }
            pointsJoueur += pointsGagnes;
            break;
        }

            // Si le monstre gagne

        case 'M':
        {
            // Réduction des points de vie du joueur
            joueur->nbPv -= monstreActuel.degats;

            // Si le joueur est mort
            if (joueur->nbPv <= 0)
            {

                // On met les points de vie du joueur à 0
                joueur->nbPv = 0;
                printf(ROUGE "\t[DEFAITE] %s(%dptV) perd l'attaque contre %s(%dptV)\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);
                printf(BLEU "[INFO] PERDU... Nombre de points acquis : %d\n\n" RESET, pointsJoueur);
          
                return pointsJoueur;
            }

            printf(ROUGE "\t[DEFAITE] %s(%dptV) perd contre %s(%dptV)\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);
            
            break;
        }

        // Si égalité
        case 'E':
        {
            printf(MAGENTA "\t[EGALITE] Aucun de %s(%dptV) et %s(%dptV) ne gagne l'attaque\n\n" RESET, joueur->pseudo, joueur->nbPv, monstreActuel.nom, monstreActuel.pv);
            break;
        }
        }

        // Si le monstre n'est pas mort, on le retire de la file et on le remet à la fin
        if (monstreActuel.pv > 0)
        {
            file = supprimerTete(file);
            file = ajouter(file, monstreActuelIndex);
        }
    }
    
    return pointsJoueur;
}

int PartiePredefinie(int *nbJoueurs, Joueur **joueursTab, Monstre monstresTab[], int indexMonstresTabGroupe1[], int indexMonstresTabGroupe2[], int nbMonstresGroupe1, int nbMonstresGroupe2)
{
    char pseudoJoueur[50];
    clearScreen();
    int index, trouve;
    printf(GRAS VERT "▁ ▂ ▄ ▅ ▆ ▇ █ Créer une nouvelle partie █ ▇ ▆ ▅ ▄ ▂ ▁\n\n" RESET);
    printf(JAUNE "[ACTION] Entrer votre pseudo > " RESET);
    scanf("%s", pseudoJoueur);
    index = rechercheDicoJoueur(pseudoJoueur, *joueursTab, *nbJoueurs, &trouve);
    if (trouve == 0)
    {
        // charger les données du joueur
        Joueur joueur;
        strcpy(joueur.pseudo, pseudoJoueur);
        joueur.nbPv = 20;
        joueur.nbDegats = 1;
        joueur.nbParties = 0;
        joueur.scores = NULL;
        strcpy(joueur.armes, "PFC");

        // realloc joueursTab
        *nbJoueurs = *nbJoueurs + 1;
        *joueursTab = (Joueur *)realloc(*joueursTab, (*nbJoueurs) * sizeof(Joueur));
        if (joueursTab == NULL)
        {
            printf("Erreur d'allocation mémoire\n");
            return -1;
        }

        // ajouter le joueur
        (*joueursTab)[*nbJoueurs - 1] = joueur;

        (*joueursTab)[*nbJoueurs - 1].nbParties++;

        int pointsGagnes = 0;
        // Jouer le premier groupe
        pointsGagnes = gameGroupe1(&(*joueursTab)[*nbJoueurs - 1], *joueursTab, monstresTab, indexMonstresTabGroupe1, nbMonstresGroupe1, pointsGagnes);
        sauvegarderScoreJoueur(*joueursTab, *nbJoueurs - 1, pointsGagnes);

        if ((*joueursTab)[*nbJoueurs - 1].nbPv > 0)
        {
            printf(BLEU "[INFO] Tous les monstres sont morts...\n\n" RESET);

            // Jouer le deuxième groupe
            pointsGagnes = gameGroupe2(&(*joueursTab)[*nbJoueurs - 1], *joueursTab, monstresTab, indexMonstresTabGroupe2, nbMonstresGroupe2, pointsGagnes);
            sauvegarderScoreJoueur(*joueursTab, *nbJoueurs - 1, pointsGagnes);

            if ((*joueursTab)[*nbJoueurs - 1].nbPv > 0)
                printf(VERT "[PARTIE GAGNEE] Bravo ! Vous avez survécu aux terribles monstres !!!\n" RESET);
            else
                printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);

            printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
            getchar();
            getchar();
        }
        else
        {
            printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);
            printf(GRAS "Appuyer sur entrée pour continuer...\n" RESET);
            getchar();
            getchar();
        }
        return *nbJoueurs - 1;
    }
    else
    {
        (*joueursTab)[index].nbParties++;
        int pointsGagnes = 0;
        // Jouer le premier groupe
        pointsGagnes = gameGroupe1(&(*joueursTab)[index], *joueursTab, monstresTab, indexMonstresTabGroupe1, nbMonstresGroupe1, pointsGagnes);
        sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);

        if ((*joueursTab)[index].nbPv > 0)
        {
            printf(BLEU "[INFO] Tous les monstres sont morts...\n\n" RESET);

            // Jouer le deuxième groupe
            pointsGagnes = gameGroupe2(&(*joueursTab)[index], *joueursTab, monstresTab, indexMonstresTabGroupe2, nbMonstresGroupe2, pointsGagnes);
            sauvegarderScoreJoueur(*joueursTab, index, pointsGagnes);

            if ((*joueursTab)[index].nbPv > 0)
                printf(VERT "[PARTIE GAGNEE] Bravo ! Vous avez survécu aux terribles monstres !!!\n" RESET);
            else
                printf(ROUGE "[PARTIE PERDUE] Vous avez été vaincu par les monstres de fou furieux...\n" RESET);

            printf("\nAppuyer sur entrée pour continuer...");
            getchar();
            getchar();
        }
    }
    return index;
}

void sauvegarderScoreJoueur(Joueur *joueursTab, int indexJoueur, int pointsGagnes)
{
    joueursTab[indexJoueur].scores = (int *)realloc(joueursTab[indexJoueur].scores, (joueursTab[indexJoueur].nbParties) * sizeof(int));
    joueursTab[indexJoueur].scores[joueursTab[indexJoueur].nbParties - 1] = pointsGagnes;
}

char determinerGagnant(char armeJoueur, char armeMonstre)
{
    if (armeJoueur == armeMonstre)
    {
        return 'E';
    }
    if (armeJoueur == '#')
    {
        return 'P';
    }
    if (armeMonstre == '#')
    {
        return 'M';
    }
    if (armeJoueur == 'O')
    {
        return 'M';
    }
    if (armeMonstre == 'O')
    {
        return 'P';
    }
    if (armeJoueur == 'P' && armeMonstre == 'C')
    {
        return 'P';
    }
    if (armeJoueur == 'C' && armeMonstre == 'F')
    {
        return 'P';
    }
    if (armeJoueur == 'F' && armeMonstre == 'P')
    {
        return 'P';
    }
    if (armeMonstre == 'P' && armeJoueur == 'C')
    {
        return 'M';
    }
    if (armeMonstre == 'C' && armeJoueur == 'F')
    {
        return 'M';
    }
    if (armeMonstre == 'F' && armeJoueur == 'P')
    {
        return 'M';
    }
    return 'E';
}

Monstre *loadMonstres(char *filenom, int *nbMonstres, int **indexMonstresGroupe1, int **indexMonstresGroupe2, int *nbMonstresGroupe1, int *nbMonstresGroupe2)
{
    FILE *file = fopen(filenom, "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    // Lecture du nombre de monstres (première ligne)
    fscanf(file, "%d", nbMonstres);

    if (*nbMonstres < 1)
    {
        printf("Erreur lors de la lecture du nombre de monstres\n");
        fclose(file);
        return NULL;
    }

    // Allocation dynamique pour le tableau des monstres
    Monstre *monstres = (Monstre *)malloc(*nbMonstres * sizeof(Monstre));
    if (monstres == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        fclose(file);
        return NULL;
    }

    // Lecture des monstres
    for (int i = 0; i < *nbMonstres; i++)
    {
        Monstre *monstre = &monstres[i];
        int j = 0;
        char c;

        // Lecture du nom du monstre
        fscanf(file, " %c", &c);
        while (c != ',')
        {
            // Vérification de la taille du nom
            if (j >= 50)
            {
                printf("Erreur lors de la lecture du nom du monstre\n");
                fclose(file);
                return NULL;
            }
            monstre->nom[j] = c;
            j++;
            fscanf(file, "%c", &c);
        }
        // Ajout du caractère de fin de chaîne
        monstre->nom[j] = '\0';

        // Vérification de la lecture du nom
        if (j == 0)
        {
            printf("Erreur lors de la lecture du nom du monstre\n");
            fclose(file);
            return NULL;
        }

        fscanf(file, "%d,%d,%d,%d",
               &monstre->niveau,
               &monstre->pv,
               &monstre->degats,
               &monstre->type);

        // Allocation dynamique pour les armes du monstre
        switch (monstre->niveau)
        {
        case 1:
            monstre->nbArmes = 4;
            monstre->armes = (char *)malloc(monstre->nbArmes * sizeof(char));
            if (monstre->armes == NULL)
            {
                printf("Erreur d'allocation mémoire\n");
                fclose(file);
                return NULL;
            }

            strcpy(monstre->armes, "PFCO");
            break;
        case 2:
            monstre->nbArmes = 3;
            monstre->armes = (char *)malloc(monstre->nbArmes * sizeof(char));
            if (monstre->armes == NULL)
            {
                printf("Erreur d'allocation mémoire\n");
                fclose(file);
                return NULL;
            }

            strcpy(monstre->armes, "PFC");
            break;
        case 3:
            monstre->nbArmes = 5;
            monstre->armes = (char *)malloc(monstre->nbArmes * sizeof(char));
            if (monstre->armes == NULL)
            {
                printf("Erreur d'allocation mémoire\n");
                fclose(file);
                return NULL;
            }

            strcpy(monstre->armes, "PFCO#");
            break;
        default:
            printf("Niveau de monstre invalide\n");
            fclose(file);
            return NULL;
        }

        switch (monstre->type)
        {
        case 1:
            *nbMonstresGroupe1 = *nbMonstresGroupe1 + 1;
            *indexMonstresGroupe1 = realloc(*indexMonstresGroupe1, *nbMonstresGroupe1 * sizeof(int));
            if (*indexMonstresGroupe1 == NULL)
            {
                printf("Erreur d'allocation mémoire\n");
                fclose(file);
                return NULL;
            }
            (*indexMonstresGroupe1)[*nbMonstresGroupe1 - 1] = i;

            break;
        case 2:
            *nbMonstresGroupe2 = *nbMonstresGroupe2 + 1;
            *indexMonstresGroupe2 = realloc(*indexMonstresGroupe2, *nbMonstresGroupe2 * sizeof(int));
            if (*indexMonstresGroupe2 == NULL)
            {
                printf("Erreur d'allocation mémoire\n");
                fclose(file);
                return NULL;
            }
            (*indexMonstresGroupe2)[*nbMonstresGroupe2 - 1] = i;

            break;
        }
    }
    fclose(file);
    return monstres;
}

// Fonction pour charger les joueurs depuis un fichier binaire
Joueur *loadJoueursFromBinary(char *filenom, int *nbJoueurs)
{
    // Ouvre le fichier binaire en mode lecture
    FILE *file = fopen(filenom, "rb");

    // Vérification de l'ouverture du fichier
    if (file == NULL)
    {
        clearScreen();
        printf("Fichier de sauvegarde non crée ou non ouvrable\n\n");

        // Variable pour la sauvegarde du choix de l'utilisateur
        char choice;

        // Demande à l'utilisateur s'il veut créer un nouveau fichier
        printf("Voulez-vous créer un nouveau fichier de sauvegarde ? (o/n) > ");

        // Lecture du choix de l'utilisateur
        scanf(" %c", &choice);

        // Si l'utilisateur ne veut pas créer de fichier, on retourne NULL
        if (choice == 'n' || choice == 'N')
        {
            exit(1);
        }

        // Sinon on crée un nouveau fichier
        file = fopen(filenom, "wb");
        if (file == NULL)
        {
            printf("Erreur lors de la création du fichier\n");
            exit(1);
        }

        // On écrit le nombre de joueurs (0) dans le fichier
        *nbJoueurs = 0;
        fwrite(nbJoueurs, sizeof(int), 1, file);

        // On ferme le fichier
        fclose(file);

        return NULL;
    }

    // Lecture du nombre de joueurs
    fread(nbJoueurs, sizeof(int), 1, file);

    // Vérification de la lecture du nombre de joueurs
    if (*nbJoueurs <= 0)
    {
        printf("Erreur lors de la lecture du nombre de joueurs\n");
        fclose(file);
        return NULL;
    }

    // Allocation dynamique pour le tableau des joueurs
    Joueur *joueurs = (Joueur *)malloc(*nbJoueurs * sizeof(Joueur));
    if (joueurs == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        fclose(file);
        return NULL;
    }

    // Lecture des données de chaque joueur
    for (int i = 0; i < *nbJoueurs; i++)
    {
        Joueur *joueur = &joueurs[i];

        // Lecture du pseudo
        fread(joueur->pseudo, sizeof(char), 50, file);

        // Lecture des caractéristiques simples
        fread(&joueur->nbPv, sizeof(int), 1, file);
        fread(&joueur->nbDegats, sizeof(int), 1, file);
        fread(&joueur->nbParties, sizeof(int), 1, file);

        if (joueur->nbParties > 0)
        {
            // Allocation dynamique pour les scores
            joueur->scores = (int *)malloc(joueur->nbParties * sizeof(int));

            // Vérification de l'allocation
            if (joueur->scores == NULL)
            {
                printf("Erreur d'allocation mémoire pour les scores\n");
                fclose(file);
                return NULL;
            }

            // Lecture des scores
            fread(joueur->scores, sizeof(int), joueur->nbParties, file);
        }

        // Si le joueur n'a pas encore joué de parties
        else
            joueur->scores = NULL;

        // Lecture des armes
        fread(joueur->armes, sizeof(char), 4, file);
    }
    fclose(file);
    return joueurs;
}

void saveJoueursToBinary(char *filenom, Joueur *joueurs, int nbJoueurs)
{
    // Ouvre le fichier binaire en mode écriture
    FILE *file = fopen(filenom, "wb");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    // Écrit le nombre de joueurs en premier
    fwrite(&nbJoueurs, sizeof(int), 1, file);

    // Parcourt chaque joueur et écrit ses données
    for (int i = 0; i < nbJoueurs; i++)
    {
        Joueur *joueur = &joueurs[i];

        // Écrit le pseudo
        fwrite(joueur->pseudo, sizeof(char), 50, file);

        // Écrit les caractéristiques simples
        fwrite(&joueur->nbPv, sizeof(int), 1, file);
        fwrite(&joueur->nbDegats, sizeof(int), 1, file);
        fwrite(&joueur->nbParties, sizeof(int), 1, file);

        // Écrit les scores
        fwrite(joueur->scores, sizeof(int), joueur->nbParties, file);

        // Écrit les armes
        fwrite(joueur->armes, sizeof(char), 4, file);
    }
    fclose(file);
}

void trierScoresJoueur(Joueur *joueur)
{
    // Vérifier que le joueur a des scores
    if (joueur->scores == NULL || joueur->nbParties <= 0)
    {
        printf("Le joueur n'a pas de scores à trier.\n");
        return;
    }

    // Tri des scores par ordre décroissant
    for (int i = 0; i < joueur->nbParties - 1; i++)
    {
        for (int j = 0; j < joueur->nbParties - 1 - i; j++)
        {
            if (joueur->scores[j] < joueur->scores[j + 1])
            {
                // Échanger les valeurs si nécessaire
                int temp = joueur->scores[j];
                joueur->scores[j] = joueur->scores[j + 1];
                joueur->scores[j + 1] = temp;
            }
        }
    }
}

void remplirIndexJoueursTriesParScore(Joueur *joueursTab, int nbJoueurs, int **indexJoueursTriesParScore)
{
    *indexJoueursTriesParScore = (int *)malloc(nbJoueurs * sizeof(int));

    if (indexJoueursTriesParScore == NULL)
    {
        printf("Erreur d'allocation mémoire\n");
        return;
    }

    // Initialisation du tableau d'index
    for (int i = 0; i < nbJoueurs; i++)
    {
        (*indexJoueursTriesParScore)[i] = i;
    }

    // Tri des index en fonction des scores
    for (int i = 0; i < nbJoueurs - 1; i++)
    {
        for (int j = i + 1; j < nbJoueurs; j++)
        {
            // Comparaison des scores
            if (joueursTab[(*indexJoueursTriesParScore)[i]].scores[0] < joueursTab[(*indexJoueursTriesParScore)[j]].scores[0])
            {
                // Échange des index si nécessaire
                int temp = (*indexJoueursTriesParScore)[i];
                (*indexJoueursTriesParScore)[i] = (*indexJoueursTriesParScore)[j];
                (*indexJoueursTriesParScore)[j] = temp;
            }
        }
    }
}

void afficherJoueursTriesParNom(Joueur *joueursTab, int nbJoueurs)
{
    clearScreen();
    printf("Pesudo -> Points\n\n");
    for (int i = 0; i < nbJoueurs; i++)
    {
        printf("%s -> %d\n", joueursTab[i].pseudo, joueursTab[i].scores[0]);
    }
    printf("\nAppuyer sur entrée pour continuer...");
    getchar();
    getchar();
}

void afficherJoueursTriesParScore(Joueur *joueursTab, int nbJoueurs, int indexJoueursTriesParScore[])
{
    clearScreen();
    printf("Pesudo -> Points\n\n");
    for (int i = 0; i < nbJoueurs; i++)
    {
        printf("%s -> %d\n", joueursTab[indexJoueursTriesParScore[i]].pseudo, joueursTab[indexJoueursTriesParScore[i]].scores[0]);
    }

    printf("\nAppuyer sur entrée pour continuer...");
    getchar();
    getchar();
}

float calculerScoreMoyen(Joueur joueur)
{
    int somme = 0;
    for (int i = 0; i < joueur.nbParties; i++)
    {
        somme += joueur.scores[i];
    }
    return somme / joueur.nbParties;
}

void afficherStatsJoueur(Joueur *joueursTab, int nbJoueurs)
{
    clearScreen();
    char pseudoJoueur[50];
    int index, trouve;
    printf("Entrez le pseudo du joueur dont vous voulez afficher les statistiques : ");
    scanf("%s", pseudoJoueur);

    // Recherche du joueur
    index = rechercheDicoJoueur(pseudoJoueur, joueursTab, nbJoueurs, &trouve);
    if (trouve == 0)
    {
        printf("Joueur introuvable\n");
        printf("Appuyer sur entrée pour continuer...");
        getchar();
        getchar();
    }
    else
    {
        printf("\nPseudo : %s\n", joueursTab[index].pseudo);
        printf("Points de vie : %d\n", joueursTab[index].nbPv);
        printf("Dégâts : %d\n", joueursTab[index].nbDegats);
        printf("Nombre de parties jouées : %d\n", joueursTab[index].nbParties);
        printf("Score minimum : %d\n", joueursTab[index].scores[joueursTab[index].nbParties - 1]);
        printf("Score moyen : %.2f\n", calculerScoreMoyen(joueursTab[index]));
        printf("Score maximum : %d\n", joueursTab[index].scores[0]);
        printf("\n");
        printf("Armes : %s\n", joueursTab[index].armes);

        printf("\nAppuyer sur entrée pour continuer...");
        getchar();
        getchar();
    }
}

void showAllPlayers(Joueur *joueursTab, int nbJoueurs)
{
    printf("Pseudo\n\n");
    for (int i = 0; i < nbJoueurs; i++)
    {
        printf("%s\n", joueursTab[i].pseudo);
    }

    printf("Appuyer sur entrée pour continuer...");
    getchar();
    getchar();
}

void insererJoueurOrdreAlphabetique(Joueur **joueursTab, int *nbJoueurs, int indexTemp)
{
    // Stocker temporairement le joueur à déplacer
    Joueur joueurTemp = (*joueursTab)[indexTemp];
    int indexOuInserer, trouve;

    // Recherche de l'index où insérer le joueur
    indexOuInserer = rechercheDicoJoueur(joueurTemp.pseudo, *joueursTab, *nbJoueurs, &trouve);

    // Si le joueur est déjà à la bonne position, rien à faire
    if (indexTemp == indexOuInserer)
    {
        return;
    }

    // Décaler les joueurs pour insérer à la bonne position
    if (indexTemp < indexOuInserer)
    {
        // Décaler les joueurs vers la gauche si le joueur doit aller après
        for (int i = indexTemp; i < indexOuInserer - 1; i++)
        {
            (*joueursTab)[i] = (*joueursTab)[i + 1];
        }
    }
    else if (indexTemp > indexOuInserer)
    {
        // Décaler les joueurs vers la droite si le joueur doit aller avant
        for (int i = indexTemp; i > indexOuInserer; i--)
        {
            (*joueursTab)[i] = (*joueursTab)[i - 1];
        }
    }

    /// Insérer le joueur à la bonne position
    (*joueursTab)[indexOuInserer] = joueurTemp;
}

void global(void)
{
    Boolean quit = False;
    int choice, nbJoueurs, nbMonstres;
    int *indexMonstresGroupe1 = NULL;
    int *indexMonstresGroupe2 = NULL;
    int nbMonstresGroupe1 = 0;
    int nbMonstresGroupe2 = 0;
    int indexTemp;

    // Chargement des données
    Joueur *joueursTab = loadJoueursFromBinary("game.dat", &nbJoueurs);
    Monstre *monstresTab = loadMonstres("monstres.txt", &nbMonstres, &indexMonstresGroupe1, &indexMonstresGroupe2, &nbMonstresGroupe1, &nbMonstresGroupe2);
    int *indexJoueursTriesParScore = NULL;

    while (!quit)
    {
        clearScreen();
        printf(GRAS VERT "\n\t▁ ▂ ▄ ▅ ▆ ▇ █  MENU  █ ▇ ▆ ▅ ▄ ▂ ▁\n\n\n" RESET);
        printf("[1] Jouer une partie prédéfinie\n");
        printf("[2] Créer une nouvelle partie\n");
        printf("[3] Afficher la liste des joueurs triée par nom\n");
        printf("[4] Afficher la liste des joueurs triée par meilleur score\n");
        printf("[5] Afficher les statistiques d'un joueur précis\n");

        printf("[9] Quitter et sauvegarder\n\n");
        printf("Votre choix > ");
        scanf(" %d", &choice);
        switch (choice)
        {
        case 1:
            indexTemp = PartiePredefinie(&nbJoueurs, &joueursTab, monstresTab, indexMonstresGroupe1, indexMonstresGroupe2, nbMonstresGroupe1, nbMonstresGroupe2);
            joueursTab[indexTemp].nbPv = 20;
            trierScoresJoueur(&joueursTab[indexTemp]);
            insererJoueurOrdreAlphabetique(&joueursTab, &nbJoueurs, indexTemp);
            break;
        case 2:
            indexTemp = CreerPartie(&joueursTab, &nbJoueurs);
            joueursTab[indexTemp].nbPv = 20;
            trierScoresJoueur(&joueursTab[indexTemp]);
            insererJoueurOrdreAlphabetique(&joueursTab, &nbJoueurs, indexTemp);
            break;
        case 3:
            afficherJoueursTriesParNom(joueursTab, nbJoueurs);
            break;
        case 4:
            remplirIndexJoueursTriesParScore(joueursTab, nbJoueurs, &indexJoueursTriesParScore);
            afficherJoueursTriesParScore(joueursTab, nbJoueurs, indexJoueursTriesParScore);
            break;
        case 5:
            afficherStatsJoueur(joueursTab, nbJoueurs);
            break;
        case 6:
            showAllPlayers(joueursTab, nbJoueurs);
            break;
        case 9:
            printf("Au revoir...\n");
            quit = True;
            break;
        case 666:
            printf("Tu t'es fait troll AHHAHA\n");
            printf("Appuyer sur entrée pour continuer...");
            getchar();
            getchar();
            break;
        default:
            printf("Choix invalide\n");

            printf("Appuyer sur entrée pour continuer...");
            getchar();
            getchar();
            break;
        }
        choice = 0;
    }

    saveJoueursToBinary("game.dat", joueursTab, nbJoueurs);

    for (int i = 0; i < nbJoueurs; i++)
    {
        free(joueursTab[i].scores);
    }

    for (int i = 0; i < nbMonstres; i++)
    {
        free(monstresTab[i].armes);
    }

    free(joueursTab);
    free(monstresTab);
    free(indexMonstresGroupe1);
    free(indexMonstresGroupe2);
    free(indexJoueursTriesParScore);
}

void clearScreen(void)
{
    for (int i = 0; i < 20; i++)
    {
        printf("\n");
    }
}
