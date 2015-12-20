
# ------------------------------------------------- #

import os
import sys
import getopt
import locale
import pydusa

# ------------------------------------------------- #

# Defines

ERROR    = pydusa.Emulator.ERROR
BREAK    = pydusa.Emulator.BREAK
CONTINUE = pydusa.Emulator.CONTINUE
STOP     = pydusa.Emulator.STOP

R_W = pydusa.MemoryAccess.READ | pydusa.MemoryAccess.WRITE

OUTPUT = ''

# ------------------------------------------------- #

def on_insn(cpu, mem, ad):
    global core

    print '_' * 80
    print cpu
    insn = core.disasm_cur_insn(cpu, mem)
    print core.fmt_cell(ad, insn)

    # d = mem.read(0x00000000bedd38c0, 8)
    # print repr(d)

    return CONTINUE

def on_unk_api(cpu, mem, ad):
    global exe
    print 'unhandled function: %s' % exe.get_hook_name()

    return STOP

def on_libc_start_main(cpu, mem, ad):
    global exe
    # ref: https://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA.junk/baselib---libc-start-main-.html
    main      = exe.get_func_param('system_v', 0)
    argc      = exe.get_func_param('system_v', 1)
    ubp_av    = exe.get_func_param('system_v', 2)
    init      = exe.get_func_param('system_v', 3)
    fini      = exe.get_func_param('system_v', 4)
    rtld_fini = exe.get_func_param('system_v', 5)
    stack_end = exe.get_func_param('system_v', 6)

    print '__libc_start_main(%016x, %d, %016x, %016x, %016x, %016x, %016x)' % (main, argc, ubp_av, init, fini, rtld_fini, stack_end)

    # there's no set_func_param function yet
    cpu.rdi = argc
    cpu.rsi = ubp_av
    cpu.rdx = 0 # environ is nullptr

    cpu.rip   = main

    return BREAK

def on_strrchr(cpu, mem, ad):
    global exe
    # ref: http://www.cplusplus.com/reference/cstring/strrchr/
    s = exe.get_func_param('system_v', 0)
    c = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    s_str = mem.read_utf8(s)

    off = s_str.rfind(chr(c))
    res = 0
    if off != -1:
        res = s + off

    res_s = mem.read_utf8(res)

    print 'strrchr(%016x["%s"], \'%c\') = %s' % (s, s_str, c, res_s)

    cpu.rax = res
    return BREAK

def on_setlocale(cpu, mem, ad):
    global exe
    # ref: http://www.cplusplus.com/reference/clocale/setlocale/
    category = exe.get_func_param('system_v', 0)
    locale   = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    locale_str = mem.read_utf8(locale)

    print 'locale(%d, %016x["%s"])' % (category, locale, locale_str)

    cpu.rax = locale
    return BREAK

DIRNAME = None

def on_bindtextdomain(cpu, mem, ad):
    global exe, DIRNAME
    # ref: http://linux.die.net/man/3/bindtextdomain
    domainname = exe.get_func_param('system_v', 0)
    DIRNAME    = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    domainname_str = mem.read_utf8(domainname)
    dirname_str    = mem.read_utf8(DIRNAME)

    print 'bindtextdomain(%016x["%s"], %016x["%s"])' % (domainname, domainname_str, DIRNAME, dirname_str)

    cpu.rax = DIRNAME
    return BREAK

def on_textdomain(cpu, mem, ad):
    global exe, DIRNAME
    # ref: http://linux.die.net/man/3/textdomain
    domainname = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    domainname_str = mem.read_utf8(domainname)

    print 'textdomain(%016x["%s"])' % (domainname, domainname_str)

    cpu.rax = DIRNAME
    return BREAK

def on___cxa_atexit(cpu, mem, ad):
    global exe
    # ref: http://refspecs.linuxbase.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/baselib---cxa-atexit.html

    exe.ret_from_func('system_v', 3)

    cpu.rax = 0 # ???
    return BREAK

def on_isatty(cpu, mem, ad):
    global exe
    # ref: http://linux.die.net/man/3/isatty
    fd = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print 'isatty(%d)' % fd

    cpu.rax = 0
    return BREAK

def on_getenv(cpu, mem, ad):
    global exe
    # ref: http://man7.org/linux/man-pages/man3/getenv.3.html

    name = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    name_str = mem.read_utf8(name)

    print 'getenv(%016x["%s"])' % (name, name_str)

    cpu.rax = 0
    return BREAK

def on_ioctl(cpu, mem, ad):
    global exe
    # ref: http://man7.org/linux/man-pages/man2/ioctl.2.html

    fd = exe.get_func_param('system_v', 0)
    request = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2) # ....

    print 'ioctl(%d, %08x, ...)' % (fd, request)

    cpu.rax = 0xffffffff
    return BREAK

