#pragma once
#include"concept.h"
#include<memory>

namespace fast_io
{

class dynamic_input_stream
{
public:
	using char_type = char;
private:
	struct base
	{
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual ~base() = default;
	};
	template<input_stream input>
	struct derv:base
	{
		input in;
		template<typename ...Args>
		derv(std::in_place_type_t<input>,Args&& ...args):in(std::forward<Args>(args)...){}
		char_type* read(char_type* b,char_type* e) {return in.read(b,e);}
	};
	std::unique_ptr<base> up;
public:
	template<input_stream P,typename ...Args>
	dynamic_input_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::addressof(*e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
};

class dynamic_standard_input_stream
{
public:
	using char_type = char;
private:
	struct base
	{
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual char_type get() = 0;
		virtual std::pair<char_type,bool> try_get() = 0;
		virtual ~base() = default;
	};
	template<standard_input_stream input>
	struct derv:base
	{
		input in;
		template<typename ...Args>
		derv(std::in_place_type_t<input>,Args&& ...args):in(std::forward<Args>(args)...){}
		char_type* read(char_type* b,char_type* e) {return in.read(b,e);}
		char_type get() {return in.get();}
		std::pair<char_type,bool> try_get() {return in.try_get();}
	};
	std::unique_ptr<base> up;
public:
	template<standard_input_stream P,typename ...Args>
	dynamic_standard_input_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::addressof(*e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
	char_type get() {return up->get();}
};

class dynamic_output_stream
{
public:
	using char_type = char;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual ~base() = default;
	};
	template<output_stream output>
	struct derv:base
	{
		output out;
		template<typename ...Args>
		derv(std::in_place_type_t<output>,Args&& ...args):out(std::forward<Args>(args)...){}
		void write(char_type const* b,char_type const* e) {out.write(b,e);}
		void flush() {out.flush();}
	};
	std::unique_ptr<base> up;
public:
	template<output_stream P,typename ...Args>
	dynamic_output_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator b,Contiguous_iterator e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))),
							static_cast<char_type const*>(static_cast<void const*>(std::addressof(*e))));
	}
	void flush() { return up->flush();}
};

class dynamic_standard_output_stream
{
public:
	using char_type = char;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual void put(char_type) = 0;
		virtual ~base() = default;
	};
	template<standard_output_stream output>
	struct derv:base
	{
		output out;
		template<typename ...Args>
		derv(std::in_place_type_t<output>,Args&& ...args):out(std::forward<Args>(args)...){}
		void write(char_type const* b,char_type const* e) {out.write(b,e);}
		void flush() {out.flush();}
		void put(char_type ch) {out.put(ch);};
	};
	std::unique_ptr<base> up;
public:
	template<standard_output_stream P,typename ...Args>
	dynamic_standard_output_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator b,Contiguous_iterator e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))),
							static_cast<char_type const*>(static_cast<void const*>(std::addressof(*e))));
	}
	void flush() { return up->flush();}
	void put(char_type ch){up->put(ch);}
};

class dynamic_io_stream
{
public:
	using char_type = char;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual ~base() = default;
	};
	template<io_stream iod>
	struct derv:base
	{
		iod out;
		template<typename ...Args>
		derv(std::in_place_type_t<iod>,Args&& ...args):out(std::forward<Args>(args)...){}
		void write(char_type const* b,char_type const* e) {out.write(b,e);}
		void flush() {out.flush();}
		char_type* read(char_type* b,char_type* e) {return out.read(b,e);}
	};
	std::unique_ptr<base> up;
public:
	template<input_stream P,typename ...Args>
	dynamic_io_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<typename Contiguous_iterator>
	void write(Contiguous_iterator b,Contiguous_iterator e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::addressof(*b))),
							static_cast<char_type const*>(static_cast<void const*>(std::addressof(*e))));
	}
	void flush() { return up->flush();}
	template<typename Contiguous_iterator>
	Contiguous_iterator read(Contiguous_iterator b,Contiguous_iterator e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::addressof(*b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::addressof(*e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
};

}