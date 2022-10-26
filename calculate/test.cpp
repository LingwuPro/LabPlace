
#include <iostream>
#include <vector>
#include "D:\\library\\eigen\\Eigen\\Dense"

#include <stdlib.h>
using namespace std;
using namespace Eigen;

int main()
{
     Matrix2d c, d, e;
     c << 1, 2, 3, 4;
     d << 2, 3, 4, 5;
     e = c * d;
     std::cout << "mul\n"
               << e << std::endl;
     std::cout << "转置\n"
               << c.transpose() << std::endl
               << std::endl;
     std::cout << "逆矩阵\n"
               << c.inverse() << std::endl;
     std::cout << "行列式： " << c.determinant() << std::endl;

     /*Matrix3d A; //定义3*3  double 型矩阵
     A << 1, 2, 3, 5, -5, 3, 4, 6, 9;

     cout << "Here is a matrix, A:" << endl
          << A << endl
          << endl;
     EigenSolver<MatrixXd> es(A);

     cout << "The eigenvalues of A are:" << endl
          << es.eigenvalues() << endl;

     MatrixXcd vect = es.eigenvectors(); //特征向量矩阵
     cout << "复数形式输出：" << endl
          << vect << endl;

     MatrixXd realvect(vect.rows(), vect.cols());
     MatrixXd imagvect(vect.rows(), vect.cols());

     cout << "虚实拆分:" << endl;
 //    两种拆分方式等价
 //  for (size_t i = 0; i < vect.rows(); i++)
 //  {
 //      for (size_t j = 0; j < vect.cols(); j++)
 //      {
 //          realvect(i, j) = real(vect(i, j));
 //          imagvect(i, j) = imag(vect(i, j));
 //      }
 //  }
     for (size_t i = 0; i < vect.size(); i++)
     {
         *(realvect.data() + i) = real(*(vect.data() + i));
         *(imagvect.data() + i) = imag(*(vect.data() + i));
     }

     cout << "realvect:" << endl
          << realvect << endl;
     cout << "imagvect:" << endl
          << imagvect << endl;
     */

     return 0;
}