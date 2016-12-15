
char const * build_date() {
    return _DATECOMPILED_;
}

char const * build_time() {
    return _TIMECOMPILED_;
}

const char * build_commit() {
#ifdef _CURRENT_HEAD_
    return _CURRENT_HEAD_;
#else
    return "unknown";
#endif
}
