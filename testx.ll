; ModuleID = 'Simple C'

declare double @MABI()

define i32 @f(i32 %x1, i32 %y2, i32 %z3) {
entry:
  %x = alloca i32
  store i32 %x1, i32* %x
  %y = alloca i32
  store i32 %y2, i32* %y
  %z = alloca i32
  store i32 %z3, i32* %z
  ret i32 1
}

define i32 @main() {
entry:
  %i = alloca i32
  %x = alloca i32
  store i32 7, i32* %x
  %x1 = load i32, i32* %x
  %0 = add i32 %x1, 5
  store i32 %0, i32* %x
  %i2 = load i32, i32* %i
  store i32 1, i32* %i
  br label %Cond

Cond:                                             ; preds = %loop, %entry
  %i3 = load i32, i32* %i
  %1 = icmp slt i32 %i3, 10
  br i1 %1, label %loop, label %outloop

loop:                                             ; preds = %Cond
  %x4 = load i32, i32* %x
  %x5 = load i32, i32* %x
  %2 = add i32 %x5, 1
  store i32 %2, i32* %x
  %i6 = load i32, i32* %i
  %i7 = load i32, i32* %i
  %3 = add i32 %i7, 1
  store i32 %3, i32* %i
  br label %Cond

outloop:                                          ; preds = %Cond
  %x8 = load i32, i32* %x
  ret i32 %x8
}
