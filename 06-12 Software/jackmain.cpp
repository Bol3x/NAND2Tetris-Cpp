#include <iostream>
#include <filesystem>
#include <vector>

#include "Parsing/Compiler/CompilerEngine.cpp"

using namespace parsing;
using namespace std;
namespace fs = std::filesystem;

int main()
{
    //check whether its a folder or a single file
    String source;
    std::cout << "enter path: ";
    std::cin >> source;

    std::cout << source << std::endl;

    std::vector<fs::path> files;
    fs::path filePath = source;
    
    if (! filePath.extension().empty()){
        files.push_back(source);
        filePath = filePath.parent_path();
    }
    else 
    {
        try
        {
            for (const auto& file : fs::recursive_directory_iterator(source)){
                std::cout << file.path() << std::endl;

                if (file.path().extension() == ".jack"){
                    files.push_back(file.path().string());
                }
            }
        }
        catch (const fs::filesystem_error& ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    JackCompiler::CompilerEngine engine;

    for (const fs::path& str : files){
        std::cout << "path: " << str << std::endl;
        String filename = str.stem().string();
        std::cout << filename << std::endl;

        engine.writer.openFile(filePath.string()+"/"+filename);
        engine.compileFile(str.string());
        engine.writer.closeFile();
    }
    
    return 0;
}