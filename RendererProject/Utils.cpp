#include "Utils.h"

#include <fstream>
#include <cassert>

std::string DumpToString(const std::string& InputPath)
{
    std::string OutputString;

    if (std::ifstream InputFile{ InputPath, std::ios::in | std::ios::ate })
    {
        assert(InputFile.is_open() && "File did NOT actually open");

        auto Size = InputFile.tellg();

        OutputString = std::string(Size, '\0');

        InputFile.seekg(0);

        InputFile.read(&OutputString[0], Size);

        InputFile.close();
    }

    return OutputString;

}
