// C++ include
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"
#include "utils.h"
#include "ray.cpp"

// Shortcut to avoid Eigen:: and std:: everywhere, DO NOT USE IN .h
using namespace std;
using namespace Eigen;

void rayTracer(vector<Sphere> spheres,bool projectionType,int shadingType){
    std::cout << "Simple ray tracer V.1" << std::endl;
    const std::string filename("result_1.png");
    MatrixXd C = MatrixXd::Zero(800,800); // Store the color
    MatrixXd R = MatrixXd::Zero(800,800);
    MatrixXd G = MatrixXd::Zero(800,800);
    MatrixXd B = MatrixXd::Zero(800,800);
    MatrixXd A = MatrixXd::Zero(800,800); // Store the alpha mask

    // The camera is orthographic, pointing in the direction -z and covering the unit square (-1,1) in x and y
    Vector3d e(0,0,1);
    Vector3d x_displacement(2.0/C.cols(),0,0);
    Vector3d y_displacement(0,-2.0/C.rows(),0);
    Vector3d halfX(-1,0,0);
    Vector3d halfY(0,1,0);
    // Multiple light sources
    const Vector3d light_position1(0,0,1);
    const Vector3d light_position2(-0.5,0.5,0.5);
    for (unsigned i=0;i<C.cols();i++)
    {
        for (unsigned j=0;j<C.rows();j++)
        {
            // Prepare the ray
            Vector3d ray_origin = e + double(i)*x_displacement + double(j)*y_displacement+halfX+halfY;
            Vector3d ray_direction = RowVector3d(0,0,-1);
            Vector3d pix(e(0),e(1),0.42);
            Vector3d p = pix + double(i)*x_displacement + double(j)*y_displacement+halfX+halfY;
            Ray ray = projectionType == 1 ? Ray(e,(p-e).normalized()) : Ray(ray_origin,ray_direction);
      
            for(Sphere o : spheres) {
                if(ray.ifIntersect(o)){
                Vector3d n = (ray.getIntersect()-ray.getCenter()).normalized();

                // Lambertian Shading
                // C(i,j) = (light_position-ray.getIntersect()).normalized().dot(n.normalized());

                Vector3d l1 =(light_position1-ray.getIntersect()).normalized();
                Vector3d l2 =(light_position2-ray.getIntersect()).normalized();
                Vector3d v = projectionType == 1 ? (e-ray.getIntersect()).normalized() : (p-ray.getIntersect()).normalized();
                Vector3d h1 = (v+l1)/((v+l1).norm());
                Vector3d h2 = (v+l2)/((v+l2).norm());
                vector<float> m = ray.getSphere().getMaterials();
                float a = 1.0*m[0] + pow(max(0.0,n.dot(h1)),m[3])*m[2] 
                +pow(max(0.0,n.dot(h2)),m[3])*m[2] 
                + max(0.0,n.dot(l1))*m[1] 
                + max(0.0,n.dot(l2))*m[1]
                ;
                //C(i,j) = a;
                R(i,j) = a * ray.getSphere().getR();
                G(i,j) = a * ray.getSphere().getG();
                B(i,j) = a * ray.getSphere().getB();
                // Disable the alpha mask for this pixel
                A(i,j) = 1;
                }
            }
        }
    }
    // Save to png
    write_matrix_to_png(R,G,B,A,filename);
}

vector<float> parserFloat(string str){
    vector<float> v;
    string delimiter = " ";
    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        v.push_back(stof(token));
        str.erase(0, pos + delimiter.length());
    }
    return v;
}

vector<int> parserInt(string str){
    vector<int> v;
    string delimiter = " ";
    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        v.push_back(stoi(token));
        str.erase(0, pos + delimiter.length());
    }
    v.push_back(stoi(str));
    return v;
}

