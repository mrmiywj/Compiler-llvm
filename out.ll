; ModuleID = 'Simple C'

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
  %0 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @T, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%doubleO, %doubleO* @T, i32 0, i32 0))
  %1 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0))
  %2 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0))
  %3 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @T, i32 0, i32 0)
  %4 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  %5 = xor i32 %3, %4
  store i32 %5, i32* getelementptr inbounds (%doubleO, %doubleO* @G, i32 0, i32 0)
  %6 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @G, i32 0, i32 0)
  %7 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0)
  %8 = icmp eq i32 %6, %7
  br i1 %8, label %then, label %else

then:                                             ; preds = %entry
  store i32 215, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  br label %ifcont

else:                                             ; preds = %entry
  store i32 0, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %9 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  call void @write(i32 %9)
  %10 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @R, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%doubleO, %doubleO* @R, i32 0, i32 0))
  %11 = load i32, i32* getelementptr inbounds (%doubleO, %doubleO* @R, i32 0, i32 0)
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %then1, label %else.2

then1:                                            ; preds = %ifcont
  call void @write(i32 215)
  br label %ifcont.3

else.2:                                           ; preds = %ifcont
  call void @write(i32 -215)
  br label %ifcont.3

ifcont.3:                                         ; preds = %else.2, %then1
  ret i32 0
}
