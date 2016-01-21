; ModuleID = 'Simple C'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

%acrossthegreatxxxxwallwecanreacheverycornerintheworld = type { i32 }
%haoehaoehaoewozhendehaoe = type { i32, i32, i32, i32 }
%doubleO = type { i32, i32, i32, i32, i32 }

@.str = private constant [4 x i8] c"%d\0A\00"
@.str.1 = private constant [3 x i8] c"%d\00"
@A = global %acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer
@B = global %acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer
@C = global %haoehaoehaoewozhendehaoe zeroinitializer
@R = global %doubleO zeroinitializer
@G = global %doubleO zeroinitializer
@T = global %doubleO zeroinitializer

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
  call void @read(i32* getelementptr inbounds (%doubleO, %doubleO* @T, i64 0, i32 0))
  call void @read(i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i64 0, i32 0))
  call void @read(i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i64 0, i32 0))
  %0 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @T, i64 0, i32 0), align 16
  %1 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i64 0, i32 0), align 8
  %2 = xor i32 %1, %0
  store i32 %2, i32* getelementptr inbounds (%doubleO, %doubleO* @G, i64 0, i32 0), align 16
  %3 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i64 0, i32 0), align 8
  %4 = icmp eq i32 %2, %3
  %. = select i1 %4, i32 215, i32 0
  store i32 %., i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i64 0, i32 0), align 8
  call void @write(i32 %.)
  call void @read(i32* getelementptr inbounds (%doubleO, %doubleO* @R, i64 0, i32 0))
  %5 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @R, i64 0, i32 0), align 16
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %else.2, label %then1

then1:                                            ; preds = %entry
  call void @write(i32 215)
  br label %ifcont.3

else.2:                                           ; preds = %entry
  call void @write(i32 -215)
  br label %ifcont.3

ifcont.3:                                         ; preds = %else.2, %then1
  ret i32 0
}
