
int main(int argc, char *args[]) {
    
    json j_2 = convertCSVToJSONForUPGMC("C:/Projects/ClasterizationBC/files_for_test/CD3_B.csv",2000);

    upgmc::cinfo c_info_2 = *upgmc::importClasterizationDataJSON(j_2);
    std::cout << "Clasterization data imported from " << "C:/Projects/ClasterizationBC/files_for_test/CD3_B.csv" << "\n";

    c_info_2.attract_coef = 0.0000001;
    c_info_2.trend_coef = 2;

    size_t start_time ,end_time;

    std::cout << "Start clearing noise" << "\n";
    start_time = clock();
    upgmc::clearNoises(c_info_2, 1, 50);
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
    
    upgmc::exportClasterizationDataJSON(c_info_2,"C:/Projects/ClasterizationBC/build/3.json"); 
    std::cout << "Clasterization data exported in " << "C:/Projects/ClasterizationBC/build/3.json" << "\n";  
    return 1;
}