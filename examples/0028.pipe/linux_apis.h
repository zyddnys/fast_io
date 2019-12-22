#pragma once

#include<span>
#include<string_view>
#include<memory>
#include <sys/wait.h>

namespace linux_apis
{

template<typename ...Args>
[[nodiscard]] inline auto fork(Args&& ...args)
{
	auto child_process_id(::fork(std::forward<Args>(args)...));
	if(child_process_id<0)
		throw std::system_error(errno,std::generic_category());
	return child_process_id;
}

inline void execve(std::string_view pathname,std::span<std::string_view const> argv,char* const *envp = environ)
{
	std::vector<std::unique_ptr<char[]>> vec;
	for(auto const & e : argv)
	{
		auto uptr(std::make_unique<char[]>(e.size()+1));
		std::uninitialized_copy(e.cbegin(),e.cend(),uptr.get());
		vec.emplace_back(std::move(uptr));
	}
	vec.emplace_back();
	if(::execve(pathname.data(),static_cast<char* const*>(static_cast<void* const>(vec.data())),envp)<0)
		throw std::system_error(errno,std::generic_category());
}

template<typename ...Args>
inline auto waitpid(Args&& ...args)
{
	auto pd(::waitpid(std::forward<Args>(args)...));
	if(pd<0)
		throw std::system_error(errno,std::generic_category());
	return pd;
}

}