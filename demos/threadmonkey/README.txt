------------------------------------------------------------------
                          threadmonkey
------------------------------------------------------------------
Sample program for method of using thread profiler

Program executed is exactly the same as blockmonkey. In block.c, 
a function call for the thread profiler and display result 
routine have been built in.  Therefore, after a normal operation 
mode and a key function debug print, the number of operations for 
each thread and the time are displayed.  

In the code added to block the comment,"<== added for profiler", 
has been entered, so refer to the source on how to use it. For 
the actual details on API, please refer to the column in 
osThreadProfile of N64 Online Manuals.
  
