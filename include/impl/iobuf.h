#pragma once

#include<memory>
#include<string>
#include<cstddef>
#include"concept.h"

namespace fast_io
{
template<input_stream Ihandler,typename CharT,typename Traits,typename Buf>
class basic_ibuf;
template<output_stream Ohandler,typename CharT,typename Traits,typename Buf>
class basic_obuf;

template<typename CharT,typename Allocator = std::allocator<CharT>,std::size_t buffer_size = 4096>
class basic_buf_handler
{
	Allocator alloc;
public:
	CharT *beg,*curr,*end;
	explicit basic_buf_handler():beg(alloc.allocate(buffer_size)),end(beg+buffer_size){}
	basic_buf_handler& operator=(const basic_buf_handler&)=delete;
	basic_buf_handler(const basic_buf_handler&)=delete;
	static constexpr std::size_t size() {return buffer_size;}
	basic_buf_handler(basic_buf_handler&& m) noexcept:beg(m.beg),curr(m.curr),end(m.end)
	{
		m.beg=nullptr;
	}
	basic_buf_handler& operator=(basic_buf_handler&& m) noexcept
	{
		if(&m!=this)
		{
			alloc.deallocate(beg,buffer_size);
			beg=m.beg;
			curr=m.curr;
			end=m.end;
			m.beg=nullptr;
		}
		return *this;
	}
	~basic_buf_handler()
	{
		alloc.deallocate(beg,buffer_size);
	}
	Allocator get_allocator() const{	return alloc;}
};

template<input_stream Ihandler,typename CharT=Ihandler::char_type,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
class basic_ibuf
{
	Ihandler ih;
	Buf bh;
public:
	using handle_type = Ihandler;
	using char_type = CharT;
	using char_traits = Traits;
	using int_type = typename Traits::int_type;
	template<typename... Args>
	basic_ibuf(Args&&... args):ih(std::forward<Args>(args)...){bh.curr=bh.end;}
	CharT* read(CharT* begin,CharT* end)
	{
		std::size_t n(end-begin);
		if(bh.end-bh.curr<n)			//cache miss
		{
			begin=std::uninitialized_copy(bh.curr,bh.end,begin);
			if(begin+Buf::size()<end)
			{
				begin=ih.read(begin,end);
				if(begin!=end)
				{
					bh.end=bh.curr=bh.beg;
					return begin;
				}
			}
			bh.end=ih.read(bh.beg,bh.beg+Buf::size());
			bh.curr=bh.beg;
			n=end-begin;
		}
		begin=std::uninitialized_copy_n(bh.curr,n,begin);
		bh.curr+=n;
		return begin;
	}
	template<typename Contiguous_Iterator>
	auto read(Contiguous_Iterator begin,Contiguous_Iterator end)
	{
		return read(static_cast<char_type*>(static_cast<void*>(std::addressof(*begin))),static_cast<char_type*>(static_cast<void*>(std::addressof(*end))));
	}
	bool eof() const
	{
		return bh.beg==bh.end;
	}
	operator bool() const
	{
		return bh.beg!=bh.end;
	}
	int_type get()
	{
		if(bh.end==bh.curr)		//cache miss
		{
			if((bh.end=ih.read(bh.beg,bh.beg+Buf::size()))==bh.beg)
				return Traits::eof();
			bh.curr=bh.beg;
		}
		return *bh.curr++;
	}
	auto& native_handle()
	{
		return ih;
	}
};

template<output_stream Ohandler,typename CharT,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
class basic_obuf
{
	Ohandler oh;
	Buf bh;
	
	void close_impl() noexcept
	try
	{
		if(bh.beg)
			oh.write(bh.beg,bh.curr);
	}
	catch(...){}
public:
	using native_handle_type = Ohandler;
	using char_type = CharT;
	using char_traits = Traits;
	using int_type = typename Traits::int_type;
	template<typename... Args>
	basic_obuf(Args&&... args):oh(std::forward<Args>(args)...){bh.curr=bh.beg;}
	void flush()
	{
		oh.write(bh.beg,bh.curr);
		bh.curr=bh.beg;
		oh.flush();
	}
	~basic_obuf()
	{
		close_impl();
	}
	basic_obuf& operator=(const basic_obuf&)=delete;
	basic_obuf(const basic_obuf&)=delete;
	basic_obuf(basic_obuf&& bmv) noexcept = default;
	basic_obuf& operator=(basic_obuf&& b) noexcept
	{
		if(&b!=this)
		{
			close_impl();
			oh=std::move(b.oh);
			bh=std::move(b.bh);
		}
		return *this;
	}
	void write(CharT const* cbegin,CharT const* cend)
	{
		std::size_t const n(bh.end-bh.curr);
		if(n<cend-cbegin)
		{
			std::uninitialized_copy_n(cbegin,n,bh.curr);
			cbegin+=n;
			oh.write(bh.beg,bh.end);
			if(cbegin+Buf::size()<cend)
			{
				oh.write(cbegin,cend);
				bh.curr=bh.beg;
			}
			else
				bh.curr=std::uninitialized_copy(cbegin,cend,bh.beg);
		}
		else
			bh.curr=std::uninitialized_copy(cbegin,cend,bh.curr);
	}
	template<typename Contiguous_Iterator>
	auto write(Contiguous_Iterator cbegin,Contiguous_Iterator cend)
	{
		return write(static_cast<char_type const*>(static_cast<void*>(std::addressof(*cbegin))),static_cast<char_type const*>(static_cast<void*>(std::addressof(*cend))));
	}
	void put(int_type ch)
	{
		if(bh.curr==bh.end)		//buffer full
		{
			oh.write(bh.beg,bh.end);
			bh.curr=bh.beg;
		}
		*bh.curr++=ch;
	}
	auto& native_handle()
	{
		return oh;
	}
};

namespace details
{
template<io_stream io_handler,typename CharT,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
struct fake_basic_ihandler:basic_obuf<io_handler,CharT,Traits,Buf>
{
template<typename ...Args>
auto read(Args&& ...args)
{
	return native_handle().read(std::forward<Args>(args)...);
}
};
}


template<io_stream io_handler,typename CharT,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
class basic_iobuf
{
	basic_ibuf<details::fake_basic_ihandler<io_handler,CharT,Traits,Buf>> ibf;
public:	
	using native_handle_type = io_handler;
	using char_type = CharT;
	using char_traits = Traits;
	using int_type = typename Traits::int_type;
	auto& native_handle()
	{
		return ibf.native_handle().native_handle();
	}
	void flush()
	{
		ibf.native_handle().flush();
	}
	void put(int_type ch)
	{
		ibf.native_handle().put(ch);
	}
	template<typename ...Args>
	auto write(Args&& ...args)
	{
		return ibf.native_handle().write(std::forward<Args>(args)...);
	}
	template<typename ...Args>
	auto read(Args&& ...args)
	{
		return ibf.read(std::forward<Args>(args)...);
	}
	auto get()
	{
		return ibf.get();
	}
	bool eof() const
	{
		return ibf.eof();
	}
	operator bool() const
	{
		return !eof();
	}
};

}
