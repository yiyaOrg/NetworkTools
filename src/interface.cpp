// interface.cpp

#include <interface.h>

#include <cstring>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

//debug
#include <errno.h>
#include <iostream>

namespace nts
{

#define MAX_IF  64

interface::interface(bool generic) : _sd(-1)
{
    if(generic)
    {
        _sd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sd == -1)
        {
            std::cout << strerror(errno) << std::endl;
        }
    }
}

interface::~interface()
{
    close(_sd);
}

int interface::names(std::vector<std::string>& v) const
{
    struct ifreq ifr[MAX_IF];
    struct ifconf ifc;

    ifc.ifc_len = sizeof(ifr);
    ifc.ifc_req = ifr;

    if(ioctl(_sd, SIOCGIFCONF, &ifc) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
    
    // success
    struct ifreq* start = ifr;
    struct ifreq* end = ifr + ifc.ifc_len / sizeof(struct ifreq);

    while( start < end)
    {
        v.push_back(start->ifr_name);
        start++;
    }

    return 0;
}

int interface::rename(const std::string& which, const std::string& newname) const
{
    bool state = false;
    struct ifreq ifr;

    strcpy(ifr.ifr_name, which.c_str());
    strcpy(ifr.ifr_newname, newname.c_str());

    if(is_interface_up(which))
    {
        state = true;
        down_interface(which);
    }

    if(ioctl(_sd, SIOCSIFNAME, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    if(state)
    {
        up_interface(newname);
    }

    return 0;
}

bool interface::is_interface_up(const std::string& which) const
{
    int ret = -1;
    struct ifreq ifr;

    strcpy(ifr.ifr_name, which.c_str());

    if(ioctl(_sd, SIOCGIFFLAGS, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return false;
    }

    if(IFF_UP & ifr.ifr_flags)
    {
        return true;
    }

    return false;
}

int interface::up_interface(const std::string& which) const
{
    struct ifreq ifr;

    strcpy(ifr.ifr_name, which.c_str());

    if(ioctl(_sd, SIOCGIFFLAGS, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    if(IFF_UP & ifr.ifr_flags)
    {
        return 0;
    }
    
    ifr.ifr_flags = ifr.ifr_flags | IFF_UP;

    if(ioctl(_sd, SIOCSIFFLAGS, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int interface::down_interface(const std::string& which) const
{
    struct ifreq ifr;

    strcpy(ifr.ifr_name, which.c_str());

    if(ioctl(_sd, SIOCGIFFLAGS, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
   
    if(!(IFF_UP & ifr.ifr_flags))
    {
        return 0;
    }

    ifr.ifr_flags = ifr.ifr_flags & (~IFF_UP);

    if(ioctl(_sd, SIOCSIFFLAGS, &ifr) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}


} // namespace nts