GETOPT_IDX = 1

def on_getopt_long(cpu, mem, ad):
    global exe, doc, GETOPT_IDX
    # ref: http://linux.die.net/man/3/getopt_long
    argc      = exe.get_func_param('system_v', 0)
    argv      = exe.get_func_param('system_v', 1)
    optstring = exe.get_func_param('system_v', 2)
    longopts  = exe.get_func_param('system_v', 3)
    longindex = exe.get_func_param('system_v', 4)

    exe.ret_from_func('system_v', 5)

    argv_list = []
    for i in range(argc):
        arg_ptr = mem.read_u64(argv + i * 8)
        arg_str = mem.read_utf8(arg_ptr)
        argv_list.append(arg_str)

    optstring_str = mem.read_utf8(optstring)

    res = None

    try:
        # ref: https://docs.python.org/2/library/getopt.html
        opts, args = getopt.getopt(argv_list[GETOPT_IDX:], optstring_str)
        print opts, args

        if len(opts) == 0:
            res = -1
        else:
            for opt, val in opts:
                GETOPT_IDX += 1
                res = ord(opt[1])

    except getopt.GetoptError as err:
        res = ord('?')

    res_str = None
    if res == -1:
        res_str = '-1'
    else:
        res_str = '\'%c\'' % res

    print 'getopt_long(%d, %016x["%s"], %016x["%s"], %016x, %016x) = %s' %\
        (argc, argv, ';'.join(argv_list), optstring, optstring_str, longopts, longindex, res_str)

    cpu.rax = res & 0xffffffff


    optind_addr = doc.get_label_addr('optind')
    mem.write_u32(optind_addr.offset, GETOPT_IDX)

    return BREAK

ERRNO_LOCATION = None

def on___errno_location(cpu, mem, ad):
    global exe, ERRNO_LOCATION

    if not ERRNO_LOCATION:
        ERRNO_LOCATION = 0x1100000000
        mem.alloc(ERRNO_LOCATION, 4, R_W)
        mem.write_u32(ERRNO_LOCATION, 0)

    exe.ret_from_func('system_v', 0)

    print '__errno_location()'

    cpu.rax = ERRNO_LOCATION
    return BREAK

alloc_addr = 0xB0000000
alloc_pos  = 0

def on_malloc(cpu, mem, ad):
    global exe, alloc_addr, alloc_pos, alloced

    size = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    ptr_alloc = alloc_addr + alloc_pos
    print("malloc(0x%08x) = 0x%016x" % (size, ptr_alloc))

    size = (size + 0x1000) & 0xFFFFF000 # Pages alignment
    mem.alloc(ptr_alloc, size, R_W)
    alloc_pos += size

    cpu.rax = ptr_alloc
    return BREAK

def on_free(cpu, mem, ad):
    ptr = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print 'free(%016x)' % ptr
    mem.free(ptr)

    return BREAK

def on_memcpy(cpu, mem, ad):
    global exe

    dst = exe.get_func_param('system_v', 0)
    src = exe.get_func_param('system_v', 1)
    sz  = exe.get_func_param('system_v', 2)

    exe.ret_from_func('system_v', 3)

    print 'memcpy(%016x, %016x, %016x)' % (dst, src, sz)

    data = mem.read(src, sz)
    mem.write(dst, data)

    cpu.rax = dst
    return BREAK

