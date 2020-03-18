#include <Eigen/Core>
#include "shape.cpp"
using namespace std;
using namespace Eigen;
#include <vector>

class Sphere: public Shape{
    private:
        float radius;
        Vector3d center;
        float k1;
        float k2;
        float k3;
        float p;
    
    public:
        Sphere(){
            
        }

        Sphere(float Radius, Vector3d Center,float R, float G, float B,float K1, float K2, float K3,float P){
            radius = Radius;
            center = Center;
            r = R;
            g = G;
            b = B;
            k1 = K1;
            k2 = K2;
            k3 = K3;
            p = P;
        }

        Vector3d get_center(){
            return center;
        }
        
        float get_radius(){
            return radius;
        }

        vector<float> getMaterials(){
            vector<float> m ;
            m.push_back(k1);
            m.push_back(k2);
            m.push_back(k3);
            m.push_back(p);
            return m;
        }
};