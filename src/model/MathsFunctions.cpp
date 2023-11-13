#include "MathsFunctions.hpp"

#include <math.h>

c_vector<double, 1> Create_c_vector(double x)
{
    c_vector<double, 1> v;
    v[0] = x;
    return v;
}

c_vector<double, 2> Create_c_vector(double x, double y)
{
    c_vector<double, 2> v;
    v[0] = x;
    v[1] = y;
    return v;
}

c_vector<double, 3> Create_c_vector(double x, double y, double z)
{
    c_vector<double, 3> v;
    v[0] = x;
    v[1] = y;
    v[2] = z;
    return v;
}

c_vector<double, 4> Create_c_vector(double x, double y, double z, double p)
{
    c_vector<double, 4> v;
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = p;
    return v;
}

template <typename T>
std::set<T> convertToSet(std::vector<T> v)
{
    // Declaring the  set
    std::set<T> s;

    // Traverse the Vector
    for (T x : v) {
        // Insert each element
        // into the Set
        s.insert(x);
    }

    // Return the resultant Set
    return s;
}

template <typename T>
c_vector<c_vector<T, 3>, 3> MatrixMultiplication(c_vector<c_vector<T, 3>, 3> Matrix1,
                                                 c_vector<c_vector<T, 3>, 3> Matrix2)
{
    c_vector<c_vector<T, 3>, 3> MatrixTranspose;
    c_vector<c_vector<T, 3>, 3> Answer;

    // This will give us the determinat
    for (int i = 0; i < 3; i++) {
        MatrixTranspose[i] = Create_c_vector(Matrix2[0][i], Matrix2[1][i], Matrix2[2][i]);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Answer[i][j] = inner_prod(Matrix1[i], MatrixTranspose[j]);
        }
    }

    return Answer;
}

template <typename T>
c_vector<T, 3> MatrixMultiplication(c_vector<c_vector<T, 3>, 3> Matrix, c_vector<T, 3> Vector)
{
    c_vector<T, 3> Answer;

    for (int i = 0; i < 3; i++) {
        Answer[i] = inner_prod(Matrix[i], Vector);
    }

    return Answer;
}

template <typename T>
std::pair<T, T> Create_pair(T x, T y)
{
    std::pair<T, T> NodePair = std::make_pair(std::min(x, y), std::max(x, y));
    return NodePair;
}

// std::pair<unsigned, unsigned> Create_pair(unsigned x, unsigned y)
// {
//     std::pair<unsigned, unsigned> NodePair = std::make_pair(std::min(x, y), std::max(x, y));
//     return NodePair;
// }

template <typename T>
double MaintainOutwardsPointingNormal(c_vector<T, 3> Normal, c_vector<T, 3> x1)
{
    T direction = 1;
    c_vector<T, 2> Normal2D = Create_c_vector(Normal[0], Normal[1]);

    c_vector<T, 2> x12D = Create_c_vector(x1[0], x1[1]);

    c_vector<T, 2> Extension = Normal2D + x12D;

    T absExtensin = norm_2(Extension);
    T absx1 = norm_2(x12D);
    if (absExtensin < absx1)  // meaning the normal points towards the center
    {
        direction = -1;
        //  Normal = -Normal;// reverse the normal so it points out
    }
    return direction;
}

template <typename T>
void PRINT_PAIR(std::pair<T, T> Pair)
{
    std::cout << "DEBUG: Pair = [" << Pair.first << ", " << Pair.second << "] " << std::endl;
}

template <typename T>
double AreVectorsSame(std::vector<T> Vector1, std::vector<T> Vector2)
{
    sort(Vector1.begin(), Vector1.end());
    sort(Vector2.begin(), Vector2.end());

    std::vector<T> DifferenceVector;
    std::set_difference(Vector1.begin(), Vector1.end(), Vector2.begin(), Vector2.end(),
                        std::inserter(DifferenceVector, DifferenceVector.begin()));

    double difference = DifferenceVector.size();
    return difference;
}

