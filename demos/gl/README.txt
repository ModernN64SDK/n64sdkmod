------------------------------------------------------------------
                      Partner-N64 gload 'gl'
------------------------------------------------------------------

gload for Partner-N64 and a Partner command execution tool. 

These tools allow control of Partner-N64 from the command line.  
Convenient uses include the ability to use the editor to invoke a 
series of operations, such as 'make -> load & execute' and, with a 
PC, the ability to associate an extension such as .n64 with gl, 
enabling the N64 program to be launched simply by (double) 
clicking in Explorer. 

The program uses the host IO functions of Partner-N64.  For 
information on items such as how to use host IO, see the sample 
hostio/README. 

[Usage]

Always be sure to start Partner-N64 before executing these 
commands.  It can be running while minimized. 

Note:  Even if the Partner-N64 window is minimized, proper 
invocation of the commands may not be possible if the window is 
not first widened somewhat.  (In Partner, the window width limits 
the string length of the commands that can be executed.) 

gl ... gload for Partner-N64

        % gl <ROM filename>

Loads the specified ROM file into Partner-N64 and executes it. 

If no filename is specified, and if there is a current gwrite,  
<gwrite, resetdd, and g are executed.  If there is no gwrite, a 
file named rom or *.n64 is loaded and executed. 

pt ... Partner-N64 command execution tool

        % pt <command>

Executes the specified Partner-N64 command. Multiple commands 
delimited by ';' can be entered at one time.

 Examples  
        Program break: 
        % pt esc
        
        Program re-execution:
        % pt 'reset;g'