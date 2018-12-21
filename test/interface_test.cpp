// interface.cpp

// g++ -g -std=c++11 -I../include ../src/interface.cpp interface_test.cpp -o interface_test

#include <interface.h>

#include <algorithm>
#include <iostream>

int main()
{
    int ret = -1;
    nts::interface inf;
    std::vector<std::string> names;
    
    ret = inf.names(names);
    if(ret == -1)
    {
        return -1;
    }
    
    std::for_each(names.cbegin(), names.cend(), [](const std::string& name)
    {
        std::cout << name << std::endl;
    });

    std::cout << std::boolalpha << inf.is_interface_up("eno16777728") << std::endl;

    inf.rename("eth0", "eno16777728");
    //inf.up_interface("eno16777728");

    return 0;
}