template <typename T>
std::vector<T> Intersection(std::vector<T> Vector1, std::vector<T> Vector2)
{
    std::vector<T> IntersectionVector;

    for (typename std::vector<T>::iterator it = Vector1.begin(); it != Vector1.end(); ++it) {
        for (typename std::vector<T>::iterator it2 = Vector2.begin(); it2 != Vector2.end(); ++it2) {
            if (*it == *it2) {
                IntersectionVector.push_back(*it);
            }
        }
    }

    return IntersectionVector;
}

template <typename T>
std::vector<T> RemoveElement(std::vector<T> Vector1, T number)
{
    T ElementToRemove;
    for (typename std::vector<T>::iterator it = Vector1.begin(); it != Vector1.end(); ++it) {
        if (*it == number) {
            ElementToRemove = *it;
        }
    }
    Vector1.erase(Vector1.begin() + ElementToRemove);

    return Vector1;
}

template <typename T>
bool IsPairInVector(std::vector<std::pair<T, T>> Vector, std::pair<T, T> Pair)
{
    bool IsInVector = 0;

    for (typename std::vector<std::pair<T, T>>::iterator it = Vector.begin(); it != Vector.end(); ++it) {
        if (*it == Pair) {
            IsInVector = 1;
            return IsInVector;
        }
    }

    return IsInVector;
}

template <typename T>
bool IsNumberInVector(std::vector<T> Vector, T number)
{
    bool IsInVector = 0;
    for (typename std::vector<T>::iterator it = Vector.begin(); it != Vector.end(); ++it) {
        if (*it == number) {
            IsInVector = 1;
        }
    }
    return IsInVector;
}

template <typename T>
double min_value(std::vector<T> Vector)
{
    T min = Vector[0];
    for (typename std::vector<T>::iterator it = Vector.begin(); it != Vector.end(); ++it) {
        /* std::cout << *it; ... */
        if (*it < min) {
            min = *it;
        }
    }
    return min;
}

template <typename T>
T max_value(std::vector<T> Vector)
{
    T max = Vector[0];
    for (typename std::vector<T>::iterator it = Vector.begin(); it != Vector.end(); ++it) {
        /* std::cout << *it; ... */
        if (*it > max) {
            max = *it;
        }
    }
    return max;
}

template <typename T>
T AddAngles(T alpha, T beta)
{
    T eta = alpha + beta;

    if (eta > M_PI) {  // This anlge is now negative
        eta = eta - 2 * M_PI;
    } else if (eta < -M_PI) {  // This anlge is now positive
        eta = eta + 2 * M_PI;
    }
    return eta;
}

template <typename T>
T PeriodicAngle(T eta)
{
    if (eta > M_PI) {  // This anlge is now negative
        eta = eta - 2 * M_PI;
    } else if (eta < -M_PI) {  // This anlge is now positive
        eta = eta + 2 * M_PI;
    }
    return eta;
}

template <typename T>
bool IsVectorInVector(std::vector<c_vector<T, 3>> Vector, c_vector<T, 3> Location)
{
    bool IsInVector = 0;
    for (typename std::vector<c_vector<T, 3>>::iterator it = Vector.begin(); it != Vector.end(); ++it) {
        c_vector<T, 3> AVector = *it;
        if ((AVector[0] == Location[0]) && (AVector[1] == Location[1])) {
            //  IsInVector = 1;
            return 1;
        }
    }
    return IsInVector;
}

template <typename T>
c_vector<c_vector<T, 3>, 3> MatrixTranspose(c_vector<c_vector<T, 3>, 3> Matrix)
{
    c_vector<c_vector<T, 3>, 3> MatrixTranspose;
    for (int i = 0; i < 3; i++) {  // Transporse of the matrix
        MatrixTranspose[i] = Create_c_vector(Matrix[0][i], Matrix[1][i], Matrix[2][i]);
    }
    return MatrixTranspose;
}

