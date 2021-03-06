#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "../headers/temps.h"
#include "../headers/service.h"
#include "../headers/color.h"
#include "../headers/requetes.h"
#include "../headers/train.h"
#include "../headers/outils.h"

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
	// PID du process courant
	int pid = getpid();
	printf("nouveauService ok : %d\n",pid);
	int sizeRead;
	int connecter = 0;

	//Recuépration du fichier Trains.txt a chaque client
	//Cela permet que chaque client ets le dernier fichier bien a jour

	char *nomFichier = "../ressources/Trains.txt";
	Train *listeTrain;
	int nbTrain;
	listeTrain = trainFromFile(nomFichier, &nbTrain); // Récupération de la liste de train
	char *listeVilleDepart[nbTrain];
	char *listeVilleArrive[nbTrain];
	for(int iterateurTrain = 0; iterateurTrain < nbTrain; iterateurTrain++){
		listeVilleDepart[iterateurTrain] = (listeTrain + iterateurTrain)->villeDepart;
		strToUpper(listeVilleDepart[iterateurTrain]);
		listeVilleArrive[iterateurTrain] = (listeTrain + iterateurTrain)->villeArrivee;
		strToUpper(listeVilleArrive[iterateurTrain]);
	}

	//Affichage d'un petit train et envoie du message au client
	strcpy(commandeAEnvoyer, "noread;___________________________________________________________________\n       /\\                    /\\															 \n   ____\\/____============____\\/___    ___==========================\n /__|     OOOOOOOOOOOOO    [_]   |    |  |[]|  [_]    [_]    [_] \n/             S N C F            |    |  |  |										 \n\\________________________________|_ii_|__|__|______________________\n   ()==()    === ++++ ===  ()==()       ()==()     +++   ++++++++\n===================================================================\n\n");
	envoyerMessage(descripteurSocketService, commandeAEnvoyer);
	do{
		printf("%d "MAG"MENU"RESET"\n", pid);
		// Envoie du menu au client
		strcpy(commandeAEnvoyer,"---"CYN"MENU"RESET" ---\n 1: Recherche du premier train a partir de l'heure de depart \n 2: Recherche de trains dans une tranche horaire\n 3: Recherche tous les trains pour une ville de depart et d'arrivee\n 42 : Connexion en administrateur\nChoix : ");
		envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);

		// Lecture de la commande reçu et gestion des differents cas
		int commande = atoi(commandeRecu);
		printf("La commande que veux effectuer le client est %d\n", commande);
		char* villeDepart;
		char* villeArrivee;
		int h,m, h2,m2;
		switch (commande) {
			case 1:
				//Fonction 1 : Ville de départ + ville d'arrivée +  horaire de départ
				// Envoie et reception des informations a propos de la ville de depart
				demanderVille(descripteurSocketService, commandeRecu, commandeAEnvoyer, &villeDepart,&villeArrivee, pid, nbTrain, listeVilleDepart, listeVilleArrive);

				// Envoie et reception des informations a propos de la ville d'arrivee

				if(strcmp(villeArrivee, villeDepart) == 0)
				{
					printf("%d "RED"MAUVAIS ENTREE UTILISATEUR "MAG"VILLES IDENTIQUES"RESET"\n", pid);
					envoyerMessage(descripteurSocketService, "noread;"RED"Les villes sont identiques."RESET"\n\n");
					break;
				}

					printf("%s %s\n", villeDepart, villeArrivee);
					// Envoie et reception des informations a propos de l'horaire
					printf("%d "MAG"CHOIX HORAIRE"RESET"\n", pid);
					strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure de depart (HH:MN) : ");
					choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h,&m, pid);
					printf("Le client veut partir a partir de : %d:%d\n", h,m);

					trouverTrainLePlusProche(listeTrain, nbTrain, villeDepart, villeArrivee, h, m, commandeAEnvoyer);
					envoyerMessage(descripteurSocketService, commandeAEnvoyer);
					envoyerMessage(descripteurSocketService, "Voulez vous retourner au menu ou quitter?\n 1 : Retourner au menu\n 2 : Quitter\n Choix : ");
					recevoirMessage(descripteurSocketService, commandeRecu);
					if(atoi(commandeRecu)==2)
					{
						envoyerMessage(descripteurSocketService,"stop");
						close(descripteurSocketService);
						exit(0);
					}


				//free(t);
				break;
			case 2:
			//Fonction 2 : ville de départ + ville d'arrivée + tranche horaire pour le départ
			// Envoie et reception des informations a propos de la ville de depart
			demanderVille(descripteurSocketService, commandeRecu, commandeAEnvoyer, &villeDepart,&villeArrivee, pid, nbTrain, listeVilleDepart, listeVilleArrive);



				if(strcmp(villeDepart, villeArrivee) == 0)
				{
					printf("%d "RED"MAUVAIS ENTREE UTILISATEUR "MAG"VILLES IDENTIQUES"RESET"\n", pid);
					envoyerMessage(descripteurSocketService, "noread;"RED"Les villes sont identiques."RESET"\n");
					break;
				}

				// Envoie et reception des informations a propos de l'horaire de debut
				printf("%d "MAG"CHOIX HORAIRE 1/2"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure de debut (HH:MN) : ");
				choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h,&m, pid);
				printf("Le client veut partir a partir de : %d:%d\n", h,m);

				printf("%d "MAG"CHOIX HORAIRE 2/2"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "\nVeuillez entrer l'heure d'arrivee (HH:MN) : ");
				choixHoraire(descripteurSocketService, commandeRecu, commandeAEnvoyer, &h2,&m2, pid);
				printf("Le client veut partir a partir de : %d:%d\n", h2,m2);

				int compteLigneRequete2;
				compteLigneRequete2 = nbTrain;
				strToUpper(villeDepart);
				strToUpper(villeArrivee);
				trouverTrainParTranche(listeTrain, &compteLigneRequete2, villeDepart, villeArrivee, h, m, h2, m2, commandeAEnvoyer);

				printf("\n");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				envoyerMessage(descripteurSocketService, "Voulez vous retourner au menu ou quitter?\n 1 : Retourner au menu\n 2 : Quitter\n Choix : ");
				recevoirMessage(descripteurSocketService, commandeRecu);
				if(atoi(commandeRecu)==2)
				{
					envoyerMessage(descripteurSocketService,"stop");
					close(descripteurSocketService);
					exit(0);
				}

			break;
			case 3:
				//Fonction 3 : Ville de départ + arrivée = renvoi une liste
				// Envoie et reception des informations a propos de la ville de depart
				demanderVille(descripteurSocketService, commandeRecu, commandeAEnvoyer, &villeDepart,&villeArrivee, pid, nbTrain, listeVilleDepart, listeVilleArrive);

				if(strcmp(villeArrivee, villeDepart) == 0)
				{
					printf("%d "RED"MAUVAIS ENTREE UTILISATEUR "MAG"VILLES IDENTIQUES"RESET"\n", pid);
					envoyerMessage(descripteurSocketService, "noread;"RED"Les villes sont identiques."RESET"\n");
					break;
				}

				// Affichage de la liste de tous les trains satisfaisant les critères ville de départ et ville d'arrivée
				strToUpper(villeDepart);
				strToUpper(villeArrivee);
				int compteLigneRequete3;
				compteLigneRequete3 = nbTrain;

				fprintf(stderr,"Compte LIGNE :%d\n", compteLigneRequete3);
				Train * lstTrainParVille = malloc(sizeof(Train));
				lstTrainParVille = listeTrainParVille(listeTrain, &compteLigneRequete3, villeDepart, villeArrivee, commandeAEnvoyer);
				//fprintf(stderr, "TEEEST : %s\n", commandeAEnvoyer);
				//envoyerMessage(descripteurSocketService, commandeAEnvoyer);


				// Demander s'il veut le trajet le moins cher ou le plus court
				/* Affichage du train qui répond soit :
					- Au trajet au meilleur prix (reduction comprise) critere = 1
					- Au trajet de durée optimum critere = 2*/
				strcat(commandeAEnvoyer, "\nQuel critere faut-il appliquer ?\n 1 : Meilleur prix\n 2 : Trajet le plus court\n Choix : ");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				recevoirMessage(descripteurSocketService, commandeRecu);
				int critere = 0;
				switch(atoi(commandeRecu))
				{
					case 1:
						//Trajet au meilleur prix
						critere = 1;
						break;
					case 2:
						//trajet au court
						critere = 2;
						break;
					default:
						critere = 0;

				}
				if(critere !=0)
				{
					printf("Critere : %d\n", critere);
					trajetSelonCritere(lstTrainParVille, compteLigneRequete3, critere, commandeAEnvoyer);
					fprintf(stderr, "%s\n", commandeAEnvoyer);
					envoyerMessage(descripteurSocketService, commandeAEnvoyer);
				}
				else
				{
					envoyerMessage(descripteurSocketService, "noread;"RED"Mauvais choix du critere."RESET"\n");
				}

				envoyerMessage(descripteurSocketService, "Voulez vous retourner au menu ou quitter?\n 1 : Retourner au menu\n 2 : Quitter\n Choix : ");
				recevoirMessage(descripteurSocketService, commandeRecu);
				if(atoi(commandeRecu)==2)
				{
					envoyerMessage(descripteurSocketService,"stop");
					close(descripteurSocketService);
					exit(0);
				}

			break;
			case 42:

				do {
					printf("test\n");
					envoyerMessage(descripteurSocketService, "login : ");
					recevoirMessage(descripteurSocketService, commandeRecu);
					char * login = strdup(commandeRecu);
					envoyerMessage(descripteurSocketService, "password : ");
					recevoirMessage(descripteurSocketService, commandeRecu);
					char * pass = strdup(commandeRecu);

					if(strcmp(login,"admin") == 0 && strcmp(commandeRecu,"root") == 0)
					{
						connecter =1;
						envoyerMessage(descripteurSocketService, "noread;"CYN"Bienvenue admin"RESET"\n\n\n");
						envoyerMessage(descripteurSocketService, "Voulez vous retourner au menu ou quitter?\n 1 : Retourner au menu\n 2 : Quitter\n Choix : ");
					}
					else
					{
						envoyerMessage(descripteurSocketService, "noread;"RED"Erreur d'authentification"RESET"\n\n\n");
					}
				} while(connecter == 0);


				break;
			default:
				//Erreur dans une entree client, on retourne au menu
				printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "noread;\n"RED"Mauvais choix dans le menu, veuillez reessayer"RESET"\n");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
	}
}while(strcmp(commandeRecu,"stop") != 0); //Si sizeRead == 0 le client n'envoie plus rien, est-il mort ?
	printf("Communication terminé\n");
	printf("%d\n",sizeRead);
	exit(-1);
}


