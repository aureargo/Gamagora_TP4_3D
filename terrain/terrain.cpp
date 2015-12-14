#include "terrain.h"


Terrain::Terrain():
    Terrain(0,0)
{
}

Terrain::Terrain(float longueur, float largeur):
    Terrain(longueur, largeur, 0)
{   
}

Terrain::Terrain(float longueur, float largeur, float amplitude):
    longueur(longueur), largeur(largeur)
{
    box = Box(vec3(0,0,0),vec3(largeur,longueur,amplitude));
    heatMapGradient.createDefaultHeatMapGradient();
}

/**********************************************************/


float Terrain::getHauteur(const vec2& pointXY) const
{
    return getHauteur(pointXY.x, pointXY.y);
}

float Terrain::getHauteur(const vec3& pointXYZ) const
{
    return getHauteur(pointXYZ.x, pointXYZ.y);
}


/**********************************************************/


vec3 Terrain::getNormal(const vec2& pointXY, float eps) const
{
    return getNormal(pointXY.x, pointXY.y, eps);
}


vec3 Terrain::getNormal(const vec3& pointXYZ, float eps) const
{
    return getNormal(pointXYZ.x, pointXYZ.y, eps);
}

/**********************************************************/


bool Terrain::inOut(const vec3& pointXYZ) const
{
    if(pointXYZ.z < 0)
        return false;
    float h = getHauteur(pointXYZ.x, pointXYZ.y);
    return (h != HAUTEUR_HORS_MAP && pointXYZ.z <= h); //
}
float Terrain::potentiel(const glm::vec3& p) const
{
    return inOut(p) ?   1.f :   0.f;
}

bool Terrain::intersect(const Rayon& rayon, float &coeffDistance, int &i) const
{
    float dmin;
    float dmax;

    if(!box.intersect(rayon, dmin, dmax ))
        return false;

    //à modifier à cause de la précision des floats, mais c'est bizarre que ça marche comme un gant. faire attention que le point de départ ne soit pas en dehors de la box.
    //dmin = 0.0;
    //dmax = 3000.0;

    coeffDistance = dmin;

    for(i = 0;  i<128;  i++)
    {
        vec3 pos = rayon.getOrigine() + coeffDistance*rayon.getDirection();
        float h = getHauteur( pos );
        if(h == HAUTEUR_HORS_MAP)        {
            if(i == 0)  {
                coeffDistance += 0.01f;
                continue;
            }
            else
                break;
        }
        /*if(i == 0 && h == 0)
            h += 0.01;  //dans le cas des lancée de rayon vers le ciel
        else*/
        h = fabsf(pos.z - h);

        if( h <(0.002 * coeffDistance) ) {
                return true;
        }else if(coeffDistance > dmax )
                break;

        coeffDistance += 0.3*h;

    }

    return false;
}

/************************************************************************/



/*void Terrain::getColor2(float& r, float& g, float& b, float hauteur, const Eigen::vec3& n) const
{
    (void) n;
    heatMapGradient.getColorAtValue(hauteur, r,g,b);
}*/


Material Terrain::getMaterial(const vec3& p) const
{
    return getMaterial(p.x, p.y, p.z);
}

