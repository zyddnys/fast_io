#pragma once

#include"concept.h"
#include"stringbuf.h"

namespace fast_io
{
template<output_stream output,typename ostr = basic_ostring<std::string>>			// the output device itself must be atomic
class basic_sync
{
public:
	using native_handle_type = output;
	using char_type = typename native_handle_type::char_type;
	using buffer_type = ostr;
private:
	native_handle_type handle;
	buffer_type mostr;
public:
	auto& buffer(){return mostr;}
	auto& native_handle() {return handle;}
	template<typename ...Args>
	basic_sync(Args&& ...args):output(std::forward<Args>(args)...){}
	void flush()
	{
		handle.write(mostr.cbegin(),mostr.cend());
		mostr.clear();
	}
	template<typename ...Args>
	void write(Args&& ...args)
	{
		mostr.write(std::forward<Args>(args)...);
	}
	void put(char_type ch)
	{
		mostr.put(ch);
	}
	template<typename... Args>
	void seek(Args&& ...args) requires random_access_stream<native_handle_type>
	{
		flush();
		handle.seek(std::forward<Args>(args)...);
	}
};

template<output_stream output,typename ostr = basic_ostring<std::string>>
class basic_fsync:public basic_sync<output,ostr>
{
	bool need_sync = true;
	void sync()
	try
	{
		if(need_sync)
			basic_sync<output,ostr>::flush();
	}
	catch(...){}
public:
	using native_handle_type = output;
	using char_type = typename native_handle_type::char_type;
	using buffer_type = ostr;
	template<typename... Args>
	constexpr basic_fsync(Args&& ...args):basic_sync<output,ostr>(std::forward<Args>(args)...) {}
	basic_fsync(basic_fsync const&) = delete;
	basic_fsync& operator=(basic_fsync const&) = delete;
	basic_fsync(basic_fsync&& other) noexcept:basic_sync<output,ostr>(std::move(other)),need_sync(other.need_sync)
	{
		other.need_sync=false;
	}
	basic_fsync& operator=(basic_fsync&& other) noexcept
	{
		if(&other!=this)
		{
			sync();
			basic_sync<output,ostr>::operator=(std::move(other));
			other.need_sync=false;
		}
		return *this;
	}
	~basic_fsync(){sync();}
};

}