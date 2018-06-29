/**
* \file Graphe.cpp
* \brief Implémentation d'un graphe orienté.
* \author Étudiant(e)
* \version 0.5
* \date Juin-Juillet 2018
*
*  Travail pratique numéro 2
*
*/

#include "Graphe.h"

//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{


// Constructeur
Graphe::Graphe(size_t p_nbSommets) :
    m_nbSommets(p_nbSommets),
    m_nbArcs(0),
    m_listesAdj(),
    m_villes()
{
    //m_villes.resize(p_nbSommets);
    for (int i = 0; i < p_nbSommets; i++)
    {
        m_villes.push_back(Sommet());
        m_listesAdj.push_back(std::list<Arc>());
    }
}

// Destructeur
Graphe::~Graphe() {}

// Change la taille du graphe en utilisant un nombre de sommet = nouvelleTaille
void Graphe::resize(size_t p_nouvelleTaille)
{
    //possiblement utiliser resize de la stl
    if (p_nouvelleTaille > m_nbSommets)
    {
        for (int i = 0; i < (p_nouvelleTaille - m_nbSommets); i++)
        {
            m_villes.push_back(Sommet());
            m_listesAdj.push_back(std::list<Arc>());
        }
    }

    //m_villes.resize(p_nouvelleTaille);
    m_nbSommets = p_nouvelleTaille;

    //ici il va falloir ajuster les listes si la fonction permet de reduire le nombre de sommets
}

// Donne un nom et des coordonnées à un sommet en utlisant son numéro (indice dans le vector).
// Exception logic_error si sommet supérieur à nbSommets
void Graphe::nommer(size_t p_sommet, const std::string& p_nom, float p_lt, float p_lg)
{
    sommetExiste(p_sommet);

    m_villes[p_sommet].m_nom = p_nom;
    m_villes[p_sommet].m_coord = Coordonnees(p_lt, p_lg);
}

// Ajoute un arc au graphe
// Exception logic_error si source ou destination supérieur à nbSommets
// Exception logic_error si l'arc existe déjà dans le graphe
void Graphe::ajouterArc(size_t p_source, size_t p_destination, float p_duree, float p_cout, int p_ns)
{
    sommetsExistent(p_source, p_destination);

    if (arcExiste(p_source, p_destination))
    {
        throw std::logic_error("Un arc existe deja entre ces deux sommets");
    }

    m_nbArcs += 1;

    Arc nouvelArc(p_destination, Ponderations(p_duree, p_cout, p_ns));

    m_listesAdj[p_source].push_back(nouvelArc);
}

// Supprime un arc du graphe
// Exception logic_error si source ou destination supérieur à nbSommets
// Exception logic_error si l'arc n'existe pas dans le graphe
void Graphe::enleverArc(size_t p_source, size_t p_destination)
{
    sommetsExistent(p_source, p_destination);
    if (!arcExiste(p_source, p_destination))
    {
        throw std::logic_error("Cet arc n'existe pas");
    }

    for (std::list<Arc>::iterator it = m_listesAdj[p_source].begin(); it != m_listesAdj[p_source].end(); ++it)
    {
        if (it -> m_destination == p_destination)
        {
            m_listesAdj[p_source].erase(it);
            break;
        }
    }
}

// Vérifie si un arc existe
// Exception logic_error si source ou destination supérieur à nbSommets
bool Graphe::arcExiste(size_t p_source, size_t p_destination) const
{
    sommetsExistent(p_source, p_destination);

    std::vector<size_t> listeSommetsAdjacents = listerSommetsAdjacents(p_source);

    for (int i = 0; i < listeSommetsAdjacents.size(); i++)
    {
        if (listeSommetsAdjacents[i] == p_destination)
        {
            return true;
        }
    }

    return false;
}

// Retourne la liste de successeurs d'un sommmet
// Exception logic_error si sommet supérieur à nbSommets
std::vector<size_t> Graphe::listerSommetsAdjacents(size_t p_sommet) const
{
    sommetExiste(p_sommet);

    std::vector<size_t> destinations;

    for (std::list<Arc>::const_iterator it = m_listesAdj[p_sommet].begin(); it != m_listesAdj[p_sommet].end(); ++it)
    {
        destinations.push_back(it -> m_destination);
    }

    return destinations;
}

// Retourne le nom d'un sommet
// Exception logic_error si sommet supérieur à nbSommets
std::string Graphe::getNomSommet(size_t p_sommet) const
{
    sommetExiste(p_sommet);

    return m_villes[p_sommet].m_nom;
}

// Retourne le numéro d'un sommet
// Exception logic_error si nom n'existe pas dans le graphe
size_t Graphe::getNumeroSommet(const std::string& p_nom) const
{
    int numero = -1;

    for (int i = 0; i <= m_nbSommets; i++)
    {
        if (m_villes[i].m_nom == p_nom)
        {
            numero = i;
            break;
        }
    }

    if (numero == -1)
    {
        throw std::logic_error("Le sommet n'existe pas");
    }

    return (size_t) numero;

}

// Retourne le nombre de sommet du graphe
int Graphe::getNombreSommets() const
{
    return (int) m_nbSommets;
}

// Retourne le nombre des arcs du graphe
int Graphe::getNombreArcs() const
{
    return (int) m_nbArcs;
}

// Retourne les pondérations se trouvant dans un arc (source -> destination)
// Exception logic_error si source ou destination supérieur à nbSommets
Ponderations Graphe::getPonderationsArc(size_t p_source, size_t p_destination) const
{
    sommetsExistent(p_source, p_destination);

    for (std::list<Arc>::const_iterator it = m_listesAdj[p_source].begin(); it != m_listesAdj[p_source].end(); ++it)
    {
        if (it -> m_destination == p_destination)
        {
            return it -> m_poids;
        }
    }
}

// Retourne les coordonnées d'un sommet
// Exception logic_error si sommet supérieur à nbSommets
Coordonnees Graphe::getCoordonnesSommet(size_t p_sommet) const
{
    sommetExiste(p_sommet);

    return m_villes[p_sommet].m_coord;
}

void Graphe::sommetsExistent(size_t p_source, size_t p_destination) const
{
    if (p_source > m_nbSommets)
    {
        throw std::logic_error("La source n'existe pas");
    }

    if (p_destination > m_nbSommets)
    {
        throw std::logic_error("La destination n'existe pas");
    }
}

void Graphe::sommetExiste(size_t p_sommet) const
{
    if (p_sommet > m_nbSommets)
    {
        throw std::logic_error("Ce sommet n'existe pas");
    }
}

}//Fin du namespace