template <typename T>
c_vector<c_vector<T, 3>, 3> Inverse(c_vector<c_vector<T, 3>, 3> Matrix)
{
    c_vector<c_vector<T, 3>, 3> MTranspose = MatrixTranspose(Matrix);

    T det = Matrix[0][0] * (Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1]) -
            Matrix[0][1] * (Matrix[1][0] * Matrix[2][2] - Matrix[1][2] * Matrix[2][0]) +
            Matrix[0][2] * (Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0]);

    double l = 0, m = 1;
    c_vector<c_vector<T, 3>, 3> InverseMatrix;
    for (int k = 0; k < 3; k++) {
        l += 1, m += 1;
        if (l == 3) {
            l = 0;
        }
        if (m == 3) {
            m = 0;
        }
        InverseMatrix[k] = (1 / det) * VectorProduct(MTranspose[l], MTranspose[m]);
    }
    return InverseMatrix;
}

//  Bunch of print statements
// template <typename T>
void print(double statement) { std::cout << statement << std::endl; }
void print(std::string statement) { std::cout << statement << std::endl; }

void print(c_vector<double, 3> Locaiton)
{
    std::cout << Locaiton[0] << "," << Locaiton[1] << "," << Locaiton[2] << "," << std::endl;
}

void print(c_vector<int, 3> Locaiton)
{
    std::cout << Locaiton[0] << "," << Locaiton[1] << "," << Locaiton[2] << "," << std::endl;
}
void print(c_vector<float, 3> Locaiton)
{
    std::cout << Locaiton[0] << "," << Locaiton[1] << "," << Locaiton[2] << "," << std::endl;
}

void print(Coordinates Locaiton)
{
    std::cout << "Node location: " << Locaiton.x << "," << Locaiton.y << "," << Locaiton.z << "," << std::endl;
}

// template <typename T, typename U>
// void print(T statement1, U statement2)
// {
//     std::cout << statement1 << " " << statement2 << std::endl;
// }

// template <typename T, typename U, typename S>
// void print(T statement1, U statement2, S statement3)
// {
//     std::cout << statement1 << " " << statement2 << " " << statement3 << std::endl;
// }

// template <typename T, typename U, typename S, typename V>
// void print(T statement1, U statement2, S statement3, V statement4)
// {
//     std::cout << statement1 << " " << statement2 << " " << statement3 << " " << statement4 << std::endl;
// }

// c_vector<c_vector<double, 3>, 3> RowReduction(c_vector<c_vector<double, 3>, 3> Matrix)
// {
//     // row reduction to reduced row eshioln form
//     c_vector<c_vector<double, 3>, 3> Identity;
//     Identity[0] = Create_c_vector(1, 0, 0);
//     Identity[1] = Create_c_vector(0, 1, 0);
//     Identity[2] = Create_c_vector(0, 0, 1);
//     for (int i = 0; i < 3; i++) {
//         Identity[i] /= Matrix[i][i];
//         Matrix[i] /= Matrix[i][i];  // sets all the diagonal elements to 1

//         for (int j = 0; j < 3; j++) {
//             if (j != i)  // prevents taking out the t row that we just make 1
//             {
//                 Identity[j] -= (Identity[i] * Matrix[j][i]);
//             }
//             if (j != i)  // prevents taking out the t row that we just make 1
//             {
//                 Matrix[j] -= (Matrix[i] * Matrix[j][i]);
//             }
//         }
//     }

//     // by this point the oridinal identity matrix is now the inverse
//     return Identity;
// }

// // c_vector<c_vector<double, 3>, 3> Mapping(c_vector<c_vector<double, 3>, 3> PositionVector,
// // c_vector<c_vector<double, 3>, 3> NewPositionVector)
// // {

// //         int SpConst =450; // prevents matrix begin singular
// //         for (int i=0; i<3; i++)
// //         {
// //            PositionVector[i][2] += SpConst;
// //            NewPositionVector[i][2] = SpConst;
// //         }
// //           PositionVector = MatrixTranspose(PositionVector);
// //           NewPositionVector = MatrixTranspose(NewPositionVector);

// //         c_vector<c_vector<double, 3>, 3>  InverseE  = MatrixMultiplication(PositionVector,
// //         Inverse(NewPositionVector));

// // return InverseE;
// // }

// double det(c_vector<c_vector<double, 2>, 2> Matrix)
// {
//     double Determinate = Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0];
//     return Determinate;
// }

// double tr(c_vector<c_vector<double, 2>, 2> Matrix)
// {
//     double Trace = Matrix[0][0] + Matrix[1][1];
//     return Trace;
// }