; ModuleID = 'Simple C'

@z = global i32 1

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
  %j = alloca i32
  %x = alloca i32
  store i32 7, i32* %x
  %x1 = load i32, i32* %x
  %0 = add i32 %x1, 5
  store i32 %0, i32* %x
  %i2 = load i32, i32* %i
  store i32 1, i32* %i
  br label %Cond

Cond:                                             ; preds = %step.15, %entry
  %i3 = load i32, i32* %i
  %1 = icmp slt i32 %i3, 10
  br i1 %1, label %loop, label %outloop.16

loop:                                             ; preds = %Cond
  %j4 = load i32, i32* %j
  store i32 1, i32* %j
  br label %Cond5

Cond5:                                            ; preds = %step, %loop
  %j6 = load i32, i32* %j
  %2 = icmp slt i32 %j6, 10
  br i1 %2, label %loop7, label %outloop

loop7:                                            ; preds = %Cond5
  %i8 = load i32, i32* %i
  %3 = srem i32 %i8, 2
  %4 = icmp eq i32 %3, 0
  br i1 %4, label %then, label %else

then:                                             ; preds = %loop7
  %x9 = load i32, i32* %x
  %5 = add i32 %x9, 1
  store i32 %5, i32* %x
  br label %ifcont

else:                                             ; preds = %loop7
  %x10 = load i32, i32* %x
  %6 = add i32 %x10, 2
  store i32 %6, i32* %x
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %step

step:                                             ; preds = %ifcont
  %j.11 = load i32, i32* %j
  %j.12 = load i32, i32* %j
  %7 = add i32 %j.12, 1
  store i32 %7, i32* %j
  br label %Cond5

outloop:                                          ; preds = %Cond5
  br label %step.15

step.15:                                          ; preds = %outloop
  %i.13 = load i32, i32* %i
  %i.14 = load i32, i32* %i
  %8 = add i32 %i.14, 1
  store i32 %8, i32* %i
  br label %Cond

outloop.16:                                       ; preds = %Cond
  %x17 = load i32, i32* %x
  ret i32 %x17
}
