#include "bindump.hpp"

#include <iomanip>

void dump(std::ostream& oss, const unsigned char* buffer, size_t length)
{
    oss << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);
    for (unsigned index = 0; index < length; ++index) {
        if (index % 8 == 0) {
            if (index != 0) {
                oss << std::endl;
            }
            oss << "0x";
        }
        oss << std::setw(2) << static_cast<unsigned>(buffer[index]);
    }
    oss << std::endl;
}
