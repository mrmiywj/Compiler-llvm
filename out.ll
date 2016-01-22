; ModuleID = 'Simple C'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

%A = type { i32, i32 }
%B = type { i32, i32 }

@.str = private constant [4 x i8] c"%d\0A\00"
@.str.1 = private constant [3 x i8] c"%d\00"

declare i32 @printf(i8*, ...)

define internal void @write(i32 %toPrint) {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %toPrint)
  ret void
}

declare i32 @scanf(i8*, ...)

define internal void @read(i32*) {
entry:
  %1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* %0)
  ret void
}

define i32 @main() {
entry:
  %x = alloca %A
  %y = alloca %B
  %0 = getelementptr inbounds %A, %A* %x, i32 0, i32 0
  store i32 1, i32* %0
  %1 = getelementptr inbounds %B, %B* %y, i32 0, i32 0
  store i32 2, i32* %1
  %2 = getelementptr inbounds %A, %A* %x, i32 0, i32 0
  %3 = load i32, i32* %2
  call void @write(i32 %3)
  %4 = getelementptr inbounds %A, %A* %x, i32 0, i32 0
  %5 = load i32, i32* %4
  %6 = getelementptr inbounds %B, %B* %y, i32 0, i32 1
  store i32 %5, i32* %6
  ret i32 0
}
