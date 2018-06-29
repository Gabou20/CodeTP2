/**
 * \file ReseauAerien.h
 * \brief Gestion d'un réseau aérien.
 * \author IFT-2008
 * \version 0.1
 * \date Juin-Juillet 2018
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <climits>
#include <vector>
#include <map>


#ifndef _RESEAUAERIEN__H
#define _RESEAUAERIEN__H
#define NIL -1

namespace TP2
{

struct Chemin 
{
   //listeVilles doit contenir la liste des villes formant le chemin
   //Le point de départ du chemin correspond à l'élément d'indice 0 du vecteur
   //le point d'arrivée correspond au dernier élément du vecteur.
   std::vector<std::string> listeVilles;

   float dureeTotale;		// Durée totale du vol du chemin en heures incluant le temps d'escale
   float coutTotal;			// Cout total en dollars du chemin entre la ville de départ et la ville de destination
   int nsTotal;				// Niveau de sécurité total du chemin
   bool reussi;				// Un booléen qui vaut true si chemin a été trouvé, false sinon
};

class ReseauAerien{

public:

	// Constructeur
	ReseauAerien(std::string p_nomReseau, size_t p_nbVilles = 5);

	// Destructeur
	~ReseauAerien();

	// Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
	void resize(size_t p_nouvelleTaille);

	//Surcharge de l'opérateur de sortie.
	//Ne touchez pas à cette fonction !
	friend std::ostream& operator<<(std::ostream& out, const ReseauAerien& g)
	{
		out << g.m_unReseau << std::endl;
		return out;
	}

	// Charger un réseau à partir d'un fichier texte en entrée (voir format du fichier dans l'énoncé du Tp).
	// Construit un réseau routier à partir d'une liste de villes ainsi que leurs liens.
	// fichierEntree est ouvert corectement. 
	// fichierEntree n'est pas fermé par la fonction.
	// Exception logic_error si fichierEntree n'est pas ouvert correctement.
	void chargerReseau(std::ifstream & p_fichierEntree);

	// Retourne le plus court chemin selon l'algorithme de Dijkstra
	// origine et destination font partie du graphe
	// Exception std::logic_error si origine et/ou destination absent du réseau
	Chemin rechercheCheminDijkstra(const std::string &p_origine, const std::string &p_destination, bool p_dureeCout) const;

	// Retourne le plus court chemin selon l'algorithme Bellman-Ford
	// origine et destination font partie du graphe
	// 1 <= dureeCoutNiveau <= 3
	// Exception std::logic_error si dureeCoutNiveau hors limite
	// Exception std::logic_error si origine et/ou destination absent du réseau
	Chemin rechercheCheminBellManFord(const std::string &p_origine, const std::string &p_destination, int p_dureeCoutNiveau) const;

	//Vous pouvez ajoutez d'autres méthodes publiques si vous sentez leur nécessité

private:
	Graphe m_unReseau;			// Le type ReseauAerien est composé d'un graphe
	std::string m_nomReseau;		// Le nom du reseau (exemple: Air Canada)

    size_t minVecteur(std::vector<float> p_vecteur) const;

    bool sommetPresent(std::list<size_t> p_liste, size_t p_element) const;

	// Vous pouvez définir des constantes ici.

	//Vous pouvez ajoutez des méthodes privées si vous sentez leur nécessité

};

}//Fin du namespace

#endif
