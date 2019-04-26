	.file	"test.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
 ;  GNU C17 (GCC) version 8.3.0 (avr)
 ; 	compiled by GNU C version 8.2.1 20181127, GMP version 6.1.2, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.20-GMP

 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  options passed:  test.c -mn-flash=6 -mskip-bug -O1 -fverbose-asm
 ;  options enabled:  -Wmisspelled-isr -faggressive-loop-optimizations
 ;  -fauto-inc-dec -fbranch-count-reg -fchkp-check-incomplete-type
 ;  -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
 ;  -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
 ;  -fchkp-use-static-bounds -fchkp-use-static-const-bounds
 ;  -fchkp-use-wrappers -fcombine-stack-adjustments -fcommon -fcompare-elim
 ;  -fcprop-registers -fdefer-pop -fdwarf2-cfi-asm -fearly-inlining
 ;  -feliminate-unused-debug-types -fforward-propagate
 ;  -ffp-int-builtin-inexact -ffunction-cse -fgcse-lm -fgnu-runtime
 ;  -fgnu-unique -fguess-branch-probability -fident -fif-conversion
 ;  -fif-conversion2 -finline -finline-atomics
 ;  -finline-functions-called-once -fipa-profile -fipa-pure-const
 ;  -fipa-reference -fira-hoist-pressure -fira-share-save-slots
 ;  -fira-share-spill-slots -fivopts -fkeep-static-consts
 ;  -fleading-underscore -flifetime-dse -flto-odr-type-merging -fmath-errno
 ;  -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
 ;  -fomit-frame-pointer -fpeephole -fplt -fprefetch-loop-arrays
 ;  -freg-struct-return -freorder-blocks -fsched-critical-path-heuristic
 ;  -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
 ;  -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
 ;  -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
 ;  -fsemantic-interposition -fshow-column -fshrink-wrap
 ;  -fshrink-wrap-separate -fsigned-zeros -fsplit-ivs-in-unroller
 ;  -fsplit-wide-types -fssa-backprop -fssa-phiopt -fstdarg-opt
 ;  -fstrict-volatile-bitfields -fsync-libcalls -ftoplevel-reorder
 ;  -ftrapping-math -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp
 ;  -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce
 ;  -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
 ;  -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
 ;  -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
 ;  -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr
 ;  -ftree-sra -ftree-ter -funit-at-a-time -fverbose-asm
 ;  -fzero-initialized-in-bss -mgas-isr-prologues -mmain-is-OS_task
 ;  -mskip-bug

	.text
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
 ;  test.c:19: }
	ldi r24,0		 ; 
	lds r25,x_thread_id	 ; , x_thread_id
/* epilogue start */
	ret	
	.size	main, .-main
.global	stackControlTable
	.data
	.type	stackControlTable, @object
	.size	stackControlTable, 32
stackControlTable:
 ;  sp:
	.word	mem+255
 ;  spBase:
	.word	mem+255
 ;  sp:
	.word	mem+511
 ;  spBase:
	.word	mem+511
 ;  sp:
	.word	mem+767
 ;  spBase:
	.word	mem+767
 ;  sp:
	.word	mem+1023
 ;  spBase:
	.word	mem+1023
 ;  sp:
	.word	mem+1279
 ;  spBase:
	.word	mem+1279
 ;  sp:
	.word	mem+1535
 ;  spBase:
	.word	mem+1535
 ;  sp:
	.word	mem+1791
 ;  spBase:
	.word	mem+1791
 ;  sp:
	.word	mem+2047
 ;  spBase:
	.word	mem+2047
	.comm	mem,2048,1
	.comm	x_thread_mask,1,1
	.comm	x_thread_id,1,1
	.comm	delayCounters,2048,1
	.comm	delay,1,1
	.comm	suspend,1,1
	.comm	disable,1,1
	.ident	"GCC: (GNU) 8.3.0"
.global __do_copy_data
.global __do_clear_bss