def on___xstat(cpu, mem, ad):
    global exe
    # ref: https://refspecs.linuxfoundation.org/LSB_3.0.0/LSB-PDA/LSB-PDA/baselib-xstat-1.html
    ver      = exe.get_func_param('system_v', 0)
    path     = exe.get_func_param('system_v', 1)
    stat_buf = exe.get_func_param('system_v', 2)

    exe.ret_from_func('system_v', 3)

    path_str = mem.read_utf8(path)

    print '__xstat(%d, %016x["%s"], %016x)' % (ver, path, path_str, stat_buf)

    STAT_SIZE = 4 + 8 + 8 + 4 + 4 + 4 + 4 + 4 + 8 + 8 + 8 + 4 + 4 + 4

    mem.write(stat_buf, '\x00' * STAT_SIZE)

    for i in range(STAT_SIZE):
        mem.write_u32(stat_buf + i * 4, i * 4)

    st_mode = 0755;
    if os.path.isdir(path_str):
        st_mode |=  040000 # S_IFDIR
    else:
        st_mode |= 0100000 # S_IFREG

    st_size = os.path.getsize(path_str)

    mem.write_u32(stat_buf + 0x18, st_mode)
    mem.write_u64(stat_buf + 0x30, st_size)

    ## This is old
    ##  struct stat {
    ##    dev_t     st_dev;     /* ID of device containing file */
    ##    ino_t     st_ino;     /* inode number */
    ##    mode_t    st_mode;    /* protection */
    ##    nlink_t   st_nlink;   /* number of hard links */
    ##    uid_t     st_uid;     /* user ID of owner */
    ##    gid_t     st_gid;     /* group ID of owner */
    ##    dev_t     st_rdev;    /* device ID (if special file) */
    ##    off_t     st_size;    /* total size, in bytes */
    ##    blksize_t st_blksize; /* blocksize for file system I/O */
    ##    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    ##    time_t    st_atime;   /* time of last access */
    ##    time_t    st_mtime;   /* time of last modification */
    ##    time_t    st_ctime;   /* time of last status change */
    ##  };

    ## Actually it looks more like that:
    ##     38 struct stat
    ##     39   {
    ##     40     __dev_t st_dev;          /* Device.  */
    ##     41     unsigned short int __pad1;
    ##     42 #ifndef __USE_FILE_OFFSET64
    ##     43     __ino_t st_ino;          /* File serial number    */
    ##     44 #else
    ##     45     __ino_t __st_ino;            /* 32bit file serial number    */
    ##     46 #endif
    ##     47     __mode_t st_mode;            /* File mode.  */
    ##     48     __nlink_t st_nlink;          /* Link count.  */
    ##     49     __uid_t st_uid;          /* User ID of the file's owner  */
    ##     50     __gid_t st_gid;          /* Group ID of the file's group.*/
    ##     51     __dev_t st_rdev;            /* Device number, if device.  */
    ##     52     unsigned short int __pad2;
    ##     53 #ifndef __USE_FILE_OFFSET64
    ##     54     __off_t st_size;            /* Size of file, in bytes.  */
    ##     55 #else
    ##     56     __off64_t st_size;            /* Size of file, in bytes.  */
    ##     57 #endif
    ##     58     __blksize_t st_blksize;      /* Optimal block size for I/O.  */
    ##     59
    ##     60 #ifndef __USE_FILE_OFFSET64
    ##     61     __blkcnt_t st_blocks;        /* Number 512-byte blocks allocated. */
    ##     62 #else
    ##     63     __blkcnt64_t st_blocks;      /* Number 512-byte blocks allocated. */
    ##     64 #endif
    ##     65 #ifdef __USE_XOPEN2K8
    ##     66     /* Nanosecond resolution timestamps are stored in a format
    ##     67        equivalent to 'struct timespec'.  This is the type used
    ##     68        whenever possible but the Unix namespace rules do not allow the
    ##     69        identifier 'timespec' to appear in the <sys/stat.h> header.
    ##     70        Therefore we have to handle the use of this header in strictly
    ##     71        standard-compliant sources special.  */
    ##     72     struct timespec st_atim;        /* Time of last access.  */
    ##     73     struct timespec st_mtim;        /* Time of last modification.  */
    ##     74     struct timespec st_ctim;        /* Time of last status change.  */
    ##     75 # define st_atime st_atim.tv_sec    /* Backward compatibility.  */
    ##     76 # define st_mtime st_mtim.tv_sec
    ##     77 # define st_ctime st_ctim.tv_sec
    ##     78 #else
    ##     79     __time_t st_atime;            /* Time of last access.  */
    ##     80     unsigned long int st_atimensec;  /* Nscecs of last access.  */
    ##     81     __time_t st_mtime;            /* Time of last modification.  */
    ##     82     unsigned long int st_mtimensec;  /* Nsecs of last modification.  */
    ##     83     __time_t st_ctime;            /* Time of last status change.  */
    ##     84     unsigned long int st_ctimensec;  /* Nsecs of last status change.  */
    ##     85 #endif
    ##     86 #ifndef __USE_FILE_OFFSET64
    ##     87     unsigned long int __glibc_reserved4;
    ##     88     unsigned long int __glibc_reserved5;
    ##     89 #else
    ##     90     __ino64_t st_ino;            /* File serial number    */
    ##     91 #endif
    ##     92   };


    cpu.rax = 0x0
    return BREAK

def on_dcgettext(cpu, mem, ad):
    global exe
    # ref: https://refspecs.linuxbase.org/LSB_2.1.0/LSB-generic/LSB-generic/baselib-dcgettext.html
    domainname = exe.get_func_param('system_v', 0)
    msgid      = exe.get_func_param('system_v', 1)
    category   = exe.get_func_param('system_v', 2)

    exe.ret_from_func('system_v', 3)

    domainname_str = mem.read_utf8(domainname)
    msgid_str = mem.read_utf8(msgid)

    print 'dcgettext(%016x["%s"], %016x["%s"], %d)' % (domainname, domainname_str, msgid, msgid_str, category)

    cpu.rax = msgid
    return BREAK

