// interface.h

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <string>
#include <vector>

namespace nts
{

class interface
{
public:
    interface(bool generic = true);
    ~interface();

    /**
        Get all interface names.
        
        @ v - output
        @ return - -1: error; else: success 
    */
    int names(std::vector<std::string>& v) const;

    /**
        Change interface name.
        
        @ which - like eth0, eth1 ...
        @ return - -1: error; else: success
        @ post - The specific interface name is changed immediately.
    */
    int rename(const std::string& which, const std::string& newname) const;

    bool is_interface_up(const std::string& which) const;
    int up_interface(const std::string& which) const;
    int down_interface(const std::string& which) const;

protected:
    void set_sd(int sd);

private:
    int _sd;
};

} // namespace nts

#endif // _INTERFACE_H_
