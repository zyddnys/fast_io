
#include"../../include/fast_io.h"

#include <utility>

using key = std::pair<fast_io::natural, fast_io::natural>;

std::pair<fast_io::natural, fast_io::natural> generate_random_prime_pair()
{
    return {fast_io::natural(11), fast_io::natural(7)};
}

fast_io::natural gcd(fast_io::natural a, fast_io::natural b)
{
    if (!b)
        return a;
    return gcd(b, a%b);
}
/*
std::tuple<fast_io::natural, fast_io::natural, fast_io::natural> exgcd2(fast_io::natural a, fast_io::natural b)
{
    if (!b)
        return {a, fast_io::natural(1), fast_io::natural(0)};
    println_flush(fast_io::out, "auto [r, x1, y1] = exgcd(b, a % b);");
    auto [r, x1, y1] = exgcd(b, a % b);
    return {r, y1, x1 - a / b * y1};
}
*/
std::tuple<fast_io::natural, fast_io::natural, fast_io::natural> exgcd(fast_io::natural r0, fast_io::natural r1) // calc ax+by = gcd(a, b) return x
{
    fast_io::natural x0(1);
    fast_io::natural y0(0);
    fast_io::natural x1(0);
    fast_io::natural y1(1);
    fast_io::natural x(r0);
    fast_io::natural y(r1);
    auto r = r0 % r1;
    auto q = r0; // r1
    while (r){
        x = x0 - q * x1;
        y = y0 - q * y1;
        x0 = x1;
        y0 = y1;
        x1 = x;
        y1 = y;
        r0 = r1;
        r1 = r;
        r = r0 % r1;
        q = r0; // r1
    }
    return {r, x, y};
}

std::pair<key, key> gen_key(fast_io::natural p, fast_io::natural q)
{
    auto n = p * q;
    auto fy = (p - 1) * (q - 1);
    fast_io::natural e(3889);          // 选取e  一般选取65537
    // generate d
    auto a = e;
    auto b = fy;
    auto [r, x, y] = exgcd(a, b);
    auto d = x;
    // 返回：  公钥   私钥
    return {{n, e}, {n, d}};
}

inline auto crypt(fast_io::natural m, key k)
{
    auto n = k.first;
    auto e = k.second;
    auto c = pow_mod(m, e, n);
    return c;
}

int main()
{
    fast_io::natural message("11451419123212312312312");
    auto [p, q] = generate_random_prime_pair();
    auto [pubkey, prikey] = gen_key(p, q);
    //auto cipher = crypt(message, pubkey);
    //println_flush(fast_io::out, "cipher:", cipher);
    //auto plain = crypt(message, prikey);
    //println_flush(fast_io::out, "plain:", plain);
}
