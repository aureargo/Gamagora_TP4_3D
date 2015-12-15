#include "scene.h"


Scene::Scene()
{
}


bool Scene::rendu() const{
    QTime timer;

    ColorGradient grad;
    grad.createDefaultHeatMapGradient();

    for(unsigned int ic = 0;  ic < cameras.size(); ic++)
    {
        const Camera& c = cameras[ic];
        int _lu = c.getLu(), _lv = c.getLv();
        std::cout << "ic: " << ic << //"   origine: " << c.getOrigine() <<
                     "  lu: " << _lu << "  lv: " << _lv << std::endl;
        int pourcent2 = -1;
        QImage *img = new QImage(_lu, _lv, QImage::Format_RGB888);
        QImage eric(_lu, _lv, QImage::Format_RGB888);
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
                if(!intersect(ray, dist, i))
                    img->setPixel(x, y, default_color.rgba());
                else
                {
                    vec3 color = calculPixel(ray, dist, c.getOrigine());
                    img->setPixel(x,y, qRgb(color.r*255,color.g*255,color.b*255));
                }
                float r,v,b;
                grad.getColorAtValue(i/255.0f,r,v,b);
                eric.setPixel(x,y, qRgb(r*255,v*255,b*255));
            }
        }
        if(ic<10)        {
            std::cout << std::endl;
            int time = timer.elapsed();
            std::cout << time/1000 << "." << time%1000<< " secondes" << std::endl;
            //std::cout << (100.f*nbpixrouge) / (_lu*_lv) << "%" << std::endl;
            img->save(("test000" + std::to_string(ic) + ".png").c_str());
            eric.save(("eric" + std::to_string(ic) + ".png").c_str());


            std::cout << ("test000" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<100)        {
            img->save(("test00" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test00" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else if(ic<1000)        {
            img->save(("test0" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test0" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        else    {
            img->save(("test" + std::to_string(ic) + ".png").c_str());
            std::cout << ("test" + std::to_string(ic) + ".png").c_str() << std::endl;
        }
        delete img;
    }
    return true;
}

/*****************************************************/

bool Scene::intersect(const Rayon& r, float& dist, int& i) const
{
    return intersect(r.getOrigine(), r.getDirection(), dist, i);
}


bool Scene::intersect(vec3 p, const vec3& n, float& dist, int& i) const
{
    dist = 0;
    for(i = 0;  i < 512;    i++)
    {
        if(node->inOut(p))
            return true;

        float d = node->distance(p)+0.02f;
        dist += d;
        if(dist > 9999) //si on va trop loin, on arrête la progression.
            return false;
        p += n*(d);
    }
    return false;
}

/*****************************************************/

float Scene::calculPoisson(const vec3& pos, const vec3& n, std::vector<Lumiere>& lumieresCiel) const
{
    const std::vector<vec3> vecPoisson = poissonDemiSphere(n, NB_RAYONS_CIEL, 0.05);   //liste de vecteur aléatoire de taille 1 dans la demi-sphere de n;

    lumieresCiel.reserve(vecPoisson.size());

    int rayonOmbre = 0; //nombre de rayon ayant été intersecté avant d'atteindre les cieux
    for(const vec3& n2: vecPoisson)
    {
        vec3 p = pos+n2*0.05f; //on crée un rayon un peu décalé par rapport à notre point et on souhaite l'envoyer vers le ciel.

        int i;        float dist;
        if(!intersect(p, n2, dist, i))  {
            if(ciel != nullptr)
                lumieresCiel.push_back(ciel->getLumiere(pos, n2));
        }
        else
            rayonOmbre++;
    }
    return (vecPoisson.size()-rayonOmbre)/((float)vecPoisson.size());
}

vec3 Scene::calculPixel(const Rayon& ray, float dist, const vec3& oeil) const
{
    #if 0
        return vec3(dist*0.17, dist*0.19, dist*0.23);
    #else

        const vec3 dRay = ray.getDirection();
        vec3 p(ray.getOrigine() + dRay*dist);
        node->repositionne(p);
        const vec3 n(node->getNormal(p));

        Material texture = node->getMaterial(p);

        std::vector<Lumiere> lumieres2;
        lumieres2.reserve(NB_RAYONS_CIEL+this->lumieres.size());
        float ombre = calculPoisson(p, n, lumieres2);
        if(ombre > 0)
        {
            for(const Lumiere& l: this->lumieres)
                lumieres2.push_back(l);

            vec3 color = phong(texture, p, lumieres2, n, oeil);
            color *= ombre*ombre;
            color = glm::clamp(color, vec3(0,0,0), vec3(1,1,1));
            return color;
        }
        else
            return NOIR;

    #endif
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
