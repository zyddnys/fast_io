#pragma once

#include <array>
#include <memory>
#include <iostream>
#include "../concept.h"

namespace fast_io::crypto
{

template <input_stream T, typename Dec>
class basic_icbc
{
public:
    using native_interface_t = T;
    using char_type = typename native_interface_t::char_type;
    using cipher_type = Dec;

private:
    using unsigned_char_type = std::make_unsigned_t<char_type>;

public:
    using key_type = std::array<unsigned_char_type, cipher_type::key_size>;
    using block_type = std::array<unsigned_char_type, cipher_type::block_size>;

private:
	using block_iterator = typename block_type::iterator;
    block_type cipher_buf = {};
	block_iterator cipher_buf_pos = cipher_buf.begin();
    block_type plaintext_buf = {};
	block_iterator plaintext_buf_pos = plaintext_buf.begin();
    key_type key;
    block_type iv;
    Dec dec;
	T ib;
    unsigned_char_type *mread(unsigned_char_type *pb, unsigned_char_type *pe)
    {
        auto pi(pb);

        std::size_t const input_length(pe - pi);

        if (plaintext_buf_pos != plaintext_buf.begin())
        {
            std::size_t buffer_length(plaintext_buf_pos - plaintext_buf.begin());
            std::size_t min_length(input_length);
            if (buffer_length < min_length)
                min_length = buffer_length;
			auto const buf_it(plaintext_buf.begin() + min_length);
            pi = std::uninitialized_copy(plaintext_buf.begin(), plaintext_buf.begin() + min_length, pi);
            std::size_t plaintext_remain_length(plaintext_buf_pos - plaintext_buf.begin() - min_length);
            if (plaintext_remain_length)
            {
                std::uninitialized_copy(buf_it, buf_it + plaintext_remain_length, plaintext_buf.begin());
                plaintext_buf_pos = plaintext_buf.begin() + plaintext_remain_length;
                return pi;
            }
            plaintext_buf_pos = plaintext_buf.begin();
        }

        for (; pi != pe;)
        {
            cipher_buf_pos = ib.read(cipher_buf_pos, cipher_buf.end());
            if (cipher_buf_pos != cipher_buf.end())
                return pi;

            auto xored_text(dec(cipher_buf.data()));
            cipher_buf_pos = cipher_buf.begin();

            block_type plain(iv);
            for (std::size_t i(0); i != iv.size(); ++i)
                plain[i] ^= xored_text[i];

            iv = cipher_buf;

            std::size_t available_out_space(pe - pi);
            if (available_out_space < cipher_type::block_size)
            {
                pi = std::uninitialized_copy(plain.begin(), plain.begin() + available_out_space, pi);
                plaintext_buf_pos = std::uninitialized_copy(plain.begin() + available_out_space, plain.end(), plaintext_buf.begin());
                break;
            }
            else
            {
                pi = std::uninitialized_copy(plain.begin(), plain.end(), pi);
            }

        }
        return pi;
    }

public:
	template<typename IV,typename K,typename... Args>
	requires (std::constructible_from<key_type,K>&&std::constructible_from<block_type,IV>&&std::constructible_from<T,Args...>)
    basic_icbc(K&& init_key, IV&& init_iv, Args&& ...args) :  key(std::forward<K>(init_key)),iv(std::forward<IV>(init_iv)), dec(key.data()), ib(std::forward<Args>(args)...)
    {
    }
    template<std::contiguous_iterator Iter>
	Iter read(Iter begin, Iter end)
    {
        auto bgchadd(static_cast<unsigned_char_type *>(static_cast<void *>(std::to_address(begin))));
        return begin + (mread(bgchadd, static_cast<unsigned_char_type *>(static_cast<void *>(std::to_address(end)))) - bgchadd) / sizeof(*begin);
    }
    char_type get()
	{
        if (plaintext_buf_pos == plaintext_buf.begin())
        {
            block_type tmp;
            auto next_ch(tmp.begin() + 1);
            auto ret(read(tmp.begin(), next_ch));
            if (ret != next_ch)
                throw eof();
            return static_cast<char_type>(*tmp.begin());
        }
        auto ch(*plaintext_buf_pos);
        if (plaintext_buf_pos == plaintext_buf.end())
            plaintext_buf_pos = plaintext_buf.begin();
        else
            ++plaintext_buf_pos;
        return static_cast<char_type>(ch);
	}