/**
 * @fn void envoyerMessage(int descripteurSocketService, char *commandeAEnvoyer)
 * @brief Envoie un message au client
 * @param descripteurSocketService descripteur de fichier associé au nouveau service, il est créé lorsque le serveur reçoit une nouvelle connexion
 * @param commandeAEnvoyer la chaine de caractère a transmettre
 */
void envoyerMessage(int descripteurSocketService, char *commandeAEnvoyer)
{
	write(descripteurSocketService, commandeAEnvoyer, SIZE_MSG);
}


/**
 * @fn void recevoirMessage(int descripteurSocketService, char *commandeRecu)
 * @brief Recoit un messsage venant du client
 * @param descripteurSocketService descripteur de fichier associé au nouveau service, il est créé lorsque le serveur reçoit une nouvelle connexion
 * @param commandeRecu la chaine de caractère a emettre
 */
void recevoirMessage(int descripteurSocketService, char *commandeRecu)
{
	int sizeRead;
	sizeRead = read(descripteurSocketService, commandeRecu, SIZE_MSG);
	if(sizeRead == -1 || sizeRead == 0){
		fprintf(stderr, RED"Erreur lecture socket!\n"RESET);
		exit(-1);
	}
	trimwhitespace(commandeRecu);
	printf("Commande reçu du client : %s (taille = %d)\n", commandeRecu, sizeRead);
}

