#ifndef RAYON_H
#define RAYON_H

#include <glm/vec3.hpp>
#include "glm/gtx/norm.hpp"
#include <math.h>


/**
 * @brief Classe représentant un rayon lumineux
 */
class Rayon
{
protected:
    /**
     * @brief position d'origine du rayon.
     */
    glm::vec3 _origine;
    /**
     * @brief direction du rayon.
     */
    glm::vec3 _direction;

public:

    Rayon();

    /**
     * @brief Constructeur paramètré.
     * @param[in] Position d'origine du rayon.
     * @param[in] Vecteur direction du rayon.
     */
    Rayon(const glm::vec3& origine, const glm::vec3& direction);

    /**
     * @brief Calcul la direction du rayon réfléchie par rapport à une surface.
     * @param[in] normal La normale à de la surface.
     * @return Le vecteur direction du rayon après réflection.
     */
    glm::vec3 Reflect(const glm::vec3& normal);

    inline glm::vec3 getOrigine()const{return _origine;}

    inline glm::vec3 getDirection()const{return _direction;}
    inline void setDirection(const glm::vec3& dir){ _direction = glm::normalize(dir);   }

};

#endif // RAYON_H
