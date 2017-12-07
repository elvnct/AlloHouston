#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/temps.h"
#include "../headers/train.h"
#include "../headers/requetes.h"
#include "../headers/outils.h"
#include "../headers/color.h"

#define SIZE_MSG 1024

/**
* @file requetes.c
* @date 5 Dec 2017
* @brief Traitement des différentes requetes possibles
*
* @see ../headers/requetes.h
*/

/**
* @fn void trouverTrainLePlusProche(struct Train* listeTrain, int compteLigne, char * villeDepart, char * villeArrivee, char * heureDepart, char * minuteDepart, char* commandeAEnvoyer)
* @brief Renvoie le train le plus proche
* @param ListeTrain L'ensemble des trains contenus dans trains.txt
* @param compteLigne Nombre de train contenus dans la base
* @param villeDepart Ville de départ souhaitée
* @param villeArrivee Ville d'arrivée souhaitée
* @param heureDepart Heure de départ souhaitée
* @param minuteDepart Minute de départ souhaitée
* @param commandeAEnvoyer Pointeur pour indiquer le message à envoyer au serveur
* @return Train * Pointeur sur le Train le plus proche des paramètres fournit par le client
*/
Train * trouverTrainLePlusProche(struct Train* listeTrain, int compteLigne, char * villeDepart, char * villeArrivee, int heureDepart, int minuteDepart, char* commandeAEnvoyer)
{
	//printf("Compte ligne : %d, villeDepart : %s, ville arrivee : %s, heure depart : %d, minute depart : %d\n",compteLigne, VD, VA, heureDepart, minuteDepart );
	for (int u = 0; u < compteLigne; u++) {
		//printf("Ville depart : %s, ville arrivee : %s\n", (listeTrain+u)->villeDepart, (listeTrain+u)->villeDepart);
	}
	struct Train* trainFiltre = malloc(sizeof(Train));
	/*// Concatenation de l'heure et minutes de départ souhaité
	char concatenation[8];
	strcat(concatenation,heureDepart);
	strcat(concatenation,minuteDepart);
	int heureConcat = atoi(concatenation);*/

	heureDepart = heureDepart * 100;
	int heureConcat = heureDepart + minuteDepart;
	////printf("Heure de depart concatené: %d, %s, %s, %s \n", heureConcat, heureDepart, minuteDepart, concatenation);

	// Permet de matcher la ville de départ et la ville d'arrivée souhaitées
	// avec la structure contenant l'ensemble des trains de la base de données
	// Crée un nouveau tableau contenant les structures Trains compatible
	//printf("PASSAGE 1\n");
	int nbTrainFiltre = 0;
	for (int increment = 0; increment < compteLigne; increment++) {
		//printf("PASSAGE boucle i \n");
		if (strcmp (villeDepart, (listeTrain + increment)->villeDepart) == 0)
		// Si les villes de départs sont identiques
		{
			//printf("PASSAGE strcmp ville depart \n");
			if (strcmp (villeArrivee, (listeTrain + increment)->villeArrivee) == 0)
			// Si les villes d'arrivées sont identiques
			{
				//printf("PASSAGE strcmp ville arrivee \n");
				if (heureConcat < tempsVersInt((listeTrain + increment)->heureDepart))
				//Si l'heure du train est après l'heure souhaitée
				{
					//printf("PASSAGE heure concat \n");
					trainFiltre[nbTrainFiltre] = *(listeTrain + increment);
					nbTrainFiltre++;
				}
			}
		}
	}

	// Fait la différence entre l'heure de départ souhaité et l'heure de
	// départ des trains compatible (heure A - heure B)
	int difference[nbTrainFiltre][2];
	int compteDifference;
	//printf("nbTrainFiltre = %d\n",nbTrainFiltre );
	for (compteDifference = 0; compteDifference < nbTrainFiltre; compteDifference++)
	{
		// On récupère la valeur absolue de : heureConcat - heure du train
		difference[compteDifference][0] = abs(heureConcat- tempsVersInt(trainFiltre[compteDifference].heureDepart));
		// On associe l'index
		difference[compteDifference][1] = compteDifference;
		//printf("PASSAGE 2\n");
		////printf("heure de difference %d, array %d\n", difference[compteDifference][1], difference[compteDifference][2]);
		////printf("heure d'entree :%d , heure dans la base :%d\n", atoi(heureDepart), trainFiltre[compteDifference].heureDepart->heure);

	}
	// Recherche du plus petit
	// Considerons comme le premier element comme le plus petit
	int plusPetit = difference[0][0];
	int index = difference[0][1];
	// Comparer chaque ligne au plus petit, remplacer le plus petit
	// si : valeur < plus petit
	for (compteDifference = 0; compteDifference < nbTrainFiltre; compteDifference++)
	{
		if (difference[compteDifference][0] < plusPetit) {
			plusPetit = difference[compteDifference][0];
			////printf("Arg 1 %d, plus petit que Arg2 %d\n", difference[compteDifference][1], plusPetit );
			index = difference[compteDifference][1];
		}
	}
	//printf("PASSAGE 3\n");
	////printf("Le plus petit nombre est : %d, son index est : %d\n", plusPetit, index);
	////printf("VIlle de depart %d\n", trainFiltre[index].heureDepart->minute );
	//printTrain(trainFiltre + index);
	trainFiltre = trainFiltre + index;
	if(nbTrainFiltre > 0)
	{

		snprintf(commandeAEnvoyer, SIZE_MSG, "noread;%sVoici le train correspondant a votre recherche :%s\n%d : %s -> %s Départ %d:%d arrivée %d:%d Prix : %f Reduc : %d\n\n", MAG, RESET, trainFiltre->id, trainFiltre->villeDepart, trainFiltre->villeArrivee, trainFiltre->heureDepart->heure, trainFiltre->heureDepart->minute, trainFiltre->heureArrivee->heure, trainFiltre->heureArrivee->minute, trainFiltre->prix, trainFiltre->reduc);
		return trainFiltre;
	}
	else
	{

		printf("Aucun train ne correspond à ces critères\n");
		snprintf(commandeAEnvoyer, SIZE_MSG,"noread;%sAucun train ne correspond à ces critères.%s\n", MAG, RESET);
		return NULL;
	}


}