def on_strlen(cpu, mem, ad):
    global exe

    s = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    s_str = mem.read_utf8(s)
    res = len(s_str)

    print 'strlen(%016x["%r"]) = %d' % (s, s_str, res)

    cpu.rax = res
    return BREAK

def on__setjmp(cpu, mem, ad):
    global exe
    # ref: http://linux.die.net/man/3/_setjmp
    env = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print '_setjmp(%016x)' % env

    cpu.rax = 0
    return BREAK

def on___ctype_get_mb_cur_max(cpu, mem, ad):
    global exe
    # ref: https://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA.junk/baselib---ctype-get-mb-cur-max-1.html

    exe.ret_from_func('system_v', 0)

    print '__ctype_get_mb_cur_max()'

    cpu.rax = 4
    return BREAK

def on_fwrite_unlocked(cpu, mem, ad):
    global exe, OUTPUT
    # ref: http://linux.die.net/man/3/fwrite_unlocked
    ptr    = exe.get_func_param('system_v', 0)
    size   = exe.get_func_param('system_v', 1)
    n      = exe.get_func_param('system_v', 2)
    stream = exe.get_func_param('system_v', 3)

    exe.ret_from_func('system_v', 4)

    ptr_s = mem.read_utf8(ptr)

    print 'fwrite_unlocked(%016x["%s"], %016x, %016x, %016x)' % (ptr, ptr_s, size, n, stream)

    OUTPUT += ptr_s[:size * n]

    cpu.rax = size * n
    return BREAK

def on___overflow(cpu, mem, ad):
    global exe, OUTPUT

    f = exe.get_func_param('system_v', 0)
    c = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    print '__overflow(%016x, %08x)' % (f, c)

    cpu.rax = c

    OUTPUT += chr(c)
    return BREAK

def on_strcoll(cpu, mem, ad):
    global exe
    # ref: http://www.cplusplus.com/reference/cstring/strcoll/
    str1 = exe.get_func_param('system_v', 0)
    str2 = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    str1_str = mem.read_utf8(str1)
    str2_str = mem.read_utf8(str2)

    res = locale.strcoll(str1_str, str2_str)
    print 'strcoll(%016x["%s"], %016x["%s"]) = %d' % (str1, str1_str, str2, str2_str, res)

    cpu.rax = res & 0xffffffff
    return BREAK

def on___fprintf_chk(cpu, mem, ad):
    global exe

    stream = exe.get_func_param('system_v', 0)
    fmt = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 0) # ...

    fmt_str = mem.read_utf8(fmt)

    print 'printf(%016x, %016x["%s"])' % (stream, fmt, fmt_str)

    return BREAK

def on_strncmp(cpu, mem, ad):
    global exe
    # ref: http://www.cplusplus.com/reference/cstring/strcoll/
    str1 = exe.get_func_param('system_v', 0)
    str2 = exe.get_func_param('system_v', 1)
    n    = exe.get_func_param('system_v', 2)

    exe.ret_from_func('system_v', 3)

    str1_str = mem.read_utf8(str1)[:n]
    str2_str = mem.read_utf8(str2)[:n]

    res = locale.strcoll(str1_str, str2_str)
    print 'strcoll(%016x["%s"], %016x["%s"]) = %d' % (str1, str1_str, str2, str2_str, res)

    cpu.rax = res & 0xffffffff
    return BREAK

def on_strstr(cpu, mem, ad):
    global exe
    str1 = exe.get_func_param('system_v', 0)
    str2 = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    str1_str = mem.read_utf8(str1)
    str2_str = mem.read_utf8(str2)

    off = str1_str.find(str2_str)
    res = 0
    if off != -1:
        res = str1 + off

    res_s = mem.read_utf8(res)

    print 'strstr(%016x["%s"], %016x["%s"]) = %s' % (str1, str1_str, str2, str2_str, res_s)

    cpu.rax = res
    return BREAK

def on_lgetfilecon(cpu, mem, ad):
    global exe
    # ref: http://manpages.ubuntu.com/manpages/dapper/man3/getfilecon.3.html
    path = exe.get_func_param('system_v', 0)
    con  = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    path_str = mem.read_utf8(path)

    print 'lgetfilecon(%016x["%s"], %016x)' % (path, path_str, con)

    cpu.rax = 0xffffffff
    return BREAK

