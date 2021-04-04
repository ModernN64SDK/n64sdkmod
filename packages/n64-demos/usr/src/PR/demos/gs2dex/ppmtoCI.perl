#! /usr/sbin/perl
#---------------------------------------------------------------------
#	Copyright (C) 1997, Nintendo.
#	
#	File		ppmtoCI.perl
#	Coded    by	Yoshitaka Yasumoto.	Apr  8, 1997.
#	Modified by	
#	Comments	
#	
#	$Id: ppmtoCI.perl,v 1.3 1997/08/07 03:24:18 yasu Exp $
#---------------------------------------------------------------------
#
#  [How to use]
#	perl ppmtoCI.perl width height txtrlabel [tlutoffset] [xlucolor]
#
#       width:      Width output
#       height:     Height output
#	txtrlabel:  Texture label
#	tlutoffset: Use values after tlutoffset as Index # values.  
#		      Default is 1.
#	xlucolor:   Translucent color.  When translucent color is 
#                 specified, 0 is set for the Index value of the   
#		      pixel that is the same color as the translucent color.  
#		      Please specify #rrggbb for translucent color.
#

$argWidth    = $ARGV[0];
$argHeight   = $ARGV[1];
$argLavel    = $ARGV[2];
$argTLUTofs  = $ARGV[3];
$argXLUcolor = $ARGV[4];

# Analyze the ppm file header
$i = 0;
while (<STDIN>){
	s/#.*$//;	# Process comments
	s/^\s+//;	# Remove spaces
	while (s/^\S+//){
		$param[$i ++] = $&;	# Save parameters
		s/^\s+//;		# Remove spaces
	}	
	last if ($i == 4);
}
die "This is not P6 ppm format!!\n" if ($param[0] ne "P6");

$width  = $param[1];
$height = $param[2];
$pixel  = $param[1] * $param[2];
$size   = $pixel * 3;

# Get the TULT offset value
if ($argTLUTofs ne ""){
    $offset = $argTLUTofs;
    $offset = hex($offset) if $offset =~ /^0[xX]/;
} else {
    $offset = 1;
}

# Get the translucent color
if ($argXLUcolor ne ""){
    if ($argXLUcolor =~ /^#([0-9a-fA-F]{6})/){
	$_ = hex($1);
        $xlucolor = &RGB32to16(($_>>16)&0xff, ($_>>8)&0xff, $_&0xff, 1);
    } else {
	die "Illegal XLU color format. must be \"#rrggbb\".\n";
    }
} else {
    $xlucolor = 0xffffffff;
}

# Output height and width
printf( "/*==========================*\n".
	"    Name  : $argLavel\n".
	"    Width : %d\n".
	"    Height: %d\n".
	" *==========================*/\n", $argWidth, $argHeight);

# Output the texture data
printf("unsigned char %s[] = {\n", $argLavel);

# If size is large, reserve palette to make the frame black.  
$c = $offset;
$col = 0;
if ($width < $argWidth || $height < $argHeight){
    $tlut[$c] = 0x0001;
    $pal{0x0001} = $c;
    $c ++;
}

$x0 = ($width  - $argWidth ) / 2;
$y0 = ($height - $argHeight) / 2;

# Convert to palette and output index
# Fix palette 0 to translucent color
for ($y = $y0; $y < $y0 + $argHeight; $y ++){
    for ($x = $x0; $x < $x0 + $argWidth; $x ++){
	# Judge whether out of the range
	if ($y < 0 || $y >= $height || $x < 0 || $x >= $width){
	    # Output black 
	    $index = $offset;
	} else {
	    # Get 1 Pixel of data
	    read(STDIN, $buf, 3) == 3 || die "Too short file\n";
	    @tmp = unpack("C3", $buf);
	    
	    # Get the color value
	    $color = &RGB32to16($tmp[0], $tmp[1], $tmp[2], 1);
	    
	    # Judge the translucent color
	    if ($color == $xlucolor){
		$index = 0;
	    } else {
		# Compare color value with palette entry
		$index = $pal{$color};
		if ($index == 0){
		    # Register in palette
		    $index = $pal{$color} = $c;
		    $tlut[$c] = $color;
		    $c ++;
		}
	    }
	}
	# Output
	printf("    ") if $col == 0;    # Indent if head column  
	printf("0x%02x,", $index);
	if (($col % 4) == 3){
	    printf(" ");
	}
	if ($col ++ == 11){
	    printf("\n");
	    $col = 0;
	}
    }
}
# Align the last line
if ($col != 0){
    printf("\n");
}
printf("};\n\n");

# Output the palette range
printf( "/*==========================*\n".
	"    Name      : $argLavel_pal\n".
	"    Index from: 0x%02x (%d)\n".
	"    Index to  : 0x%02x (%d)\n".
	" *==========================*/\n", $offset, $offset, $c-1, $c-1);

# Output the palette data
printf("unsigned short %s_pal[] = {", $argLavel);
$col = 0;
for ($i = $offset; $i < $c; $i ++){
    if (($col % 8) == 0){
	printf("\n    ");
    }
    printf("0x%04x, ", $tlut[$i]);
    $col ++;
}
if ($col != 0){
    printf("\n");
}
printf("};\n\n");


#
# Routine for converting from RGB 32bit to RGB 16bit
#
sub RGB32to16 {
   local($r, $g, $b, $a) = @_;
   $r = ($r & 0xf8) << 8;
   $g = ($g & 0xf8) << 3;
   $b = ($b & 0xf8) >> 2;
   $_ = $r | $g | $b | $a;
}

#======== End of ppmtoCI.perl ========

