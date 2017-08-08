dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(JikesDBG, 1.0, hsh@gist.ac.kr)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([subdir-objects])
AM_CONFIG_HEADER([config.h])

AC_PROG_CXX

AC_CHECK_HEADER(unistd.h)
AC_CHECK_HEADER(sys/wait.h)
AC_CHECK_HEADER(sys/types.h)
AC_CHECK_HEADER(sys/user.h)
AC_CHECK_HEADER(sys/ptrace.h)
AC_CHECK_HEADER(netinet/in.h)
AC_CHECK_HEADER(netdb.h)
AC_CHECK_HEADER(sys/socket.h)
AC_CHECK_HEADER(arpa/inet.h)

AC_CHECK_TYPE(pid_t, [], [], [#include <sys/types.h>])
AC_CHECK_TYPE(struct user_regs_struct, [], [], [#include <sys/user.h>])
AC_CHECK_TYPE(struct sockaddr_in, [], [], [#include <netinet/in.h>])
AC_CHECK_TYPE(struct hostent, [], [], [#include <netdb.h>])

AC_CHECK_FUNC(execvp)
AC_CHECK_FUNC(fork)
AC_CHECK_FUNC(wait)
AC_CHECK_FUNC(waitpid)
AC_CHECK_FUNC(ptrace)
AC_CHECK_FUNC(socket)
AC_CHECK_FUNC(inet_addr)


AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

