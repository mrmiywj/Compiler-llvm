struct A
{
  int a;
  int b;
};

struct B
{
  int a;
  int b;
};

int main()
{
  struct A x;
  struct B y;
  x.a = 1;
  y.a = 2;
  write(x.a);
  y.b = x.a;
  return 0;
}
