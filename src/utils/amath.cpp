ldouble euclidianDistance(const Vector& point1,const Vector& point2){ 
    assert(point1.size() == point2.size());

    ldouble otv = 0, buff;

    for(int i = 0; i < point1.size();i++){
        buff = point1[i]-point2[i];
        otv += buff*buff;
    }

    return std::sqrt(otv);
}

Matrix calculateMatrixOfDistances(const Matrix& points, std::function<ldouble(const Vector&,const Vector&)> dist){ 
    size_t width = points.size();
    size_t height = points.front().size();
    
    assert(width > 0);
    assert(height > 0);

    Matrix dmat(width);

    for(int i = 0;i < width;i++){
        dmat[i].resize(width);
        for(int j = 0;j < i;j++){
            dmat[i][j] = dist(points[i],points[j]);
        }
    }
    return dmat;
}



std::pair<size_t,size_t> getMinForDistance(const Matrix& mat){ 
      const size_t mat_size = mat.size();  

      assert(mat_size==mat.front().size());
      
      ldouble min_elem = INT_MAX;
      size_t x_min = -1,y_min = -1; 
      for(int x = 0;x < mat_size;x++){
        for(int y = 0;y < x;y++){
            if(min_elem >= mat[x][y]){
                x_min = x;
                y_min = y;
                min_elem = mat[x][y];
            }
        }
      }
      return std::make_pair(x_min,y_min);    
}


void writeMatrix(const Matrix& mat, std::ostream& stream = std::cout){
    int width = mat.size();
    int height = mat.front().size();
    for(int y = 0;y < height;y++){
        for(int x = 0;x < width;x++){
            stream << std::setw(10) << mat[x][y] << " ";
        }
        stream << "\n";
    }
}