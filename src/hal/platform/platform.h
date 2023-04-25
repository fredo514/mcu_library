// use preprocessor defines to include the correct platform hal file

TICKS_t getSysTicks(void) 

{ 

  tReg int_state; 

  tTime ticks; 

     

  // disable timer interrupt and save state 

  Int_state = disable_interrupts(); 

   

  // get ticks 

  ticks = sysTickTimerCnt_get(); 

     

  // restore timer interrupt 

  Restore_interrupts(int_state); 

   

  return ticks; 

} 

// for each platform, define:
// processor data bus size
// endianess
// aligned access (for casting)