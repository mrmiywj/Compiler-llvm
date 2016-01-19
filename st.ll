; ModuleID = 'st.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.double0 = type { i32, i32, i32, i32, i32 }
%struct.acrossthegreatxxxxwallwecanreacheverycornerintheworld = type { i32 }
%struct.haoehaoehaoewozhendehaoe = type { i32, i32, i32, i32 }

@G = common global %struct.double0 zeroinitializer, align 4
@A = common global %struct.acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer, align 4
@B = common global %struct.acrossthegreatxxxxwallwecanreacheverycornerintheworld zeroinitializer, align 4
@C = common global %struct.haoehaoehaoewozhendehaoe zeroinitializer, align 4
@R = common global %struct.double0 zeroinitializer, align 4
@T = common global %struct.double0 zeroinitializer, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %Q = alloca %struct.double0, align 4
  store i32 0, i32* %1
  %2 = getelementptr inbounds %struct.double0, %struct.double0* %Q, i32 0, i32 4
  store i32 1, i32* %2, align 4
  %3 = getelementptr inbounds %struct.double0, %struct.double0* %Q, i32 0, i32 4
  %4 = load i32, i32* %3, align 4
  store i32 %4, i32* getelementptr inbounds (%struct.double0, %struct.double0* @G, i32 0, i32 4), align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}

