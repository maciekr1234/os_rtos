



#if defined(__GNUC__)
#    define PREFIX(x)    __wrap_##x
#else
#warning "This compiler is not yet supported."
#endif

