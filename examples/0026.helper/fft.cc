#include<complex>
#include<vector>
#include<iostream>
/*
template<int op>
void fft(std::vector<std::complex<long double>>& a)
{
	long double constexpr two_pi_op(2*3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825*op);
	if(a.size()<2)
		return;
	std::vector<std::complex<long double>> a1,a2;
	{
		std::size_t const half_n(a.size()>>1);
		a1.reserve(half_n);
		a2.reserve(half_n);
	}
	for(std::size_t i(0);i<a.size();i+=2)
	{
		a1.emplace_back(a[i]);
		a2.emplace_back(a[i+1]);
	}
	fft<op>(a1);
	fft<op>(a2);
	std::complex<long double> w(1,0);
	std::size_t const n(a.size());
	std::complex<long double> wn(cos(two_pi_op/n),sin(two_pi_op/n));
	for(std::size_t i(0),n2(a.size()>>1);i!=n2;++i)
	{
		a[i]=a1[i]+w*a2[i];
		a[i+n2]=a1[i]-w*a2[i];
		w*=wn;
	}
}*/


template<int op>
inline void fft(std::vector<std::complex<long double>>& P)
{
	using std::swap;
	for (std::size_t i(1), j(0),nm1(P.size()-1); i<nm1;++i)
	{
		for (std::size_t s(n); j^=s>>=1,~j&s;);
		if (i<j)
			swap(P[i], P[j]);
	}
	Complex unit_p0;
	for (int d = 0; (1 << d) < n; d++) {
	int m = 1 << d, m2 = m * 2;
	double p0 = pi / m * oper;
	sincos(p0, unit_p0.y, unit_p0.x);
	for (int i = 0; i < n; i += m2) {
	Complex unit = 1;
	for (int j = 0; j < m; j++) {
	Complex &P1 = P[i + j + m], &P2 = P[i + j];
	Complex t = unit * P1;
	P1 = P2 - t;
	P2 = P2 + t;
	unit = unit * unit_p0;
	}
	}
	}
}



int main()
{
	std::size_t n,m;
	std::cin>>n>>m;
	std::vector<std::complex<long double>> a(n+1);
	for(auto & e : a)
	{
		long double t;
		std::cin>>t;
		e={t,0};
//		std::cin>>e.real();
	}
	std::vector<std::complex<long double>> b(m+1);
	for(auto & e : b)
	{
		long double t;
		std::cin>>t;
		e={t,0};
//		std::cin>>e.real();
	}
//		scan(fast_io::in,e.real());
	m += n;
	for(n=1;n<=m;n<<=1);
	a.resize(n);
	fft<1>(a);
	b.resize(n);
	fft<1>(b);
	std::vector<std::complex<long double>> c(n);
//	std::cout<<n<<'\n';
/*&	m += n;
	for(n=1;n<= m;n<<=1);*/
	for(std::size_t i(0);i!=n;++i)
		c[i]=a[i]*b[i];
	fft<-1>(c);
	for(std::size_t i(0);i<=m;++i)
	{
		std::size_t const ci(c[i].real()/n+0.5);
		std::cout<<ci<<' ';
	}
}