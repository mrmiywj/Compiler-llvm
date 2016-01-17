; ModuleID = 'Simple C'

declare i32 @printf(i32, ...)

define i32 @main() {
entry:
  %i = alloca i32
  store i32 10, i32* %i
  %i1 = load i32, i32* %i
  %0 = srem i32 %i1, 2
  %1 = icmp eq i32 %0, 0
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  %i2 = load i32, i32* %i
  %calltmp = call i32 (i32, ...) @printf(i32 %i2)
  br label %ifcont.24

else:                                             ; preds = %entry
  %i3 = load i32, i32* %i
  %i4 = load i32, i32* %i
  %2 = sub i32 %i4, 1
  %3 = sdiv i32 %2, 2
  store i32 %3, i32* %i
  %i5 = load i32, i32* %i
  %4 = srem i32 %i5, 2
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %then6, label %else.9

then6:                                            ; preds = %else
  %i7 = load i32, i32* %i
  %calltmp8 = call i32 (i32, ...) @printf(i32 %i7)
  br label %ifcont.23

else.9:                                           ; preds = %else
  %i10 = load i32, i32* %i
  %i11 = load i32, i32* %i
  %6 = sub i32 %i11, 1
  %7 = sdiv i32 %6, 2
  store i32 %7, i32* %i
  %i12 = load i32, i32* %i
  %8 = srem i32 %i12, 2
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %then13, label %else.16

then13:                                           ; preds = %else.9
  %i14 = load i32, i32* %i
  %calltmp15 = call i32 (i32, ...) @printf(i32 %i14)
  br label %ifcont.22

else.16:                                          ; preds = %else.9
  %i17 = load i32, i32* %i
  %10 = srem i32 %i17, 2
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %then18, label %else.21

then18:                                           ; preds = %else.16
  %i19 = load i32, i32* %i
  %calltmp20 = call i32 (i32, ...) @printf(i32 %i19)
  br label %ifcont

else.21:                                          ; preds = %else.16
  br label %ifcont

ifcont:                                           ; preds = %else.21, %then18
  br label %ifcont.22

ifcont.22:                                        ; preds = %ifcont, %then13
  br label %ifcont.23

ifcont.23:                                        ; preds = %ifcont.22, %then6
  br label %ifcont.24

ifcont.24:                                        ; preds = %ifcont.23, %then
  ret i32 0
}
