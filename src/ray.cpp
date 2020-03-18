#include <Eigen/Core>
#include <iostream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
#include "sphere.cpp"
#include "triangle.cpp"
class Ray 
{ 
    private:
        Vector3d origin;
        Vector3d direction;
        float lengthOtoI;
        Vector3d originToIntersect;
        Vector3d intersectCenter;
        Sphere interSphere;
        float oldt;
        
    public:
        Ray(Vector3d Origin, Vector3d Direction){
            origin = Origin;
            direction = Direction;
            lengthOtoI = INFINITY;
            interSphere = Sphere();
        }

        Vector3d getOrigin(){
            return origin;
        }

        Vector3d getDirection(){
            return direction;
        }

        Vector3d getIntersect(){
            return origin+originToIntersect;
        }

        Vector3d getCenter(){
            return intersectCenter;
        }

        Sphere getSphere(){
            return interSphere;
        }

        bool ifIntersect(Sphere sphere){
            Vector3d originToSphere = sphere.get_center() - origin;
            float projection = originToSphere.dot(direction);
            Vector3d centerToRay = originToSphere - direction*projection;
            float distSq =  centerToRay.dot(centerToRay);
            float radiusSq = sphere.get_radius()*sphere.get_radius();

            if(radiusSq < distSq || projection < 0){
                //cout << (radiusSq < distSq) << endl;
                return false;
            }

            float adjacent = sqrtf(radiusSq - distSq);
            float currLength = projection - adjacent;
            if (currLength < lengthOtoI){
                intersectCenter = sphere.get_center();
                originToIntersect = currLength*direction;
                interSphere = sphere;
                return true;
            }
            return false; 
        }

        bool ifIntersect(Triangle triangle){
            Matrix3f A;
            Vector3f b;
            Vector3d ver1 = triangle.getVer1();
            Vector3d ver2 = triangle.getVer2();
            Vector3d ver3 = triangle.getVer3();
            A << ver1(0)-ver2(0),ver1(0)-ver3(0),direction(0), ver1(1)-ver2(1),ver1(1)-ver3(1),direction(1), ver1(2)-ver2(2),ver1(2)-ver3(2),direction(2);
            b << ver1(0)-origin(0), ver1(1)-origin(1), ver1(2)-origin(2);
            Vector3f x = A.colPivHouseholderQr().solve(b); 
            float u = x(0);
            float v = x(1);
            float t = x(2);
            // cout << u << endl;
            // cout << v << endl;
            // cout << t << endl;
            // cout << direction << endl;
            // cout << b << endl; 
            if(u>=0 && v>=0 && t>0 && u+v<=1 && t < oldt){
                originToIntersect = t*direction;
                oldt = t;
                //cout << "iftrue:" << true << endl;
                return true;
            }
            return false;
        }
}; 