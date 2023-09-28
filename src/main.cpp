
void startClasterization(std::string input, std::string output, ldouble n_attract_coef, ldouble n_trend_coef, size_t size){

    json j_2 = convertCSVToJSONForUPGMC(input,size);

    upgmc::cinfo c_info_2 = *upgmc::importClasterizationDataJSON(j_2);
    std::cout << "Clasterization data imported from " << input << "\n";

    c_info_2.attract_coef = n_attract_coef;
    c_info_2.trend_coef = n_trend_coef;

    size_t start_time ,end_time;

    std::cout << "Start clearing noise" << "\n";
    start_time = clock();
    upgmc::clearNoises(c_info_2, 2, 50);
    end_time = clock();
    std::cout << "End clearing noise, time: " << end_time - start_time << "ms\n";    

    std::cout << "Start clasterization" << "\n";
    start_time = clock();
    size_t iteration = 0; 
    while(!upgmc::stopingCriteria(c_info_2)){
        if(!upgmc::nextClasterizationIteration(c_info_2)) break;
        std::cout << "\rIteration: " << iteration << std::flush;
        iteration++;
    }
    end_time = clock();
    std::cout << "\rDone" << std::endl;

    std::cout << "End clasterization, time:" << end_time - start_time << "ms\n";
    
    upgmc::exportClasterizationDataJSON(c_info_2, output); 
    std::cout << "Clasterization data exported in " << output << "\n";

}

int main(int argc, char *args[]) {
    argparse::ArgumentParser program("bcclaster");
    
    program.add_argument("-a", "--attract_coef")
    .help("Coefficient of attraction between clusters")
    .default_value(static_cast<ldouble>(0.0000001))
    .scan<'g', ldouble>();
    
    program.add_argument("-t", "--trend_coef")
    .help("Sensitivity factor: 0 - Largest number of clusters, >0 - Reducing the number of clusters")
    .default_value(static_cast<ldouble>(1))
    .scan<'g', ldouble>();

    program.add_argument("-s", "--size")
    .help("Sensitivity factor: 0 - Largest number of clusters, >0 - Reducing the number of clusters")
    .default_value(static_cast<size_t>(2000))
    .scan<'i', size_t>();

    program.add_argument("-i", "--input")
    .default_value(std::string("input.csv"))
    .required()
    .help("Specify the input file");
    
    program.add_argument("-o", "--output")
    .default_value(std::string("output.json"))
    .required()
    .help("Specify the output file");
    
    try {
        program.parse_args(argc, args);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    auto attract_coef = program.get<ldouble>("-a");
    auto trend_coef = program.get<ldouble>("-t");
    auto size = program.get<size_t>("-s");
    auto output = program.get<std::string>("-o");
    auto input = program.get<std::string>("-i");
    

    startClasterization(input, output ,attract_coef, trend_coef, size);
    return EXIT_SUCCESS;
}