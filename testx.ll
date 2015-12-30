; ModuleID = 'Simple C'

declare double @MABI()

define i32 @f(i32, i32, i32) {
entry:
  %x = alloca i32
  %y = alloca i32
  %z = alloca i32
}

define i32 @main() {
entry:
  %x = alloca i32
  store i32 7, i32* %x
  %x1 = load i32, i32* %x
  %x2 = load i32, i32* %x
  %0 = add i32 %x2, 5
  store i32 %0, i32* %x
  %x3 = load i32, i32* %x
  ret i32 %x3
}

