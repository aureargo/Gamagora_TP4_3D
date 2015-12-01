#include "scene.h"


Scene::Scene()
{
}


bool Scene::rendu(){
    QTime timer;

    ColorGradient grad;
    grad.createDefaultHeatMapGradient();

    for(unsigned int ic = 0;  ic < cameras.size(); ic++)
    {
        Camera* c = cameras[ic];
        int _lu = c->getLu(), _lv = c->getLv();
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
            for(int x = 0; x < _lu ; x++){  // pour chaque pixel de la ligne
                Rayon ray(c->getOrigine(),c->vecScreen(x,y));   //rayon correspondant au pixel
                bool touche = false;


                glm::vec3 p = c->getOrigine();
                int i;
                float dist = 0;
                for(i = 0;  i < 512;    i++)
                {
                    if(node->inOut(p)){
                        touche = true;
                        break;
                    }
                    float d = node->distance(p)+0.05f;
                    dist += d;
                    if(dist > 9999) //si on va trop loin, on arrête la progression.
                        break;
                    p += ray.getDirection()*(d);

                }
                if(!touche)
                    img->setPixel(x, y, default_color.rgba());
                else
                {
                    #if 0
                        img->setPixel(x,y, qRgb(dist*17, dist*19, dist*23));
                    #else

                        const vec3& dRay = ray.getDirection();
                        const vec3 p2(ray.getOrigine()+dRay*dist);
                        const vec3 n(node->getNormal(p2));

                        double norm = dot(-dRay, n);    //le rayon va normalement dans le sens inverse de la normal du triangle qu'il touche,
                        //double norm = (1+dot(-dRay, n))*0.5;    //version Galin

                        QRgb color;
                        if(norm <= 0)
                            color = qRgb(0,0,0); //Black
                        else
                        {
                            //float c = 255*norm;
                            QRgb c = node->getColor(p2);
                            color = qRgb(qRed(c)*norm,qGreen(c)*norm, qBlue(c)*norm); // Grey
                        }
                        img->setPixel(x,y, color);

                    #endif
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

