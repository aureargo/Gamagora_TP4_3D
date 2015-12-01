#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "./node.h"
#include "QTime"
#include "colorgradient.h"

#include <QColor>
#include <QImage>

/**
    @author Aurelien Argoud
    @brief Correspond à un scène dans sa globalité. C'est-à-dire lumières, caméras et objets à observer.
*/
class Scene
{
public:

    Scene();

    /**
    * @brief ajoute un objet dans la scène
    * @param[in] l'objet à ajouter dans la scène (un terrain pour l'instant)
    */
    /*void addO(Node* obj){
        objects.push_back(obj);
    }*/
    void setNode(Node* n){
        node = n;
    }

    /** @param[in] c camera à ajouter à la scène*/
    void addC(Camera* c){
        cameras.push_back(c);
    }

    /**
     * @brief Créer la visualisation de la scène dans une image. Utilise le procédé de lancé de rayons. \n
     * Utilise render().
     * @return Si le rendu c'est bien déroulé.
     */
    bool rendu();


private:

    /**
     * @brief Liste des objets dans la scène.
     */
    //std::vector<Terrain*> objects;
    Node* node;

    /**
     * @brief Liste des caméras dans la scène.
     */
    std::vector<Camera*> cameras;

    /**
     * @brief Couleur de fond
     */
    const QColor default_color = QColor(116, 208, 241);


};

#endif // SCENE_H
