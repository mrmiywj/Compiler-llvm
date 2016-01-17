; ModuleID = 'Simple C'

@x = global i32 1
@y = external global i32
@z = global i32 2
@a = global i32 10
@arr = global [2 x i32] [i32 1, i32 2]

declare double @MABI()

define i32 @f(i32 %x1) {
entry:
  %x = alloca i32
  store i32 %x1, i32* %x
  %x2 = load i32, i32* %x
  %0 = icmp eq i32 %x2, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  ret i32 1

else:                                             ; preds = %entry
  %x3 = load i32, i32* %x
  %x4 = load i32, i32* %x
  %1 = sub i32 %x4, 1
  %calltmp = call i32 @f(i32 %1)
  %2 = mul i32 %x3, %calltmp
  ret i32 %2
}

define i32 @main() {
entry:
  %res = alloca i32
  %calltmp = call i32 @f(i32 5)
  store i32 %calltmp, i32* %res
  %res1 = load i32, i32* %res
  ret i32 %res1
}
