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

		if (plaintext_buf_pos != plaintext_buf.end())
		{
			std::size_t buffer_length(plaintext_buf.end() - plaintext_buf_pos);
			std::size_t min_length(input_length);
			if (buffer_length < min_length)
				min_length = buffer_length;
			pi = std::uninitialized_copy(plaintext_buf_pos, plaintext_buf_pos + min_length, pi);
			plaintext_buf_pos += min_length;
			if (plaintext_buf_pos != plaintext_buf.end())
				return pi;
		}

		for (; pi != pe;)
		{
			cipher_buf_pos = ib.reads(cipher_buf_pos, cipher_buf.end());
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
				plaintext_buf_pos = plaintext_buf.end() - cipher_type::block_size + available_out_space;
				std::uninitialized_copy(plain.begin() + available_out_space, plain.end(), plaintext_buf_pos);
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
	Iter reads(Iter begin, Iter end)
	{
		auto bgchadd(static_cast<unsigned_char_type *>(static_cast<void *>(std::to_address(begin))));
		return begin + (mread(bgchadd, static_cast<unsigned_char_type *>(static_cast<void *>(std::to_address(end)))) - bgchadd) / sizeof(*begin);
	}
	char_type get()
	{
		if (plaintext_buf_pos == plaintext_buf.end())
		{
			block_type tmp;
			auto next_ch(tmp.begin() + 1);
			auto ret(reads(tmp.begin(), next_ch));
			if (ret != next_ch)
			{
				plaintext_buf_pos = plaintext_buf.begin();
				throw eof();
			}
			return static_cast<char_type>(*tmp.begin());
		}
		return static_cast<char_type>(*plaintext_buf_pos++);
	}

	std::pair<char_type, bool> try_get()
	{
		if (plaintext_buf_pos == plaintext_buf.end())
		{
			block_type tmp;
			auto next_ch(tmp.begin() + 1);
			auto ret(reads(tmp.begin(), next_ch));
			if (ret != next_ch)
			{
				plaintext_buf_pos = plaintext_buf.begin(); // TODO: begin or end
				return {0, true};
			}
			return {static_cast<char_type>(*tmp.begin()), false};
		}
		return {static_cast<char_type>(*plaintext_buf_pos++), false};
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
private:
	using block_iterator = typename block_type::iterator;
	block_type plaintext_buf = {};
	block_iterator plaintext_buf_pos = plaintext_buf.begin();
	key_type key;
	block_type iv;
	Enc enc;
	T ob;

	void write_remain()
	{
		if (plaintext_buf_pos != plaintext_buf.begin())
		{
			std::uninitialized_fill(plaintext_buf_pos, plaintext_buf.end(), 0);

			block_type xored_text(iv);
			for (std::size_t i(0); i != iv.size(); ++i)
				xored_text[i] ^= plaintext_buf[i];
			
			auto cipher(enc(xored_text.data()));
			ob.writes(cipher.cbegin(), cipher.cend());
			iv = cipher;
			plaintext_buf_pos = plaintext_buf.begin();
		}
	}

public:
	template<typename K,typename IV,typename... Args>
	requires (std::constructible_from<key_type,K>&&std::constructible_from<block_type,IV>&&std::constructible_from<T,Args...>)
	basic_ocbc(K&& init_key, IV&& init_iv, Args&& ...args) : key(std::forward<K>(init_key)), iv(std::forward<IV>(init_iv)), enc(key.data()), ob(std::forward<Args>(args)...)
	{
	}

	void flush()
	{
		write_remain();
		ob.flush();
	}

	template<std::contiguous_iterator Iter>
	void writes(Iter b, Iter e)
	{
		writes_precondition<unsigned_char_type>(b, e);
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
			ob.writes(cipher.cbegin(), cipher.cend());
			iv = cipher;

			plaintext_buf_pos = plaintext_buf.begin();
		}

		for (; pi + cipher_type::block_size <= pe; pi += cipher_type::block_size)
		{
			block_type xored_text(iv);
			for (std::size_t i(0); i != iv.size(); ++i)
				xored_text[i] ^= pi[i];
			auto cipher(enc(xored_text.data()));
			ob.writes(cipher.cbegin(), cipher.cend());
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
			ob.writes(cipher.cbegin(), cipher.cend());
			iv = cipher;
			plaintext_buf_pos = plaintext_buf.begin();
		}
		*plaintext_buf_pos = static_cast<unsigned_char_type>(ch);
		++plaintext_buf_pos;
	}
	basic_ocbc(basic_ocbc const&)=delete;
	basic_ocbc& operator=(basic_ocbc const&)=delete;
	basic_ocbc(basic_ocbc&& ocbc) noexcept:
		plaintext_buf(std::move(plaintext_buf)),
		plaintext_buf_pos(std::move(ocbc.plaintext_buf_pos)),
		key(std::move(ocbc.key)),
		iv(std::move(ocbc.iv)),
		ob(std::move(ocbc.ob)),
		enc(std::move(ocbc.enc))
	{
		ocbc.plaintext_buf_pos = plaintext_buf.begin();
	}
	basic_ocbc& operator=(basic_ocbc&& ocbc) noexcept
	{
		if(std::addressof(ocbc)!=this)
		{
			try
			{
				flush();
			}
			catch(...){}
			plaintext_buf=std::move(ocbc.plaintext_buf);
			plaintext_buf_pos=std::move(ocbc.plaintext_buf_pos);
			key=std::move(ocbc.key);
			iv=std::move(ocbc.iv);
			ob=std::move(ocbc.ob);
			enc=std::move(ocbc.enc);
			ocbc.plaintext_buf_pos=plaintext_buf.begin();
		}
	}
	~basic_ocbc()
	try
	{
		write_remain();
	}
	catch (...)
	{
	}
	void swap(basic_ocbc &other) noexcept
	{
		using std::swap;
		swap(plaintext_buf,other.plaintext_buf);
		swap(plaintext_buf_pos,other.plaintext_buf_pos);
		swap(key,other.key);
		swap(key,other.iv);
		swap(ob,other.ob);
		swap(enc,other.enc);
	}
};

template <output_stream T, typename Enc>
inline void swap(basic_ocbc<T,Enc>& a,basic_ocbc<T,Enc>& b) noexcept
{
	a.swap(b);
}

} // namespace fast_io::crypto