def on_error(cpu, mem, ad):
    global exe
    # ref: http://linux.die.net/man/3/error
    status = exe.get_func_param('system_v', 0)
    errnum = exe.get_func_param('system_v', 1)
    format = exe.get_func_param('system_v', 2)
    exe.ret_from_func('system_v', 1)

    format_str = mem.read_utf8(format)
    param_no = 3
    param = []

    for i in range(len(format_str)):
        c = format_str[i]
        if c == '%':
            p = format_str[i + 1]
            if p == '*':
                param.append(exe.get_func_param('system_v', param_no))
                param_no += 1
                p = format_str[i + 2]

            if p == 's':
                p_ptr = exe.get_func_param('system_v', param_no)
                param.append(mem.read_utf8(p_ptr))
                param_no += 1
                continue

            param.append(exe.get_func_param('system_v', param_no))
            param_no += 1

    res = format_str % tuple(param)

    print 'error(%d, %08x, %016x["%s"]) = "%s"' % (status, errnum, format, format_str, res)

    return BREAK

LC_ADDR = None

def on_localeconv(cpu, mem, ad):
    global exe, LC_ADDR

    if not LC_ADDR:
        ##struct lconv {
        ##  char *decimal_point;
        ##  char *thousands_sep;
        ##  char *grouping;
        ##  char *int_curr_symbol;
        ##  char *currency_symbol;
        ##  char *mon_decimal_point;
        ##  char *mon_thousands_sep;
        ##  char *mon_grouping;
        ##  char *positive_sign;
        ##  char *negative_sign;
        ##  char int_frac_digits;
        ##  char frac_digits;
        ##  char p_cs_precedes;
        ##  char p_sep_by_space;
        ##  char n_cs_precedes;
        ##  char n_sep_by_space;
        ##  char p_sign_posn;
        ##  char n_sign_posn;
        ##};

        LC_ADDR = 0xe0000000
        LC_DATA = LC_ADDR + 0x100

        # ref: http://www.cplusplus.com/reference/clocale/lconv/
        lc_info = ".\x00"
        mem.alloc(LC_DATA, 2, R_W)
        mem.write(LC_DATA, lc_info)

        mem.alloc(LC_ADDR, 8 * 10 + 1 * 8, R_W)
        mem.write_u64(LC_ADDR + 0 * 8, LC_DATA + 0)
        mem.write_u64(LC_ADDR + 1 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 2 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 3 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 4 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 5 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 6 * 8, LC_DATA + 1)
        mem.write_u64(LC_ADDR + 7 * 8, LC_DATA + 1)
        mem.write(LC_ADDR + 8 * 8, '\xff' * 8)

    exe.ret_from_func('system_v', 0)

    print 'localeconv() = %016x' % LC_ADDR

    cpu.rax = LC_ADDR
    return BREAK

PW_ADDR = None

def on_getpwuid(cpu, mem, ad):
    global exe, PW_ADDR

    uid = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print 'getpwuid(%d)' % uid

    if not PW_ADDR:
        ##  struct passwd {
        ##    char *pw_name;
        ##    char *pw_passwd;
        ##    uid_t pw_uid;
        ##    gid_t pw_gid;
        ##    time_t pw_change;
        ##    char *pw_class;
        ##    char *pw_gecos;
        ##    char *pw_dir;
        ##    char *pw_shell;
        ##    time_t pw_expire;
        ##  };

        PW_ADDR = 0x90000000

        mem.alloc(PW_ADDR, 0x200, R_W)
        mem.write(PW_ADDR, '\x00' * 0x200)

        PW_NAME = PW_ADDR + 8 + 8 + 4 + 4 + 4 + 8 + 8 + 8 + 8 + 4
        mem.write(PW_NAME, 'wisk')
        mem.write_u64(PW_ADDR, PW_NAME)

    cpu.rax = PW_ADDR
    return BREAK

GR_ADDR = None

def on_getgrgid(cpu, mem, ad):
    global exe, GR_ADDR

    gid = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print 'getgrgid(%d)' % gid

    if not GR_ADDR:
        ##  struct group {
        ##    char   *gr_name;       /* group name */
        ##    char   *gr_passwd;     /* group password */
        ##    gid_t   gr_gid;        /* group ID */
        ##    char  **gr_mem;        /* group members */
        ##  };

        GR_ADDR = PW_ADDR + 0x400

        mem.alloc(GR_ADDR, 0x200, R_W)
        mem.write(GR_ADDR, '\x00' * 0x200)

        GR_NAME = GR_ADDR + 0x100
        mem.write(GR_NAME, 'iki')
        mem.write_u64(GR_ADDR, GR_NAME)

    cpu.rax = GR_ADDR
    return BREAK



