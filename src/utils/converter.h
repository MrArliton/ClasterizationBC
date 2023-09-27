
json convertCSVToJSONForUPGMC(std::string path, size_t amount_points = 10000, char delimiter = ';', size_t arg_1 = 0, size_t arg_2 = 3){
    std::ifstream file(path);
    
    if(!file.is_open()){
        std::cout << "Input file isn't open: " << path << std::endl;
        std::exit(EXIT_SUCCESS);
    }


    std::string header;

    std::getline(file,header);

    json j;
    j["clasters"] = json::array();
    
    // Fill clasters
    size_t i = 0;
    for(std::string line; std::getline(file, line);){
        Vector point;
        
        size_t del = 0;
        //  Find arg_1
            for(int i = 0;i < arg_1;i++){
                del = line.find(delimiter,del);
                del++;
            }
        //
        std::string buff = line.substr(del,line.find(delimiter,del)-del);
        point.push_back(std::stold(buff));
        //  Find arg_2
            for(int i = 0;i < arg_2-arg_1;i++){
                del = line.find(delimiter,del);
                del++;
            }
        //
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
    //
    j["amount_clasters"] = i;
    j["size_dimension"] = 2;
    return j;
}