Train * trouverTrainParTranche(struct Train* listeTrain, int* tailleListe , char * villeDepart, char * villeArrivee, int heureDepartDebut, int minuteDepartDebut, int heureDepartFin, int minuteDepartFin)
{
	strToUpper(villeDepart);
	strToUpper(villeArrivee);
	printf("%d\n",*tailleListe );
	int tab[*tailleListe];
	int nbTrains;
	//struct Train *listeTrainNouvelle[*tailleListe];
	int trancheDebut = heureDepartDebut * 100  + minuteDepartDebut;
	int trancheFin = heureDepartFin * 100  + minuteDepartFin;

	for (int trainCourant = 0; trainCourant < *tailleListe; trainCourant++)
	{
		// Tri en fonction de la ville de départ et d'arrivée
		if (strcmp(villeDepart, listeTrain[trainCourant].villeDepart) == 0)
		{
			if(strcmp(villeArrivee, listeTrain[trainCourant].villeArrivee) == 0)
			{
				// Heure où le train doit partir
				int heureDebut = 			tempsVersInt(listeTrain[trainCourant].heureDepart);

				if (heureDebut >= trancheDebut && heureDebut <= trancheFin)
				{
					// On ajoute ce train à la nouvelle liste
					tab[nbTrains] = trainCourant;
					nbTrains++;
				}
			}
		}
	}
	Train * trainFiltre = malloc(sizeof(Train) * nbTrains);
	for(int inc=0; inc< nbTrains; inc++)
	{
		trainFiltre[inc] = listeTrain[tab[inc]];
	}

	*tailleListe = nbTrains;

	return trainFiltre;
}

Train * listeTrainParVille(struct Train* listeTrain, int* compteLigne, char * villeDepart, char * villeArrivee, char* commandeAEnvoyer)
{
	int tab[*compteLigne];
	// Permet de matcher la ville de départ et la ville d'arrivée souhaitées
	// avec la structure contenant l'ensemble des trains de la base de données
	// Crée un nouveau tableau contenant les structures Trains compatible
	int nbTrainFiltre = 0;
	for (int index = 0; index < *compteLigne; index++) {
		if (strcmp (villeDepart, (listeTrain + index)->villeDepart) == 0)
		// Si les villes de départs sont identiques
		{
			if (strcmp (villeArrivee, (listeTrain + index)->villeArrivee) == 0)
			// Si les villes d'arrivées sont identiques
			{
				tab[nbTrainFiltre] = index;
				nbTrainFiltre++;
			}
		}
	}


	/*Train * trainFiltre = malloc(sizeof(Train)*nbTrainFiltre);
	for (int inc = 0; inc < nbTrainFiltre; inc++) {
	trainFiltre[inc] = listeTrain[tab[inc]];}*/

	if(nbTrainFiltre > 0)
	{
		Train * trainFiltre = malloc(sizeof(Train)*nbTrainFiltre);
		for (int inc = 0; inc < nbTrainFiltre; inc++) {
			trainFiltre[inc] = listeTrain[tab[inc]];

			snprintf(commandeAEnvoyer, SIZE_MSG, "noread;%sVoici le train correspondant a votre recherche :%s\n%d : %s -> %s Départ %d:%d arrivée %d:%d Prix : %f Reduc : %d\n\n", MAG, RESET, trainFiltre[inc].id, trainFiltre[inc].villeDepart, trainFiltre[inc].villeArrivee, trainFiltre[inc].heureDepart->heure, trainFiltre[inc].heureDepart->minute, trainFiltre[inc].heureArrivee->heure, trainFiltre[inc].heureArrivee->minute, trainFiltre[inc].prix, trainFiltre[inc].reduc);
		}

		*compteLigne = nbTrainFiltre;
		return trainFiltre;

	}
	else
	{

		printf("Aucun train ne correspond à ces critères\n");
		snprintf(commandeAEnvoyer, SIZE_MSG,"noread;%sAucun train ne correspond à ces critères.%s\n", MAG, RESET);

		*compteLigne = nbTrainFiltre;
		return NULL;
	}

}

