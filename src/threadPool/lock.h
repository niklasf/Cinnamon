/*
 *******************************************************************************
 *                                                                             *
 *  Crafty, copyright 1996-2011 by Robert M. Hyatt, Ph.D., Associate Professor *
 *  of Computer and Information Sciences, University of Alabama at Birmingham. *
 *                                                                             *
 *  Crafty is a team project consisting of the following members.  These are   *
 *  the people involved in the continuing development of this program, there   *
 *  are no particular members responsible for any specific aspect of Crafty,   *
 *  although R. Hyatt wrote 99%+ of the existing code, excepting the Magic .   *
 *  move stuff by Pradu Kanan, egtb.cpp written by Eugene Nalimov, and the epd *
 *  stuff written by S. Edwards.                                               *
 *                                                                             *
 *     Robert Hyatt, University of Alabama at Birmingham.                      *
 *     Tracy Riegle, Hershey, PA.                                              *
 *     Peter Skinner, Edmonton, AB  Canada.                                    *
 *                                                                             *
 *  All rights reserved.  No part of this program may be reproduced in any     *
 *  form or by any means, for other than your personal use, without the        *
 *  express written permission of the authors.  This program may not be used   *
 *  in whole, nor in part, to enter any computer chess competition without     *
 *  written permission from the authors.  Such permission will include the     *
 *  requirement that the program be entered under the name "Crafty" so that    *
 *  the program's ancestry will be known.                                      *
 *                                                                             *
 *  Copies of the source must contain the original copyright notice intact.    *
 *                                                                             *
 *  Any changes made to this software must also be made public to comply with  *
 *  the original intent of this software distribution project.  These          *
 *  restrictions apply whether the distribution is being done for free or as   *
 *  part or all of a commercial product.  The authors retain sole ownership    *
 *  and copyright on this program except for 'personal use' explained below.   *
 *                                                                             *
 *  Personal use includes any use you make of the program yourself, either by  *
 *  playing games with it yourself, or allowing others to play it on your      *
 *  machine,  and requires that if others use the program, it must be clearly  *
 *  identified as "Crafty" to anyone playing it (on a chess server as one      *
 *  example).  Personal use does not allow anyone to enter this into a chess   *
 *  tournament where other program authors are invited to participate.  IE you *
 *  can do your own local tournament, with Crafty + other programs, since this *
 *  is for your personal enjoyment.  But you may not enter Crafty into an      *
 *  event where it will be in competition with other programs/programmers      *
 *  without permission as stated previously.                                   *
 *                                                                             *
 *  Crafty is the "son" (direct descendent) of Cray Blitz.  it is designed     *
 *  totally around the bit-board data structure for reasons of speed of ex-    *
 *  ecution, ease of adding new knowledge, and a *significantly* cleaner       *
 *  overall design.  it is written totally in ANSI C with some few UNIX system */

#if defined(_WIN32)
/*
 *******************************************************************************
 *                                                                             *
 *  this is a Microsoft windows-based operating system.                        *
 *                                                                             *
 *******************************************************************************
 */
#    include <windows.h>
#    define pthread_attr_t  HANDLE
#    define pthread_t       HANDLE
#    define thread_t        HANDLE
extern pthread_t NumaStartThread(void *func, void *args);

#    include <windows.h>
#    pragma intrinsic (_InterlockedExchange)
typedef volatile LONG lock_t[1];

#    define LockInit(v)      ((v)[0] = 0)
#    define LockFree(v)      ((v)[0] = 0)
#    define Unlock(v)        ((v)[0] = 0)
__forceinline void Lock(volatile LONG * hPtr) {
  int iValue;

  for (;;) {
    iValue = _InterlockedExchange((LPLONG) hPtr, 1);
    if (0 == iValue)
      return;
    while (*hPtr);
  }
}
void Pause() {
}
#  else
/*
 *******************************************************************************
 *                                                                             *
 *  this is a Unix-based operating system.  define the spinlock code as needed *
 *  for SMP synchronization.                                                   *
 *                                                                             *
 *******************************************************************************
 */
static void __inline__ LockX86(volatile int *lock) {
  int dummy;
  asm __volatile__(
      "1:          movl    $1, %0"   "\n\t"
      "            xchgl   (%1), %0" "\n\t"
      "            testl   %0, %0"   "\n\t"
      "            jz      3f"       "\n\t"
      "2:          pause"            "\n\t"
      "            movl    (%1), %0" "\n\t"
      "            testl   %0, %0"   "\n\t"
      "            jnz     2b"       "\n\t"
      "            jmp     1b"       "\n\t"
      "3:"                           "\n\t"
      :"=&q"(dummy)
      :"q"(lock)
      :"cc", "memory");
}
static void __inline__ Pause() {
  asm __volatile__(
      "            pause"            "\n\t");
}
static void __inline__ UnlockX86(volatile int *lock) {
  asm __volatile__(
      "movl    $0, (%0)"
      :
      :"q"(lock)
      :"memory");
}

#    define LockInit(p)           (p=0)
#    define LockFree(p)           (p=0)
#    define Unlock(p)             (UnlockX86(&p))
#    define Lock(p)               (LockX86(&p))
#    define lock_t                volatile int
#  endif
