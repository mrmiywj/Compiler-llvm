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
  %x = alloca i32, align 4
  %a = alloca [5 x i32], align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 0
  store i32 1, i32* %0, align 4
  %1 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 1
  store i32 1, i32* %1, align 4
  %2 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 2
  store i32 1, i32* %2, align 4
  %3 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 3
  store i32 1, i32* %3, align 4
  %4 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 4
  store i32 1, i32* %4, align 4
  call void @read(i32* nonnull %x)
  %x2 = load i32, i32* %x, align 4
  call void @write(i32 %x2)
  call void @write(i32 8)
  call void @write(i32 21)
  call void @write(i32 1)
  ret i32 0
}
