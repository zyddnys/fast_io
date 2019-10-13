#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf ob("generated_file.txt");
	for(std::size_t i(0);i!=10000000000ull;++i)
		ob.put('a');
	ob.put('\n');
}
