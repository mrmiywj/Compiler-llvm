; ModuleID = 'Simple C'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@.str = private constant [4 x i8] c"%d\0A\00"
@.str.1 = private constant [3 x i8] c"%d\00"

declare i32 @printf(i8*, ...)

define internal void @write(i32 %toPrint) {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %toPrint)
  ret void
}

declare i32 @scanf(i8*, ...)

define internal void @read(i32*) {
entry:
  %1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0), i32* %0)
  ret void
}

define i32 @main() {
entry:
  call void @write(i32 2)
  ret i32 0
}
