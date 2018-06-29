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
    Chemin chemin = Chemin();

    std::vector<float> coutChemin (m_unReseau.getNombreSommets(), std::numeric_limits<float>::max());
    coutChemin[m_unReseau.getNumeroSommet(p_origine)] = 0;

    std::vector<size_t> sommetPrecedent (m_unReseau.getNombreSommets(), NIL);

    std::list<size_t> sommetsRestants;
    for (size_t i = 0; i < m_unReseau.getNombreSommets(); i++)
    {
        sommetsRestants.push_back(i);
    }

    for (int i = 0; i < m_unReseau.getNombreSommets(); i++)
    {
        size_t sommetChoisi = ReseauAerien::minVecteur(coutChemin);
        sommetsRestants.remove(sommetChoisi);
        //chemin.listeVilles.push_back(m_unReseau.getNomSommet(sommetChoisi));

        if (sommetChoisi == m_unReseau.getNumeroSommet(p_destination))
        {
            chemin.reussi = true;
            if (p_dureeCout)
            {
                chemin.dureeTotale = coutChemin[sommetChoisi];
            } else
            {
                chemin.coutTotal = coutChemin[sommetChoisi];
            }

            std::stack<std::string> villesChemin;
            size_t predecesseur = m_unReseau.getNumeroSommet(p_destination);
            villesChemin.push(p_destination);

            do
            {
                predecesseur = sommetPrecedent[predecesseur];
                villesChemin.push(m_unReseau.getNomSommet(predecesseur));
            }
            while (predecesseur != m_unReseau.getNumeroSommet(p_origine));

            do
            {
                std::string& ville = villesChemin.top();
                chemin.listeVilles.push_back(ville);
                villesChemin.pop();
            } while (!villesChemin.empty());

            break;
        }

        //Relachement des sommets adjacents
        std::vector<size_t> listeSommetsAdjacents = m_unReseau.listerSommetsAdjacents(sommetChoisi);

        for (size_t i = 0; i < listeSommetsAdjacents.size(); i++)
        {
            if (ReseauAerien::sommetPresent(sommetsRestants, listeSommetsAdjacents[i]))
            {
                float coutPotentiel;
                if (p_dureeCout)
                {
                    coutPotentiel = coutChemin[sommetChoisi] + m_unReseau.getPonderationsArc(sommetChoisi, listeSommetsAdjacents[i]).duree;
                }
                else
                {
                    coutPotentiel = coutChemin[sommetChoisi] + m_unReseau.getPonderationsArc(sommetChoisi, listeSommetsAdjacents[i]).cout;
                }
                if (coutPotentiel < coutChemin[listeSommetsAdjacents[i]])
                {
                    coutChemin[listeSommetsAdjacents[i]] = coutPotentiel;
                    sommetPrecedent[listeSommetsAdjacents[i]] = sommetChoisi;
                }

            }
        }
        listeSommetsAdjacents.clear();
        coutChemin[sommetChoisi] = std::numeric_limits<float>::max();

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
