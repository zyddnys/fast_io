#include"../../include/fast_io.h"

void transfer_ownership(fast_io::stream auto strm)
{

}

int main()
{
	static_assert(std::movable<fast_io::system_io_handle>,"system_io_handle not movable");
	static_assert(std::movable<fast_io::obuf>,"obuf_mutex not movable");
	static_assert(std::movable<fast_io::obuf_mutex>,"obuf_mutex not movable");
	static_assert(std::movable<fast_io::system_file>,"system_file not movable");
	static_assert(std::movable<fast_io::isystem_file>,"isystem_file not movable");
	static_assert(std::movable<fast_io::ibuf>,"ibuf not movable");
	static_assert(std::movable<fast_io::ibuf_mutex>,"ibuf_mutex not movable");
	fast_io::obuf ob("abcd");
	transfer_ownership(std::move(ob));
}