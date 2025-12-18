#include <fileinput.h>
#include <sstream>
#include <fstream>

std::unique_ptr<KL_FileInput> KL_NewFileInput(std::string name) {
    auto input = std::make_unique<KL_FileInput>();
    if (!input) return NULL;

    input->name = name;

    return input;
}

bool KL_InitFileInput(KL_FileInput* fileInput) {
    std::ifstream f(fileInput->name);
    if (!f.is_open()) return false;
    std::ostringstream oss;
    
    oss << f.rdbuf();
    fileInput->content = oss.str().c_str();

    f.close();
    return true;
}