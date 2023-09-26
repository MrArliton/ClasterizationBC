
struct cinfo{ // Contains information for clasterization

    struct claster{
        
        private:
            Vector coords_center;
            Matrix points;
        
            size_t size = 0;
            size_t id = 0;
        public:

            claster(size_t c_id, size_t c_size_dimension) : id(c_id) { coords_center.resize(c_size_dimension); };

            void addPoints(const Matrix& n_points){
                const size_t dimension_size = coords_center.size();
                
                size += n_points.size();
                
                for(int i = 0;i < n_points.size();i++){
                    points.push_back(n_points[i]);
                    for(int j = 0;j < dimension_size;j++){
                        coords_center[j] = (coords_center[j]*(size-1)+n_points[i][j])/(size);
                    }        
                }            
            };    

            void addPoint(const Vector& point){
                const size_t dimension_size = coords_center.size();
                
                if(point.size() != dimension_size){
                    assert(false);
                }    
                
                points.push_back(point);
                size++;
                for(int i = 0;i < dimension_size;i++){
                    coords_center[i] = (coords_center[i]*(size-1)+point[i])/(size);
                }
            };

            // Getters
            size_t getSize() const { return size; };
            size_t getId() const { return id; };
            const Matrix& getPoints() const { return points; };
            const Vector& getCenter() const { return coords_center; }; 
    };
    // Info about clasters     
    std::vector<claster> clasters;
    size_t size_dimension;
      

    // For Clasterization Process
    Matrix matrix_dist;
    Vector last_min_distances;

    ldouble attract_coef = 0.0000001;
    ldouble trend_coef = 1;
    std::string distance_function = "euclidian";

    cinfo(std::vector<claster>& clasters_vec, size_t dim_size) :
     clasters(clasters_vec),
      size_dimension(dim_size)
     {}
    
};

//Additional functions
Matrix getDistanceMatrixOfClasters(const std::vector<cinfo::claster>& clasters, ldouble attract_coef, DistanceFunc dist = euclidianDistance);
void updateDistanceMatrixOfClasters(cinfo& c_info, size_t coords_point_1, size_t coords_point_2); // Insert into matrix updated claster and delete two old
void updateShiftsMatrixOfClasters(cinfo& c_info, size_t coords_point_1, size_t coords_point_2); // Update matrix for new cluster and make shifts for old 
void clearNoises(cinfo& c_info, ldouble n, size_t l);

//Work with clasterixation
bool stopingCriteria(const cinfo& c_info);

bool nextClasterizationIteration(cinfo& c_info);


// Export and import
std::optional<cinfo> importClasterizationDataJSON(std::string path);
std::optional<cinfo> importClasterizationDataJSON(const json& j);


void exportClasterizationDataJSON(const cinfo& c_info, std::string path);


