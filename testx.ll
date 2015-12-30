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
  %x = alloca i32
  store i32 7, i32* %x
  %x1 = load i32, i32* %x
  %0 = add i32 %x1, 5
  store i32 %0, i32* %x
  %x2 = load i32, i32* %x
  %1 = icmp eq i32 %x2, 12
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  ret i32 1
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %x3 = load i32, i32* %x
  ret i32 %x3
}
