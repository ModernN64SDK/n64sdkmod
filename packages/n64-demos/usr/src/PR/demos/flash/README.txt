------------------------------------------------------------------
                             flash
------------------------------------------------------------------
This program executes tests for 1M Flash ROM. The basic controls 
are as follows:

	Control Pad Up/Down	: Select command
	A Button		: Execute command
	C Button Unit		: Change each parameter

The following are the definition of each command:
	FLASH CHANGE	:	Switches the Flash being used. Can
				specify from 0 to 3.
	READ ID		:	Read Flash ID.
	READ STATUS	:	Read the status of Flash.
	CLEAR STATUS	:	Clear the status of Flash.
	ALL ERASE	:	Deletes all the data of Flash.
	SECTOR ERASE	:	Deletes a specific sector of 
				Flash. The sector to be deleted is
				a sector which contains the page
				specified by page number.
	READ ARRAY(DMA)	:	Reads the specified page of Flash
				with DMA. The page to be read is
				the page length from the page
				specified by page number.
	WRITE BUFFER	:	Writes Flash buffer.
	WRITE ARRAY	:	Writes each page of Flash from
				Flash buffer. The page to be read
				is the page specified by page
				number.
	FILL WRITE DATA	:	Creates the write data pattern of
				a page length (1 PAGE)for the page
				specified by page number. Page
				numbers are inserted in the last 4
				bytes of the data.
	CLEAR ALL	:	Deletes the data in the read
	READ DATA		buffer of the test program.
	CLEAR ALL	:	Deletes the data in the write
				buffer of the test WRITE DATA
				program.
	ALL ERASE &	:	Deletes, creates write data and
	FILL & WRITE		writes the entire area of Flash.
	ALL READ	:	Reads data from the entire area of
				Flash. Data is read per page.
	ALL COMPARE	:	Compares the entire content of
				read buffer and the write buffer
				of the test program.
	REGION COMPARE	:	Compares the content of the
				specified area of the read buffer
				and the write buffer of the test
				program.  The area to be compared
				is the page length from the page
				specified by page number.
	ALL TEST	:	Executes ALL ERASE -> ALL FILL ->
				ALL WRITE -> ALL COMPARE as many
				times as specified.
	PAGE LENGTH	:	Specifies page length.  Other than
				motion experiment, specify 1 in
				principle.
	PAGE NUMBER	:	Specifies page number.
