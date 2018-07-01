/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author Étudiant(e)
 * \version 0.1
 * \date Juin-Juillet 2018
 *
 *  Travail pratique numéro 2
 *
 */

#include "ReseauAerien.h"
#include <sstream>
#include <fstream>
#include <stack>
#include <limits>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{

// Méthode fournie
/**
 * \fn void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
 * \param[in] le fichier contenant l'information sur le réseau
 */
void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
{
	if (!fichierEntree.is_open())
		throw std::logic_error("ReseauAerien::chargerReseau: Le fichier n'est pas ouvert !");

	std::string buff;
	getline(fichierEntree, m_nomReseau);
    m_nomReseau.erase(0, 15);

	int nbVilles;

	fichierEntree >> nbVilles;
	getline(fichierEntree, buff); //villes

    m_unReseau.resize(nbVilles);

	getline(fichierEntree, buff); //Liste des villes

	size_t i = 0;

	while(getline(fichierEntree, buff) && buff != "Liste des trajets:")
	{
		std::string ville = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float lt;
		iss >> lt;

		float lg;
		iss >> lg;

        m_unReseau.nommer(i, ville, lt, lg);
		i++; 
	}

	while(getline(fichierEntree, buff))
	{
		std::string source = buff;

		getline(fichierEntree, buff);
		std::string destination = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float duree;
		iss >> duree;

		float cout;
		iss >> cout;

		int ns;
		iss >> ns;

        m_unReseau.ajouterArc(m_unReseau.getNumeroSommet(source), m_unReseau.getNumeroSommet(destination), duree, cout, ns);
	}
}

// Constructeur
ReseauAerien::ReseauAerien(std::string p_nomReseau, size_t p_nbVilles) :
    m_nomReseau(p_nomReseau),
    m_unReseau(Graphe(p_nbVilles))
{}

// Destructeur
ReseauAerien::~ReseauAerien()
{}

// Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
void ReseauAerien::resize(size_t p_nouvelleTaille)
{
    m_unReseau.resize(p_nouvelleTaille);
}

void ReseauAerien::relachementArc(size_t p_sommet1, size_t p_sommet2, int p_dureeCoutNiveau, std::vector<float>* p_coutChemin, std::vector<size_t>* p_sommetPrecedent) const
{
    float coutPotentiel;
    if (p_dureeCoutNiveau == 1) {
        coutPotentiel = (*p_coutChemin)[p_sommet1] +
                        m_unReseau.getPonderationsArc(p_sommet1, p_sommet2).duree;
    }
    else if (p_dureeCoutNiveau == 2)
    {
        coutPotentiel = (*p_coutChemin)[p_sommet1] +
                        m_unReseau.getPonderationsArc(p_sommet1, p_sommet2).cout;
    }
    else if (p_dureeCoutNiveau == 3)
    {
        coutPotentiel = (*p_coutChemin)[p_sommet1] +
                        m_unReseau.getPonderationsArc(p_sommet1, p_sommet2).ns;
    } else
    {
        throw std::logic_error("dureeCoutNiveau hors limite");
    }

    if (coutPotentiel < (*p_coutChemin)[p_sommet2])
    {
        (*p_coutChemin)[p_sommet2] = coutPotentiel;
        (*p_sommetPrecedent)[p_sommet2] = p_sommet1;
    }
}

Chemin ReseauAerien::determinerChemin(size_t p_origine, size_t p_destination, int p_dureeCoutNiveau, std::vector<float>& p_coutChemin, std::vector<size_t>& p_sommetPrecedent) const
{
    Chemin chemin;
    //chemin.dureeTotale = 0;
    //chemin.coutTotal = 0;
    //chemin.nsTotal = 0;

    chemin.reussi = true;

    if (p_dureeCoutNiveau == 1)
    {
        chemin.dureeTotale = p_coutChemin[p_destination];
    }
    else if (p_dureeCoutNiveau == 2)
    {
        chemin.coutTotal = p_coutChemin[p_destination];
    }
    else
    {
        chemin.nsTotal = p_coutChemin[p_destination];
    }

    std::stack<std::string> villesChemin;
    size_t predecesseur = p_destination;
    villesChemin.push(m_unReseau.getNomSommet(p_destination));

    do
    {
        predecesseur = p_sommetPrecedent[predecesseur];
        villesChemin.push(m_unReseau.getNomSommet(predecesseur));
    }
    while (predecesseur != p_origine);

    do
    {
        std::string& ville = villesChemin.top();
        chemin.listeVilles.push_back(ville);
        villesChemin.pop();
    } while (!villesChemin.empty());

    return chemin;
}

