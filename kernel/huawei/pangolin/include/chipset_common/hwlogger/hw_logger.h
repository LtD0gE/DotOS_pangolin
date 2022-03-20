/* include/linux/logger.h
 *
 * Copyright (C) 2007-2008 Google, Inc.
 * Author: Robert Love <rlove@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_LOGGER_H
#define _LINUX_LOGGER_H

#include <linux/types.h>
#include <linux/ioctl.h>

/**
 * struct user_logger_entry_compat - defines a single entry that is given to a logger
 * @len:	The length of the payload
 * @__pad:	Two bytes of padding that appear to be required
 * @pid:	The generating process' process ID
 * @tid:	The generating process' thread ID
 * @sec:	The number of seconds that have elapsed since the Epoch
 * @nsec:	The number of nanoseconds that have elapsed since @sec
 * @msg:	The message that is to be logged
 *
 * The userspace structure for version 1 of the logger_entry ABI.
 * This structure is returned to userspace unless the caller requests
 * an upgrade to a newer ABI version.
 */
struct user_logger_entry_compat {
	__u16 len;
	__u16 __pad;
	__s32 pid;
	__s32 tid;
	__s32 sec;
	__s32 nsec;
	char msg[0];
};

/**
 * struct logger_entry - defines a single entry that is given to a logger
 * @len:	The length of the payload
 * @hdr_size:	sizeof(struct logger_entry_v2)
 * @pid:	The generating process' process ID
 * @tid:	The generating process' thread ID
 * @sec:	The number of seconds that have elapsed since the Epoch
 * @nsec:	The number of nanoseconds that have elapsed since @sec
 * @euid:	Effective UID of logger
 * @msg:	The message that is to be logged
 *
 * The structure for version 2 of the logger_entry ABI.
 * This structure is returned to userspace if ioctl(LOGGER_SET_VERSION)
 * is called with version >= 2
 */
struct logger_entry {
	__u16 len;
	__u16 hdr_size;
	__s32 pid;
	__s32 tid;
	__s32 sec;
	__s32 nsec;
	kuid_t euid;
	char msg[0];
};

struct hw_logger_msg {
	struct iovec *iov;
	unsigned long nr_segs;
};

extern struct huawei_log_tag __start_hwlog_tag, __stop_hwlog_tag;

#define LOGGER_LOG_EXCEPTION	"hwlog_exception"	/* exception */
#define LOGGER_LOG_JANK    "hwlog_jank"	/* system performance messages */
#define LOGGER_LOG_DUBAI   "hwlog_dubai" /* system dubai messages */

#define LOGGER_ENTRY_MAX_PAYLOAD	4076

#define __LOGGERIO	0xAE
#define CHECK_CODE	0x7BCDABCD

enum CheckError {
    EVecSeg = 1,
    EVecLen,
    EVecCopy,
    EVecCode,
};

#define O_HWLOGGER_RDDEL		01000000000		/* will be deleted after being read */
#define LOGGER_GET_LOG_BUF_SIZE	_IO(__LOGGERIO, 1)	/* size of log */
#define LOGGER_GET_LOG_LEN		_IO(__LOGGERIO, 2)	/* used log len */
#define LOGGER_GET_NEXT_ENTRY_LEN	_IO(__LOGGERIO, 3)	/* next entry len */
#define LOGGER_FLUSH_LOG		_IO(__LOGGERIO, 4)	/* flush log */
#define LOGGER_SET_RDDEL		_IO(__LOGGERIO, 5)	/* set O_HWLOGGER_RDDEL to flag */
#define LOGGER_RESET_RDDEL		_IO(__LOGGERIO, 6)	/* reset O_HWLOGGER_RDDEL to flag */

#endif /* _LINUX_LOGGER_H */
