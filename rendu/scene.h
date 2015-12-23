#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texturing/ciel/ciel.h"
#include "texturing/phong.h"
#include "./node.h"
#include "QTime"
#include "colorgradient.h"
#include "lib/sphere.h"

#include <QColor>
#include <QImage>
#include "terrain/terrain.h"

#define DIST_HORS_PORTEE 9999
#define MAX_ITERATION 512

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
    void setNode(Node* n);

    /** @param[in] c camera à ajouter à la scène*/
    void addC(const Camera& c);

    void addL(const Lumiere& l);
    void addL(const std::vector<Lumiere>& lumieres);

    void setCiel(Ciel* ciel);

    /**
     * @brief Créer la visualisation de la scène dans une image. Utilise le procédé de lancé de rayons. \n
     * Utilise render().
     * @return Si le rendu c'est bien déroulé.
     */
    bool rendu() const;

    bool renduTerrain() const;


private:

    /**
     * @brief Liste des objets dans la scène.
     */
    //std::vector<Terrain*> objects;
    Node* node;

    /**
     * @brief Liste des caméras dans la scène.
     */
    std::vector<Camera> cameras;

    std::vector<Lumiere> lumieres;

    Ciel* ciel = nullptr;

    /**
     * @brief Couleur de fond
     */
    const QColor default_color = QColor(116, 208, 241);

    vec3 calculPixel(const Rayon& ray, float dist, float &oclu) const;

    bool intersect(const Rayon& r, float& dist, int& i) const;
    bool intersect(const Rayon& r) const;
    bool intersect(vec3 p, const vec3& n, float& dist, int& i) const;
    bool intersect(vec3 p, const vec3& n) const;

    float calculPoisson(const vec3& pos, const vec3& n, std::vector<Lumiere>& lumieresCiel) const;

    float calculOcclusion(const vec3& pos) const;

    vec3 calculEffetAtmospherique(const vec3& colorOrigin, const vec3 &colorContribution, float distance, float& contribution) const;


    //performance accru pour un terrain uniquement
    float calculOcclusionTerrain(const Terrain* terrain, const vec3& pos) const;
    vec3 calculPixelTerrain(const Terrain* terrain, const Rayon& ray, float dist, float &occlusion) const;
};

#endif // SCENE_H