    std::pair<char_type, bool> try_get()
	{
        if (plaintext_buf_pos == plaintext_buf.begin())
        {
            block_type tmp;
            auto next_ch(tmp.begin() + 1);
            auto ret(read(tmp.begin(), next_ch));
            if (ret != next_ch)
                return {0, true};
            return {static_cast<char_type>(*tmp.begin()), false};
        }
        auto ch(*plaintext_buf_pos);
        if (plaintext_buf_pos == plaintext_buf.end())
            plaintext_buf_pos = plaintext_buf.begin();
        else
            ++plaintext_buf_pos;
        return {static_cast<char_type>(ch), false};
	}
};

template <output_stream T, typename Enc>
class basic_ocbc
{
public:
    using native_interface_t = T;
    using char_type = typename native_interface_t::char_type;
    using cipher_type = Enc;

private:
    using unsigned_char_type = std::make_unsigned_t<char_type>;

public:
    using key_type = std::array<unsigned_char_type, cipher_type::key_size>;
    using block_type = std::array<unsigned_char_type, cipher_type::block_size>;
	using block_iterator = typename block_type::iterator;
    block_type plaintext_buf = {};
    block_iterator plaintext_buf_pos = plaintext_buf.begin();
    key_type key;
    block_type iv;
    Enc enc;
	T ob;
public:
	template<typename K,typename IV,typename... Args>
	requires (std::constructible_from<key_type,K>&&std::constructible_from<block_type,IV>&&std::constructible_from<T,Args...>)
    basic_ocbc(K&& init_key, IV&& init_iv, Args&& ...args) : key(std::forward<K>(init_key)), iv(std::forward<IV>(init_iv)), enc(key.data()), ob(std::forward<Args>(args)...)
    {
    }

    void flush()
    {
        if (plaintext_buf_pos)
        {
            std::uninitialized_fill(plaintext_buf_pos, plaintext_buf.end(), 0);

            block_type xored_text(iv);
            for (std::size_t i(0); i != iv.size(); ++i)
                xored_text[i] ^= plaintext_buf[i];
            
            auto cipher(enc(xored_text.data()));
            ob.write(cipher.cbegin(), cipher.cend());
            iv = cipher;
            plaintext_buf_pos = plaintext_buf.begin();
        }
    }

    template<std::contiguous_iterator Iter>
    void write(Iter b, Iter e)
    {
        write_precondition<unsigned_char_type>(b, e);
        auto pb(static_cast<unsigned_char_type const *>(static_cast<void const *>(std::addressof(*b))));
        auto pi(pb), pe(pb + (e - b) * sizeof(*b) / sizeof(unsigned_char_type));
        std::size_t const input_length(pe - pi);

        if (plaintext_buf_pos != plaintext_buf.begin())
        {
            std::size_t min_length(plaintext_buf.end() - plaintext_buf_pos);
            if (input_length < min_length)
                min_length = input_length;
            plaintext_buf_pos = std::uninitialized_copy(pi, pi + min_length, plaintext_buf_pos);
            pi += min_length;

            if (plaintext_buf_pos != plaintext_buf.end())
                return;

            block_type xored_text(iv);
            for (std::size_t i = 0; i != iv.size(); ++i)
                xored_text[i] ^= plaintext_buf[i];
            
            auto cipher(enc(xored_text.data()));
            ob.write(cipher.cbegin(), cipher.cend());
            iv = cipher;

            plaintext_buf_pos = plaintext_buf.begin();
        }

        for (auto const last_length(pe - cipher_type::block_size); pi <= last_length; pi += cipher_type::block_size)
        {
            block_type xored_text(iv);
            for (std::size_t i(0); i != iv.size(); ++i)
                xored_text[i] ^= pi[i];
            auto cipher(enc(xored_text.data()));
            ob.write(cipher.cbegin(), cipher.cend());
            iv = cipher;
        }
        plaintext_buf_pos = std::uninitialized_copy(pi, pe, plaintext_buf.begin());
    }

    void put(char_type ch) {
        if (plaintext_buf_pos == plaintext_buf.end())
        {
            block_type xored_text(iv);
            for (std::size_t i = 0; i != iv.size(); ++i)
                xored_text[i] ^= plaintext_buf[i];
            
            auto cipher(enc(xored_text.data()));
            ob.write(cipher.cbegin(), cipher.cend());
            iv = cipher;
            plaintext_buf_pos = plaintext_buf.begin();
        }
        *plaintext_buf_pos = static_cast<unsigned_char_type>(ch);
        ++plaintext_buf_pos;
    }

    ~basic_ocbc()
    try
    {
        flush();
    }
    catch (...)
    {}
};

} // namespace fast_io::crypto