def on_strcpy(cpu, mem, ad):
    global exe

    dst = exe.get_func_param('system_v', 0)
    src = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    data = mem.read_utf8(src)
    mem.write(dst, data + '\x00')

    print 'strcpy(%016x, %016x["%s"])' % (dst, src, data)

    cpu.rax = dst
    return BREAK

def on___sprintf_chk(cpu, mem, ad):
    global exe
    # ref: http://refspecs.linux-foundation.org/LSB_4.0.0/LSB-Core-generic/LSB-Core-generic/libc---sprintf-chk-1.html
    buf    = exe.get_func_param('system_v', 0)
    flag   = exe.get_func_param('system_v', 1)
    buflen = exe.get_func_param('system_v', 2)
    fmt    = exe.get_func_param('system_v', 3)

    exe.ret_from_func('system_v', 0)

    fmt_str = mem.read_utf8(fmt)

    param_no = 4
    param = []

    for i in range(len(fmt_str)):
        c = fmt_str[i]
        if c == '%':
            p = fmt_str[i + 1]
            if p == '*':
                param.append(exe.get_func_param('system_v', param_no))
                param_no += 1
                p = fmt_str[i + 2]

            if p == 's':
                p_ptr = exe.get_func_param('system_v', param_no)
                param.append(mem.read_utf8(p_ptr))
                param_no += 1
                continue

            param.append(exe.get_func_param('system_v', param_no))
            param_no += 1

    res = fmt_str % tuple(param)

    print '__sprintf_chk(%016x, %d, %016x, %016x["%s"], ...) = "%s"' % (buf, flag, buflen, fmt, fmt_str, res)

    mem.write(buf, res + '\x00')
    cpu.rax = len(res) + 1

    return BREAK

def on_fputs_unlocked(cpu, mem, ad):
    global exe, OUTPUT
    # ref: http://www.cplusplus.com/reference/cstdio/fputs/
    s      = exe.get_func_param('system_v', 0)
    stream = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    s_str = mem.read_utf8(s)

    print 'fputs_unlocked(%016x["%r"], %016x)' % (s, s_str, stream)
    cpu.rax = len(s_str)
    OUTPUT += s_str

    return BREAK

TIME_ADDR = None

def on_localtime(cpu, mem, ad):
    global exe, TIME_ADDR
    # ref: http://linux.die.net/man/3/localtime
    timep = exe.get_func_param('system_v', 0)

    if not TIME_ADDR:
        TIME_ADDR = GR_ADDR + 0x200
        mem.alloc(TIME_ADDR, 0x200, R_W)
        mem.write(TIME_ADDR, '\x00' * 0x200)
        ## struct tm {
        ##    int tm_sec;         /* seconds */
        ##    int tm_min;         /* minutes */
        ##    int tm_hour;        /* hours */
        ##    int tm_mday;        /* day of the month */
        ##    int tm_mon;         /* month */
        ##    int tm_year;        /* year */
        ##    int tm_wday;        /* day of the week */
        ##    int tm_yday;        /* day in the year */
        ##    int tm_isdst;       /* daylight saving time */
        ##};
        mem.write_u32(TIME_ADDR + 0x00,  55) # sec
        mem.write_u32(TIME_ADDR + 0x04,  38) # min
        mem.write_u32(TIME_ADDR + 0x08,  20) # hr
        mem.write_u32(TIME_ADDR + 0x0c,   7) # dom
        mem.write_u32(TIME_ADDR + 0x10,  11) # mon
        mem.write_u32(TIME_ADDR + 0x14, 115) # yr
        mem.write_u32(TIME_ADDR + 0x18,   1) # dow
        mem.write_u32(TIME_ADDR + 0x1c, 340) # doy
        mem.write_u32(TIME_ADDR + 0x20,   0) #dst

    exe.ret_from_func('system_v', 1)

    print 'localtime(%016x)' % timep

    cpu.rax = TIME_ADDR
    return BREAK

def on_clock_gettime(cpu, mem, ad):
    global exe
    # ref: http://linux.die.net/man/3/clock_gettime
    clk_id = exe.get_func_param('system_v', 0)
    tp     = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    print 'clock_gettime(%d, %016x)' % (clk_id, tp)

    mem.write_u64(tp + 0, 0x5665e5da)
    mem.write_u64(tp + 4, 0x146d164f)

    cpu.rax = 0x0
    return BREAK

def on_gettimeofday(cpu, mem, ad):
    global exe
    # ref: http://linux.die.net/man/2/gettimeofday
    exe.ret_from_func('system_v', 2)

    print 'gettimeofday(...)'

    cpu.rax = 0xaabbccdd
    return BREAK

CHR_TBL_ADDR = None

