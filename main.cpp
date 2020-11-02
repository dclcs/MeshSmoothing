#include <iostream>
#include <cstdlib> //srand
#include <igl/readOFF.h>
#include <igl/readOBJ.h>
#include <igl/writeOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/adjacency_list.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/per_vertex_normals.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <imgui/imgui.h>
#include <map>
#include <random>

using namespace std;
using namespace igl;
using namespace Eigen;

MatrixXd generate_noise(int n, int m){
//    srand((unsigned)time(nullptr));
//    n = (MatrixXd::Random(rows, 3)).array().abs();
    MatrixXd M(n, m);
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> nd(0.0, 1.0);
    for(int i = 0 ; i < n; i ++){
        for(int j = 0 ; j < m ; j ++){
            M(i, j) = nd(gen);
        }
    }
    return M;
}

MatrixXd bilateral_mesh_denosing(MatrixXd& V, MatrixXi& F){
    MatrixXd vertex_normal;
    per_vertex_normals(V, F, vertex_normal);
    vector<vector<int>> idx;
    double theta_s = 0.1, theta_r = 0.1;
    MatrixXd newV(V.rows(), V.cols());
    adjacency_list(F, idx, true);
    for(int i = 0 ; i < V.rows(); i ++){
        VectorXd norm = vertex_normal.row(i);
        double d_sum = 0. , k_v = 0.;
        for(int j = 0 ; j < idx[i].size() ; j ++){
            int jidx = idx[i][j];
            VectorXd vij = V.row(i) - V.row(jidx);
            double t = (V.row(i) - V.row(jidx)).norm();
            double d = norm.dot(vij);
            double w_s = exp(-t * t / (2 * theta_s));
            double w_r = exp(-d * d/ (2 * theta_r));
            d_sum += w_s * w_r * d;
            k_v += w_s * w_r;
        }
        newV.row(i) = V.row(i) + (norm * d_sum / k_v).transpose();
    }
    return newV;
}

int main() {
    MatrixXd V, NoiseV, ShowV;
    MatrixXi F, NoiseF, ShowF;
    MatrixXd noise;
    readOFF("../data/fandisk.off", V, F);
    readOBJ("../data/fandisk-noisy.obj", NoiseV, NoiseF);
//    noise = generate_noise(V.rows(), V.cols());
    // Init the viewer
    igl::opengl::glfw::Viewer viewer;
    // Attach a menu plugin
    igl::opengl::glfw::imgui::ImGuiMenu menu;
    viewer.plugins.push_back(&menu);

    // Customize the menu
    double doubleVariable = 0.1f; // Shared between two menus

    // Add content to the default menu window
    menu.callback_draw_viewer_menu = [&]()
    {
        // Draw parent menu content
        menu.draw_viewer_menu();
        // Add new group
        if (ImGui::CollapsingHeader("New Group", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // ... or using a custom callback
            static bool add_noise = false;
            if (ImGui::Checkbox("bool", &add_noise))
            {
                std::cout << "add_noise: " << std::boolalpha << add_noise << std::endl;
                if(add_noise){
                    cout << "add noise " << endl;
                    ShowV = NoiseV;
                    ShowF = NoiseF;
                }else{
                    ShowV = V;
                    ShowF = F;
                }
                viewer.data().clear();
                viewer.data().set_mesh(ShowV, ShowF);

            }


            static bool denoising = false;
            if(ImGui::Checkbox("denoising", &denoising)){
                MatrixXd newV = bilateral_mesh_denosing(NoiseV, NoiseF);
                viewer.data().clear();
                viewer.data().set_mesh(newV, NoiseF);
            }
        }
    };


//    // Draw additional windows
//    menu.callback_draw_custom_window = [&]()
//    {
//        // Define next window position + size
//        ImGui::SetNextWindowPos(ImVec2(180.f * menu.menu_scaling(), 10), ImGuiCond_FirstUseEver);
//        ImGui::SetNextWindowSize(ImVec2(200, 160), ImGuiCond_FirstUseEver);
//        ImGui::Begin(
//                "New Window", nullptr,
//                ImGuiWindowFlags_NoSavedSettings
//        );
//
//        // Expose the same variable directly ...
//        ImGui::PushItemWidth(-80);
//        ImGui::DragScalar("double", ImGuiDataType_Double, &doubleVariable, 0.1, 0, 0, "%.4f");
//        ImGui::PopItemWidth();
//
//        static std::string str = "bunny";
//        ImGui::InputText("Name", str);
//
//        ImGui::End();
//    };

    // Plot the mesh
    viewer.data().set_mesh(NoiseV, NoiseF);
//    viewer.data().add_label(viewer.data().V.row(0) + viewer.data().V_normals.row(0).normalized()*0.005, "Hello World!");
    viewer.launch();


    return 0;
}
