
bool nextClasterizationIteration(cinfo& c_info){
    assert(c_info.clasters.size() > 0);    
    
    if(c_info.clasters.size()==1){
        return false;
    }
    
    // Default prepearing
    if(c_info.matrix_dist.size()==0){
        c_info.matrix_dist = getDistanceMatrixOfClasters(c_info.clasters, c_info.attract_coef);
        c_info.removed_indexes_mdist.resize(c_info.clasters.size());
    }
    //

    //// Find closest elements 
    auto m_elems = getMinForDistance(c_info.matrix_dist);

    // Add to the primitive distance vector  
        c_info.last_min_distances.push_back(c_info.matrix_dist[m_elems.first][m_elems.second]);
    //
    
    size_t first_c = std::min(m_elems.first,m_elems.second), second_c = std::max(m_elems.first,m_elems.second);
    ////

    //// Union clusters
    size_t shift_first_c = 0, shift_second_c;
    for(int i = 0;i < first_c;i++){ if(c_info.removed_indexes_mdist[i]) shift_first_c++;} 
    shift_second_c = shift_first_c;
    for(int i = first_c;i < second_c;i++){ if(c_info.removed_indexes_mdist[i]) shift_second_c++;}

    c_info.clasters[first_c - shift_first_c].addPoints(c_info.clasters[second_c- shift_second_c].getPoints());

    c_info.clasters.erase(c_info.clasters.begin()+second_c - shift_second_c);
    ////
    c_info.removed_indexes_mdist[second_c]++;
    //// Update matrix of distances
    auto start_time = std::chrono::steady_clock::now();
    updateDistanceMatrixOfClasters(c_info, first_c, second_c, shift_first_c);
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Time of update matrix " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << "ns\n";    
    ////
    return true;
}
//-------------------------------------------------------------
Matrix getDistanceMatrixOfClasters(const std::vector<cinfo::claster>& clasters, ldouble attract_coef, DistanceFunc dist){
    size_t width = clasters.size();
     
    assert(width > 0);

    Matrix dmat(width);

    for(int i = 0;i < width;i++){
        dmat[i].resize(width);
        for(int j = 0;j < i;j++){
            const ldouble buff = dist(clasters[i].getCenter(),clasters[j].getCenter())
             - attract_coef * clasters[i].getSize() * clasters[j].getSize();
            if(buff < 0){
                dmat[i][j] = 0; 
            }else{
                dmat[i][j] = buff;
            }
        }
    }
    return dmat;
}
void updateDistanceMatrixOfClasters(cinfo& c_info, size_t coords_point_1, size_t coords_point_2, size_t coords_point_1_shift){
    Matrix& mat = c_info.matrix_dist;
    DistanceFunc dist = euclidianDistance;
    const ldouble attract_coef = c_info.attract_coef;

    size_t shift = 0;
    for(int i = 0; i < coords_point_1;i++){
        if(!c_info.removed_indexes_mdist[i]){
            const ldouble buff = dist(c_info.clasters[i-shift].getCenter(),c_info.clasters[coords_point_1-coords_point_1_shift].getCenter())
                - attract_coef * c_info.clasters[i-shift].getSize() * c_info.clasters[coords_point_1-coords_point_1_shift].getSize();
                if(buff < 0){
                    mat[coords_point_1][i] = 0; 
                }else{
                    mat[coords_point_1][i] = buff;
                }
        }else{
            shift++;
        }
    }
    for(int i = coords_point_1+1;i < mat.size();i++){
        if(!c_info.removed_indexes_mdist[i]){
        const ldouble buff = dist(c_info.clasters[i-shift].getCenter(),c_info.clasters[coords_point_1-coords_point_1_shift].getCenter())
             - attract_coef * c_info.clasters[i-shift].getSize() * c_info.clasters[coords_point_1-coords_point_1_shift].getSize();
            if(buff < 0){
                mat[i][coords_point_1] = 0; 
            }else{
                mat[i][coords_point_1] = buff;
            }
        }else{
            shift++;
        }
    }
    // coord_point_2
    for(int i = 0;i < coords_point_2;i++){
        mat[coords_point_2][i] = static_cast<ldouble>(INT_MAX);
    }
    for(int i = coords_point_2+1;i < mat.size();i++){
        mat[i][coords_point_2] = static_cast<ldouble>(INT_MAX);
    } 
}



