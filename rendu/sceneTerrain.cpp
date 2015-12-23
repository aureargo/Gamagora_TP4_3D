#include "scene.h"

bool Scene::renduTerrain() const{


    ColorGradient grad;
    grad.createDefaultHeatMapGradient();
    const Terrain* terrain = (Terrain*) node;

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

                if(!terrain->intersect(ray,dist,i)){
                    img.setPixel(x, y, default_color.rgba());
                }
                else
                {
                    float oclu;
                    float effetAtmos;
                    vec3 color = calculPixelTerrain(terrain, ray, dist, oclu);
                    color = calculEffetAtmospherique(color, ciel->color, dist, effetAtmos);
                    imEffetAtmos.setPixel(x,y,qRgb(255,255*(1-effetAtmos),255*(1-effetAtmos)));
                    imocclusion.setPixel(x,y,qRgb(255*(1-oclu),0,255*(oclu*oclu)));
                    img.setPixel(x,y, qRgb(color.r*255,color.g*255,color.b*255));
                }
                float r,v,b;
                grad.getColorAtValue(i/(float)256,r,v,b);
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

float Scene::calculOcclusionTerrain(const Terrain* terrain, const vec3& pos) const
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
        float dist; int j;
        if(!terrain->intersect(Rayon(p,dir), dist, j, 1.f))
            occlusion+= puis;
    }
    return occlusion/somme;
}

vec3 Scene::calculPixelTerrain(const Terrain* terrain, const Rayon& ray, float dist, float &occlusion) const
{
    #if 0
        return vec3(dist*0.17, dist*0.19, dist*0.23);
    #else
        const vec3 dRay = ray.getDirection();
        vec3 p(ray.getOrigine() + dRay*dist);
        if(!terrain->repositionne(p))
            return NOIR;    //on est trop profond dans le terrain, ce qui peut vouloir dire qu'on regarde sous le terrain à la bordure

        const vec3 n(terrain->getNormal(p));

        Material texture = terrain->getMaterial(p);
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
