
#include"../../include/fast_io.h"

class XXX{
    std::string s;
    public:
    XXX(std::string x):s(std::move(x)){
        println(fast_io::out,"XXX(std::string x)");
    }
    XXX(XXX const& x):s(x.s){

        println(fast_io::out,"XXX(XXX const& x)");
    }
    XXX(XXX&& x) noexcept:s(std::move(x.s)) 
    {
        
        println(fast_io::out,"XXX(XXX&& x)");
    }
    XXX& operator=(XXX const& x) {
        println(fast_io::out,"XXX& operator=(XXX const& x)");
        return *this;
    }
    XXX& operator=(XXX&& x)noexcept {
        println(fast_io::out,"XXX& operator=(XXX&& x)");
        return *this;
    }
    ~XXX()
    {
        
        
        println(fast_io::out,"~XXX");
    }
};

class YYY2 {
    XXX x;
    public:
        template<typename ...Args>
        requires std::constructible_from<XXX,Args...>
        YYY2(Args&& ...args):x(std::forward<Args>(args)...){}
};
class YYY {
    XXX x;
    public:
        template<typename U>
        requires std::constructible_from<XXX,U>
        YYY(U&& u):x(std::forward<U>(u)){}
};
class ZZZ {
    XXX x;
    public:
        ZZZ(XXX x):x(std::move(x)){}

};
struct UUU {
    XXX x;

};
int main() {
        
        
        println(fast_io::out,"ZZZ");
        UUU a{XXX(XXX("aaaa"))};
}
