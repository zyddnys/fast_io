#pragma once

#include<memory>
#include<string>
#include<cstddef>
#include"concept.h"
#include"precondition.h"
#include<memory>

namespace fast_io
{
template<input_stream Ihandler,typename Buf>
class basic_ibuf;
template<output_stream Ohandler,typename Buf>
class basic_obuf;

template<typename CharT,typename Allocator = std::allocator<CharT>,std::size_t buffer_size = 1048576>
class basic_buf_handler
{
	Allocator alloc;
public:
	CharT *beg,*curr,*end;
	explicit basic_buf_handler():beg(alloc.allocate(buffer_size)),end(beg+buffer_size){}
	basic_buf_handler& operator=(basic_buf_handler const&)=delete;
	basic_buf_handler(basic_buf_handler const&)=delete;
	static constexpr std::size_t size() {return buffer_size;}
	basic_buf_handler(basic_buf_handler&& m) noexcept:beg(m.beg),curr(m.curr),end(m.end)
	{
		m.beg=nullptr;
	}
	basic_buf_handler& operator=(basic_buf_handler&& m) noexcept
	{
		if(std::addressof(m)!=this)
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

template<input_stream Ihandler,typename Buf=basic_buf_handler<typename Ihandler::char_type>>
class basic_ibuf
{
	Ihandler ih;
	Buf bh;
public:
	using native_handle_type = Ihandler;
	using char_type = typename native_handle_type::char_type;	
private:
	char_type* mread(char_type* begin,char_type* end)
	{
		std::size_t n(end-begin);
		if(bh.end<n+bh.curr)			//cache miss
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
public:
	template<typename... Args>
	basic_ibuf(Args&&... args):ih(std::forward<Args>(args)...){bh.curr=bh.end;}
	template<typename Contiguous_Iterator>
	auto read(Contiguous_Iterator begin,Contiguous_Iterator end)
	{
		auto bgchadd(static_cast<char_type*>(static_cast<void*>(std::addressof(*begin))));
		return begin+(mread(bgchadd,static_cast<char_type*>(static_cast<void*>(std::addressof(*end))))-bgchadd)/sizeof(*begin);
	}
	bool eof() const
	{
		return bh.beg==bh.end;
	}
	std::pair<char_type,bool> try_get()
	{
		if(bh.end==bh.curr)		//cache miss
		{
			if((bh.end=ih.read(bh.beg,bh.beg+Buf::size()))==bh.beg)
				return {0,true};
			bh.curr=bh.beg;
		}
		return {*bh.curr++,false};
	}
	char_type get()
	{
		if(bh.end==bh.curr)		//cache miss
		{
			if((bh.end=ih.read(bh.beg,bh.beg+Buf::size()))==bh.beg)
				throw std::runtime_error("Try to read data from EOF stream");
			bh.curr=bh.beg;
		}
		return *bh.curr++;
	}
	auto& native_handle()
	{
		return ih;
	}
	template<typename... Args>
	void seek(Args&& ...args) requires(random_access_stream<Ihandler>)
	{
		bh.curr=bh.end;
		ih.seek(std::forward<Args>(args)...);
	}
};
template<output_stream Ohandler,typename Buf=basic_buf_handler<typename Ohandler::char_type>>
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
	using char_type = typename native_handle_type::char_type;
private:
	void mwrite(char_type const* cbegin,char_type const* cend)
	{
		std::size_t const n(bh.end-bh.curr);
		if(n<static_cast<std::size_t>(cend-cbegin))
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
public:
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
	basic_obuf& operator=(basic_obuf const&)=delete;
	basic_obuf(basic_obuf const&)=delete;
	basic_obuf(basic_obuf&& bmv) noexcept = default;
	basic_obuf& operator=(basic_obuf&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			oh=std::move(b.oh);
			bh=std::move(b.bh);
		}
		return *this;
	}
	template<typename Contiguous_Iterator>
	void write(Contiguous_Iterator cbegin,Contiguous_Iterator cend)
	{
		write_precondition<char_type>(cbegin,cend);
		mwrite(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*cbegin))),static_cast<char_type const*>(static_cast<void const*>(std::addressof(*cend))));
	}
	void put(char_type ch)
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
	template<typename... Args>
	void seek(Args&& ...args) requires(random_access_stream<Ohandler>)
	{
		oh.write(bh.beg,bh.curr);
		bh.curr=bh.beg;
		oh.seek(std::forward<Args>(args)...);
	}
};

namespace details
{
template<io_stream io_handler,typename Buf>
struct fake_basic_ihandler:basic_obuf<io_handler,Buf>
{
template<typename ...Args>
fake_basic_ihandler(Args&& ...args):basic_obuf<io_handler,Buf>(std::forward<Args>(args)...){}
template<typename ...Args>
auto read(Args&& ...args)
{
	return this->native_handle().read(std::forward<Args>(args)...);
}
};
}

template<io_stream io_handler,typename Buf=basic_buf_handler<typename io_handler::char_type>>
class basic_iobuf
{
public:
	using native_handle_type = io_handler;
	using char_type = typename native_handle_type::char_type;
private:
	basic_ibuf<details::fake_basic_ihandler<native_handle_type,Buf>> ibf;
public:
	template<typename ...Args>
	basic_iobuf(Args&& ...args):ibf(std::forward<Args>(args)...){}
	auto& native_handle()
	{
		return ibf.native_handle().native_handle();
	}
	void flush()
	{
		ibf.native_handle().flush();
	}
	void put(char_type ch)
	{
		ibf.native_handle().put(ch);
	}
	template<typename ...Args>
	void write(Args&& ...args)
	{
		ibf.native_handle().write(std::forward<Args>(args)...);
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
	auto try_get()
	{
		return ibf.try_get();
	}
	bool eof() const
	{
		return ibf.eof();
	}
	template<typename... Args>
	void seek(Args&& ...args) requires(random_access_stream<io_handler>)
	{
		ibf.native_handle().flush();
		ibf.seek(std::forward<Args>(args)...);
	}
};

}
