------------------------------------------------------------------
NOTE:  This sample program supports the Japanese language.  
Support for this program in the U.S. is not currently available.  
------------------------------------------------------------------
This is a sample program that implements the voice recognition 
process using the voice recognition library function. 

To use the sample program:

Connect the voice recognition system to the Controller port.  When
you speak into the microphone, the recognition result will be 
displayed on the screen. 
	
1) The Screen Display
	
[Voice recognition testing program]

Detecting			<- (1) Voice recognition command 
				       status
UP		734		<- (2) Recognized word and its 
				       distance value
Down		741
Tendon		877
Migi		956
Renkon		997
Speak into the microphone	<- (3) Message prompts the user to
				       speak 
Low voice			<- (4) Display a warning or halt 
				       status


(1) Voice Recognition Command Status
         
Displays the voice recognition command status along with the 
following message. 
		 
Standby -> VOICE_STATUS_READY
Not detected yet -> VOICE_STATUS_START
Detecting -> VOICE_STATUS_BUSY
Cancel -> VOICE_STATUS_CANCEL
Recognition Complete -> VOICE_STATUS_END


(2) Recognized Word and its Distance Value

The recognized word and its distance value are displayed.  The 
first 5 matching words are displayed on the screen.  The program 
displays all of the results (matching words) on the screen, 
regardless of their validity.  Words displayed in white are 
potentially valid words.  Words displayed in yellow are not valid.


(3) Message prompts the user to speak into the microphone
	
If the message appears, please speak into microphone.


(4) Display a Warning or halt

Display a warning or halt status of the voice recognition process.
		
Halt -> Voice recognition system is halted.  
Noisy -> displays VOICE_WARN_TOO_NOISY
No matching words -> displays VOICE_WARN_NOT_FIT
Too large voice -> displays VOICE_WARN_TOO_LARGE
Too small voice -> displays VOICE_WARN_TOO_SMALL


(5) Error Codes
	
If an error is returned, instead of the above messages (1)-(3), 
the error message is displayed on the center of the screen. If an 
error is not returned, one of the above messages (1)-(4) is 
displayed.  The voice recognition process will be halted if any 
error other than "CONT_ERR_NOT_READY"  is returned.  After the 
error correction, to restart the process, connect the Controller 
to an empty port, and press the A button. 


2) Controller Button Functions

Once the Controller is connected, the following functions are 
enabled:
    
A button
Retry the voice recognition process from the initialization stage.  
The voice recognition process will be halted if any error other 
than "CONT_ERR_NOT_READY"  is returned.  After the error 
correction, to restart the process, connect the Controller to an 
empty port, and press the A button. 

B Button
Halt the voice recognition process.  To retry, press the A button.