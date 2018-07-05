/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author Gabrielle Martin-Fortier
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

namespace TP2
{


/**
 * \fn void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
 *
 * \param[in] fichierEntree le fichier contenant l'information sur le réseau
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


/**
 * \fn ReseauAerien::ReseauAerien(std::string p_nomReseau, size_t p_nbVilles)
 *
 * \brief Constructeur de la classe ReseauAerien
 *
 * \param[in] p_nomReseau le nom du reseau
 * \param[in] p_nbVilles le nombre de villes du reseau, valeur par defaut de 5
 */
ReseauAerien::ReseauAerien(std::string p_nomReseau, size_t p_nbVilles) :
    m_nomReseau(p_nomReseau),
    m_unReseau(p_nbVilles)
{}


/**
 * \fn ReseauAerien::~ReseauAerien()
 *
 * \brief Destructeur de la classe ReseauAerien
 */
ReseauAerien::~ReseauAerien()
{}


/**
 * \fn void ReseauAerien::resize(size_t p_nouvelleTaille)
 *
 * \brief Change la taille du réseau en utilisant un nombre de villes = p_nouvelleTaille
 *
 * \param[in] p_nouvelleTaille le nouveau nombre de sommets du reseau
 */
void ReseauAerien::resize(size_t p_nouvelleTaille)
{
    m_unReseau.resize(p_nouvelleTaille);
}

/**
 * \fn void ReseauAerien::relachementArc(size_t p_sommet1, size_t p_sommet2, int p_dureeCoutNiveau, std::vector<float>* p_coutChemin, std::vector<size_t>* p_sommetPrecedent) const
 *
 * \brief Si un chemin plus court partant de la source vers p_sommet2 est trouve, la valeur de ce sommet dans coutChemin est modifiee et le predecesseur de p_sommet2 devient p_sommet1
 *
 * \param[in] p_sommet1 Le potentiel predecesseur de p_sommet2
 * \param[in] p_sommet2 Le sommet qui sera possiblement relache
 * \param[in] p_dureeCoutNiveau L'indicateur de la donnee utilisee dans le calcul du plus court chemin
 * \param[in] p_coutChemin Le vecteur des couts du chemin pour aller de la source vers chaque sommet
 * \param[in] p_sommetPrecedent Le vecteur des predecesseurs de chaque sommet dans le plus court chemin
 *
 * \exception logic_error si le parametre dureeCoutNiveau n'est pas dans les choix, soit 1, 2 ou 3
 */
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

/**
 * \fn Chemin ReseauAerien::determinerChemin(size_t p_origine, size_t p_destination, int p_dureeCoutNiveau, std::vector<float>& p_coutChemin, std::vector<size_t>& p_sommetPrecedent) const
 *
 * \brief Lorsque l'algorithme de recherche du plus court chemin est termine, cette fonction
 *
 * \pre La fonction appelante doit s'assurer qu'un chemin a bien ete trouve entre la source et la destination
 *
 * \param[in] p_source Le sommet de depart du chemin
 * \param[in] p_destination Le sommet d'arrivee du chemin
 * \param[in] p_dureeCoutNiveau L'indicateur de la donnee utilisee dans le calcul du plus court chemin
 * \param[in] p_coutChemin Le vecteur des couts du chemin pour aller de la source vers chaque sommet
 * \param[in] p_sommetPrecedent Le vecteur des predecesseurs de chaque sommet dans le plus court chemin
 *
 * \return Le chemin contenant les bonnes informations : le booleen confirmant le succes de l'algorithme de recherche du plus court chemin, la liste des villes parcourues (dans l'ordre) et le cout, la duree ou le niveau de securite total, selon le cas.
 */
Chemin ReseauAerien::determinerChemin(size_t p_source, size_t p_destination, int p_dureeCoutNiveau, std::vector<float>& p_coutChemin, std::vector<size_t>& p_sommetPrecedent) const
{
    Chemin chemin;

    if (p_sommetPrecedent[p_destination] == NIL)
    {
        return chemin;
    }

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
        chemin.nsTotal = (int) p_coutChemin[p_destination];
    }

    std::stack<std::string> villesChemin;
    size_t predecesseur = p_destination;
    villesChemin.push(m_unReseau.getNomSommet(p_destination));

    do
    {
        predecesseur = p_sommetPrecedent[predecesseur];
        villesChemin.push(m_unReseau.getNomSommet(predecesseur));
    }
    while (predecesseur != p_source);

    do
    {
        std::string& ville = villesChemin.top();
        chemin.listeVilles.push_back(ville);
        villesChemin.pop();
    } while (!villesChemin.empty());

    return chemin;
}


