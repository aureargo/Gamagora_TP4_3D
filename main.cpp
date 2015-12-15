#include <QApplication>
#include "node.h"
#include "rendu/scene.h"


#include "terrain/terrainnoise.h"

#include "QDebug"
#include "lib/voxel.h"
#include "rendu/myWindow.h"

#include "export/objmanager.h"

#include "texturing/ciel/cielsoleil.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



/*
 * test = 0 : Visualisation du voxel sous OpenGL
 * test = 1 : Rendu jpg du lancé de rayons sur SI+CSG+TERRAIN
 * test = 2 : Export du voxel en Mesh (obj)
 */

#define test 1

#if(test == 0)
    const int n = 75;

    Voxel vox(n, &node);

    myWindow myWin; //Ajout de notre classe myWindow
    myWin.vox = vox;
    myWin.show();   //Exécution de notre fenêtre de rendu OpenGL
    std::cout << "+ , - = speed"<<std::endl<<"z , s = zoom"<<std::endl<<"Up, Down = hauteur objet";

    return app.exec();

#elif(test == 1)
    Scene scene;

    TerrainNoise* terre = new TerrainNoise(4000,4000);
    //scene.addL(Ciel::poissonLumieres(vec3(terre->largeur/2,terre->longueur/2, 0)));

    //Camera cam = Camera(vec3(-1500,2000,2000), vec3(2000,2000,-200), 200,480,360);
    Camera cam = Camera(vec3(-1500,2000,2000), vec3(2000,2000,-200), 600,1024,720);
    //Camera cam = Camera(vec3(700,700,300), vec3(800,800,250), 300,480,360);
    //Camera cam = Camera(vec3(-300,00,-100), vec3(100,000,0), 600,480,360);
    //Camera cam = Camera(vec3(350,350,400), vec3(2000,2000,-200), 700,900,720);
    //Camera cam = Camera(vec3(700,700,300), vec3(800,800,000), 400,480,360);

    //SourceLumiere(vec3(0.25,0.25,0.75),0.2,1,1,1)
    //                                        couleur de base du ciel                  couleur du soleil            direction du soleil dans le ciel
    Ciel* ciel = new CielSoleil(SourceLumiere(vec3(0.8,0.8,1.0)), Soleil(SourceLumiere(vec3(1,1,0.1),0.2,0.8,0.9,1),glm::normalize(vec3(1,-1,1))));
    //Ciel* ciel = new Ciel(SourceLumiere());

    //Lumiere l = Lumiere(vec3(2000,2000,700));
    //Lumiere l2 = Lumiere(vec3(600,1500,300), vec3(0.1,0.1,0.9),0.2,1,1,2,500);
    //scene.setNode(new OpUnion(&node, new OpT(terre,vec3(-500,-500,-170))));

    scene.setNode(terre);
    //scene.setNode(sphere);

    scene.addC(cam);
    //scene.addC(cam1);
    //scene.addL(l);
    //scene.addL(l2);
    scene.setCiel(ciel);

    scene.rendu();

    delete terre;
    delete ciel;
    return 0;

#elif(test == 2)

    const int n = 50;

    Voxel vox(n, &node);

    ObjManager::voxelSave("./test.obj", vox);

    return 0;

#endif

    (void) argc;    (void) argv;

}

