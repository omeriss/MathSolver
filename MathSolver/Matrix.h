#include <iostream>
#include <tuple>
#include <vector>

template <typename T>
class Matrix {
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="n">n</param>
    /// <param name="m">m</param>
    /// <param name="mat">pointer to data</param>
    Matrix(int n, int m, T* mat = nullptr)
    {
        this->n = n;
        this->m = m;
        if (mat) {
            this->mat = mat;
            selfAllocated = false;
        }
        else {
            this->mat = new T[n * m];
            selfAllocated = true;
        }
    }

    Matrix(const Matrix<T>& copy)
    {
        std::cout << "no no " << std::endl;
        this->n = copy.n;
        this->m = copy.m;
        this->mat = new T[n * m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                this->mat[i * m + j] = copy.mat[i * m + j];
            }
        }

    }

    T* operator[](int i)
    {
        return mat + (i * m);
    }

    /// <summary>
    /// matrix addition
    /// </summary>
    /// <param name="other"></param>
    void operator+=(Matrix& other)
    {
        for (int i = 0; i < this->n * this->m; i++) {
            this->mat[i] += other.mat[i];
        }
    }

    /// <summary>
    /// matrix mul
    /// </summary>
    /// <param name="val"></param>
    void operator*=(T val)
    {
        for (int i = 0; i < n * m; i++) {
            mat[i] *= val;
        }
    }

    /// <summary>
    /// val sub
    /// </summary>
    /// <param name="val"></param>
    /// <returns> matrix</returns>
    Matrix operator-=(T val)
    {
        for (int i = 0; i < n * m; i++) {
            mat[i] -= val;
        }
    }

    /// <summary>
    /// matrix sub
    /// </summary>
    /// <param name="other"></param>
    void operator-=(Matrix<T>& other)
    {
        for (int i = 0; i < n * m; i++) {
            mat[i] -= other.mat[i];
        }
    }

    void Set(Matrix<T>& other)
    {
        for (int i = 0; i < n * m; i++) {
            mat[i] = other.mat[i];
        }
    }

    /// <summary>
    /// mul the matrix by another matrix
    /// </summary>
    /// <param name="other"> the other matrix</param>
    /// <param name="into"> the matrix to save the result to</param>
    /// <param name="add"> should the function add the results to the new matrix or set them </param>
    void mul(Matrix& other, Matrix& into, bool add = false)
    {
        // this function assumes that this->m == other->n and that into is the right size 
        // for the output matrix

        // for each cell of the new matrix
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < other.m; j++) {
                // if add is false, set the new matrix cell to 0
                if (!add)
                    into.mat[i * other.m + j] = 0;
                for (int k = 0; k < this->m; k++) {
                    into.mat[i * other.m + j] += this->mat[i * this->m + k] * other.mat[k * other.m + j];
                }
            }
        }
    }

    /// <summary>
    /// mul the matrix by the transpose of the other matrix
    /// </summary>
    /// <param name="other"> the other matrix </param>
    /// <param name="into"> the matrix to put the result into </param>
    /// <param name="add"> should the function add the results to the new matrix or set them </param>
    void mulTranspose(Matrix& other, Matrix& into, bool add = false) {
        // this function assumes that this->m == other->m and that into is the right size 
        // for the output matrix

        // do for each cell of the new matrix
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < other.n; j++) {
                // if add is false, set the new matrix cell to 0
                if (!add)
                    into.mat[i * other.n + j] = 0;
                // go over until m becaues we want the transpose
                for (int k = 0; k < this->m; k++) {
                    // mul by the transpose
                    into.mat[i * other.n + j] += this->mat[i * this->m + k] * other.mat[j * other.m + k];
                }
            }
        }
    }

    void TransposeMul(Matrix& other, Matrix& into, bool add = false) {
        for (int i = 0; i < this->m; i++) {
            for (int j = 0; j < other.m; j++) {
                if (!add)
                    into.mat[i * other.m + j] = 0;
                for (int k = 0; k < this->n; k++) {
                    into.mat[i * other.m + j] += this->mat[k * this->m + i] * other.mat[k * other.m + j];
                }
            }
        }
    }

    /// <summary>
    /// do cross correlation 2d
    /// </summary>
    /// <param name="other"> the kernel </param>
    /// <param name="into"> the matrix to save the result to </param>
    /// <param name="add"> should the function add the results to the new matrix or set them </param>
    void CrossCorrelation(Matrix& other, Matrix& into, bool add = false) {
        // make sure that the sizes are right
        if (this->n - other.n + 1 != into.n || this->m - other.m + 1 != into.m)
            throw "mtrix size in wrong";

        // go over the new matrix
        for (int i = 0; i < this->n - other.n + 1; i++) {
            for (int j = 0; j < this->m - other.m + 1; j++) {
                if (!add)
                    into.mat[i * into.m + j] = 0;
                // mul the values in the kernel by the ones in the matrix
                for (int y = 0; y < other.n; y++) {
                    for (int x = 0; x < other.m; x++) {
                        into.mat[i * into.m + j] += this->mat[(i + y) * this->m + j + x] * other.mat[y * other.m + x];
                    }
                }
            }
        }
    }

    /// <summary>
    /// do full convolution 
    /// </summary>
    /// <param name="other"> the kernel </param>
    /// <param name="into"> the matrix to save the result to </param>
    /// <param name="add"> should the function add the results to the new matrix or set them </param>
    void FullConvolution(Matrix& other, Matrix& into, bool add = false) {
        // make sure the sizes are correct
        if (this->n + other.n - 1 != into.n || this->m + other.m - 1 != into.m)
            throw "mtrix size in wrong";

        // convolution
        for (int i = 0 - other.n + 1; i < this->n; i++) {
            for (int j = 0 - other.m + 1; j < this->m; j++) {
                int ii = i + other.n - 1;
                int jj = j + other.m - 1;
                if (!add)
                    into.mat[ii * into.m + jj] = 0;

                // calc where I need to start the muls
                int startY = std::max(-i, 0);
                int endY = std::min(other.n, this->n - i);
                int startX = std::max(-j, 0);
                int endX = std::min(other.m, this->m - j);

                // mul the matrix in the right pace by the 180 rot of the kernel
                for (int y = startY; y < endY; y++) {
                    for (int x = startX; x < endX; x++) {
                        into.mat[ii * into.m + jj] += this->mat[(i + y) * this->m + j + x] * other.mat[(other.n - y - 1) * other.m + other.m - x - 1];
                    }
                }
            }
        }
    }

    Matrix transpose()
    {
        Matrix newmat(this->m, this->n);
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                newmat[j][i] = mat[i * m + j];
            }
        }
        return newmat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& toPrint)
    {

        for (int i = 0; i < toPrint.n; i++) {
            for (int j = 0; j < toPrint.m; j++) {
                os << toPrint.mat[i * toPrint.m + j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

    /// <summary>
    /// init with rand vals
    /// </summary>
    void RandomInit()
    {
        for (int i = 0; i < n * m; i++) {
            mat[i] = GenerateRandom();
        }
    }

    void Zeros() {
        for (int i = 0; i < n * m; i++) {
            mat[i] = 0;
        }
    }

    T* GetPointer()
    {
        return mat;
    }

    ~Matrix()
    {
        if (selfAllocated)
        {
            delete mat;
        }
    }

private:
    double GenerateRandom() {
        return (rand() % 2001 - 1000.0) / 1000.0;
    }

    int n, m;
    bool selfAllocated;
    T* mat;
};

template <typename T>
class Vector :
    public Matrix<T> {
public:
    Vector(int n, T* vec = nullptr) :Matrix<T>(n, 1, vec) {};
};