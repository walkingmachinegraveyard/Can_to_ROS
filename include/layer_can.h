#ifndef LAYER_CAN_H_
#define LAYER_CAN_H_
/**
 * @file layer_can.h
 * @brief Low-level of interfacin between middle_buffer and CAN
 * @author Alexandre Salconi-Denis
 * @date 25 september 2017
 * @version 0.1
 */

/**
 * Fonction pour savoir combien de user il y a sur le can
 * @return la valeur numerique 0 etant aucun
 */
uint16_t get_number_CanUser(void);

/**
 * Permet de savoir combien de thread est recus par ce user
 * @param userId Le numero du user, si cest invalide un erreur sera retourner
 * @return return la valeur numerique, 0 etant aucun ou erreur
 */
uint16_t get_number_Thread(uint16_t userId);

/**
 * Permet de savoir combien de message est contenue dans le thread recus par ce user
 * @param userId Le numero du user, si cest invalide un erreur sera retourner
 * @param threadId Le numero du thread, si cest invalide un erreur sera retourner
 * @return return la valeur numerique, 0 etant aucun ou erreur
 */
uint16_t get_number_Message(uint16_t userId, uint16_t threadId);

//a enlever au final mais CAN_MAX_DLEN eguale 8

/**
 * Permet de recevoir le dernier message recus. Bien faire attention que le payload soit un
 * tableau de taille CAN_MAX_DLEN 
 * @param userId Le numero du user, si cest invalide un erreur sera retourner
 * @param threadId Le numero du thread, si cest invalide un erreur sera retourner
 * @param payload Le pointer permetant de transmetre le message
 * @return renvois le nombre de byte transmis, 0 etant erreur
 */
uint8_t get_Message(uint16_t userId, uint16_t threadId, uint8_t *payload);

/**
 * @todo Voir si on fait une fonction pour lister les user dans un tableau.
 * Voir si on fait une fonction pour lister les thread d'un user dans un tableau.
 * Voir si on fait une fonction pour lister les message d'un thread dans un tableau.
 */
 
 /**
  * @todo faire les code d'erreur pour cette fonction et voir si on doit pas la modifier
  */
  
 /**
  * Permet d'envoyer un message vers un user et un thread specifique
  * @param userId Le userId a qui sadresse le message
  * @param threadId Le threadId a qui sadresse le message
  * @param payloadLenght La taille du message
  * @param payload Le tableau contenant le message
  * @return Renvois 0 si tout cest bien passer sinon renvois un code d'erreur
  */
uint8_t   send_message_to(uint16_t userId, uint16_t threadId,uint8_t payloadLenght, uint8_t *payload);

/**
  * @todo Peut etre faire une fonction attatch pour binder une fonction qui serait appler a chaque nouveau message de la part dun user, thread, ou general
  */

#endif /* LAYER_CAN_H_ */
