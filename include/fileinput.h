#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <memory>
#include <string>

typedef struct {
    std::string name;
    std::string content;
} KL_FileInput;

std::unique_ptr<KL_FileInput> KL_NewFileInput(std::string name);
bool KL_InitFileInput(KL_FileInput* fileInput);

#endif