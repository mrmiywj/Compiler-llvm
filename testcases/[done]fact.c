int fact(int n)
{
	if (n<1) return 1;
	else return n*fact(n-1);
}

int main()
{
	int n;
	read(n);
	write(fact(n));
    return 0;
}
