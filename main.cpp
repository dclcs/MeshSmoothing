#include <iostream>
#include <cstdlib>
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
using namespace std;
using namespace igl;
using namespace Eigen;
void add_noise(MatrixXd& V){
    for(int i = 0 ; V.rows() ; i ++){
        cout << V.row(i) << endl;
    }
}
int main() {
    MatrixXd V;
    MatrixXi F;
    readOFF("../data/fandisk.off", V, F);

    add_noise(V);
    opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.launch();


    return 0;
}
