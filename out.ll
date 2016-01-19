; ModuleID = 'Simple C'

%acrossthegreatxxxxwallwecanreacheverycornerintheworld = type { i32 }
%haoehaoehaoewozhendehaoe = type { i32, i32, i32, i32 }
%double0 = type { i32, i32, i32, i32, i32 }

@.str = private constant [4 x i8] c"%d\0A\00"
@.str.1 = private constant [3 x i8] c"%d\00"
@A = global %acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer
@B = global %acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer
@C = global %haoehaoehaoewozhendehaoe zeroinitializer
@R = global %double0 zeroinitializer
@G = global %double0 zeroinitializer
@T = global %double0 zeroinitializer

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
  %0 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0))
  %1 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @B, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @B, i32 0, i32 0))
  %2 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  call void @read(i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0))
  store i32 -1234567890, i32* getelementptr inbounds (%double0, %double0* @R, i32 0, i32 0)
  %3 = load i32, i32* getelementptr inbounds (%double0, %double0* @R, i32 0, i32 0)
  call void @write(i32 %3)
  %4 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @A, i32 0, i32 0)
  call void @write(i32 %4)
  %5 = load i32, i32* getelementptr inbounds (%acrossthegreatxxxxwallwecanreacheverycornerintheworld, %acrossthegreatxxxxwallwecanreacheverycornerintheworld* @B, i32 0, i32 0)
  call void @write(i32 %5)
  %6 = load i32, i32* getelementptr inbounds (%haoehaoehaoewozhendehaoe, %haoehaoehaoewozhendehaoe* @C, i32 0, i32 0)
  call void @write(i32 %6)
  ret i32 0
}
