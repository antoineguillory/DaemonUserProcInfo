#ifndef SERVER_H
#define SERVER_H

#include "global_server.h"

/*
 * init_fifo_server va initialiser la variable globale fifo_fd.
 *  si le fifo existe déjà, il est détruit et recréé.
 */
void init_fifo_server(void);

/* On renvoie la réponse lue à travers le pipe de lecture p_read
 *  qui est allouée dynamiquement.
 *  Si la fonction échoue, renvoie NULL.
 */
char *read_response(int p_read);

/*
 * 
void exec_request(request *r);

/*
 * Traitement de la requète : on effectue la commande demandée.
 * Si la fonction réussie, le client reçoit la réponse.
 * Sinon le serveur et le client de la requète sont fermés.
void treatment_request(request *r);

/*
 * On crée un nouveau fil qui va traiter la requète r.
 *  Si la fonction réussi, le serveur indique d'où vient la requète.
 *  Sinon le serveur et le client de la requète sont fermés.
 */
void create_thread_request(request *r);

/*
 * Fonction d'introduction au serveur.
 */
void greet_user(void);

/*
 * Fonction de fermeture du serveur.
 *  On ferme et détruit le fifo.
void close_server(void);

/*
 * Fonction de gestion des signaux.
 */
void manage_signals(void);

#endif   //SERVER_H
