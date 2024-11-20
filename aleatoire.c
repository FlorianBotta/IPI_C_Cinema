#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DELAI_MAX 3  /* Temps d'attente maximal en secondes */
#define PLACES_MAX 7 /* Nombre maximal de places par client */

/* Fonction pour générer un délai aléatoire */
void attente_aleatoire()
{
    /* Initialisation du générateur de nombres aléatoires */
    srandom(time(NULL) ^ getpid());

    /* Attente d'un délai aléatoire entre 1 et DELAI_MAX secondes */
    sleep((unsigned int)((random() % DELAI_MAX) + 1));
}

/* Fonction pour générer un nombre aléatoire de places à acheter */
int places_aleatoires()
{
    /* Initialisation du générateur de nombres aléatoires */
    srandom(time(NULL) ^ getpid());

    /* Retourne un nombre aléatoire entre 1 et PLACES_MAX */
    return (int)((random() % PLACES_MAX) + 1);
}