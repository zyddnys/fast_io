#pragma once

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
	requires std::constructible_from<output,Args...>
	basic_sync(Args&& ...args):handle(std::forward<Args>(args)...){}
	void flush()
	{
		handle.writes(mostr.str().cbegin(),mostr.str().cend());
		mostr.clear();
	}
	template<typename ...Args>
	void writes(Args&& ...args)
	{
		mostr.writes(std::forward<Args>(args)...);
	}
	void put(char_type ch)
	{
		mostr.put(ch);
	}
	template<typename... Args>
	auto seek(Args&& ...args) requires random_access_stream<native_handle_type>
	{
		flush();
		return handle.seek(std::forward<Args>(args)...);
	}
	template<std::contiguous_iterator Iter>
	Iter reads(Iter begin,Iter end)
		requires input_stream<native_handle_type>
	{
		flush();
		return handle.reads(begin,end);
	}
	auto get() requires standard_input_stream<native_handle_type>
	{
		flush();
		return handle.get();
	}
	auto try_get() requires standard_input_stream<native_handle_type>
	{
		flush();
		return handle.try_get();
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
	template<typename ...Args>
	requires std::constructible_from<output,Args...>
	constexpr basic_fsync(Args&& ...args):basic_sync<output,ostr>(std::forward<Args>(args)...) {}
	basic_fsync(basic_fsync const&) = delete;
	basic_fsync& operator=(basic_fsync const&) = delete;
	basic_fsync(basic_fsync&& other) noexcept:basic_sync<output,ostr>(std::move(other)),need_sync(other.need_sync)
	{
		other.need_sync=false;
	}
	basic_fsync& operator=(basic_fsync&& other) noexcept
	{
		if(std::addressof(other)!=this)
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