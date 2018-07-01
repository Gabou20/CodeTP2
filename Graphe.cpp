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

namespace TP2
{

/**
 * \fn Graphe::Graphe(size_t p_nbSommets)
 *
 * \brief Constructeur de la classe Graphe
 *
 * \param[in] p_nbSommets le nombre de sommets du nouveau graphe, valeur par defaut de 10.
 */
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

/**
 * \fn Graphe::~Graphe()
 *
 * \brief Destructeur de la classe Graphe
 */
Graphe::~Graphe() {}

/**
 * \fn void Graphe::resize(size_t p_nouvelleTaille)
 *
 * \brief Si la nouvelle taille est plus grande que la taille actuelle, change la taille du Graphe en ajoutant des sommets vides
 *
 * \param[in] p_nouvelleTaille Le nouveau nombre de sommets dans le graphe
 */
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

/**
 * \fn void Graphe::nommer(size_t p_sommet, const std::string& p_nom, float p_lt, float p_lg)
 *
 * \brief Donne un nom et des coordonnées à un sommet en utlisant son numéro
 *
 * \param[in] p_sommet Le numero du sommet a nommer
 * \param[in] p_nom Le nom a donner au sommet
 * \param[in] p_lt La latitude de la ville contenue dans le sommet
 * \param[in] p_lg La longitude de la ville contenue dans le sommet
 *
 * \exception logic_error si le sommet a nommer est inexistant
 */
void Graphe::nommer(size_t p_sommet, const std::string& p_nom, float p_lt, float p_lg)
{
    sommetExiste(p_sommet, "Tentative de nommer un sommet inexistant");

    m_villes[p_sommet].m_nom = p_nom;
    m_villes[p_sommet].m_coord = Coordonnees(p_lt, p_lg);
}


/**
 * \fn void Graphe::ajouterArc(size_t p_source, size_t p_destination, float p_duree, float p_cout, int p_ns)
 *
 * \brief Ajoute un arc au graphe courant
 *
 * \param[in] p_source Le numero du sommet constituant la source de l'arc
 * \param[in] p_destination Le numero du sommet constituant la destination de l'arc
 * \param[in] p_duree La duree du vol allant de la source a la destination
 * \param[in] p_cout Le cout du vol allant de la source a la destination
 * \param[in] p_ns Le niveau de securite du vol allant de la source a la destination
 *
 * \exception logic_error si un arc existe deja de la source a la destination
 * \exception logic_error si un des deux sommets est inexistant
 */
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


/**
 * \fn void Graphe::enleverArc(size_t p_source, size_t p_destination)
 *
 * \brief Supprime l'arc allant de la source a la destination du graphe courant
 *
 * \param[in] p_source Le numero du sommet constituant la source de l'arc
 * \param[in] p_destination Le numero du sommet constituant la destination de l'arc
 *
 * \exception logic_error s'il n'y a pas d'arc entre la source a la destination
 * \exception logic_error si un des deux sommets est inexistant
 */
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


/**
 * \fn bool Graphe::arcExiste(size_t p_source, size_t p_destination) const
 *
 * \brief Verifie s'il existe un arc entre la source et la destination
 *
 * \param[in] p_source Le numero du sommet constituant la source de l'arc
 * \param[in] p_destination Le numero du sommet constituant la destination de l'arc
 *
 * \exception logic_error si l'un des deux sommets est inexistant
 *
 * \return true si l'arc existe, false sinon
 */
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


/**
 * \fn std::vector<size_t> Graphe::listerSommetsAdjacents(size_t p_sommet) const
 *
 * \brief Donne la liste des sommets adjacents a p_sommet, c'est a dire ceux pour lesquels il existe un arc venant de p_sommet
 *
 * \param[in] p_sommet Le numero du sommet
 *
 * \exception logic_error si p_sommet est inexistant
 *
 * \return La liste des sommets adjacents a p_sommet
 */
std::vector<size_t> Graphe::listerSommetsAdjacents(size_t p_sommet) const
{
    sommetExiste(p_sommet, "Liste d'adjacence demandee pour un sommet inexistant");

    std::vector<size_t> destinations;

    for (std::list<Arc>::const_iterator it = m_listesAdj[p_sommet].begin(); it != m_listesAdj[p_sommet].end(); ++it)
    {
        destinations.push_back(it -> m_destination);
    }

    return destinations;
}


/**
 * \fn std::string Graphe::getNomSommet(size_t p_sommet) const
 *
 * \brief Donne le nom du sommet passe en argument
 *
 * \param[in] p_sommet Le numero du sommet
 *
 * \exception logic_error si p_sommet est inexistant
 *
 * \return Le nom du sommet
 */
std::string Graphe::getNomSommet(size_t p_sommet) const
{
    sommetExiste(p_sommet, "Nom de sommet demande pour un sommet inexistant");

    return m_villes[p_sommet].m_nom;
}


/**
 * \fn size_t Graphe::getNumeroSommet(const std::string& p_nom) const
 *
 * \brief Donne le numero du sommet passe en argument
 *
 * \param[in] p_nom Le nom du sommet
 *
 * \exception logic_error si p_nom est inexistant
 *
 * \return Le numero du sommet
 */
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
        throw std::logic_error("Numero de sommet demande pour un sommet inexistant");
    }

    return (size_t) numero;
}


