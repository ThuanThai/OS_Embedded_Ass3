/* Locks
00002    Copyright (C) 2000 Free Software Foundation, Inc.
00003    Written by Stephane Carrez (stcarrez@worldnet.fr)    
00004 
00005 This file is free software; you can redistribute it and/or modify it
00006 under the terms of the GNU General Public License as published by the
00007 Free Software Foundation; either version 2, or (at your option) any
00008 later version.
00009 
00010 In addition to the permissions in the GNU General Public License, the
00011 Free Software Foundation gives you unlimited permission to link the
00012 compiled version of this file with other programs, and to distribute
00013 those programs without any restriction coming from the use of this
00014 file.  (The General Public License restrictions do apply in other
00015 respects; for example, they cover modification of the file, and
00016 distribution when not linked into another program.)
00017 
00018 This file is distributed in the hope that it will be useful, but
00019 WITHOUT ANY WARRANTY; without even the implied warranty of
00020 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
00021 General Public License for more details.
00022 
00023 You should have received a copy of the GNU General Public License
00024 along with this program; see the file COPYING.  If not, write to
00025 the Free Software Foundation, 59 Temple Place - Suite 330,
00026 Boston, MA 02111-1307, USA.  */

#ifndef _TIME_H
#define _TIME_H


#include <sys/param.h>

struct tm
{
  int tm_sec;                   /* Seconds.     [0-60] (1 leap second) */
  int tm_min;                   /* Minutes.     [0-59] */
  int tm_hour;                  /* Hours.       [0-23] */
  int tm_mday;                  /* Day.         [1-31] */
  int tm_mon;                   /* Month.       [0-11] */
  int tm_year;                  /* Year - 1900.  */
  int tm_wday;                  /* Day of week. [0-6] */
  int tm_yday;                  /* Days in year.[0-365] */
  int tm_isdst;                 /* DST.         [-1/0/1]*/

  int tm_mon_length;
  int tm_year_length;
};

typedef unsigned long time_t;

/* Return the current time and put it in *TIMER if TIMER is not NULL.  */
extern time_t time(time_t *__timer);

/* Return the `struct tm' representation of *TIMER in local time,
   using *TP to store the result.  */
extern struct tm *localtime_r(const time_t * __timer,
                              struct tm * __tp);

extern void udelay (unsigned long usec);

#endif