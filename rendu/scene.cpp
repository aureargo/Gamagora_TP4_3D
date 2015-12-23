﻿#include "scene.h"


Scene::Scene()
{
}


bool Scene::rendu() const{


    ColorGradient grad;
    grad.createDefaultHeatMapGradient();

    for(unsigned int ic = 0;  ic < cameras.size(); ic++)
    {
        const Camera& c = cameras[ic];
        int _lu = c.getLu(), _lv = c.getLv();
        std::cout << "ic: " << ic << //"   origine: " << c.getOrigine() <<
                     "  lu: " << _lu << "  lv: " << _lv << std::endl;
        int pourcent2 = -1;
        QImage imEffetAtmos(_lu, _lv, QImage::Format_RGB888);
        QImage imocclusion(_lu, _lv, QImage::Format_RGB888);
        QImage img(_lu, _lv, QImage::Format_RGB888);
        QImage eric(_lu, _lv, QImage::Format_RGB888);
        QTime timer;
        timer.start();
#ifdef QT_NO_DEBUG
        #pragma omp parallel for schedule(dynamic,1)
#endif
        for(int y = 0; y < _lv ; y++){      // pour chaque ligne de l'image
            int pourcent = 100 * y / (_lv - 1);
            if(pourcent != pourcent2)            {
                pourcent2 = pourcent;
                std::cout << "\r" << ic << " Rendering: " << pourcent << "% ";  // barre de progression
            }
            else
                std::cerr << ".  \b\b";

            for(int x = 0; x < _lu ; x++){  // pour chaque pixel de la ligne
                Rayon ray(c.getOrigine(),c.vecScreen(x,y));   //rayon correspondant au pixel

                float dist;
                int i;
                float oclu;
                float effetAtmos;
                if(!intersect(ray, dist, i)){
                    img.setPixel(x, y, default_color.rgba());
                    imEffetAtmos.setPixel(x,y,default_color.rgba());
                }
                else
                {
                    vec3 color = calculPixel(ray, dist, oclu);
                    color = calculEffetAtmospherique(color, ciel->color, dist, effetAtmos);
                    imEffetAtmos.setPixel(x,y,qRgb(255,255*(1-effetAtmos),255*(1-effetAtmos)));
                    imocclusion.setPixel(x,y,qRgb(255*(1-oclu),0,255*(oclu)));
                    img.setPixel(x,y, qRgb(color.r*255,color.g*255,color.b*255));
                }
                float r,v,b;
                grad.getColorAtValue(i/(float)MAX_ITERATION,r,v,b);
                eric.setPixel(x,y, qRgb(r*255,v*255,b*255));
            }
        }

        std::cout << std::endl;
        int time = timer.elapsed();
        std::cout << time/1000 << "." << time%1000<< " secondes" << std::endl;

        if(ic<10)        {
            img.save(("test000" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric000" + std::to_string(ic) + ".png").c_str());
            imocclusion.save(("oclu000" + std::to_string(ic) + ".png").c_str());
            imEffetAtmos.save(("EffetAtmos000" + std::to_string(ic) + ".png").c_str());

            std::cout << ("test000" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<100)        {
            img.save(("test00" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric00" + std::to_string(ic) + ".png").c_str());
            imocclusion.save(("oclu00" + std::to_string(ic) + ".png").c_str());
            imEffetAtmos.save(("EffetAtmos00" + std::to_string(ic) + ".png").c_str());

            std::cout << ("test00" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<1000)        {
            img.save(("test0" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric0" + std::to_string(ic) + ".png").c_str());
            imocclusion.save(("oclu0" + std::to_string(ic) + ".png").c_str());
            imEffetAtmos.save(("EffetAtmos0" + std::to_string(ic) + ".png").c_str());

            std::cout << ("test0" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else    {
            img.save(("test" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric" + std::to_string(ic) + ".png").c_str());
            imocclusion.save(("oclu" + std::to_string(ic) + ".png").c_str());
            imEffetAtmos.save(("EffetAtmos" + std::to_string(ic) + ".png").c_str());

            std::cout << ("test" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
    }
    return true;
}

/*****************************************************/

bool Scene::intersect(const Rayon& r, float& dist, int& i) const
{
    return intersect(r.getOrigine(), r.getDirection(), dist, i);
}
bool Scene::intersect(const Rayon& r) const
{
    return intersect(r.getOrigine(), r.getDirection());
}


bool Scene::intersect(vec3 p, const vec3& n, float& dist, int& i) const
{
    dist = 0;
    for(i = 0;  i < MAX_ITERATION;    i++)
    {
        if(node->inOut(p))
            return true;

        float d = node->distance(p)+0.001f*i;
        dist += d;
        if(dist > DIST_HORS_PORTEE) //si on va trop loin, on arrête la progression.
            return false;
        p += n*(d);
    }
    return false;
}

bool Scene::intersect(vec3 p, const vec3& n) const
{
    float dist = 0;
    for(int i = 0;  i < MAX_ITERATION;    i++)
    {
        if(node->inOut(p))
            return true;

        float d = node->distance(p)+0.001f*i;
        dist += d;
        if(dist > DIST_HORS_PORTEE) //si on va trop loin, on arrête la progression.
            return false;
        p += n*(d);
    }
    return false;
}

/*****************************************************/

float Scene::calculOcclusion(const vec3& pos) const
{
    if(ciel == nullptr)
        return 1;
    float occlusion = 0;
    float somme = 0;
    for(unsigned int i = 0;  i < ciel->dirOcclusion.size();  i++)
    {
        const vec3& dir = ciel->dirOcclusion[i];
        const float puis = ciel->getPuissance(i);
        somme += puis;

        vec3 p = pos + dir*0.1f;   //un peu déplacer pour ne pas démarrer pile sur le terrain et avancer de 0
        if(!intersect(p, dir))
            occlusion+= puis;
    }
    return occlusion/somme;
}

vec3 Scene::calculPixel(const Rayon& ray, float dist, float &occlusion) const
{
    #if 0
        return vec3(dist*0.17, dist*0.19, dist*0.23);
    #else

        const vec3 dRay = ray.getDirection();
        vec3 p(ray.getOrigine() + dRay*dist);
        if(!node->repositionne(p))
            return NOIR;    //on est trop profond dans le terrain, ce qui peut vouloir dire qu'on regarde sous le terrain à la bordure

        const vec3 n(node->getNormal(p));

        Material texture = node->getMaterial(p);
        //return texture.getColor();

        occlusion = calculOcclusion(p);
        if(occlusion > 0)
        {
            vec3 color = texture.getAmbiant();
            color += phong(texture, p, lumieres, n, -dRay);
            color += ciel->phongCiel(texture, n, -dRay);

            color *= occlusion;
            color = glm::clamp(color, vec3(0,0,0), vec3(1,1,1));
            return color;
        }
        else
            return NOIR;

    #endif
}

vec3 Scene::calculEffetAtmospherique(const vec3 &colorOrigin, const vec3 &colorContribution, float distance, float &contribution) const
{
    float efficaciteAtmosphere = 2000;
    contribution = exp(- (distance-3000)/efficaciteAtmosphere );

    if(contribution >= 1){
        contribution = 1;
        return colorOrigin;
    }

    return colorOrigin - (colorOrigin-colorContribution)*(1-contribution);
}

/**********************************************************/

void Scene::setNode(Node* n){
    node = n;
}

void Scene::addC(const Camera& c){
    cameras.push_back(c);
}

void Scene::addL(const Lumiere& l){
    lumieres.push_back((l));
}
void Scene::addL(const std::vector<Lumiere>& lumieres)
{
    if(this->lumieres.empty())
        this->lumieres = lumieres;
    else
    {
        for(const Lumiere& l: lumieres)
            this->lumieres.push_back(l);
    }
}

void Scene::setCiel(Ciel* ciel)
{
    this->ciel = ciel;
}