/**
 * @fn void choixHoraire(int descripteurSocketService, char *commandeRecu, char *commandeAEnvoyer, int *h,int *m,int pid)
 * @brief Verifie la saisie de l'horaire
 * @param descripteurSocketService descripteur de fichier associé au nouveau service, il est créé lorsque le serveur reçoit une nouvelle connexion
 * @param commandeRecu la chaine de caractère a emettre
 * @param commandeAEnvoyer la commande qui s'envoie
 * @param h l'heure
 * @param m les minutes
 * @param pid le pid a afficher
 */
void choixHoraire(int descripteurSocketService, char *commandeRecu, char *commandeAEnvoyer, int *h,int *m,int pid)
{
	char *cmdAEnvoyer;
	cmdAEnvoyer = strdup(commandeAEnvoyer);
	fprintf(stderr, "choixHoraire\n");
	int valide = 0;
	do
	{

		//Envoie er reception des messages
		envoyerMessage(descripteurSocketService, cmdAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);

		//Declarations des pointeurs
		char *token1, *token2, *texte1, *texte2;
		char *str = commandeRecu;

		//Split de la reponse
		token1 = strsep(&str, ":");
		token2 = strsep(&str, ":");

		//Test du format de la repopnse (HH:MM)
		if(token2 != NULL)
		{

			// Transformation en int des entrees client
			*h = (int) strtol(token1, &texte1, 10);
			*m = (int) strtol(token2, &texte2, 10);
			printf("%d:%d\n",*h,*m);
			printf("%s\n",texte1);
			if(strcmp(texte1,"") == 0 && strcmp(texte2,"") == 0 && *h>=0 && *h<24 && *m>=0 && *m<60){
				//Tout c'est bien passe, sortie de la boucle
				valide = 1;
			}
			else
			{

				//Mauvais entree utilisateur, on redemande l'horaire
				printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
				strcpy(commandeAEnvoyer, "noread;"RED"Mauvais choix des horaires : ex : 10:10"RESET"\n");
				envoyerMessage(descripteurSocketService, commandeAEnvoyer);
			}
		}
		else
		{

			//Mauvais entree utilisateur, on redemande l'horaire
			printf("%d "RED"MAUVAIS ENTREE UTILISATEUR"RESET"\n", pid);
			strcpy(commandeAEnvoyer, "noread;"RED"Mauvais choix des horaires : ex : 10:10"RESET"\n");
			envoyerMessage(descripteurSocketService, commandeAEnvoyer);

		}

	}
	while(valide == 0);

}