void clearNoises(cinfo& c_info, ldouble n, size_t l){ // ??????
    Matrix mat = getDistanceMatrixOfClasters(c_info.clasters,0);
    
    //// Calculate Mean-square-deviation
    ldouble mean = 0;
    
    for(int x = 0;x < mat.size();x++){
        for(int y = 0;y < x;y++){
            mean+=mat[x][y];
        }
    }
    mean /= (mat.size()*(mat.front().size()-1)/2);
    ///
    ldouble mean_square_deviation = 0;
    for(int x = 0;x < mat.size();x++){
        for(int y = 0;y < x;y++){
            ldouble buff = mat[x][y]-mean;
            mean_square_deviation += buff*buff;
        }
    }
    mean_square_deviation /= (mat.size()*(mat.front().size()-1)/2);
    mean_square_deviation = std::sqrt(mean_square_deviation);
    // Find noise clasters 
    ldouble radius = mean_square_deviation/n;
    for(int i = mat.size()-1;i >= 0;i--){
        size_t amount_of_points_in_radius = 0;
        for(int k = 0;k < i;k++){
            if(mat[i][k] < radius){
                amount_of_points_in_radius++;
            }
        } 
        if(amount_of_points_in_radius < l){ 
                c_info.clasters.erase(c_info.clasters.begin()+i); // Erase noise clasters
        }
    }
    //
}
//-------------------------------------------------------------
std::optional<cinfo> importClasterizationDataJSON(std::ifstream file){

   if(!file.is_open()){
    return std::nullopt;
   }

   json j = json::parse(file);   

   size_t amount_clasters = j["amount_clasters"].template get<size_t>();
   size_t size_dimension = j["size_dimension"].template get<size_t>();
   json j_clasters = j["clasters"];

   std::vector<cinfo::claster> clasters;
   
   for(int i = 0;i < amount_clasters;i++){
        clasters.emplace_back(j_clasters[i]["id"].template get<size_t>(),size_dimension);
        const size_t size_claster = j_clasters[i]["size"].template get<size_t>();
        for(int j = 0; j < size_claster;j++){
            clasters[i].addPoint(j_clasters[i]["points"][j].template get<Vector>());
        }
        
   }    
   cinfo c_info(clasters,size_dimension); 
   return c_info;
}
std::optional<cinfo> importClasterizationDataJSON(const json& j){

   if(!j.contains("amount_clasters") ||
    !j.contains("clasters") ||
    !j.contains("size_dimension")){
        std::cout << "Incorrect json: " << j << std::endl;
        std::exit(EXIT_FAILURE);
    }

   size_t amount_clasters = j["amount_clasters"].template get<size_t>();
   size_t size_dimension = j["size_dimension"].template get<size_t>();
   json j_clasters = j["clasters"];

   std::vector<cinfo::claster> clasters;

   size_t amount_of_incorrect_clusters = 0;
   for(int i = 0;i < amount_clasters;i++){
        
        if(!j_clasters[i].contains("id") ||
        !j_clasters[i].contains("size") ||
        !j_clasters[i].contains("points")){
            amount_of_incorrect_clusters++;
            std::cout << "\rIncorrect clasters: " << amount_of_incorrect_clusters << std::flush;
            continue;    
        }

        clasters.emplace_back(j_clasters[i]["id"].template get<size_t>(),size_dimension);
        const size_t size_claster = j_clasters[i]["size"].template get<size_t>();
        for(int j = 0; j < size_claster;j++){
            clasters[i].addPoint(j_clasters[i]["points"][j].template get<Vector>());
        }
        
   }    
   std::cout << std::endl;
   cinfo c_info(clasters,size_dimension); 
   return c_info;
}


void exportClasterizationDataJSON(cinfo& c_info, std::string path){
    assert(path.size()>0);
    
    const size_t amount_clasters = c_info.clasters.size();

    json j;
    j["amount_clasters"] = amount_clasters;
    j["size_dimension"] = c_info.size_dimension;
    j["clasters"] = json::array();
    for(size_t i = 0;i < amount_clasters;i++){
        const size_t size_claster = c_info.clasters[i].getSize();
        json j_claster({{"id",c_info.clasters[i].getId()},{"size",size_claster}});
        json j_points = json::array();  
        for(int j = 0;j < size_claster;j++){
            j_points.push_back(c_info.clasters[i].getPoints()[j]);
        }
        j_claster.push_back({"points",j_points});
        j["clasters"].push_back(j_claster);
    }

    std::ofstream file(path);

    file << j.dump();
}

bool stopingCriteria(const cinfo& c_info){ // Calculating criteria of stopping upgmc method 
    const Vector& last_min_distances = c_info.last_min_distances;
    const ldouble trend_coef = c_info.trend_coef;

    if(last_min_distances.size() < 4){
        return false;    
    }
    
    size_t i = last_min_distances.size()-1;
    ldouble y_3 = last_min_distances[i];
    i--;
    for(;last_min_distances[i] > y_3;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_2 = last_min_distances[i]; 
    i--;
    for(;last_min_distances[i] > y_2;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_1 = last_min_distances[i];
    i--;
    for(;last_min_distances[i] > y_1;i--){ if(i>last_min_distances.size()){return false;}}
    ldouble y_0 = last_min_distances[i];
    
    y_3 = y_3 - y_0 + (i+3)*trend_coef;
    y_2 = y_2 - y_0 + (i+2)*trend_coef;
    y_1 = y_1 - y_0 + (i+1)*trend_coef;

    ldouble criteria = 1.0/245.0 * (19.0 * (y_1 * y_1) - 11.0 * (y_2 * y_2) + 41.0 * (y_3 * y_3) + 12.0 * y_1 * y_2 - 64.0 * y_1 * y_3 - 46.0 * y_2 * y_3);

    if(criteria <= 0){
        return false;
    }
    return true;      
}