// Retourne le plus court chemin selon l'algorithme de Dijkstra
// origine et destination font partie du graphe
// Exception std::logic_error si origine et/ou destination absent du réseau
Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &p_origine, const std::string &p_destination, bool p_dureeCout) const
{
    m_unReseau.sommetsExistent(m_unReseau.getNumeroSommet(p_origine), m_unReseau.getNumeroSommet(p_destination));
    if (p_origine == p_destination)
    {
        throw std::logic_error("L'origine et la destination doivent etre differents!");
    }

    //Initialisation des variables utilisees dans l'algorithme
    Chemin chemin = Chemin();
    std::vector<float> coutChemin(m_unReseau.getNombreSommets(), std::numeric_limits<float>::max());
    std::vector<size_t> sommetPrecedent(m_unReseau.getNombreSommets(), NIL);
    std::list<size_t> sommetsRestants;

    coutChemin[m_unReseau.getNumeroSommet(p_origine)] = 0;

    for (size_t i = 0; i < m_unReseau.getNombreSommets(); i++)
    {
        sommetsRestants.push_back(i);
    }

    for (int i = 0; i < m_unReseau.getNombreSommets(); i++)
    {
        size_t sommetChoisi = ReseauAerien::minVecteur(coutChemin); //Choix du sommet ayant la plus petite valeur
        sommetsRestants.remove(sommetChoisi);

        if (sommetChoisi == m_unReseau.getNumeroSommet(p_destination)) //Si on est rendus a la destination, on arrete et on remplis le chemin
        {
            int dc = 1;
            if (!p_dureeCout)
            {
                dc = 2;
            }
            chemin = determinerChemin(m_unReseau.getNumeroSommet(p_origine), m_unReseau.getNumeroSommet(p_destination), dc, coutChemin, sommetPrecedent);
            break;
        }
        std::vector<size_t> listeSommetsAdjacents = m_unReseau.listerSommetsAdjacents(sommetChoisi);

        for (size_t j = 0; j < listeSommetsAdjacents.size(); j++)
        {
            if (ReseauAerien::sommetPresent(sommetsRestants, listeSommetsAdjacents[j]))
            {
                int dc = 1;
                if (!p_dureeCout)
                {
                    dc = 2;
                }

                ReseauAerien::relachementArc(sommetChoisi, listeSommetsAdjacents[j], dc, &coutChemin, &sommetPrecedent);
            }
        }

        coutChemin[sommetChoisi] = std::numeric_limits<float>::max(); //Pour eviter que ce sommet ne soit choisi de nouveau
    }

    return chemin;
}

// Retourne le plus court chemin selon l'algorithme Bellman-Ford
// origine et destination font partie du graphe
// 1 <= dureeCoutNiveau <= 3
// Exception std::logic_error si dureeCoutNiveau hors limite
// Exception std::logic_error si origine et/ou destination absent du réseau
Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &p_origine, const std::string &p_destination, int p_dureeCoutNiveau) const
{
    m_unReseau.sommetsExistent(m_unReseau.getNumeroSommet(p_origine), m_unReseau.getNumeroSommet(p_destination));
    if (p_origine == p_destination)
    {
        throw std::logic_error("L'origine et la destination doivent etre differents!");
    }

    //Initialisation des variables utilisees dans l'algorithme
    Chemin chemin = Chemin();
    std::vector<float> coutChemin(m_unReseau.getNombreSommets(), std::numeric_limits<float>::max());
    std::vector<size_t> sommetPrecedent(m_unReseau.getNombreSommets(), NIL);
    //std::list<size_t> sommetsRestants;

    coutChemin[m_unReseau.getNumeroSommet(p_origine)] = 0;

    for (int i = 0; i < m_unReseau.getNombreSommets() - 1; i++)
    {
        for (size_t j = 0; j < m_unReseau.getNombreSommets(); j++)
        {
            std::vector<size_t> listeSommetsAdjacents = m_unReseau.listerSommetsAdjacents(j);
            for (size_t s = 0; s < listeSommetsAdjacents.size(); s++)
            {
                relachementArc(j, listeSommetsAdjacents[s], p_dureeCoutNiveau, &coutChemin, &sommetPrecedent);
            }
        }
    }

    for (size_t j = 0; j < m_unReseau.getNombreSommets(); j++)
    {
        std::vector<size_t> listeSommetsAdjacents = m_unReseau.listerSommetsAdjacents(j);
        for (size_t s = 0; s < listeSommetsAdjacents.size(); s++)
        {
            if (p_dureeCoutNiveau == 3 && coutChemin[listeSommetsAdjacents[s]] > (coutChemin[j] + m_unReseau.getPonderationsArc(j, listeSommetsAdjacents[s]).ns))
            {
                return chemin;
            }
        }
    }

    if (sommetPrecedent[m_unReseau.getNumeroSommet(p_destination)] != NIL)
    {
        chemin = determinerChemin(m_unReseau.getNumeroSommet(p_origine), m_unReseau.getNumeroSommet(p_destination), p_dureeCoutNiveau, coutChemin, sommetPrecedent);
    }
    return chemin;
}

size_t ReseauAerien::minVecteur(std::vector<float> p_vecteur) const
{
    float min = std::numeric_limits<float>::max();
    size_t minIndex = 0;

    for (size_t i = 0; i < p_vecteur.size(); i++)
    {
        if (p_vecteur[i] < min)
        {
            min = p_vecteur[i];
            minIndex = i;
        }
    }

    return minIndex;
}

bool ReseauAerien::sommetPresent(std::list<size_t> p_liste, size_t p_element) const
{
    for (std::list<size_t>::const_iterator it = p_liste.begin(); it != p_liste.end(); ++it)
    {
        if (*it == p_element)
        {
            return true;
        }
    }
    return false;
}


}//Fin du namespace
