#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "shm_const.h"

extern int creer_initialiser_semaphore();
extern int *attacher_segment_memoire();
extern int P();
extern int V();

extern int creer_segment_memoire();

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s nb_caisses titre_film nb_places\n", argv[0]);
        return 3;
    }

    int nb_caisse = atoi(argv[1]);
    char *titre_film = argv[2];
    int nb_places = atoi(argv[3]);

    pid_t pid_afficheur;
    pid_t pid_caisse;
    int i;

    int shmid, semid;
    int *mem;

    char shmid_str[20], semid_str[20], places_restantes_str[20];

    /* Création et initialisation des IPC */
    creer_initialiser_semaphore(CLE_SEM, &semid);
    creer_segment_memoire(CLE_SHM, &shmid);
    mem = attacher_segment_memoire(mem, &shmid);
    *mem = nb_places;

    /* Conversion des identifiants en chaînes */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);

    /* Lancer le processus afficheur */
    pid_afficheur = fork();
    if (pid_afficheur == -1)
    {
        perror("Erreur fork pour l'afficheur");
        return 1;
    }

    if (pid_afficheur == 0)
    {
        execl("afficheur", "afficheur", shmid_str, semid_str, NULL);
        perror("Erreur execl afficheur");
        exit(1);
    }
    if (pid_afficheur > 0)
    {
        /* Lancer les processus caisses */
        for (i = 0; i < nb_caisse; i++)
        {
            pid_caisse = fork();
            if (pid_caisse == -1)
            {
                perror("Erreur fork pour une caisse");
                return 1;
            }

            if (pid_caisse == 0)
            {
                char id_caisse[10];
                sprintf(id_caisse, "%d", i + 1);
                execl("caisse", "caisse", shmid_str, semid_str, id_caisse, NULL);
                perror("Erreur execl caisse");
                exit(1);
            }
        }
    }

    /* Attendre la fin des processus caisses */
    for (i = 0; i < nb_caisse; i++)
    {
        wait(NULL);
    }

    /* Attendre la fin de l'afficheur */
    waitpid(pid_afficheur, NULL, 0);

    printf("Simulation terminée : %s, %d places initiales.\n", titre_film, nb_places);
    return 0;
}