/**
 * @fn void demanderVille(int descripteurSocketService, char *commandeRecu, char *commandeAEnvoyer, char **villeDepart, char **villeArrivee, int pid, int nbTrain ,char **listeVilleDepart, char **listeVilleArrive)
 * @brief Verifie la saisie de la ville
 * @param descripteurSocketService descripteur de fichier associé au nouveau service, il est créé lorsque le serveur reçoit une nouvelle connexion
 * @param commandeRecu la chaine de caractère a emettre
 * @param commandeAEnvoyer la commande qui s'envoie
 * @param villeDepart la ville de départ
 * @param villeArrivee la ville d'arrivée
 * @param pid le pid a afficher
 * @param listeVilleDepart la liste de toutes les villes de départ des trains
 * @param listeVilleArrive la liste de toutes les villes d'arrivée des trains
 */
void demanderVille(int descripteurSocketService, char *commandeRecu, char *commandeAEnvoyer, char **villeDepart, char **villeArrivee, int pid, int nbTrain ,char **listeVilleDepart, char **listeVilleArrive)
{
	int depart = 0;
	do
	{
		fprintf(stderr, "%d "MAG"CHOIX DEPART"RESET"\n", pid);
		printf("%d "MAG"CHOIX DEPART"RESET"\n", pid);
		strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville de de depart : ");
		envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);
		*villeDepart = strdup(commandeRecu);
		strToUpper(*villeDepart);
		trimwhitespace(*villeDepart);
		printf("Le client veut partir de : %s\n", *villeDepart);
		//const char *listeVilleDepart[nbTrain];
		int i;
		int existe = 0;
		for(i=0; i<nbTrain;i++)
		{
			printf("%s\n", listeVilleDepart[i]);
			if(strcmp(*villeDepart, listeVilleDepart[i])==0)
			{
				existe = 1;
				depart = 1;
				break;
			}
		}

		if(existe == 0)
		{
			envoyerMessage(descripteurSocketService, "noread;"RED"Ville inconnue"RESET"\n");
		}
	}while(depart == 0);

	//const char *listeVilleArrive[nbTrain];
	int arrivee =0;
	do {
		printf("%d "MAG"CHOIX ARRIVEE"RESET"\n", pid);
		strcpy(commandeAEnvoyer, "\nVeuillez entrer la ville d'arrivee : ");
		envoyerMessage(descripteurSocketService, commandeAEnvoyer);
		recevoirMessage(descripteurSocketService, commandeRecu);
		*villeArrivee = strdup(commandeRecu);
		strToUpper(*villeArrivee);
		trimwhitespace(*villeArrivee);
		printf("Le client veut aller a : %s\n", *villeArrivee);

		int i;
		int existe = 0;
		for(i=0; i<nbTrain;i++)
		{
			printf("%s\n", listeVilleArrive[i]);
			if(strcmp(*villeArrivee, listeVilleArrive[i])==0)
			{
				existe = 1;
				arrivee = 1;
				break;
			}
		}

		if(existe == 0)
		{
			envoyerMessage(descripteurSocketService, "noread;"RED"Ville inconnue"RESET"\n");
		}
	} while(arrivee == 0);


}
