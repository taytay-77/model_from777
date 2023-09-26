#include <iostream>
#include <string>

std::string fixFilePath(const std::string& filePath) {
    std::string fixedPath;
    for (char c : filePath) {
        if (c == '\\') {
            fixedPath += "\\\\";
        } else {
            fixedPath += c;
        }
    }
    return fixedPath;
}