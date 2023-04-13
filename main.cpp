#include <iostream>

int main(int argc, char const *argv[])
{
    float res = *reinterpret_cast<const float*>(argv[0]);

    std::cout << res << '\n';
    return 0;
}

