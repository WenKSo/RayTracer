#include <Eigen/Core>
using namespace std;
using namespace Eigen;

class Triangle{
    private:
        Vector3d ver1;
        Vector3d ver2;
        Vector3d ver3;

    public:
        Triangle(Vector3d Ver1, Vector3d Ver2, Vector3d Ver3){
            ver1 = Ver1;
            ver2 = Ver2;
            ver3 = Ver3;
        }

        Vector3d getVer1(){
            return ver1;
        }

        Vector3d getVer2(){
            return ver2;
        }

        Vector3d getVer3(){
            return ver3;
        }
};