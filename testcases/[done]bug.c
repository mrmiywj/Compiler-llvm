int inline1(int A) {
  write(A);
  return 1;
}

int inline2(int a, int A) {
  inline1(A);
  a = 1;
  return 1;
}

int main() {
  int a;
  a = 0;
  write(a);
  inline2(9, a);
  write(a);
  a = 2;
  inline1(a);
  write(a);
  return 0;
}
