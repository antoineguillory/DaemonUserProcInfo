#ifndef CLIENT_H
#define CLIENT_H

#include "global_server.h"

#define CLIENT_HEADER  "[CLIENT] :"
#define CLIENT_VERSION "1.0"

#define SHM_NAME "/shm_"
#define SEM_NAME "/sem_"
#define NB_NUMBER_OF_SHM_SEM_NAME 8

/*
 * Ouvre le tube de communication avec le serveur.
 *  Initialise les noms de la mémoire partagée et de la sémaphore
 *  grâce à l'id.
 */
void init_client(char *id);

/*
 * On renvoie la requète conforme aux valeurs communiquées par
 *  l'utilisateur.
 *  Si l'une des valeurs n'est pas conforme, on renvoie NULL.
 */
request *extract_request();

/*
 * Fonction de renseignement des commandes disponibles.
 */
void print_help(void);

/*
 * Fonction d'introduction au client.
 */
void greet_user(char *id);

/*
 * On ferme le tube de communication avec le serveur,
 *  on supprime la mémoire partagée et la sémaphore et on libère
 *  la mémoire alloué pour les chaînes de caractères.
 */
void close_client();

/*
 * Fonction de gestion des signaux.
 */
void manage_client_signals(void);

#endif  //CLIENT_H
