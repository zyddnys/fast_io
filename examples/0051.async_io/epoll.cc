#include"../../include/fast_io.h"
#include"../../include/fast_io_async.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include<vector>

int main()
try
{
	fast_io::async_server server(2000,fast_io::sock::type::stream);
	fast_io::epoll::handle_pool pool(1000);
	add_control(pool,server,fast_io::epoll::event::in);
	std::array<fast_io::epoll::events,1000> events_buffer;
	std::vector<fast_io::acceptor> clients;
	for(;;)
		for(auto const &ele : wait(pool,events_buffer))
			switch(get(ele))
			{
			case fast_io::epoll::event::in:
				add_control(pool,clients.emplace_back(server),
				fast_io::epoll::event::out|fast_io::epoll::event::hup);
			break;
			case fast_io::epoll::event::out:
			case fast_io::epoll::event::hup:
				for(auto it(clients.begin());it!=clients.end();++it)
					if(*it==ele)
					{
						if(get(ele)==fast_io::epoll::event::out)
							print(*it,"Hello World\n");
						iter_swap(it,clients.end()-1);
						clients.pop_back();
						break;
					}
			};
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}