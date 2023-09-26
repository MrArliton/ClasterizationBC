typedef long double ldouble; 

using Matrix = std::vector<std::vector<ldouble>>;
using Vector = std::vector<ldouble>;
using DistanceFunc = std::function<ldouble(const Vector&,const Vector&)>;

ldouble euclidianDistance(const Vector& point1,const Vector& point2); //Standart euclidian distance

Matrix calculateMatrixOfDistances(const Matrix& points, std::function<ldouble(const Vector&,const Vector&)> dist = euclidianDistance); // Input - pformat of points (width - points ,height - coordinates of points)

std::pair<size_t,size_t> getMinForDistance(const Matrix& form); // For upper triangular matrix of distances

void writeMatrix(const Matrix& mat, std::ostream& io);