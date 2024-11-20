#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
 
/******************************************************************************/
// Fonctions externes
/******************************************************************************/ 
extern void attente_aleatoire();
extern int places_aleatoires();
extern int * attacher_segment_memoire();
extern int P();
extern int V();
 
/******************************************************************************/
// Programme principal
/******************************************************************************/ 
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage : %s shmid semid caisse_numero\n", argv[0]);
        return 1;
    }
 
    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);
    int caisse_numero = atoi(argv[3]);
    int *mem;
 
    /* Attacher le segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);
 
    printf("Caisse n° %d [%d]: Démarrage.\n", caisse_numero, getpid());
 
    while (1) {
        attente_aleatoire();
 
        /* Générer un nombre aléatoire de places demandées */
        int places_demandees = places_aleatoires();
        bool places_vendues = false;
 
        /* Protéger l'accès à la mémoire partagée */
        P(semid);
 
        /* Vérifier les places disponibles */
        if (*mem >= places_demandees) {
            *mem -= places_demandees;
            printf("Caisse n° %d [%d]: %d places vendues, %d restantes.\n", caisse_numero, getpid(), places_demandees, *mem);
            places_vendues = true;
        } else if (*mem == 0) {
            printf("Caisse n° %d [%d]: Plus de places disponibles. Fermeture.\n", caisse_numero, getpid());
            V(semid);
            break;
        }
 
        /* Libérer l'accès à la mémoire partagée */
        V(semid);
 
        /* Si les places n'ont pas été vendues, attendre avant de réessayer */
        if (!places_vendues) {
            sleep(1);
        }
    }
 
    printf("Caisse n° %d [%d]: Terminé.\n", caisse_numero, getpid());
    return 0;
}