void rayTracerMesh(Triangle tri){
    std::cout << "Simple ray tracer V.2" << std::endl;
    const std::string filename("result_2.png");
    MatrixXd C = MatrixXd::Zero(800,800); // Store the color
    MatrixXd A = MatrixXd::Zero(800,800); // Store the alpha mask
    Vector3d e(0,0,1);
    Vector3d x_displacement(2.0/C.cols(),0,0);
    Vector3d y_displacement(0,-2.0/C.rows(),0);

    Vector3d halfX(-1,0,0);
    Vector3d halfY(0,1,0);
    // ifstream in(filepath);
    // if(!in){
    //     cout << "Failed: The target file cannot be opened." << endl;
    // }
    // string str;
    // int count = 0;
    // int numV;
    // int numF;
    // MatrixXf V = MatrixXf::Zero(502,3);
    // MatrixXf F = MatrixXf::Zero(1000,3);
    // vector<float> tempV;
    // vector<int> vf;
    // vector<int> tempF;
    // while (getline(in, str)) {
    //     if(count == 0){
    //         count++;
    //         continue;
    //     }
    //     if(count == 1){
    //         vf = parserInt(str);
    //         numV = vf[0];
    //         numF = vf[1];
    //         // ArrayXXf V(vf[0],3);
    //         // ArrayXXf F(vf[1],3);
    //     }
    //     else if(count < numV+2){
    //         tempV = parserFloat(str);
    //         V(count-2,0) = tempV[0];
    //         V(count-2,1) = tempV[1];
    //         V(count-2,2) = tempV[2];
            
    //     }
    //     else{
    //         tempF = parserInt(str);
    //         F(count-numV-2,0) = tempF[1];
    //         F(count-numV-2,1) = tempF[2];
    //         F(count-numV-2,2) = tempF[3];
    //     }
    //     count++;
    // }

    // vector<Triangle> tris; 
    // for (unsigned i=0;i<vf[1];i++)
    // {
    //     int a = F(i,0);
    //     int b = F(i,1);
    //     int c = F(i,2);
    //     Vector3d v1(V(a,0),V(a,1),V(a,2));
    //     Vector3d v2(V(b,0),V(b,1),V(b,2));
    //     Vector3d v3(V(c,0),V(c,1),V(c,2));
    //     Triangle tri = Triangle(v1,v2,v3);
    //     tris.push_back(tri);
    // }

    const Vector3d light_position1(0,0,1);
    const Vector3d light_position2(0.5,0.5,1);
    for (unsigned i=0;i<C.cols();i++)
    {
        // cout << i << endl;
        for (unsigned j=0;j<C.rows();j++)
        {
            // Prepare the ray
            Vector3d ray_origin = e + double(i)*x_displacement + double(j)*y_displacement+halfX+halfY;
            Vector3d ray_direction = RowVector3d(0,0,-1);
            Vector3d pix(e(0),e(1),0.42);
            Vector3d p = pix + double(i)*x_displacement + double(j)*y_displacement+halfX+halfY;
            Ray ray = Ray(e,(p-e).normalized());
            if(ray.ifIntersect(tri)){
            	cout << "yes" << endl;
                // Vector3d n = (ray.getIntersect()-ray.getCenter()).normalized();
                // Vector3d l1 =(light_position1-ray.getIntersect()).normalized();
                // Vector3d l2 =(light_position2-ray.getIntersect()).normalized();
                // Vector3d v = (p-e).normalized();
                // Vector3d h1 = (v+l1)/((v+l1).norm());
                // Vector3d h2 = (v+l2)/((v+l2).norm());
                // float a = 0.2 + max(0.0,n.dot(l1));
                C(i,j) = 1;
                A(i,j) = 1;
            }
        }
    }
    write_matrix_to_png(C,C,C,A,filename);
}

int main()
{
    std::vector<Sphere> spheres;
    Vector3d center1(0.5,0.5,0);
    Vector3d center2(-0.5,0.5,0);
    Vector3d center3(-0.5,-0.5,0);
    Vector3d center4(0.5,-0.5,0);
    
    // 1.1 General Position
//    Sphere sphere1 = Sphere(0.4,center1,0.5,0.5,0.5,0,1,0,0);
//    Sphere sphere2 = Sphere(0.4,center2,0.5,0.5,0.5,0,1,0,0);
//    Sphere sphere3 = Sphere(0.4,center3,0.5,0.5,0.5,0,1,0,0);
//    Sphere sphere4 = Sphere(0.4,center4,0.5,0.5,0.5,0,1,0,0);

    // 1.2 Shading & 1.3 Perspective Projection
     Sphere sphere1 = Sphere(0.4,center1,0.8,0.12,0.5,0,0.8,0,0);
     Sphere sphere2 = Sphere(0.4,center2,0.43,0.10,0.2,0.5,0.8,0,0);
     Sphere sphere3 = Sphere(0.4,center3,0.55,0.80,0.3,0.2,0.4,0.5,1.1);
     Sphere sphere4 = Sphere(0.4,center4,0.4,0.29,0.20,0,1,0.5,1.2);
    
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);
    Vector3d v1(0.149725, 0.126524, 0.282017);
    Vector3d v2(-0.368932, 0.127793, 0.171742 );
    Vector3d v3(0.040147, 0.495206, 0.315324);
    Triangle tri = Triangle(center1,center2,center3);
    
    // Different ProjectionType, 1:Orthogonal 2:Perspective
    //rayTracer(spheres,1,0);
    rayTracer(spheres,0,0);
    // vector<Triangle> tris;
    // tris.push_back(tri);

    // Able to output several triangles correctly, but for the mesh it takes too long to finish. There could be something wrong with my laptop.
    rayTracerMesh(tri);
    return 0;
}
