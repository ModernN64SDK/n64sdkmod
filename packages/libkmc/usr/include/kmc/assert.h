#ifdef NDEBUG
#define assert(ignore)
#else
#define assert(test)  ((test) ? 0:(printf("Failed assertion %s:%d %s\n", __FILE__, __LINE__),abort()))
#endif
