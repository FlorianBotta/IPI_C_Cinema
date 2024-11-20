#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
/******************************************************************************/
// Fonctions externes
/******************************************************************************/ 
extern int * attacher_segment_memoire();
 
/******************************************************************************/
// Programme principal
/******************************************************************************/ 
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s shmid semid\n", argv[0]);
        return 1;
    }
 
    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);
    int *mem;
 
    /* Attacher le segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);
 
    printf("Afficheur [%d]: Démarrage de l'affichage des places restantes.\n", getpid());
 
    while (1) {
        sleep(5);  // Mise à jour toutes les 5 secondes
        printf("Afficheur [%d]: Places restantes : %d\n", getpid(), *mem);
 
        /* Vérification de fin */
        if (*mem == 0) {
            printf("Afficheur [%d]: Plus de places disponibles. Arrêt de l'afficheur.\n", getpid());
            break;
        }
    }
 
    printf("Afficheur [%d]: Terminé.\n", getpid());
    return 0;
}