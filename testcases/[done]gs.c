int c,b;
int a(int m, int n)
{
	if (m==n||n==0) return 1;
	else return a(m-1,n-1)+a(m-1,n);
}

int main()
{
	read(c);
	read(b);
	write(a(c,b));
}
