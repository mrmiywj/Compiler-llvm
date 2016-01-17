; ModuleID = 'Simple C'

@a = common global [2 x i32] zeroinitializer

declare i32 @printf(i32, ...)

define i32 @main() {
entry:
  store i32 3, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @a, i32 0, i32 1)
  %0 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @a, i32 0, i32 1)
  ret i32 %0
}