Train * trajetSelonCritere(struct Train* listeTrain, int compteLigne, int critere, char * commandeAEnvoyer)
{

	struct Train* trainFiltre = malloc(sizeof(Train));
	printf("Compte ligne : %d, critere : %s\n",compteLigne,critere );
	printf("villeDepart : %s\n",(listeTrain + 0)->villeDepart );
	if (compteLigne > 0) {

		printf("PASSAGE compte ligne\n");
		if (critere == 0) {
			printf("PASSAGE 1\n");
			/* trajet au meilleur prix en tenant compte de la reduc (code 0), du prix normal ou du supplement(code -1)*/
			double tableauProvisoire[compteLigne][3];
			int incrementTableau;
			for (incrementTableau = 0; incrementTableau < compteLigne; incrementTableau++) {
				/* creation d'un tableau provisoire qui contient le prix, le prix avec reduc et l'index afin de pouvoir trouver le plus petit */
				printf("prix : %f,", listeTrain[incrementTableau].prix);
				tableauProvisoire[incrementTableau][0] = listeTrain[incrementTableau].prix;
				printf("Tab 1 : %f,",tableauProvisoire[incrementTableau][0] );
				// Prise en compte de la reduction pour trouver le prix le plus faible
				printf("reduc : %d,",listeTrain[incrementTableau].reduc );
				if (listeTrain[incrementTableau].reduc == 0)
				{
					tableauProvisoire[incrementTableau][1] = listeTrain[incrementTableau].prix;
				}
				if (listeTrain[incrementTableau].reduc == 1)
				{
					tableauProvisoire[incrementTableau][1] = (listeTrain[incrementTableau].prix * 0.8);
				}
				if (listeTrain[incrementTableau].reduc == -1)
				{
					tableauProvisoire[incrementTableau][1] = (listeTrain[incrementTableau].prix * 1.1);
				}
				printf("Tab 2 : %f, ",tableauProvisoire[incrementTableau][1] );
				tableauProvisoire[incrementTableau][2] = incrementTableau;
				printf("Tab 3 : %d\n",tableauProvisoire[incrementTableau][2] );

			}

				printf("PASSAGE 3\n");
			double plusPetit = tableauProvisoire[0][1];
			int index = tableauProvisoire[0][2];
			// Comparer chaque ligne au plus petit, remplacer le plus petit
			// si : valeur < plus petit
			for (incrementTableau = 0; incrementTableau < compteLigne; incrementTableau++)
			{
				if (tableauProvisoire[incrementTableau][1] < plusPetit) {
					plusPetit = tableauProvisoire[incrementTableau][1];
					index = tableauProvisoire[incrementTableau][2];
				}
			}

				printf("PASSAGE 4\n");
				printf("Index : %d\n",index );

			struct Train* trainFiltre = malloc(sizeof(Train));
			trainFiltre = listeTrain + index;
			snprintf(commandeAEnvoyer, SIZE_MSG, "noread;%sVoici le train correspondant a votre recherche :%s\n%d : %s -> %s Départ %d:%d arrivée %d:%d Prix d'origine : %f Reduc : %d Prix final : %f\n\n", MAG, RESET, trainFiltre->id, trainFiltre->villeDepart, trainFiltre->villeArrivee, trainFiltre->heureDepart->heure, trainFiltre->heureDepart->minute, trainFiltre->heureArrivee->heure, trainFiltre->heureArrivee->minute, trainFiltre->prix, trainFiltre->reduc, plusPetit);
			/*printf("Départ %s: \n", trainFiltre->villeDepart);
			printf("Arrivee %s: \n", trainFiltre->villeArrivee);
			printf("heureDepart %d: \n", trainFiltre->heureDepart->heure);
			printf("minuteDepart %d: \n", trainFiltre->heureDepart->minute);
			printf("heureArrivee %d: \n", trainFiltre->heureArrivee->heure);
			printf("minuteArrivee %d: \n", trainFiltre->heureArrivee->minute);
			printf("prix %f: \n", trainFiltre->prix);
			printf("reduc %d: \n", trainFiltre->reduc);
			printf("prix final %f: \n", plusPetit);*/

			/*printf("Voici le train correspondant a votre recherche :\n%d : %s -> %s Départ %d:%d arrivée %d:%d Prix d'origine : %f Reduc : %d Prix final : %f\n\n",trainFiltre->id, trainFiltre->villeDepart, trainFiltre->villeArrivee, trainFiltre->heureDepart->heure, trainFiltre->heureDepart->minute, trainFiltre->heureArrivee->heure, trainFiltre->heureArrivee->minute, trainFiltre->prix, trainFiltre->reduc, plusPetit );*/
			printf("PASSAGE 5\n");
			return trainFiltre;
		}
		if (critere == 1) {
			/* trajet de durée optimum */
			return NULL;
		}

	}
}


int tempsVersInt(struct Temps* temp)
{
	int test = temp->heure * 100 + temp->minute;
	return test;
}
