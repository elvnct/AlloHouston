#include "../headers/temps.h"
#include <stdio.h>
#include <string.h>

/**
 * @file temps.c
 * @date 6 Dec 2017
 * @brief Fichier contenant les méthodes pour la structure temps
 *
 * @see ../headers/train.h
 * @see ../headers/temps.h
 */

 /**
  * @fn Temps* tempsFromCSV(char* csv)
  * @brief Crée un temps correspondant au info de la chaine csv fournit
  * @param csv Une ligne csv sous la forme : "heure:minute".
  * @return pointeur sur un temps
  * Exemple de ligne csv reçu : "16:55"
  */
Temps* tempsFromCSV(char* csv){

  struct Temps* temps = malloc(sizeof(Temps));
  char *token, *str, *tofree;
  tofree = str = strdup(csv);  // We own str's memory now.
  // heure
  token = strsep(&str, ":");
  temps->heure = atoi(token);

  // minute
  token = strsep(&str, ":");
  temps->minute = atoi(token);

  free(tofree); // We free the str we made
  return temps;
}
