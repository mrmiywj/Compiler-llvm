; ModuleID = 'Simple C'

@x = global i32 1
@y = external global i32
@z = global i32 2
@arr = global [2 x i32] [i32 1, i32 2]

declare double @MABI()

define i32 @main() {
entry:
  %0 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @arr, i32 0, i32 0)
  %1 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @arr, i32 0, i32 1)
  %2 = add i32 %1, 3
  store i32 %2, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @arr, i32 0, i32 0)
  %3 = load i32, i32* getelementptr inbounds ([2 x i32], [2 x i32]* @arr, i32 0, i32 0)
  ret i32 %3
}