/**
 * \fn Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &p_origine, const std::string &p_destination, bool p_dureeCout) const
 *
 * \brief Algorithme de Dijkstra afin de trouver le plus court chemin entre deux sommets dans un graphe oriente
 *
 * \param[in] p_origine Le sommet de depart du chemin
 * \param[in] p_destination Le sommet d'arrivee du chemin
 * \param[in] p_dureeCout L'indicateur de la donnee utilisee dans le calcul du plus court chemin
 *
 * \exception logic_error si l'un des sommets donnes en parametre n'existe pas
 *
 * \return Le chemin contenant les bonnes informations : le booleen confirmant le succes de l'algorithme de recherche du plus court chemin, la liste des villes parcourues (dans l'ordre) et le cout ou la duree total, selon le cas.
 */
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

    coutChemin[m_unReseau.getNumeroSommet(p_origine)] = 0; //Cout de l'origine = 0

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
            if (ReseauAerien::sommetPresent(sommetsRestants, listeSommetsAdjacents[j])) //Si le sommet adjacent courant n'est pas encore parcouru par l'algorithme, il est "relache"
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


/**
 * \fn Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &p_origine, const std::string &p_destination, int p_dureeCoutNiveau) const
 *
 * \brief Algorithme de Bellman-Ford afin de trouver le plus court chemin entre deux sommets dans un graphe oriente
 *
 * \param[in] p_origine Le sommet de depart du chemin
 * \param[in] p_destination Le sommet d'arrivee du chemin
 * \param[in] p_dureeCout L'indicateur de la donnee utilisee dans le calcul du plus court chemin
 *
 * \exception logic_error si l'un des sommets donnes en parametre n'existe pas
 * \exception logic_error si p_dureeCoutNiveau n'est pas 1, 2 ou 3
 *
 * \return Le chemin contenant les bonnes informations : le booleen confirmant le succes de l'algorithme de recherche du plus court chemin, la liste des villes parcourues (dans l'ordre) et le cout, la duree ou le niveau de securite total, selon le cas.
 */
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

    coutChemin[m_unReseau.getNumeroSommet(p_origine)] = 0; //Cout de l'origine a 0

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

    if (p_dureeCoutNiveau == 3) //On verifie la presence de cycles negatifs seulement pour le niveau de securite puisque les autres n'ont que des valeurs positives
    {
        if (verifierCycleNegatif(coutChemin))
        {
            return chemin;
        }
    }

    chemin = determinerChemin(m_unReseau.getNumeroSommet(p_origine), m_unReseau.getNumeroSommet(p_destination), p_dureeCoutNiveau, coutChemin, sommetPrecedent);

    return chemin;
}

/**
 * \fn size_t ReseauAerien::minVecteur(std::vector<float> p_vecteur) const
 *
 * \brief Trouve l'indice du plus petit element d'un vecteur de reels
 *
 * \param[in] p_vecteur Le vecteur d'entiers dans lequel on cherche un minimum
 *
 * \return L'indice du plus petit element du vecteur p_vecteur
 */
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


/**
 * \fn bool ReseauAerien::sommetPresent(std::list<size_t> p_liste, size_t p_sommet) const
 *
 * \brief Verifie si un sommet est present dans une liste
 *
 * \param[in] p_liste La liste de size_t dans laquelle il faut chercher le sommet
 * \param[in] p_sommet Le sommet a chercher dans la liste
 *
 * \return true si le sommet a ete trouve, false sinon
 */
bool ReseauAerien::sommetPresent(std::list<size_t> p_liste, size_t p_sommet) const
{
    for (std::list<size_t>::const_iterator it = p_liste.begin(); it != p_liste.end(); ++it)
    {
        if (*it == p_sommet)
        {
            return true;
        }
    }
    return false;
}

/**
 * \fn bool ReseauAerien::verifierCycleNegatif(const std::vector<float> &p_coutChemin) const
 *
 * \brief Verifie si un reseau aerien contient un cycle negatif
 *
 * \param[in] p_coutChemin la liste des couts des chemins pour aller de l'origine a chaque sommet
 *
 * \return true s'il existe un cycle de valeur negative, false sinon
 */
bool ReseauAerien::verifierCycleNegatif(const std::vector<float> &p_coutChemin) const
{
    for (size_t j = 0; j < m_unReseau.getNombreSommets(); j++)
    {
        std::vector<size_t> listeSommetsAdjacents = m_unReseau.listerSommetsAdjacents(j);
        for (size_t s = 0; s < listeSommetsAdjacents.size(); s++)
        {
            if (p_coutChemin[listeSommetsAdjacents[s]] > (p_coutChemin[j] + m_unReseau.getPonderationsArc(j, listeSommetsAdjacents[s]).ns))
            {
                return true;
            }
        }
    }
    return false;
}


}//Fin du namespace