CHR_TBL =[
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,
'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
91,92,93,94,95,96,
'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
123,124,125,126,127,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
]

def on___ctype_toupper_loc(cpu, mem, ad):
    global exe, CHR_TBL_ADDR
    # ref: https://searchcode.com/codesearch/view/38709730/

    exe.ret_from_func('system_v', 0)

    if not CHR_TBL_ADDR:
        CHR_TBL_ADDR = TIME_ADDR + 0x200
        mem.alloc(CHR_TBL_ADDR, 0x1000, R_W)
        mem.write_u64(CHR_TBL_ADDR, CHR_TBL_ADDR + 8 + 128 * 4)
        for i in range(len(CHR_TBL)):
            c = CHR_TBL[i]
            if type(c) is str:
                c = ord(c)
            mem.write_u32(CHR_TBL_ADDR + 8 + i * 4, c)

    print '__ctype_toupper_loc()'

    cpu.rax = CHR_TBL_ADDR
    return BREAK

def on_memset(cpu, mem, ad):
    global exe

    buf = exe.get_func_param('system_v', 0)
    val = exe.get_func_param('system_v', 1)
    sz  = exe.get_func_param('system_v', 2)

    exe.ret_from_func('system_v', 3)

    mem.write(buf, chr(val) * sz)

    print 'memset(%016x, %02x, %016x)' % (buf, val, sz)

    return BREAK

def on_opendir(cpu, mem, ad):
    global exe

    name = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    name_str = mem.read_utf8(name)

    print 'opendir(%016x[%r])' % (name, name_str)

    cpu.rax = name # We can cheat here, since we'll rely on os.listdir()

    return BREAK

LIST_DIR = {}
DIRENT_ADDR = None

def on_readdir(cpu, mem, ad):
    global exe, LIST_DIR, DIRENT_ADDR

    dirp = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    dirp_str = mem.read_utf8(dirp)

    DIRENT_SIZE = 8 + 8 + 4 + 256

    if not DIRENT_ADDR:
        DIRENT_ADDR = 0xdddd000000
        CHR_TBL_ADDR = mem.alloc(DIRENT_ADDR, DIRENT_SIZE, R_W)

    if not dirp_str in LIST_DIR:
        LIST_DIR[dirp_str] = [0, os.listdir(dirp_str)]

    #  struct dirent {
    #    ino_t          d_ino;       /* inode number */
    #    off_t          d_off;       /* offset to the next dirent */
    #    unsigned short d_reclen;    /* length of this record */
    #    unsigned char  d_type;      /* type of file; not supported by all file system types */
    #    char           d_name[256]; /* filename */
    #  };

    off_f, files = LIST_DIR[dirp_str]

    if off_f >= len(files):
        print 'readdir(%016x[%r]) = null' % (dirp, dirp_str)
        cpu.rax = 0x0
        return BREAK

    cur_file = files[off_f]
    LIST_DIR[dirp_str][0] += 1

    mem.write_u64(DIRENT_ADDR + 0x00, 0xdead)
    mem.write_u64(DIRENT_ADDR + 0x08, 0x0000)
    mem.write_u16(DIRENT_ADDR + 0x10, DIRENT_SIZE)
    mem.write_u8 (DIRENT_ADDR + 0x12, 8) # DT_REG
    mem.write    (DIRENT_ADDR + 0x13, cur_file + '\x00') # no alignment?

    print 'readdir(%016x[%r]) = "%s"' % (dirp, dirp_str, cur_file)

    cpu.rax = DIRENT_ADDR
    return BREAK

def on_realloc(cpu, mem, ad):
    global exe, alloc_addr, alloc_pos, alloced

    ptr = exe.get_func_param('system_v', 0)
    size = exe.get_func_param('system_v', 1)

    exe.ret_from_func('system_v', 2)

    ptr_alloc = alloc_addr + alloc_pos
    old_size = mem.size(ptr)

    print 'realloc(%016x, %016x) = %016x' % (ptr, size, ptr_alloc)

    size = (size + 0x1000) & 0xFFFFF000 # Pages alignment
    mem.alloc(ptr_alloc, size, R_W)
    alloc_pos += size
    data = mem.read(ptr, old_size)
    mem.write(ptr_alloc, data)
    mem.free(ptr)

    cpu.rax = ptr_alloc
    return BREAK

def on_closedir(cpu, mem, ad):
    global exe, LIST_DIR

    dirp = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    dirp_str = mem.read_utf8(dirp)

    LIST_DIR[dirp_str][0] = 0

    print 'closedir(%016x[%r])' % (dirp, dirp_str)

    cpu.rax = 0
    return BREAK

