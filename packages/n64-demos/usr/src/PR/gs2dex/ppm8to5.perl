#! /usr/sbin/perl
#---------------------------------------------------------------------
#	Copyright (C) 1997, Nintendo.
#	
#	File		ppm8to5.perl
#	Coded    by	Yoshitaka Yasumoto.	Apr 10, 1997.
#	Modified by	
#	Comments	
#	
#	$Id: ppm8to5.perl,v 1.2 1997/08/07 03:24:17 yasu Exp $
#---------------------------------------------------------------------
#
#  [How to use]
#	perl ppm8to5.perl < infile > outfile
#
#	Round-off 8 bit RGB to 5 bit RGB.
#

# Analyze the ppm file header
$i = 0;
while (<STDIN>){
	s/#.*$//;	# Comment process
	s/^\s+//;	# Removes spaces
	while (s/^\S+//){
		$param[$i ++] = $&;	# Saves parameters
		s/^\s+//;		# Removes spaces
	}	
	last if ($i == 4);
}
die "This is not P6 ppm format!!\n" if ($param[0] ne "P6");

$width  = $param[1];
$height = $param[2];
$pixel  = $param[1] * $param[2];

printf("P6\n%d %d\n255\n", $width, $height);

for ($i = 0; $i < $pixel; $i ++){

    # Get the data for 1 Pixel
    read(STDIN, $buf, 3) == 3 || die "Too short file\n";
    @tmp = unpack("C3", $buf);
    
    # Get the color value
    $tmp[0] &= 0xf8;
    $tmp[1] &= 0xf8;
    $tmp[2] &= 0xf8;

    # Convert to binary
    $buf = pack("C3", @tmp);    
	    
    # Output
    print $buf;
}

#======== End of ppm8to5.perl ========

