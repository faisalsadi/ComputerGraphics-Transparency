#include <gui/model.h>

#include <iostream>

#include "app.h"

int main() {
    App app;
    app.model.scale = app.model.scale * 5;
    app.model.position = app.model.position + crl::P3D::P3D(0, +10, 0);
    
  
    app.model2.scale = app.model2.scale * 5;
    app.model2.position = app.model2.position + crl::P3D::P3D(0, +12,0 );
    //app.model2.drawtransparency = true;


    app.model3.scale = app.model3.scale * 5;
    app.model3.position = app.model3.position + crl::P3D::P3D(0, +14, 0);

    app.model4.scale = app.model4.scale * 5;
    app.model4.position = app.model4.position + crl::P3D::P3D(3, +10, 0);


    app.model5.scale = app.model5.scale * 5;
    app.model5.position = app.model5.position + crl::P3D::P3D(3, +12, 0);

    app.model6.scale = app.model6.scale * 5;
    app.model6.position = app.model6.position + crl::P3D::P3D(3, +14, 0);


    ///


    app.model1.scale = app.model1.scale * 5;
    app.model1.position = app.model1.position + crl::P3D::P3D(0, +10, 3);


    app.model21.scale = app.model21.scale * 5;
    app.model21.position = app.model21.position + crl::P3D::P3D(0, +12, 3);
    //app.model2.drawtransparency = true;


    app.model31.scale = app.model31.scale * 5;
    app.model31.position = app.model31.position + crl::P3D::P3D(0, +14, 3);

    app.model41.scale = app.model41.scale * 5;
    app.model41.position = app.model41.position + crl::P3D::P3D(3, +10, 3);


    app.model51.scale = app.model51.scale * 5;
    app.model51.position = app.model51.position + crl::P3D::P3D(3, +12, 3);

    app.model61.scale = app.model61.scale * 5;
    app.model61.position = app.model61.position + crl::P3D::P3D(3, +14, 3);




    //app.model7.scale = app.model7.scale * 5;
    app.model7.position = app.model7.position + crl::P3D::P3D(25, 10, 7.5);
    
    app.setCallbacks();
   

    app.run();

    return 0;
}
