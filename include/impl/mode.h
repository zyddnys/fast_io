#ifndef FAST_IO_MODE_H
#define FAST_IO_MODE_H

namespace fast_io
{
namespace open_mode
{
	class in{};
	class out{};
	class app{};
}

enum class openmode
{
in=0,
out=1,
app=2,
ate=4,
trunc=8
};

template<openmode om>
struct open_interface_t
{
inline static constexpr auto in() {return om&openmode::in;}
inline static constexpr auto out() {return om&openmode::out;}
inline static constexpr auto app() {return om&openmode::app;}
inline static constexpr auto ate() {return om&openmode::ate;}
inline static constexpr auto trunc() {return om&openmode::trunc;}
};

template<openmode om>
concept bool open_mode_interface() {return true;}


class native_interface_t{}native_interface;
}
#endif