def on_exit(cpu, mem, ad):
    global exe

    status = exe.get_func_param('system_v', 0)

    exe.ret_from_func('system_v', 1)

    print 'exit(%d)' % status

    return STOP

# ------------------------------------------------- #

# Init parameters
exec_path = sys.argv[1]
db_path   = exec_path + '.mdb'

# Enable advanced Medusa logging
# pydusa.log_to_stdout()

# Init Medusa
core = pydusa.Medusa()
core.open_exe(exec_path, db_path, False)
core.wait_for_tasks()
doc = core.document
start_addr = doc.get_label_addr('start')

# Init Medusa emulator
exe = pydusa.Execution(doc)
exe.init([exec_path.replace('\\', '/'), '-l', 'C:\\'], [ ], '')
exe.set_emulator('interpreter')

exe.mem.alloc(0xd0000000, 0x100, R_W) # fake tcbhead_t
exe.mem.write_u64(0xd0000000 + 0x28, 0xdeadbeefdeadbeef) # very random cookie

exe.cpu.fs = 0x38
tcb_addr = pydusa.Address(0x38, 0x0)
exe.cpu.add_mapping(tcb_addr, 0xd0000000)

exe.mem.alloc(0xd0002000, 0x100, R_W)
exe.mem.write(0xd0002000, '\x00' * 0x100)
exe.mem.write_u64(doc.get_label_addr('stdout').offset, 0xd0002000)

for lbl in doc.labels:
    if not lbl.is_imp:
        continue
    print '[!] default handler for imported symbol: "%s"' % lbl.name
    exe.hook_fn(lbl.name, on_unk_api)

assert(exe.hook_fn('__libc_start_main', on_libc_start_main))
assert(exe.hook_fn('strrchr', on_strrchr))
assert(exe.hook_fn('setlocale', on_setlocale))
assert(exe.hook_fn('bindtextdomain', on_bindtextdomain))
assert(exe.hook_fn('textdomain', on_textdomain))
assert(exe.hook_fn('__cxa_atexit', on___cxa_atexit))
assert(exe.hook_fn('isatty', on_isatty))
assert(exe.hook_fn('getenv', on_getenv))
assert(exe.hook_fn('ioctl', on_ioctl))
assert(exe.hook_fn('getopt_long', on_getopt_long))
assert(exe.hook_fn('__errno_location', on___errno_location))
assert(exe.hook_fn('malloc', on_malloc))
assert(exe.hook_fn('memcpy', on_memcpy))
assert(exe.hook_fn('__xstat', on___xstat))
assert(exe.hook_fn('__lxstat', on___xstat))
assert(exe.hook_fn('dcgettext', on_dcgettext))
assert(exe.hook_fn('strlen', on_strlen))
assert(exe.hook_fn('free', on_free))
assert(exe.hook_fn('_setjmp', on__setjmp))
assert(exe.hook_fn('__ctype_get_mb_cur_max', on___ctype_get_mb_cur_max))
assert(exe.hook_fn('fwrite_unlocked', on_fwrite_unlocked))
assert(exe.hook_fn('__overflow', on___overflow))
assert(exe.hook_fn('strcoll', on_strcoll))
assert(exe.hook_fn('__fprintf_chk', on___fprintf_chk))
assert(exe.hook_fn('strncmp', on_strncmp))
assert(exe.hook_fn('strstr', on_strstr))
assert(exe.hook_fn('lgetfilecon', on_lgetfilecon))
assert(exe.hook_fn('error', on_error))
assert(exe.hook_fn('localeconv', on_localeconv))
assert(exe.hook_fn('getpwuid', on_getpwuid))
assert(exe.hook_fn('strcpy', on_strcpy))
assert(exe.hook_fn('getgrgid', on_getgrgid))
assert(exe.hook_fn('__sprintf_chk', on___sprintf_chk))
assert(exe.hook_fn('fputs_unlocked', on_fputs_unlocked))
assert(exe.hook_fn('localtime', on_localtime))
assert(exe.hook_fn('clock_gettime', on_clock_gettime))
assert(exe.hook_fn('gettimeofday', on_gettimeofday))
assert(exe.hook_fn('__ctype_toupper_loc', on___ctype_toupper_loc))
assert(exe.hook_fn('memset', on_memset))
assert(exe.hook_fn('opendir', on_opendir))
assert(exe.hook_fn('readdir', on_readdir))
assert(exe.hook_fn('realloc', on_realloc))
assert(exe.hook_fn('closedir', on_closedir))
assert(exe.hook_fn('exit', on_exit))

# exe.hook_insn(on_insn)

# raw_input()

# Start emulation
exe.execute(start_addr)

print 'RESULT:\n%s' % OUTPUT

# ------------------------------------------------- #
