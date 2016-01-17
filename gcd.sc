int a,b;

int gcd(int x,int y)
{
	if(!y)
		return x;
	return gcd(y,x%y);
}

int main()
{
	a = 60;
	b = 24;
	return gcd(a,b);
}
