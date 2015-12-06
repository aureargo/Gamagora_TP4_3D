#include <QApplication>
#include "node.h"
#include "rendu/scene.h"


#include "terrain/terrainnoise.h"

#include "QDebug"
#include "lib/voxel.h"
#include "rendu/myWindow.h"

#include "export/objmanager.h"


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

    TerrainNoise* terre = new TerrainNoise(2000,2000);
    Camera* cam = new Camera(vec3(-300,1000,700), vec3(1000,1000,000), 600,720,400);
    //Camera* cam = new Camera(vec3(700,700,300), vec3(800,800,000), 600,720,400);

    Lumiere* l = new Lumiere(vec3(1000,1000,1000));
    Lumiere* l2 = new Lumiere(vec3(1000,500,200), vec3(0.1,0.1,0.5), vec3(0.1,0.1,0.5), vec3(0.1,0.1,0.5), 4, 500);
    //scene.setNode(new OpUnion(&node, new OpT(terre,vec3(-500,-500,-170))));

    scene.setNode(terre);
    //scene.setNode(sphere);

    scene.addC(cam);
    scene.addL(l);
    scene.addL(l2);

    scene.rendu();

    delete terre;
    delete cam;
    delete l;
    delete l2;
    return 0;

#elif(test == 2)

    const int n = 50;

    Voxel vox(n, &node);

    ObjManager::voxelSave("./test.obj", vox);

    return 0;

#endif

    (void) argc;    (void) argv;

}

