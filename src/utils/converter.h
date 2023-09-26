
json convertCSVToJSONForUPGMC(std::string path, size_t amount_points = 10000, char delimiter = ';'){
    std::ifstream file(path);
    
    std::string header;

    std::getline(file,header);

    json j;
    j["clasters"] = json::array();
    
    size_t i = 0;
    for(std::string line; std::getline(file, line);){
        Vector point;
        
        size_t del = 0;
      
        for(int i = 0;i < 0;i++){
            del = line.find(delimiter,del);
            del++;
        }

        std::string buff = line.substr(del,line.find(delimiter,del)-del);
        point.push_back(std::stold(buff));

        for(int i = 0;i < 3;i++){
            del = line.find(delimiter,del);
            del++;
        }

        buff = line.substr(del,line.find(delimiter,del)-del);
        point.push_back(std::stold(buff));

        json claster;
        claster["points"] = json::array();
        claster["points"].push_back(point);
        claster["size"] = 1; 
        claster["id"] = i;
        j["clasters"].push_back(std::move(claster));
        i++;
        if(i > amount_points) break;
    }

    j["amount_clasters"] = i;
    j["size_dimension"] = 2;
    return j;
}