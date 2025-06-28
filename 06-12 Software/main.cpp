#include <iostream>
#include <filesystem>
#include <vector>

#include "Parsing\Assembly\HackTranslator.cpp"
#include "Parsing\VM\VMTranslator.cpp"

using namespace parsing;

namespace fs = std::filesystem;

int main(){

    //check whether its a folder or a single file
    String source;
    std::cout << "enter path: ";
    std::cin >> source;

    std::cout << source << std::endl;

    std::vector<fs::path> files;
    fs::path filePath = source;
    
    if (filePath.extension() != ""){
        files.push_back(source);
        filePath = filePath.parent_path();
    }
    else 
    {
        try
        {
            for (const auto& file : fs::recursive_directory_iterator(source)){
                std::cout << file.path() << std::endl;

                if (file.path().extension() == ".vm"){
                    //insert Sys.vm to front of translation order
                    if (files.size() > 0 && file.path().stem() == "Sys"){
                        fs::path temp = files[0];
                        files[0] = file.path();
                        files.push_back(temp);
                    } else {
                        files.push_back(file.path().string());
                    }
                }
            }
        }
        catch (const fs::filesystem_error& ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }
    
    HackVM::VMTranslator vm(filePath.string());
    
    for (const fs::path& str : files){
        std::cout << "path: " << str << std::endl;
        String filename = str.stem().string();
        std::cout << filename << std::endl;

        vm.generateAssembly(filePath.string(), filename);
    }

    HackAssembly::HackTranslator hack(filePath.string()+"\\exec.asm", filePath.string()+"\\exec.hack");

    hack.generateOpcode();
    
    return 0;
}