/*
 * acx.h
 *
 * Created: 3/26/2019
 * Author: Andrew Thorp thorpah
 * Author: Eli McGalliard mcgalliarder
 * 
 *       **************************************************
 *       **************************************************
 *       **     _   _   _                _______   __    **
 *       **    | | (_) | |         /\   / ____\ \ / /    **
 *       **    | |  _  | |__      /  \ | |     \ V /     **
 *       **    | | | | | '_ \    / /\ \| |      > <      **
 *       **    | | | | | |_) |  / ____ \ |____ / . \     **
 *       **    |_| |_| |_.__/  /_/    \_\_____/_/ \_\    **
 *       **                                              **
 *       **                                 Andrew Thorp **
 *       **                               Eli McGalliard **
 *       **************************************************
 *       **************************************************
 *
 */

#ifndef ACX_H_
#define ACX_H_



// C and Assembly definitions

#define MAX_DELAY 65535 //1024

// includes max number of threads and number used
#define CANARY 0xAA;	// If this is changed, change it in the canary check in X_yield as well

#define MAXTHREADS 8
#define THREADSUSED 8

// define stack sizes for each thread
#define	T0_STACK_SIZE 256
#define	T1_STACK_SIZE 256
#define	T2_STACK_SIZE 256
#define	T3_STACK_SIZE 256
#define	T4_STACK_SIZE 256
#define	T5_STACK_SIZE 256
#define	T6_STACK_SIZE 256
#define	T7_STACK_SIZE 256

// define total stack memory size using thread stack sizes
#define	STACK_MEM_SIZE ( T0_STACK_SIZE \
                       + T1_STACK_SIZE \
                       + T2_STACK_SIZE \
                       + T3_STACK_SIZE \
                       + T4_STACK_SIZE \
                       + T5_STACK_SIZE \
                       + T6_STACK_SIZE \
                       + T7_STACK_SIZE )

// define offsets to each thread's stack base
#define	T0_STACK_BASE_OFFS (T0_STACK_SIZE - 1)
#define	T1_STACK_BASE_OFFS (T0_STACK_BASE_OFFS + T1_STACK_SIZE)
#define	T2_STACK_BASE_OFFS (T1_STACK_BASE_OFFS + T2_STACK_SIZE)
#define	T3_STACK_BASE_OFFS (T2_STACK_BASE_OFFS + T3_STACK_SIZE)
#define	T4_STACK_BASE_OFFS (T3_STACK_BASE_OFFS + T4_STACK_SIZE)
#define	T5_STACK_BASE_OFFS (T4_STACK_BASE_OFFS + T5_STACK_SIZE)
#define	T6_STACK_BASE_OFFS (T5_STACK_BASE_OFFS + T6_STACK_SIZE)
#define	T7_STACK_BASE_OFFS (T6_STACK_BASE_OFFS + T7_STACK_SIZE)

#define	T0_CANARY_OFFS 0
#define	T1_CANARY_OFFS (T0_CANARY_OFFS + T0_STACK_SIZE)
#define	T2_CANARY_OFFS (T1_CANARY_OFFS + T1_STACK_SIZE)
#define	T3_CANARY_OFFS (T2_CANARY_OFFS + T2_STACK_SIZE)
#define	T4_CANARY_OFFS (T3_CANARY_OFFS + T3_STACK_SIZE)
#define	T5_CANARY_OFFS (T4_CANARY_OFFS + T4_STACK_SIZE)
#define	T6_CANARY_OFFS (T5_CANARY_OFFS + T5_STACK_SIZE)
#define	T7_CANARY_OFFS (T6_CANARY_OFFS + T6_STACK_SIZE)

// define thread IDs for each thread
#define	T0_ID 0
#define	T1_ID 1
#define	T2_ID 2
#define	T3_ID 3
#define	T4_ID 4
#define	T5_ID 5
#define	T6_ID 6
#define	T7_ID 7

// define thread context size 
#define THREAD_CONTEXT_SIZE  18 

#ifndef __ASSEMBLER__    // The following only apply to C files...
#ifndef byte
typedef uint8_t byte;
#endif



// macro to access the current thread id
#define getTID() x_thread_id

//macro to return value of 4-byte tick counter
#define getTime() 

//---------------------------------------------------------------------------
// PTHREAD is a type that represents how threads are called--
// It is just a pointer to a function returning void
// that is a passed an int and a char * as parameters.
//---------------------------------------------------------------------------

typedef void (*PTHREAD)(void);

//---------------------------------------------------------------------------
// This union is used to provide access to individual bytes of a thread address
//---------------------------------------------------------------------------

typedef union {
 	PTHREAD pthread;
 	byte addr[3];
} PTUnion;

//---------------------------------------------------------------------------
// This type is used for entries in the stack control table
//---------------------------------------------------------------------------
typedef struct ctrl {
    int sp;
    int spBase;
} stackControl;


//----------------------------------------------------------------------------
// ACX Function prototypes
//----------------------------------------------------------------------------
void	        x_init(void);
void	        x_delay(unsigned int);
unsigned long   x_gtime();
void	        x_schedule(void);
void            x_new(uint8_t, PTHREAD , bool);
void            x_yield(void);
uint8_t         bit2mask8(uint8_t);
void            x_suspend(uint8_t);
void            x_resume(uint8_t);
void            x_disable(uint8_t);
void            x_enable(uint8_t);
void            kernalInit(void);
void            placeCanaries(void);
void			x_stack_overflow(void);
void			setTimer(void);

#endif


#endif /* ACX_H_ */

