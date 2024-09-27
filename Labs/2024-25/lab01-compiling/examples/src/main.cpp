#include <Eigen/Eigen>
#include <iostream>
#include "exe.h"

#ifndef M_PII
#define M_PII 2
#endif

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        throw std::runtime_error(
            (std::ostringstream()
            << "please provide just two inputs '"
            ).str()
            );

        std::cerr << "please provide just two inputs " << argv[1] << std::endl;
    }
    #ifdef M_PI
    std::cout << "here inside" << std::endl;
    #endif

    std::cout << "Successfully included Eigen. " << M_PI << " " << M_PII << " " << whos_lower(*argv[1], *argv[2]) << std::endl;
    return 0;
}
