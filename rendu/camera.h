#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "float.h"  //FLT_MIN FLT_MAX
#include <QDebug>
#include <glm/vec3.hpp>
#include "glm/gtx/norm.hpp"

using namespace glm;

/**
 * @brief Une caméra utilisé pour visualiser la scène.
 */
class Camera
{
protected:

    /**
     * @brief Point d'origine de la caméra
     */
    vec3 _origine;

    /**
     * @brief Vecteur direction en "x" du plan écran.
     */
    vec3 _u;
    /**
     * @brief Vecteur direction en "y" du plan écran.
     */
    vec3 _v;
    /**
     * @brief Vecteur direction en "z" du plan écran.
     */
    vec3 _w;

    /**
     * @brief Largeur en pixel de l'écran.
     */
    int _lu;
    /**
     * @brief Hauteur en pixel de l'écran.
     */
    int _lv;
    /**
     * @brief Distance du plan de l'écran par rapport #_origine.
     */
    float _lw;


public:
    Camera();
    /**
     * @brief Constructeur de la classe caméra avec des paramètres.
     * @param[in] pOr Point d'origine de la caméra.
     * @param[in] pAt Point cible de la caméra. Détermine la direction de la caméra.
     * @param[in] distance Distance entre l'origine de la caméra et le plan de l'écran.
     * @param[in] l Largeur en pixel de l'écran.
     * @param[in] h Hauteur en pixel de l'écran.
     */
    Camera(const vec3& pOr, const vec3& pAt, const float& distance, int l, int h);

    /**
     * @brief Calcul le vecteur direction entre l'origine de la caméra et le pixel (i,j) de l'écran.\n
     * Utilise la méthode pointScreen().
     * @param[in] i Indice du pixel en largeur de l'écran.
     * @param[in] j Indice du pixel en longueur de l'écran.
     * @return Le vecteur direction entre #_origine et le pixel (i,j).
     * @warning Si i ou j sont hors écran, le programme s'arretera.
     */
    vec3 vecScreen(int i, int j) const;
    /**
     * @brief Calcul la position du pixel (i,j) de l'écran.
     * @param[in] i Indice du pixel en largeur de l'écran.
     * @param[in] j Indice du pixel en longueur de l'écran.
     * @return La position du pixel (i,j).
     * @warning Si i ou j sont hors écran, le programme s'arretera.
     */
    vec3 pointScreen(int i, int j) const;

    int getLu() const{
        return _lu;
    }

    int getLv() const{
        return _lv;
    }

    vec3 getOrigine() const{
        return _origine;
    }

};
#endif // CAMERA_H
