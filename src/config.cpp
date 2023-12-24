// MyClass.h
#include <string>
class Config {
public:
    static const bool WRITE_BOARD = true;
    static const std::string BOARD_FILE;
};

const std::string MyClass::BOARD_FILE = "ChArUcoBoard";