/**
 * \fn int Graphe::getNombreSommets() const
 *
 * \brief Accesseur de l'attribut m_nbSommets
 *
 * \return L'attribut m_nbSommets, correspondant au nombre de sommets dans le graphe courant
 */
int Graphe::getNombreSommets() const
{
    return (int) m_nbSommets;
}

/**
 * \fn int Graphe::getNombreArcs() const
 *
 * \brief Accesseur de l'attribut m_nbArcs
 *
 * \return L'attribut m_nbArcs, correspondant au nombre d'arcs dans le graphe courant
 */
int Graphe::getNombreArcs() const
{
    return (int) m_nbArcs;
}


/**
 * \fn Ponderations Graphe::getPonderationsArc(size_t p_source, size_t p_destination) const
 *
 * \brief Donne les ponderations de l'arc allant de la source a la destination
 *
 * \param[in] p_source La source de l'arc
 * \param[in] p_destination La destination de l'arc
 *
 * \exception logic_error si l'un des sommets est inexistant
 * \exception logic_error s'il n'y a pas arc allant de la source a la destination
 *
 * \return La structure Ponderations contenue dans l'arc entre la source et la destination
 */
Ponderations Graphe::getPonderationsArc(size_t p_source, size_t p_destination) const
{
    if (!arcExiste(p_source, p_destination))
    {
        throw std::logic_error("Il n'y a pas d'arc entre ces deux sommets");
    }

    for (std::list<Arc>::const_iterator it = m_listesAdj[p_source].begin(); it != m_listesAdj[p_source].end(); ++it)
    {
        if (it -> m_destination == p_destination)
        {
            return it -> m_poids;
        }
    }
}


/**
 * \fn Coordonnees Graphe::getCoordonnesSommet(size_t p_sommet) const
 *
 * \brief Donne les coordonnees du sommet p_sommet
 *
 * \param[in] p_sommet Le numero du sommet
 *
 * \exception logic_error si le sommet est inexistant
 *
 * \return La structure Coordonnees contenant les coordonnes de la ville representee par le sommet
 */
Coordonnees Graphe::getCoordonnesSommet(size_t p_sommet) const
{
    sommetExiste(p_sommet, "Ce sommet n'existe pas");

    return m_villes[p_sommet].m_coord;
}

/**
 * \fn void Graphe::sommetsExistent(size_t p_source, size_t p_destination) const
 *
 * \brief Lance une exception si l'un des deux sommets d'une paire source-destination n'existe pas
 *
 * \param[in] p_source Le numero de la source
 * \param[in] p_destination Le numero de la destination
 *
 * \exception logic_error si l'un des deux sommets n'existe pas
 */
void Graphe::sommetsExistent(size_t p_source, size_t p_destination) const
{
    sommetExiste(p_source, "La source n'existe pas");
    sommetExiste(p_destination, "La destination n'existe pas");
}

/**
 * \fn void Graphe::sommetExiste(size_t p_sommet, std::string p_message) const
 *
 * \brief Lance une exception contenant le message p_message si le sommet n'existe pas
 *
 * \param[in] p_sommet Le numero du sommet
 * \param[in] p_message Le message a mettre dans l'exception
 *
 * \exception logic_error si le sommet n'existe pas
 */
void Graphe::sommetExiste(size_t p_sommet, std::string p_message) const
{
    if (p_sommet > m_nbSommets)
    {
        throw std::logic_error(p_message);
    }
}

}//Fin du namespace
