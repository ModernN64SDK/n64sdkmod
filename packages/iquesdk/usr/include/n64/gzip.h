/*
 * Unzip function which wraps around zlib
 * 
 * Returns -ve if decompress fails, else returns size of output
 */
int expand_gzip(char *in, char *outbuf, unsigned int inSize, unsigned int outbufsize);
