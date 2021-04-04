

This directory contains the NuSYSTEM overlay sample. 
It basically conforms to nu5.


Main changes:

-- stage00.c and stage01.c were changed to perform overlays.

-- shadetri()can be called from anywhere, so it was moved
   to graphics.c

-- The overlay function overlay() was created in main.c

-- The -o option was added to the SGI version to suppress
   makerom errors. 


