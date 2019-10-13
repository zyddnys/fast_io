#pragma once

namespace fast_io
{

template<typename T>
class basic_dynamic_input_stream
{
public:
	using char_type = T;
private:
	struct base
	{
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual ~base() = default;
	};
	template<stream input>
	requires input_stream<input>||mutex_input_stream<input>
	struct derv:base
	{
		input in;
		template<typename ...Args>
		derv(std::in_place_type_t<input>,Args&& ...args):in(std::forward<Args>(args)...){}
		char_type* read(char_type* b,char_type* e) {return in.read(b,e);}
	};
	std::unique_ptr<base> up;
public:
	template<stream P,typename ...Args>
	requires input_stream<P>||mutex_input_stream<P>
	basic_dynamic_input_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<std::contiguous_iterator Iter>
	Iter read(Iter b,Iter e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::to_address(b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::to_address(e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
};

using dynamic_input_stream = basic_dynamic_input_stream<char>;

template<typename T>
class basic_dynamic_standard_input_stream
{
public:
	using char_type = T;
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
	basic_dynamic_standard_input_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<std::contiguous_iterator Iter>
	Iter read(Iter b,Iter e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::to_address(b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::to_address(e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
	char_type get() {return up->get();}
	auto try_get() {return up->try_get();}
};

using dynamic_standard_input_stream = basic_dynamic_standard_input_stream<char>;

template<typename T>
class basic_dynamic_output_stream
{
public:
	using char_type = T;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual ~base() = default;
	};
	template<stream output>
	requires output_stream<output>||mutex_output_stream<output>
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
	template<stream P,typename ...Args>
	requires output_stream<P>||mutex_output_stream<P>
	basic_dynamic_output_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<std::contiguous_iterator Iter>
	void write(Iter b,Iter e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::to_address(b))),
							static_cast<char_type const*>(static_cast<void const*>(std::to_address(e))));
	}
	void flush() { return up->flush();}
};

using dynamic_output_stream = basic_dynamic_output_stream<char>;

template<typename T>
class basic_dynamic_standard_output_stream
{
public:
	using char_type = T;
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
	basic_dynamic_standard_output_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<std::contiguous_iterator Iter>
	void write(Iter b,Iter e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::to_address(b))),
							static_cast<char_type const*>(static_cast<void const*>(std::to_address(e))));
	}
	void flush() { return up->flush();}
	void put(char_type ch){up->put(ch);}
};

using dynamic_standard_output_stream = basic_dynamic_standard_output_stream<char>;

template<typename T>
class basic_dynamic_io_stream
{
public:
	using char_type = T;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual ~base() = default;
	};
	template<stream iod>
	requires io_stream<iod>||mutex_io_stream<iod>
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
	template<stream P,typename ...Args>
	requires io_stream<P>||mutex_io_stream<P>
	basic_dynamic_io_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<std::contiguous_iterator Iter>
	void write(Iter b,Iter e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::to_address(b))),
							static_cast<char_type const*>(static_cast<void const*>(std::to_address(e))));
	}
	void flush() { return up->flush();}
	template<std::contiguous_iterator Iter>
	Iter read(Iter b,Iter e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::to_address(b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::to_address(e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
};
using dynamic_io_stream = basic_dynamic_io_stream<char>;

template<typename T>
class basic_dynamic_standard_io_stream
{
public:
	using char_type = T;
private:
	struct base
	{
		virtual void write(char_type const*,char_type const*) = 0;
		virtual void flush() = 0;
		virtual char_type* read(char_type*,char_type*) = 0;
		virtual void put(char_type) = 0;
		virtual char_type get() = 0;
		virtual std::pair<char_type,bool> try_get() = 0;
		virtual ~base() = default;
	};
	template<standard_io_stream iod>
	struct derv:base
	{
		iod out;
		template<typename ...Args>
		derv(std::in_place_type_t<iod>,Args&& ...args):out(std::forward<Args>(args)...){}
		void write(char_type const* b,char_type const* e) {out.write(b,e);}
		void flush() {out.flush();}
		char_type* read(char_type* b,char_type* e) {return out.read(b,e);}
		void put(char_type ch){up->put(ch);}
		char_type get() {return out.get();}
		std::pair<char_type,bool> try_get() {return out.try_get();}
	};
	std::unique_ptr<base> up;
public:
	template<standard_io_stream P,typename ...Args>
	basic_dynamic_standard_io_stream(std::in_place_type_t<P>,Args&& ...args):
		up(new derv<P>(std::in_place_type<P>,std::forward<Args>(args)...)){}
	template<standard_io_stream P>
	requires (!std::same_as<std::decay_t<P>,basic_dynamic_standard_io_stream<T>>)
	basic_dynamic_standard_io_stream(P&& p):basic_dynamic_standard_io_stream(std::in_place_type<std::decay_t<P>>,std::forward<P>(p)){}

	template<std::contiguous_iterator Iter>
	void write(Iter b,Iter e)
	{
		up->write(static_cast<char_type const*>(static_cast<void const*>(std::to_address(b))),
							static_cast<char_type const*>(static_cast<void const*>(std::to_address(e))));
	}
	void flush() { return up->flush();}
	template<std::contiguous_iterator Iter>
	Iter read(Iter b,Iter e)
	{
		char_type *pb(static_cast<char_type*>(static_cast<void*>(std::to_address(b))));
		char_type *pe(static_cast<char_type*>(static_cast<void*>(std::to_address(e))));
		return b+(up->read(pb,pe)-pb)*sizeof(*b)/sizeof(char_type);
	}
	void put(char_type ch){up->put(ch);}
	char_type get() {return up->get();}
	auto try_get() {return up->try_get();}
};
using dynamic_standard_io_stream = basic_dynamic_standard_io_stream<char>;
}