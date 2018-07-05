/**
 * \file Graphe.h
 * \brief Gestion d'un graphe.
 * \author IFT-2008
 * \version 0.1
 * \date Juin-Juillet 2018
 *
 *  Travail pratique numéro 2
 *
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
//Vous pouvez ajouter d'autres librairies de la STL

#ifndef _GRAPHE__H
#define _GRAPHE__H

namespace TP2
{

/**
 * \struct Ponderations
 *
 * \brief Une struture utilisée pour stocker les pondérations utilisées sur les arcs
 */
struct Ponderations
{
	float duree;		// La durée du vol en heures (ex. 0.8 heures entre Québec et Montréal)
	float cout;			// Le cout du trajet en dollars (ex. 220.46 $ pour un vol entre Québec et Montréal)
	int ns;				// Un niveau de sécurité représentant un indice qui peut être négatif

	Ponderations(float d, float c, int n) : duree(d), cout(c), ns(n) {}
};


/**
 * \struct Coordonnees
 *
 * \brief Une structure utilisée pour stocker les coordonnées d'un sommet
 */
struct Coordonnees
{
	float lt;			// La latitude en décimale (ex. pour la ville de Québec : 46.80)
	float lg;			// La longitude en décimale (ex. pour la ville de Québec : -71.24)

	Coordonnees(float t, float g) : lt(t), lg(g) {}
};


/**
 * \class Graphe
 *
 * \brief Classe représentant un graphe oriente
 */
class Graphe
{
public:

	Graphe(size_t p_nbSommets = 10);

	~Graphe();

	void resize(size_t p_nouvelleTaille);

	void nommer(size_t p_sommet, const std::string& p_nom, float p_lt, float p_lg);

	void ajouterArc(size_t p_source, size_t p_destination, float p_duree, float p_cout, int p_ns);

	void enleverArc(size_t p_source, size_t p_destination);

	bool arcExiste(size_t p_source, size_t p_destination) const;

	std::vector<size_t> listerSommetsAdjacents(size_t p_sommet) const;

	std::string getNomSommet(size_t p_sommet) const;

	size_t getNumeroSommet(const std::string& p_nom) const;

	int getNombreSommets() const;

	int getNombreArcs() const;

	Ponderations getPonderationsArc(size_t p_source, size_t p_destination) const;

	Coordonnees getCoordonnesSommet(size_t p_sommet) const;

    /**
     * \fn friend std::ostream& operator<<(std::ostream& out, const Graphe& g)
     *
     * \brief Surcharge de l'operateur de sortie
     */
	friend std::ostream& operator<<(std::ostream& out, const Graphe& g)
	{
		out << "Le graphe contient " << g.m_nbSommets << " sommet(s) et "
			<< g.m_nbArcs << " arc(s)" << std::endl << std::endl;
		for (size_t i = 0; i < g.m_nbSommets; ++i)
		{
			out << "Sommet: " << g.m_villes[i].m_nom << std::endl << "Voisin(s): ";

			if (g.m_listesAdj[i].size() == 0)
				out << "Rien";
			else
			{
				for (auto it = g.m_listesAdj[i].begin(); it != g.m_listesAdj[i].end(); ++it)
					if (it == --g.m_listesAdj[i].end())
						out << g.m_villes[it->m_destination].m_nom;
					else
						out << g.m_villes[it->m_destination].m_nom << ", ";
			}
			out << std::endl;
		}
		return out;
	}

    void sommetsExistent(size_t p_source, size_t p_destination) const;

private:
    /**
     * \struct Sommet
     *
     * \brief Une struture utilisée pour stocker les informations d'un sommet : son nom, ses coordonnees et son etat
     */
	struct Sommet
	{
		std::string m_nom;			// Le nom de la ville
		Coordonnees m_coord;		// Les coordonnées géospatiales de la ville
		bool m_etat;			    // Pour marquer une ville

		Sommet() : m_nom(""), m_coord(Coordonnees(0,0)), m_etat(false) {}

		Sommet(std::string nv, Coordonnees cv, bool ev) : m_nom(nv), m_coord(cv), m_etat(ev) {}

        void setNom(std::string p_nom);

        void setCoordonnees(Coordonnees p_coord);

	};

	std::vector<Sommet> m_villes; /*!< Le vecteur des sommets */

    /**
     * \struct Arc
     *
     * \brief Une struture utilisée pour stocker les informations d'un arc : sa destination et ses ponderations
     */
	struct Arc
	{
		size_t m_destination;
		Ponderations m_poids;	// Les pondérations utilisées pour un trajet entre deux villes

		Arc(size_t dest, Ponderations p) : m_destination(dest), m_poids(p) {}
	};

	std::vector<std::list<Arc> > m_listesAdj; /*!< les listes d'adjacence */

	size_t m_nbSommets;					// Le nombre de sommets dans le graphe
	size_t m_nbArcs;					// Le nombre des arcs dans le graphe

    void sommetExiste(size_t p_sommet, std::string message) const;

};

}//Fin du namespace

#endif
