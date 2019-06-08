#ifndef FAST_IO_IOBUF_H
#define FAST_IO_IOBUF_H

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
		template<typename Ihandler,typename CharTA,typename Traits,typename Buf>
		friend class basic_ibuf;
		template<typename Ohandler,typename CharTA,typename Traits,typename Buf>
		friend class basic_obuf;
		CharT *beg,*curr,*end;
	public:

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
	
	template<input_stream Ihandler,typename CharT,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
	class basic_ibuf
	{
		Ihandler ih;
		Buf bh;
	public:
		using input_handle_type = Ihandler;
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
		template<Pointer T>
		T read(T begin,T end)
		{
			return reinterpret_cast<T>(read(reinterpret_cast<CharT*>(begin),reinterpret_cast<CharT*>(end)));
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
	};

	template<output_stream Ohandler,typename CharT,typename Traits = std::char_traits<CharT>,typename Buf=basic_buf_handler<CharT>>
	class basic_obuf
	{
		Ohandler oh;
		Buf bh;
	public:
		using output_handle_type = Ohandler;
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
			try
			{
				if(bh.beg)
					oh.write(bh.beg,bh.curr);
			}
			catch(...)
			{
				
			}
		}
		basic_obuf& operator=(const basic_obuf&)=delete;
		basic_obuf(const basic_obuf&)=delete;
		basic_obuf(basic_obuf&& bmv) noexcept = default;
		basic_obuf& operator=(basic_obuf&& b) noexcept
		{
			if(&b!=this)
			{
				~basic_obuf();
				oh=std::move(b.oh);
				bh=std::move(b.bh);
			}
			return *this;
		}
		void write(const CharT* cbegin,const CharT* cend)
		{
			const std::size_t n(bh.end-bh.curr);
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

		void write(Pointer cbegin,Pointer cend)
		{
			write(reinterpret_cast<const CharT*>(cbegin),reinterpret_cast<const CharT*>(cend));
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
	};



}

#endif