Material Terrain::getMaterial(float x, float y, float z) const
{
    float h = getHauteur(x,y);
    //if(z+1 < h)
    //    return getTextureSousSol(x,y,z);    //on retourne une texture de terre;
    (void) z;


    float hauteur = h / box.diffZ();
    vec3 normale = getNormal(x,y);
    float pente = 1-dot(normale, vec3(0,0,1));

    float roche = 3 + NoiseGenerator::perlinNoiseGradiant2(x, y, 1000) + NoiseGenerator::perlinNoiseGradiant2(x+30, y-60, 103) + NoiseGenerator::perlinNoiseGradiant2(x,y, 3);
    roche *= 0.25f+pente*0.75f;
    roche *= 0.40f;

    float herbe = 3 + NoiseGenerator::perlinNoiseGradiant2(x+10100, y+10100, 1234) + NoiseGenerator::perlinNoiseGradiant2(x-100,y-50, 123) + NoiseGenerator::perlinNoiseGradiant2(x-1, y-30,2);
    herbe *= 0.25f+(1-hauteur)*(1-hauteur)*0.75f;
    herbe *= 0.3f;

    float neige = 3 + NoiseGenerator::perlinNoiseGradiant2(x+555, y+1010, 1324) + NoiseGenerator::perlinNoiseGradiant2(x-200,y-54, 223) + NoiseGenerator::perlinNoiseGradiant2(x-10, y+12,0.5);
    neige *= 0.1f+hauteur*hauteur*0.9f;
    neige *= 0.40f;

    Material texture;
    if(herbe > roche && herbe > neige)    {
        if(herbe > 1)
            herbe = 1;
        texture = Herbe(herbe);
    }
    else if(roche > herbe && roche > neige)    {
        if(roche > 1)
            roche = 1;
        texture = Roche(roche);
    }
    else    {
        if(neige > 1)
            neige = 1;
        texture = Neige(neige);
    }
    return texture;
}

Material Terrain::getTextureSousSol(float x, float y, float z) const
{
    return Material(vec3(0,0,0));
    //float roche = 3 + NoiseGenerator::perlinNoiseGradiant2(z, z, 1000) + NoiseGenerator::perlinNoiseGradiant2(z, z, 103) + NoiseGenerator::perlinNoiseGradiant2(z,z, 3);
    //return Roche(roche/6);
    (void) x;   (void) y;   (void) z;
}

vec3 Terrain::getColor(const vec3& p) const
{
    return getColor(p.x, p.y);
}

vec3 Terrain::getColor(float x, float y) const
{
    float h = getHauteur(x,y);
    float hauteur = h / box.diffZ();
    vec3 normale = getNormal(x,y);
    float pente = 1-dot(normale, vec3(0,0,1));

    float roche = 3 + NoiseGenerator::perlinNoiseGradiant2(x, y, 1000) + NoiseGenerator::perlinNoiseGradiant2(x+30, y-60, 103) + NoiseGenerator::perlinNoiseGradiant2(x,y, 3);
    roche *= 0.25f+pente*0.75f;
    roche *= 0.3f;

    float herbe = 3 + NoiseGenerator::perlinNoiseGradiant2(x+10100, y+10100, 1234) + NoiseGenerator::perlinNoiseGradiant2(x-100,y-50, 123) + NoiseGenerator::perlinNoiseGradiant2(x-1, y-30,2);
    herbe *= 0.25f+(1-hauteur)*(1-hauteur)*0.75f;
    herbe *= 0.25f;

    float neige = 3 + NoiseGenerator::perlinNoiseGradiant2(x+555, y+1010, 1324) + NoiseGenerator::perlinNoiseGradiant2(x-200,y-54, 223) + NoiseGenerator::perlinNoiseGradiant2(x-10, y+12,0.5);
    neige *= 0.1f+hauteur*hauteur*0.9f;
    neige *= 0.25f;


    Material texture;
    if(herbe > roche && herbe > neige)    {
        if(herbe > 1)
            herbe = 1;
        texture = Herbe(herbe);
    }
    else if(roche > herbe && roche > neige)    {
        if(roche > 1)
            roche = 1;
        texture = Roche(roche);
    }
    else    {
        if(neige > 1)
            neige = 1;
        texture = Neige(neige);
    }
    return texture.getColor();
}

/***********************************************************************/


float Terrain::distance(const glm::vec3& p) const
{
    float dist = box.distance(p);
    if(dist > 0)
        return dist+0.1f;   //on essaye de rentrer dans la box
    else
        return (p.z-getHauteur(p))*0.25f;
}




void Terrain::setColor(const vec3& color)
{
    std::cerr << "le terrain ne doit pas changer de couleur" << std::endl;
    (void) color;
}

Box Terrain::getBox() const
{
    return box;
}
