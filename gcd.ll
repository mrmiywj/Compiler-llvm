; ModuleID = 'Simple C'

@a = global i32 0
@b = global i32 0

declare double @MABI()

define i32 @gcd(i32 %x1, i32 %y2) {
entry:
  %x = alloca i32
  store i32 %x1, i32* %x
  %y = alloca i32
  store i32 %y2, i32* %y
  %y3 = load i32, i32* %y
  %0 = icmp eq i32 %y3, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  %x4 = load i32, i32* %x
  ret i32 %x4

else:                                             ; preds = %entry
  %y5 = load i32, i32* %y
  %x6 = load i32, i32* %x
  %y7 = load i32, i32* %y
  %1 = srem i32 %x6, %y7
  %calltmp = call i32 @gcd(i32 %y5, i32 %1)
  ret i32 %calltmp
}

define i32 @main() {
entry:
  %a = load i32, i32* @a
  store i32 60, i32* @a
  %b = load i32, i32* @b
  store i32 24, i32* @b
  %a1 = load i32, i32* @a
  %b2 = load i32, i32* @b
  %calltmp = call i32 @gcd(i32 %a1, i32 %b2)
  ret i32 %calltmp
}
