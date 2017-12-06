#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "../headers/service.h"
#include "../headers/color.h"
#define SIZE_MSG 1024

/**
 * @file service.c
 * @date 5 Dec 2017
 * @brief Fichier permettant la création d'un nouveau service
 *
 * @see ../headers/service.h
 */

 /**
  * @fn void nouveauService(int descripteurSocketService)
  * @brief Crée un nouveau service
  * @param descripteurSocketService descripteur de fichier associé au nouveau service, il est créé lorsque le serveur reçoit une nouvelle connexion
  * @TODO créer la communication read write pour effectuer des actions sur le serveur
  */
void nouveauService(int descripteurSocketService)
{
	// Rechercher par :
	// 					- ville de départ
	//					- ville d'arrivée
	//					- horaire de départ (train en question si l'heure est exacte, sinon le suivant)
	//					- tranche horaire (départ, obtention d'une liste)
	char commandeRecu[SIZE_MSG];
	char commandeAEnvoyer[SIZE_MSG];
	int pid = getpid();
	printf("nouveauService ok : %d\n",pid);
	int sizeRead;
	strcpy(commandeAEnvoyer, "noread;___________________________________________________________________\n       /\\                    /\\															 \n   ____\\/____============____\\/___    ___==========================\n /__|     OOOOOOOOOOOOO    [_]   |    |  |[]|  [_]    [_]    [_] \n/             S N C F            |    |  |  |										 \n\\________________________________|_ii_|__|__|______________________\n   ()==()    === ++++ ===  ()==()       ()==()     +++   ++++++++\n===================================================================\n\n");
	envoyerMessage(descripteurSocketService, commandeAEnvoyer);
	do{
		printf("%d "MAG"MENU"RESET"\n", pid);
		strcpy(commandeAEnvoyer,"---"CYN"MENU"RESET" ---\n 1: Recherche du premier train a partir de l'heure de depart \n 2: Recherche de trains dans une tranche horaire\n 3: Recherche tous les trains pour une ville de depart et d'arrivee\n Choix : ");
		envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);

		//TODO voir en fonction du protocole d'échange
		int commande = atoi(commandeRecu);
		printf("La commande que veux effectuer le client est %d\n", commande);
		int h,m, h2,m2;
		switch (commande) {
			case 1:
			//Fonction 1 : Ville de départ + ville d'arrivée +  horaire de départ TODO faire les fonctions par ici
				printf("%d "MAG"CHOIX DEPART"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville de de depart : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut partir de : %s (taille = %d)\n", commandeRecu, sizeRead);

				printf("%d "MAG"CHOIX ARRIVEE"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville d'arrivee : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut aller a : %s (taille = %d)\n", commandeRecu, sizeRead);


				printf("%d "MAG"CHOIX HORAIRE"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure de depart (HH:MN) : ");
				choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h,&m, pid);
				printf("Le client veut partir a partir de : %d:%d\n", h,m);

				break;
			case 2:
			//Fonction 2 : ville de départ + ville d'arrivée + tranche horaire pour le départ TODO faire les fonctions par ici
				printf("%d "MAG"CHOIX DEPART"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville de de depart : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut partir de : %s (taille = %d)\n", commandeRecu, sizeRead);

				printf("%d "MAG"CHOIX ARRIVEE"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville d'arrivee : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut aller a : %s (taille = %d)\n", commandeRecu, sizeRead);


				printf("%d "MAG"CHOIX HORAIRE 1/2"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure de debut (HH:MN) : ");
				choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h,&m, pid);

				printf("Le client veut partir a partir de : %d:%d\n", h,m);


				printf("%d "MAG"CHOIX HORAIRE 2/2"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure de fin (HH:MN) : ");
				choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h,&m, pid);
				printf("Le client veut partir a partir de : %d:%d\n", h2,m2);

			break;
			case 3:
			//Fonction 3 : Ville de départ + arrivée = renvoi une liste TODO faire les fonctions par ici
				printf("%d "MAG"CHOIX DEPART"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville de de depart : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut partir de : %s (taille = %d)\n", commandeRecu, sizeRead);

				printf("%d "MAG"CHOIX ARRIVEE"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville d'arrivee : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				printf("Le client veut aller a : %s (taille = %d)\n", commandeRecu, sizeRead);
			break;
			default:
				printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "noread;\n"RED"Mauvais choix dans le menu, veuillez reessayer"RESET"\n");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
	}
}while(strcmp(commandeRecu,"stop") != 0 && sizeRead != 0); //Si sizeRead == 0 le client n'envoie plus rien, est-il mort ?
	printf("Communication terminé\n");
	exit(-1);
}

void envoyerMessage(int descripteurSocketService, char *commandeAEnvoyer)
{
	write(descripteurSocketService, commandeAEnvoyer, SIZE_MSG);
}

void recevoirMessage(int descripteurSocketService, char *commandeRecu)
{
	int sizeRead;
	sizeRead = read(descripteurSocketService, commandeRecu, SIZE_MSG);
	printf("Commande reçu du client : %s (taille = %d)\n", commandeRecu, sizeRead);
}

void choixHoraire(int descripteurSocketService, char *commandeRecu, char *commandeAEnvoyer, int *h,int *m,int pid)
{
	int valide = 0;
	do
	{
		envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);


		char *token1, *token2, *texte1, *texte2;
		char *str = commandeRecu;

		token1 = strsep(&str, ":");
		token2 = strsep(&str, ":");
		if(token2 != NULL)
		{
			*h = (int) strtol(token1, &texte1, 10);
			*m = (int) strtol(token2, &texte2, 10);
			printf("%d:%d\n",*h,*m);
			printf("%s\n",texte1);
			if(strcmp(texte1,"") == 0 && strcmp(texte2,"") == 0 && *h>=0 && *h<24 && *m>=0 && *m<60){
				printf("ok");
				valide =1;
			}
			else
			{
				printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "noread;\n"RED"Mauvais choix des horaires : ex : 10:10"RESET"\n");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
			}
		}
		else
		{
			printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
			strcpy(commandeAEnvoyer, "noread;\n"RED"Mauvais choix des horaires : ex : 10:10"RESET"\n");
			envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		}

	}
	while(valide == 0);

}
