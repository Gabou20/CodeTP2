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
/**
 * \struct Chemin
 *
 * \brief Une struture utilisée pour stocker les informations d'un chemin : sa duree totale, son cout total, son niveau de securite total et s"il est reussi ou non.
 */
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

/**
 * \class ReseauAerien
 *
 * \brief Classe représentant un reseau aerien
 */
class ReseauAerien{

public:

	ReseauAerien(std::string p_nomReseau, size_t p_nbVilles = 5);

	~ReseauAerien();

	void resize(size_t p_nouvelleTaille);

    /**
     * \fn friend std::ostream& operator<<(std::ostream& out, const ReseauAerien& g)
     *
     * \brief Surcharge de l'operateur de sortie
     */
	friend std::ostream& operator<<(std::ostream& out, const ReseauAerien& g)
	{
		out << g.m_unReseau << std::endl;
		return out;
	}

	void chargerReseau(std::ifstream & p_fichierEntree);

	Chemin rechercheCheminDijkstra(const std::string &p_origine, const std::string &p_destination, bool p_dureeCout) const;

	Chemin rechercheCheminBellManFord(const std::string &p_origine, const std::string &p_destination, int p_dureeCoutNiveau) const;


private:

	Graphe m_unReseau;			// Le type ReseauAerien est composé d'un graphe
	std::string m_nomReseau;		// Le nom du reseau (exemple: Air Canada)

    size_t minVecteur(std::vector<float> p_vecteur) const;

    bool sommetPresent(std::list<size_t> p_liste, size_t p_sommet) const;

	void relachementArc(size_t p_sommet1, size_t p_sommet2, int p_dureeCoutNiveau, std::vector<float>* p_coutChemin, std::vector<size_t>* p_sommetPrecedent) const;

	Chemin determinerChemin(size_t p_source, size_t p_destination, int p_dureeCoutNiveau, std::vector<float>& p_coutChemin, std::vector<size_t>& p_sommetPrecedent) const;

};

}//Fin du namespace

#endif
