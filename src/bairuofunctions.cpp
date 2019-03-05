#include "bairuofunctions.h"

namespace bairuo
{
    std::string int2str(int value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    std::string uns2str(unsigned value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }
}
