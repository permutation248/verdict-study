/* Machdep-specific info for Frama-C's libc */
#ifndef __FC_MACHDEP
#define __FC_MACHDEP
#define __FC_MACHDEP_X86_64 1
#define __FC_BYTE_ORDER __LITTLE_ENDIAN
#define __INT8_T signed char
#define __UINT8_T unsigned char
#define __INT8_MIN (-127 - 1)
#define __INT8_MAX 127
#define __UINT8_MAX 255U
#define __PRI8_PREFIX "hh"
#define __INT_LEAST8_T signed char
#define __UINT_LEAST8_T unsigned char
#define __INT_LEAST8_MIN (-127 - 1)
#define __INT_LEAST8_MAX 127
#define __UINT_LEAST8_MAX 255U
#define __PRI_LEAST8_PREFIX "hh"
#define __INT16_T signed short
#define __UINT16_T unsigned short
#define __INT16_MIN (-32767 - 1)
#define __INT16_MAX 32767
#define __UINT16_MAX 65535U
#define __PRI16_PREFIX "h"
#define __INT_LEAST16_T signed short
#define __UINT_LEAST16_T unsigned short
#define __INT_LEAST16_MIN (-32767 - 1)
#define __INT_LEAST16_MAX 32767
#define __UINT_LEAST16_MAX 65535U
#define __PRI_LEAST16_PREFIX "h"
#define __INT32_T signed int
#define __UINT32_T unsigned int
#define __INT32_MIN (-2147483647 - 1)
#define __INT32_MAX 2147483647
#define __UINT32_MAX 4294967295U
#define __PRI32_PREFIX ""
#define __INT_LEAST32_T signed int
#define __UINT_LEAST32_T unsigned int
#define __INT_LEAST32_MIN (-2147483647 - 1)
#define __INT_LEAST32_MAX 2147483647
#define __UINT_LEAST32_MAX 4294967295U
#define __PRI_LEAST32_PREFIX ""
#define __INT64_T signed long
#define __UINT64_T unsigned long
#define __INT64_MIN (-9223372036854775807L - 1L)
#define __INT64_MAX 9223372036854775807L
#define __UINT64_MAX 18446744073709551615UL
#define __PRI64_PREFIX "l"
#define __INT_LEAST64_T signed long
#define __UINT_LEAST64_T unsigned long
#define __INT_LEAST64_MIN (-9223372036854775807L - 1L)
#define __INT_LEAST64_MAX 9223372036854775807L
#define __UINT_LEAST64_MAX 18446744073709551615UL
#define __PRI_LEAST64_PREFIX "l"
#define __INT_FAST8_T signed char
#define __INT_FAST8_MIN __FC_SCHAR_MIN
#define __INT_FAST8_MAX __FC_SCHAR_MAX
#define __PRI_FAST8_PREFIX "hh"
#define __INT_FAST16_T long
#define __INT_FAST16_MIN __FC_LONG_MIN
#define __INT_FAST16_MAX __FC_LONG_MAX
#define __PRI_FAST16_PREFIX "l"
#define __INT_FAST32_T long
#define __INT_FAST32_MIN __FC_LONG_MIN
#define __INT_FAST32_MAX __FC_LONG_MAX
#define __PRI_FAST32_PREFIX "l"
#define __INT_FAST64_T long
#define __INT_FAST64_MIN __FC_LONG_MIN
#define __INT_FAST64_MAX __FC_LONG_MAX
#define __PRI_FAST64_PREFIX "l"
#define __UINT_FAST8_T unsigned char
#define __UINT_FAST8_MAX __FC_UCHAR_MAX
#define __UINT_FAST16_T unsigned long
#define __UINT_FAST16_MAX __FC_ULONG_MAX
#define __UINT_FAST32_T unsigned long
#define __UINT_FAST32_MAX __FC_ULONG_MAX
#define __UINT_FAST64_T unsigned long
#define __UINT_FAST64_MAX __FC_ULONG_MAX
#define __INTMAX_T signed long long
#define __UINTMAX_T unsigned long long
#define __INTMAX_MIN (-9223372036854775807LL - 1LL)
#define __INTMAX_MAX 9223372036854775807LL
#define __UINTMAX_MAX 18446744073709551615ULL
#define __PRIMAX_PREFIX "ll"
#define __FC_SCHAR_MIN (-127 - 1)
#define __FC_SCHAR_MAX 127
#define __FC_UCHAR_MAX 255U
#define __FC_SHRT_MIN (-32767 - 1)
#define __FC_SHRT_MAX 32767
#define __FC_USHRT_MAX 65535U
#define __FC_INT_MIN (-2147483647 - 1)
#define __FC_INT_MAX 2147483647
#define __FC_UINT_MAX 4294967295U
#define __FC_LONG_MIN (-9223372036854775807L - 1L)
#define __FC_LONG_MAX 9223372036854775807L
#define __FC_ULONG_MAX 18446744073709551615UL
#define __FC_LLONG_MIN (-9223372036854775807LL - 1LL)
#define __FC_LLONG_MAX 9223372036854775807LL
#define __FC_ULLONG_MAX 18446744073709551615ULL
#define __FC_VA_LIST_T __builtin_va_list
#undef __CHAR_UNSIGNED__
#define __SIZEOF_SHORT 2
#define __SIZEOF_INT 4
#define __SIZEOF_LONG 8
#define __SIZEOF_LONGLONG 8
#define __CHAR_BIT 8
#define INT8_C(c) (c)
#define INT16_C(c) (c)
#define INT32_C(c) (c)
#define INT64_C(c) (c## L)
#define UINT8_C(c) (c## U)
#define UINT16_C(c) (c## U)
#define UINT32_C(c) (c## U)
#define UINT64_C(c) (c## UL)
#define __PTRDIFF_T long
#define __SIZE_T unsigned long
#define __WCHAR_T int
#define __INTPTR_T long
#define __UINTPTR_T unsigned long
#define __PTRDIFF_T long
#define __WINT_T unsigned int
#define __SSIZE_T long
#define __FC_SIZE_MAX __FC_ULONG_MAX
#define __FC_INTPTR_MIN __FC_LONG_MIN
#define __FC_INTPTR_MAX __FC_LONG_MAX
#define __FC_UINTPTR_MAX __FC_ULONG_MAX
#define __FC_WCHAR_MIN __FC_INT_MIN
#define __FC_WCHAR_MAX __FC_INT_MAX
#define __SSIZE_MAX __FC_LONG_MAX
#define __FC_PTRDIFF_MIN __FC_LONG_MIN
#define __FC_PTRDIFF_MAX __FC_LONG_MAX
#define __FC_WINT_MIN 0
#define __FC_WINT_MAX __FC_UINT_MAX
#define __FC_WEOF (0xffffffffu)
#define __PRIPTR_PREFIX "l"
#define __WORDSIZE 64
#define __FC_POSIX_VERSION 200809L
#define __FC_SIG_ATOMIC_T int
#define __FC_SIG_ATOMIC_MIN_MIN __FC_INT_MIN
#define __FC_SIG_ATOMIC_MAX_MAX __FC_INT_MAX
#define __FC_BUFSIZ 8192
#define __FC_EOF (-1)
#define __FC_FOPEN_MAX 16
#define __FC_FILENAME_MAX 4096
#define __FC_L_tmpnam 20
#define __FC_TMP_MAX 238328
#define __FC_RAND_MAX 2147483647
#define __FC_MB_CUR_MAX ((size_t) 16 )
#define __FC_PATH_MAX 4096
#define __FC_HOST_NAME_MAX 64
#define __FC_TTY_NAME_MAX 32
#define __FC_EXFULL 54
#define __FC_EXDEV 18
#define __FC_EWOULDBLOCK 11
#define __FC_EUSERS 87
#define __FC_EUNATCH 49
#define __FC_EUCLEAN 117
#define __FC_ETXTBSY 26
#define __FC_ETIMEDOUT 110
#define __FC_ETIME 62
#define __FC_ESTRPIPE 86
#define __FC_ESTALE 116
#define __FC_ESRCH 3
#define __FC_ESOCKTNOSUPPORT 94
#define __FC_ESPIPE 29
#define __FC_ESHUTDOWN 108
#define __FC_EROFS 30
#define __FC_ERESTART 85
#define __FC_EREMOTEIO 121
#define __FC_EREMOTE 66
#define __FC_EREMCHG 78
#define __FC_EPROTOTYPE 91
#define __FC_EPROTONOSUPPORT 93
#define __FC_EPROTO 71
#define __FC_EPIPE 32
#define __FC_EPFNOSUPPORT 96
#define __FC_EPERM 1
#define __FC_EOWNERDEAD 130
#define __FC_EOVERFLOW 75
#define __FC_EOPNOTSUPP 95
#define __FC_ENXIO 6
#define __FC_ENOTUNIQ 76
#define __FC_ENOTTY 25
#define __FC_ENOTSUP 95
#define __FC_ENOTSOCK 88
#define __FC_ENOTRECOVERABLE 131
#define __FC_ENOTEMPTY 39
#define __FC_ENOTDIR 20
#define __FC_ENOTCONN 107
#define __FC_ENOTBLK 15
#define __FC_ENOSYS 38
#define __FC_ENOSTR 60
#define __FC_ENOSR 63
#define __FC_ENOSPC 28
#define __FC_ENOPROTOOPT 92
#define __FC_ENOPKG 65
#define __FC_ENONET 64
#define __FC_ENOMSG 42
#define __FC_ENOMEM 12
#define __FC_ENOMEDIUM 123
#define __FC_ENOLINK 67
#define __FC_ENOLCK 37
#define __FC_ENOKEY 126
#define __FC_ENOEXEC 8
#define __FC_ENOENT 2
#define __FC_ENODEV 19
#define __FC_ENODATA 61
#define __FC_ENOBUFS 105
#define __FC_ENFILE 23
#define __FC_ENETUNREACH 101
#define __FC_ENETRESET 102
#define __FC_ENETDOWN 100
#define __FC_ENAMETOOLONG 36
#define __FC_EMULTIHOP 72
#define __FC_EMSGSIZE 90
#define __FC_EMLINK 31
#define __FC_EMFILE 24
#define __FC_EMEDIUMTYPE 124
#define __FC_ELOOP 40
#define __FC_ELIBEXEC 83
#define __FC_ELIBSCN 81
#define __FC_ELIBMAX 82
#define __FC_ELIBBAD 80
#define __FC_ELIBACC 79
#define __FC_EL3RST 47
#define __FC_EL3HLT 46
#define __FC_EL2NSYNC 45
#define __FC_EL2HLT 51
#define __FC_EKEYREVOKED 128
#define __FC_EKEYREJECTED 129
#define __FC_EKEYEXPIRED 127
#define __FC_EISNAM 120
#define __FC_EISDIR 21
#define __FC_EISCONN 106
#define __FC_EIO 5
#define __FC_EINVAL 22
#define __FC_EINTR 4
#define __FC_EINPROGRESS 115
#define __FC_EIDRM 43
#define __FC_EHOSTUNREACH 113
#define __FC_EHOSTDOWN 112
#define __FC_EFBIG 27
#define __FC_EFAULT 14
#define __FC_EEXIST 17
#define __FC_EDQUOT 122
#define __FC_EDESTADDRREQ 89
#define __FC_EDEADLOCK 35
#define __FC_EDEADLK 35
#define __FC_ECONNRESET 104
#define __FC_ECONNREFUSED 111
#define __FC_ECONNABORTED 103
#define __FC_ECOMM 70
#define __FC_ECHRNG 44
#define __FC_ECHILD 10
#define __FC_ECANCELED 125
#define __FC_EBUSY 16
#define __FC_EBADSLT 57
#define __FC_EBADRQC 56
#define __FC_EBADR 53
#define __FC_EBADMSG 74
#define __FC_EBADFD 77
#define __FC_EBADF 9
#define __FC_EBADE 52
#define __FC_EALREADY 114
#define __FC_EAGAIN 11
#define __FC_EAFNOSUPPORT 97
#define __FC_EADDRNOTAVAIL 99
#define __FC_EADDRINUSE 98
#define __FC_EACCES 13
#define __FC_E2BIG 7
#define __FC_ERANGE 34
#define __FC_EILSEQ 84
#define __FC_EDOM 33
#define __FC_TIME_T long
#define __FC_NSIG (64 + 1)
#undef __ATOMIC_RELEASE
#define __ATOMIC_RELEASE 3
#undef __ATOMIC_ACQ_REL
#define __ATOMIC_ACQ_REL 4
#undef __UINT_FAST8_TYPE__
#define __UINT_FAST8_TYPE__ unsigned char
#undef __SIZE_MAX__
#define __SIZE_MAX__ 0xffffffffffffffffUL
#undef __UINT_FAST16_MAX__
#define __UINT_FAST16_MAX__ 0xffffffffffffffffUL
#undef __FLT_DECIMAL_DIG__
#define __FLT_DECIMAL_DIG__ 9
#undef __FLT32_HAS_QUIET_NAN__
#define __FLT32_HAS_QUIET_NAN__ 1
#undef __SIZEOF_LONG_LONG__
#define __SIZEOF_LONG_LONG__ 8
#undef __DEC128_MANT_DIG__
#define __DEC128_MANT_DIG__ 34
#undef __DEC64_SUBNORMAL_MIN__
#define __DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD
#undef __UINTPTR_TYPE__
#define __UINTPTR_TYPE__ long unsigned int
#undef __FLT32X_MIN_10_EXP__
#define __FLT32X_MIN_10_EXP__ (-307)
#undef __PTRDIFF_TYPE__
#define __PTRDIFF_TYPE__ long int
#undef __INT16_C
#define __INT16_C(c) c
#undef __DBL_MAX_10_EXP__
#define __DBL_MAX_10_EXP__ 308
#undef __FLT32X_DENORM_MIN__
#define __FLT32X_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F32x
#undef __PIE__
#define __PIE__ 2
#undef __UINTMAX_MAX__
#define __UINTMAX_MAX__ 0xffffffffffffffffUL
#undef __ATOMIC_CONSUME
#define __ATOMIC_CONSUME 1
#undef __FLT32X_HAS_QUIET_NAN__
#define __FLT32X_HAS_QUIET_NAN__ 1
#undef __INTMAX_TYPE__
#define __INTMAX_TYPE__ long int
#undef __INT_LEAST32_WIDTH__
#define __INT_LEAST32_WIDTH__ 32
#undef __FLT64_MIN__
#define __FLT64_MIN__ 2.22507385850720138309023271733240406e-308F64
#undef __FLT_MIN_10_EXP__
#define __FLT_MIN_10_EXP__ (-37)
#undef __SIZEOF_DOUBLE__
#define __SIZEOF_DOUBLE__ 8
#undef __INT32_TYPE__
#define __INT32_TYPE__ int
#undef __SSE2__
#define __SSE2__ 1
#undef __FLT_MAX__
#define __FLT_MAX__ 3.40282346638528859811704183484516925e+38F
#undef __CHAR32_TYPE__
#define __CHAR32_TYPE__ unsigned int
#undef __FLT32X_NORM_MAX__
#define __FLT32X_NORM_MAX__ 1.79769313486231570814527423731704357e+308F32x
#undef __UINT_FAST32_TYPE__
#define __UINT_FAST32_TYPE__ long unsigned int
#undef __INT32_MAX__
#define __INT32_MAX__ 0x7fffffff
#undef __PIC__
#define __PIC__ 2
#undef __DBL_MIN__
#define __DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L)
#undef __LONG_WIDTH__
#define __LONG_WIDTH__ 64
#undef __INT8_MAX__
#define __INT8_MAX__ 0x7f
#undef __FLT_DENORM_MIN__
#define __FLT_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F
#undef __UINT32_C
#define __UINT32_C(c) c ## U
#undef __FLT32_DIG__
#define __FLT32_DIG__ 6
#undef __INTMAX_WIDTH__
#define __INTMAX_WIDTH__ 64
#undef __BYTE_ORDER__
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#undef __DBL_NORM_MAX__
#define __DBL_NORM_MAX__ ((double)1.79769313486231570814527423731704357e+308L)
#undef __FLT32X_MAX__
#define __FLT32X_MAX__ 1.79769313486231570814527423731704357e+308F32x
#undef __FXSR__
#define __FXSR__ 1
#undef __INT_FAST8_WIDTH__
#define __INT_FAST8_WIDTH__ 8
#undef __DBL_DECIMAL_DIG__
#define __DBL_DECIMAL_DIG__ 17
#undef __FLT32_EPSILON__
#define __FLT32_EPSILON__ 1.19209289550781250000000000000000000e-7F32
#undef __FLT64_HAS_DENORM__
#define __FLT64_HAS_DENORM__ 1
#undef __FLT64X_MIN__
#define __FLT64X_MIN__ 3.36210314311209350626267781732175260e-4932F64x
#undef __INT_FAST8_TYPE__
#define __INT_FAST8_TYPE__ signed char
#undef __INTMAX_MAX__
#define __INTMAX_MAX__ 0x7fffffffffffffffL
#undef __FLT64_NORM_MAX__
#define __FLT64_NORM_MAX__ 1.79769313486231570814527423731704357e+308F64
#undef __UINT64_TYPE__
#define __UINT64_TYPE__ long unsigned int
#undef __SIZEOF_WCHAR_T__
#define __SIZEOF_WCHAR_T__ 4
#undef __FLT64_MAX_EXP__
#define __FLT64_MAX_EXP__ 1024
#undef __UINT8_C
#define __UINT8_C(c) c
#undef _LP64
#define _LP64 1
#undef __INT_LEAST32_TYPE__
#define __INT_LEAST32_TYPE__ int
#undef __DBL_EPSILON__
#define __DBL_EPSILON__ ((double)2.22044604925031308084726333618164062e-16L)
#undef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED 0
#undef __FLT64X_IS_IEC_60559__
#define __FLT64X_IS_IEC_60559__ 1
#undef __FLT32X_MAX_10_EXP__
#define __FLT32X_MAX_10_EXP__ 308
#undef __INT_LEAST64_WIDTH__
#define __INT_LEAST64_WIDTH__ 64
#undef __INT_LEAST8_MAX__
#define __INT_LEAST8_MAX__ 0x7f
#undef __WINT_WIDTH__
#define __WINT_WIDTH__ 32
#undef __amd64__
#define __amd64__ 1
#undef __FLT64_MAX__
#define __FLT64_MAX__ 1.79769313486231570814527423731704357e+308F64
#undef __FLT32_NORM_MAX__
#define __FLT32_NORM_MAX__ 3.40282346638528859811704183484516925e+38F32
#undef __SIZEOF_SHORT__
#define __SIZEOF_SHORT__ 2
#undef __UINT_LEAST32_TYPE__
#define __UINT_LEAST32_TYPE__ unsigned int
#undef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#undef __DEC64_MIN__
#define __DEC64_MIN__ 1E-383DD
#undef __SIZEOF_FLOAT128__
#define __SIZEOF_FLOAT128__ 16
#undef __DBL_MANT_DIG__
#define __DBL_MANT_DIG__ 53
#undef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 4321
#undef __FLT_MAX_EXP__
#define __FLT_MAX_EXP__ 128
#undef __INT64_TYPE__
#define __INT64_TYPE__ long int
#undef __linux__
#define __linux__ 1
#undef __INT_MAX__
#define __INT_MAX__ 0x7fffffff
#undef __UINT_FAST64_TYPE__
#define __UINT_FAST64_TYPE__ long unsigned int
#undef __FLT64X_MAX_EXP__
#define __FLT64X_MAX_EXP__ 16384
#undef __FLT_NORM_MAX__
#define __FLT_NORM_MAX__ 3.40282346638528859811704183484516925e+38F
#undef __INT_FAST64_MAX__
#define __INT_FAST64_MAX__ 0x7fffffffffffffffL
#undef __INT_FAST64_WIDTH__
#define __INT_FAST64_WIDTH__ 64
#undef __UINTPTR_MAX__
#define __UINTPTR_MAX__ 0xffffffffffffffffUL
#undef __pic__
#define __pic__ 2
#undef __WCHAR_TYPE__
#define __WCHAR_TYPE__ int
#undef __UINT16_TYPE__
#define __UINT16_TYPE__ short unsigned int
#undef __INTPTR_TYPE__
#define __INTPTR_TYPE__ long int
#undef __k8__
#define __k8__ 1
#undef __DEC32_MANT_DIG__
#define __DEC32_MANT_DIG__ 7
#undef __code_model_small__
#define __code_model_small__ 1
#undef __SIG_ATOMIC_MIN__
#define __SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)
#undef __FLT64X_MAX__
#define __FLT64X_MAX__ 1.18973149535723176502126385303097021e+4932F64x
#undef __FLT64_HAS_INFINITY__
#define __FLT64_HAS_INFINITY__ 1
#undef __INT_FAST16_WIDTH__
#define __INT_FAST16_WIDTH__ 64
#undef __PTRDIFF_WIDTH__
#define __PTRDIFF_WIDTH__ 64
#undef __UINT16_C
#define __UINT16_C(c) c
#undef __FLT32X_DIG__
#define __FLT32X_DIG__ 15
#undef __FLT32_MIN_10_EXP__
#define __FLT32_MIN_10_EXP__ (-37)
#undef __FLT64_HAS_QUIET_NAN__
#define __FLT64_HAS_QUIET_NAN__ 1
#undef linux
#define linux 1
#undef __INTPTR_MAX__
#define __INTPTR_MAX__ 0x7fffffffffffffffL
#undef __INT_FAST8_MAX__
#define __INT_FAST8_MAX__ 0x7f
#undef __DEC32_MAX_EXP__
#define __DEC32_MAX_EXP__ 97
#undef __SIZEOF_INT__
#define __SIZEOF_INT__ 4
#undef __INT_LEAST8_TYPE__
#define __INT_LEAST8_TYPE__ signed char
#undef __INT16_TYPE__
#define __INT16_TYPE__ short int
#undef __INT_LEAST64_TYPE__
#define __INT_LEAST64_TYPE__ long int
#undef __SIG_ATOMIC_WIDTH__
#define __SIG_ATOMIC_WIDTH__ 32
#undef __FLT32_DENORM_MIN__
#define __FLT32_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F32
#undef __DEC64_MANT_DIG__
#define __DEC64_MANT_DIG__ 16
#undef __INT_LEAST16_MAX__
#define __INT_LEAST16_MAX__ 0x7fff
#undef __SEG_FS
#define __SEG_FS 1
#undef __SIZE_WIDTH__
#define __SIZE_WIDTH__ 64
#undef __PRAGMA_REDEFINE_EXTNAME
#define __PRAGMA_REDEFINE_EXTNAME 1
#undef __CHAR16_TYPE__
#define __CHAR16_TYPE__ short unsigned int
#undef __INT_FAST32_WIDTH__
#define __INT_FAST32_WIDTH__ 64
#undef __DEC64_MAX__
#define __DEC64_MAX__ 9.999999999999999E384DD
#undef __UINT_FAST16_TYPE__
#define __UINT_FAST16_TYPE__ long unsigned int
#undef __FLT_HAS_INFINITY__
#define __FLT_HAS_INFINITY__ 1
#undef __DEC128_SUBNORMAL_MIN__
#define __DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL
#undef __FLT64X_HAS_DENORM__
#define __FLT64X_HAS_DENORM__ 1
#undef __LONG_MAX__
#define __LONG_MAX__ 0x7fffffffffffffffL
#undef __FLT_MAX_10_EXP__
#define __FLT_MAX_10_EXP__ 38
#undef __FLT_HAS_QUIET_NAN__
#define __FLT_HAS_QUIET_NAN__ 1
#undef __UINT_LEAST64_TYPE__
#define __UINT_LEAST64_TYPE__ long unsigned int
#undef __UINT_FAST32_MAX__
#define __UINT_FAST32_MAX__ 0xffffffffffffffffUL
#undef __FLT64_DIG__
#define __FLT64_DIG__ 15
#undef __INT_FAST16_MAX__
#define __INT_FAST16_MAX__ 0x7fffffffffffffffL
#undef __DEC32_SUBNORMAL_MIN__
#define __DEC32_SUBNORMAL_MIN__ 0.000001E-95DF
#undef __x86_64__
#define __x86_64__ 1
#undef __FLT64_IS_IEC_60559__
#define __FLT64_IS_IEC_60559__ 1
#undef __SIZEOF_LONG__
#define __SIZEOF_LONG__ 8
#undef __FLT32_HAS_INFINITY__
#define __FLT32_HAS_INFINITY__ 1
#undef __FLT64X_HAS_INFINITY__
#define __FLT64X_HAS_INFINITY__ 1
#undef __SIZEOF_PTRDIFF_T__
#define __SIZEOF_PTRDIFF_T__ 8
#undef __USER_LABEL_PREFIX__
#undef __USER_LABEL_PREFIX__
#undef __SIG_ATOMIC_MAX__
#define __SIG_ATOMIC_MAX__ 0x7fffffff
#undef __FLT32X_MIN__
#define __FLT32X_MIN__ 2.22507385850720138309023271733240406e-308F32x
#undef __k8
#define __k8 1
#undef __SSE_MATH__
#define __SSE_MATH__ 1
#undef __UINTMAX_C
#define __UINTMAX_C(c) c ## UL
#undef __INT_LEAST16_TYPE__
#define __INT_LEAST16_TYPE__ short int
#undef __FLT_RADIX__
#define __FLT_RADIX__ 2
#undef __UINT32_TYPE__
#define __UINT32_TYPE__ unsigned int
#undef __ELF__
#define __ELF__ 1
#undef __INT8_TYPE__
#define __INT8_TYPE__ signed char
#undef __FLT64X_DIG__
#define __FLT64X_DIG__ 18
#undef __FLT_IS_IEC_60559__
#define __FLT_IS_IEC_60559__ 1
#undef __UINT64_MAX__
#define __UINT64_MAX__ 0xffffffffffffffffUL
#undef unix
#define unix 1
#undef __DEC128_MAX_EXP__
#define __DEC128_MAX_EXP__ 6145
#undef __UINT_LEAST16_TYPE__
#define __UINT_LEAST16_TYPE__ short unsigned int
#undef __INT_FAST32_TYPE__
#define __INT_FAST32_TYPE__ long int
#undef __DEC128_MAX__
#define __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
#undef __DEC128_MIN_EXP__
#define __DEC128_MIN_EXP__ (-6142)
#undef __ORDER_PDP_ENDIAN__
#define __ORDER_PDP_ENDIAN__ 3412
#undef __DEC64_EPSILON__
#define __DEC64_EPSILON__ 1E-15DD
#undef __INT32_C
#define __INT32_C(c) c
#undef __FLT32_DECIMAL_DIG__
#define __FLT32_DECIMAL_DIG__ 9
#undef __FLT32X_MIN_EXP__
#define __FLT32X_MIN_EXP__ (-1021)
#undef __FLOAT_WORD_ORDER__
#define __FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__
#undef __FLT32_MANT_DIG__
#define __FLT32_MANT_DIG__ 24
#undef __INT_LEAST32_MAX__
#define __INT_LEAST32_MAX__ 0x7fffffff
#undef _STDC_PREDEF_H
#define _STDC_PREDEF_H 1
#undef __UINT64_C
#define __UINT64_C(c) c ## UL
#undef __VERSION__
#define __VERSION__ "13.2.1 20230801"
#undef __FLT_MANT_DIG__
#define __FLT_MANT_DIG__ 24
#undef __DEC_EVAL_METHOD__
#define __DEC_EVAL_METHOD__ 2
#undef __NO_INLINE__
#define __NO_INLINE__ 1
#undef __FLT_DIG__
#define __FLT_DIG__ 6
#undef __UINT8_TYPE__
#define __UINT8_TYPE__ unsigned char
#undef __FLT32_MIN__
#define __FLT32_MIN__ 1.17549435082228750796873653722224568e-38F32
#undef __DBL_HAS_DENORM__
#define __DBL_HAS_DENORM__ 1
#undef __UINT16_MAX__
#define __UINT16_MAX__ 0xffff
#undef __REGISTER_PREFIX__
#undef __REGISTER_PREFIX__
#undef __DEC128_MIN__
#define __DEC128_MIN__ 1E-6143DL
#undef __FLT64X_DECIMAL_DIG__
#define __FLT64X_DECIMAL_DIG__ 21
#undef __FLT64_MIN_10_EXP__
#define __FLT64_MIN_10_EXP__ (-307)
#undef __FLT64_MIN_EXP__
#define __FLT64_MIN_EXP__ (-1021)
#undef __DECIMAL_BID_FORMAT__
#define __DECIMAL_BID_FORMAT__ 1
#undef __FLT32X_EPSILON__
#define __FLT32X_EPSILON__ 2.22044604925031308084726333618164062e-16F32x
#undef __DBL_HAS_QUIET_NAN__
#define __DBL_HAS_QUIET_NAN__ 1
#undef __LP64__
#define __LP64__ 1
#undef __SIZE_TYPE__
#define __SIZE_TYPE__ long unsigned int
#undef __SIZEOF_POINTER__
#define __SIZEOF_POINTER__ 8
#undef __FLT64X_NORM_MAX__
#define __FLT64X_NORM_MAX__ 1.18973149535723176502126385303097021e+4932F64x
#undef __FLT64_DENORM_MIN__
#define __FLT64_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F64
#undef __INT_FAST64_TYPE__
#define __INT_FAST64_TYPE__ long int
#undef __x86_64
#define __x86_64 1
#undef __INT16_MAX__
#define __INT16_MAX__ 0x7fff
#undef __FLT_MIN_EXP__
#define __FLT_MIN_EXP__ (-125)
#undef __FLT32_MAX_EXP__
#define __FLT32_MAX_EXP__ 128
#undef __LONG_LONG_WIDTH__
#define __LONG_LONG_WIDTH__ 64
#undef __SIZEOF_WINT_T__
#define __SIZEOF_WINT_T__ 4
#undef __FLT64X_MIN_EXP__
#define __FLT64X_MIN_EXP__ (-16381)
#undef __SIZEOF_SIZE_T__
#define __SIZEOF_SIZE_T__ 8
#undef __LONG_LONG_MAX__
#define __LONG_LONG_MAX__ 0x7fffffffffffffffLL
#undef __ATOMIC_HLE_ACQUIRE
#define __ATOMIC_HLE_ACQUIRE 65536
#undef __amd64
#define __amd64 1
#undef __PTRDIFF_MAX__
#define __PTRDIFF_MAX__ 0x7fffffffffffffffL
#undef __ATOMIC_HLE_RELEASE
#define __ATOMIC_HLE_RELEASE 131072
#undef __SSE2_MATH__
#define __SSE2_MATH__ 1
#undef __DEC128_EPSILON__
#define __DEC128_EPSILON__ 1E-33DL
#undef __FLT32_MAX__
#define __FLT32_MAX__ 3.40282346638528859811704183484516925e+38F32
#undef __WCHAR_WIDTH__
#define __WCHAR_WIDTH__ 32
#undef __DBL_MAX_EXP__
#define __DBL_MAX_EXP__ 1024
#undef __DEC32_MIN__
#define __DEC32_MIN__ 1E-95DF
#undef __SEG_GS
#define __SEG_GS 1
#undef __MMX_WITH_SSE__
#define __MMX_WITH_SSE__ 1
#undef __INT_FAST16_TYPE__
#define __INT_FAST16_TYPE__ long int
#undef __FLT32X_HAS_DENORM__
#define __FLT32X_HAS_DENORM__ 1
#undef __UINT_LEAST32_MAX__
#define __UINT_LEAST32_MAX__ 0xffffffffU
#undef __INTPTR_WIDTH__
#define __INTPTR_WIDTH__ 64
#undef __DEC32_MIN_EXP__
#define __DEC32_MIN_EXP__ (-94)
#undef __HAVE_SPECULATION_SAFE_VALUE
#define __HAVE_SPECULATION_SAFE_VALUE 1
#undef __SIZEOF_FLOAT__
#define __SIZEOF_FLOAT__ 4
#undef __INT64_MAX__
#define __INT64_MAX__ 0x7fffffffffffffffL
#undef __DBL_HAS_INFINITY__
#define __DBL_HAS_INFINITY__ 1
#undef __INT_FAST32_MAX__
#define __INT_FAST32_MAX__ 0x7fffffffffffffffL
#undef __DBL_MAX__
#define __DBL_MAX__ ((double)1.79769313486231570814527423731704357e+308L)
#undef __FLT64_MAX_10_EXP__
#define __FLT64_MAX_10_EXP__ 308
#undef __BIGGEST_ALIGNMENT__
#define __BIGGEST_ALIGNMENT__ 16
#undef __SIZEOF_LONG_DOUBLE__
#define __SIZEOF_LONG_DOUBLE__ 16
#undef __FLT_HAS_DENORM__
#define __FLT_HAS_DENORM__ 1
#undef __MMX__
#define __MMX__ 1
#undef __pie__
#define __pie__ 2
#undef __FLT64X_MANT_DIG__
#define __FLT64X_MANT_DIG__ 64
#undef __FLT64_MANT_DIG__
#define __FLT64_MANT_DIG__ 53
#undef __FLT64X_MIN_10_EXP__
#define __FLT64X_MIN_10_EXP__ (-4931)
#undef __FLT64_EPSILON__
#define __FLT64_EPSILON__ 2.22044604925031308084726333618164062e-16F64
#undef __DECIMAL_DIG__
#define __DECIMAL_DIG__ 21
#undef __INT_WIDTH__
#define __INT_WIDTH__ 32
#undef __unix__
#define __unix__ 1
#undef __FLT32X_HAS_INFINITY__
#define __FLT32X_HAS_INFINITY__ 1
#undef __FLT64X_DENORM_MIN__
#define __FLT64X_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951F64x
#undef __DEC32_MAX__
#define __DEC32_MAX__ 9.999999E96DF
#undef __DBL_IS_IEC_60559__
#define __DBL_IS_IEC_60559__ 1
#undef __FLT32_IS_IEC_60559__
#define __FLT32_IS_IEC_60559__ 1
#undef __SHRT_WIDTH__
#define __SHRT_WIDTH__ 16
#undef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209289550781250000000000000000000e-7F
#undef __DBL_DIG__
#define __DBL_DIG__ 15
#undef __DEC64_MIN_EXP__
#define __DEC64_MIN_EXP__ (-382)
#undef __FLT64X_EPSILON__
#define __FLT64X_EPSILON__ 1.08420217248550443400745280086994171e-19F64x
#undef __FLT32X_MANT_DIG__
#define __FLT32X_MANT_DIG__ 53
#undef __INT_LEAST64_MAX__
#define __INT_LEAST64_MAX__ 0x7fffffffffffffffL
#undef __unix
#define __unix 1
#undef __ATOMIC_SEQ_CST
#define __ATOMIC_SEQ_CST 5
#undef __SSP_STRONG__
#define __SSP_STRONG__ 3
#undef __INT64_C
#define __INT64_C(c) c ## L
#undef __WCHAR_MIN__
#define __WCHAR_MIN__ (-__WCHAR_MAX__ - 1)
#undef __SCHAR_MAX__
#define __SCHAR_MAX__ 0x7f
#undef __INT_LEAST16_WIDTH__
#define __INT_LEAST16_WIDTH__ 16
#undef __FLT32X_IS_IEC_60559__
#define __FLT32X_IS_IEC_60559__ 1
#undef __WINT_MIN__
#define __WINT_MIN__ 0U
#undef __DBL_DENORM_MIN__
#define __DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L)
#undef __UINT32_MAX__
#define __UINT32_MAX__ 0xffffffffU
#undef __FLT_EVAL_METHOD_TS_18661_3__
#define __FLT_EVAL_METHOD_TS_18661_3__ 0
#undef __DEC32_EPSILON__
#define __DEC32_EPSILON__ 1E-6DF
#undef __linux
#define __linux 1
#undef __UINTMAX_TYPE__
#define __UINTMAX_TYPE__ long unsigned int
#undef __UINT_LEAST8_MAX__
#define __UINT_LEAST8_MAX__ 0xff
#undef __FLT64X_HAS_QUIET_NAN__
#define __FLT64X_HAS_QUIET_NAN__ 1
#undef __FLT64X_MAX_10_EXP__
#define __FLT64X_MAX_10_EXP__ 4932
#undef __SHRT_MAX__
#define __SHRT_MAX__ 0x7fff
#undef __UINT_LEAST64_MAX__
#define __UINT_LEAST64_MAX__ 0xffffffffffffffffUL
#undef __INT_LEAST8_WIDTH__
#define __INT_LEAST8_WIDTH__ 8
#undef __INT8_C
#define __INT8_C(c) c
#undef __DEC64_MAX_EXP__
#define __DEC64_MAX_EXP__ 385
#undef __FLT32_MAX_10_EXP__
#define __FLT32_MAX_10_EXP__ 38
#undef __UINT_FAST8_MAX__
#define __UINT_FAST8_MAX__ 0xff
#undef __FLT32_HAS_DENORM__
#define __FLT32_HAS_DENORM__ 1
#undef __FLT32X_MAX_EXP__
#define __FLT32X_MAX_EXP__ 1024
#undef __FINITE_MATH_ONLY__
#define __FINITE_MATH_ONLY__ 0
#undef __DBL_MIN_10_EXP__
#define __DBL_MIN_10_EXP__ (-307)
#undef __SIG_ATOMIC_TYPE__
#define __SIG_ATOMIC_TYPE__ int
#undef __UINT_FAST64_MAX__
#define __UINT_FAST64_MAX__ 0xffffffffffffffffUL
#undef __FLT64_DECIMAL_DIG__
#define __FLT64_DECIMAL_DIG__ 17
#undef __FLT_EVAL_METHOD__
#define __FLT_EVAL_METHOD__ 0
#undef __FLT32X_DECIMAL_DIG__
#define __FLT32X_DECIMAL_DIG__ 17
#undef __WCHAR_MAX__
#define __WCHAR_MAX__ 0x7fffffff
#undef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1234
#undef __FLT32_MIN_EXP__
#define __FLT32_MIN_EXP__ (-125)
#undef __WINT_MAX__
#define __WINT_MAX__ 0xffffffffU
#undef __SCHAR_WIDTH__
#define __SCHAR_WIDTH__ 8
#undef __UINT8_MAX__
#define __UINT8_MAX__ 0xff
#undef __CHAR_BIT__
#define __CHAR_BIT__ 8
#undef __INTMAX_C
#define __INTMAX_C(c) c ## L
#undef __SIZEOF_FLOAT80__
#define __SIZEOF_FLOAT80__ 16
#undef __UINT_LEAST8_TYPE__
#define __UINT_LEAST8_TYPE__ unsigned char
#undef __FLT_MIN__
#define __FLT_MIN__ 1.17549435082228750796873653722224568e-38F
#undef __ATOMIC_ACQUIRE
#define __ATOMIC_ACQUIRE 2
#undef __UINT_LEAST16_MAX__
#define __UINT_LEAST16_MAX__ 0xffff
#undef __DBL_MIN_EXP__
#define __DBL_MIN_EXP__ (-1021)
#endif // __FC_MACHDEP