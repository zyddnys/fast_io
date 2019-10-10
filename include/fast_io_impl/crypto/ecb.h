#pragma once

#include <array>
#include <memory>
#include "../concept.h"

namespace fast_io::crypto
{

template<input_stream T, typename Dec, size_t key_size, size_t block_size>
class basic_iecb
{
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
private:
    using unsigned_char_type = std::make_unsigned_t<char_type>;
public:
    using key_type = std::array<unsigned_char_type, key_size>;
private:
    std::array<unsigned_char_type, block_size> buf={};
    std::array<unsigned_char_type, block_size>::iterator pos = buf.begin();
    std::array<unsigned_char_type, block_size> buf_out={};
    std::array<unsigned_char_type, block_size>::iterator pos_out = buf_out.begin();
    key_type key;
    T& ib;
    Dec dec;

    unsigned_char_type* mread(unsigned_char_type* pb,unsigned_char_type* pe)
    {
		auto pi(pb);
        
        std::size_t const input_length(pe - pi);

        if (pos_out!=buf_out.begin())
        {
            std::size_t bg(pos_out-buf_out.begin());
            std::size_t min_length = input_length;
            if(bg < min_length)
                min_length = bg;
            pi = std::uninitialized_copy(buf_out.begin(), buf_out.begin()+min_length, pi);
            std::size_t plaintext_remain_length = pos_out-buf_out.begin()-min_length;
            if (plaintext_remain_length)
            {
                std::uninitialized_copy(pos_out, pos_out+plaintext_remain_length, buf_out.begin());
                pos_out = buf_out.begin()+plaintext_remain_length;
                return pi;
            }
            pos_out = buf_out.begin();
            return pi;
        }

        for(;pi!=pe;)
        {
            //println(fast_io::err, "for(;pi!=pe;)");
            //println(fast_io::err, "buf size ",buf.end()-buf.begin());
            //println(fast_io::err, "pos before ",pos-buf.begin());
            pos = ib.read(pos, buf.end());
            //println(fast_io::err, "pos after ",pos-buf.begin());
            if (pos!=buf.end())
                return pi;
            //println(fast_io::err, "done reading");

            auto plain = dec(buf.begin(), key);
            pos = buf.begin();

            std::size_t available_out_space = pe - pi;
            if (available_out_space < block_size)
            {
                //println(fast_io::err, "if (available_out_space < block_size)");
                pi = std::uninitialized_copy(plain.begin(), plain.begin()+available_out_space, pi);
                pos_out = std::uninitialized_copy(plain.begin()+available_out_space, plain.end(), buf_out.begin());
                break;
            }
            else
            {
                //println(fast_io::err, "if (available_out_space < block_size) else");
                pi = std::uninitialized_copy(plain.begin(), plain.end(), pi);
            }
        }
        return pi;
    }

public:
    basic_iecb(key_type const& init_key, T& i) : key(init_key), ib(i)
    {

    }



    template<typename ContiguousIterator>
	ContiguousIterator read(ContiguousIterator begin, ContiguousIterator end)
    {
        auto bgchadd(static_cast<unsigned_char_type*>(static_cast<void*>(std::addressof(*begin))));
		return begin+(mread(bgchadd,static_cast<unsigned_char_type*>(static_cast<void*>(std::addressof(*end))))-bgchadd)/sizeof(*begin);
    }
};

template<output_stream T, typename Enc, size_t key_size, size_t block_size>
class basic_oecb
{
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
private:
    using unsigned_char_type = std::make_unsigned_t<char_type>;
public:
    using key_type = std::array<unsigned_char_type, key_size>;

    std::array<unsigned_char_type, block_size> buf={};
    std::size_t buf_pos=0;
    key_type key;
    T& ob;
    Enc enc;
public:
    basic_oecb(key_type const& init_key, T& o) : key(init_key), ob(o)
    {

    }

    void flush()
	{
        if (buf_pos)
        {
            //println(fast_io::err, "inside flush() ", "if (buf_pos)");
            //println(fast_io::err, "buf_pos ", buf_pos);
            std::uninitialized_fill(buf.begin() + buf_pos, buf.end(), 0);
            auto cipher = enc(buf.begin(), key);
            ob.write(cipher.cbegin(), cipher.cend());
            buf_pos = 0;
        }
	}


    basic_oecb(basic_oecb const&) = delete;
    basic_oecb& operator=(basic_oecb const& o) = delete;

    basic_oecb(basic_oecb&& o) noexcept = delete;
    basic_oecb& operator=(basic_oecb&& o) noexcept = delete;

    template<typename ContiguousIterator>
	void write(ContiguousIterator b, ContiguousIterator e)
	{
		write_precondition<unsigned_char_type>(b, e);
        auto pb(static_cast<unsigned_char_type const*>(static_cast<void const*>(std::addressof(*b))));
		auto pi(pb), pe(pb+(e-b)*sizeof(*b)/sizeof(unsigned_char_type));
//        std::size_t const range_size(pe - pi);
        std::size_t const input_length(pe - pi + buf_pos);
        //println(fast_io::err, "input_length: ", input_length);
        if (input_length < block_size)
        {
            buf_pos=std::uninitialized_copy(pi, pe, buf.data() + buf_pos)-buf.data();
            return;
        }
        if (buf_pos)
        {
            //println(fast_io::err, "inside ", "if (buf_pos)");
            auto bg(buf.data()-buf_pos);
            pi += std::uninitialized_copy(pi, pi+ (block_size-buf_pos), bg)-bg;
            auto cipher = enc(buf.begin(), key);
            ob.write(cipher.cbegin(), cipher.cend());
        }
        for(; pi + block_size <= pe; pi += block_size)
        {
            //println(fast_io::err, "inside ", "for(; pi < pe; pi += block_size)");
            auto cipher = enc(pi, key);
            ob.write(cipher.cbegin(), cipher.cend());
            //println(fast_io::err, "after ", "for(; pi < pe; pi += block_size)");
        }
        buf_pos=std::uninitialized_copy(pi, pe, buf.data())-buf.data();
	}

    ~basic_oecb()
    try{
        flush();
    }catch(...){

    }

};

}
