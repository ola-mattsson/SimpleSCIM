/**
 * Copyright © 2017-2018  Max Wällstedt <max.wallstedt@gmail.com>
 *
 * This file is part of SimpleSCIM.
 *
 * SimpleSCIM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSCIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SimpleSCIM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "simplescim_error_string.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

static int prefix_present = 0;
static char prefix_buffer[1024];

static int message_present = 0;
static char message_buffer[1024];

static char error_string_buffer[1024];

static const char *no_error = "No error";

/**
 * Returns an error string that contains the latest error
 * message generated by SimpleSCIM or "No error" if no
 * error string has been set.
 */
const char *simplescim_error_string_get()
{
	if (!prefix_present && !message_present) {
		return no_error;
	}

	if (!prefix_present) {
		return message_buffer;
	}

	if (!message_present) {
		return prefix_buffer;
	}

	snprintf(error_string_buffer,
	         sizeof(error_string_buffer),
	         "%s: %s",
	         prefix_buffer,
	         message_buffer);

	return error_string_buffer;
}

/**
 * Sets the prefix of the error string with 'prefix' as the
 * format string. If a message has been set, the prefix
 * will appear before the message with a colon and a space
 * appended. If no message has been set, the prefix will
 * appear as is. NULL signifies no prefix.
 */
void simplescim_error_string_set_prefix(const char *prefix, ...)
{
	va_list ap;

	if (prefix == NULL) {
		prefix_present = 0;
	} else {
		prefix_present = 1;
		va_start(ap, prefix);
		vsnprintf(prefix_buffer,
		          sizeof(prefix_buffer),
		          prefix,
		          ap);
		va_end(ap);
	}
}

/**
 * Sets the message of the error string with 'message' as
 * the format string. NULL signifies no message.
 */
void simplescim_error_string_set_message(const char *message, ...)
{
	va_list ap;

	if (message == NULL) {
		message_present = 0;
	} else {
		message_present = 1;
		va_start(ap, message);
		vsnprintf(message_buffer,
		          sizeof(message_buffer),
		          message,
		          ap);
		va_end(ap);
	}
}

/**
 * Sets the prefix of the error string with 'prefix' as the
 * format string and sets the message of the error string
 * to strerror(errno). NULL signifies no prefix.
 */
void simplescim_error_string_set_errno(const char *prefix, ...)
{
	va_list ap;
	int tmp_errno;

	tmp_errno = errno;

	/* Set prefix */
	if (prefix == NULL) {
		prefix_present = 0;
	} else {
		prefix_present = 1;
		va_start(ap, prefix);
		vsnprintf(prefix_buffer,
		          sizeof(prefix_buffer),
		          prefix,
		          ap);
		va_end(ap);
	}

	/* Set message */
	message_present = 1;
	snprintf(message_buffer,
	         sizeof(message_buffer),
	         "%s",
	         strerror(tmp_errno));
}

/**
 * Sets the prefix of the error string to 'prefix' and sets
 * the message of the error string to 'message'.
 * 'prefix' == NULL signifies no prefix and
 * 'message' == NULL signifies no message.
 */
void simplescim_error_string_set(const char *prefix,
                                 const char *message)
{
	/* Set prefix */
	if (prefix == NULL) {
		prefix_present = 0;
	} else {
		prefix_present = 1;
		snprintf(prefix_buffer,
		         sizeof(prefix_buffer),
		         "%s",
		         prefix);
	}

	/* Set message */
	if (message == NULL) {
		message_present = 0;
	} else {
		message_present = 1;
		snprintf(message_buffer,
		         sizeof(message_buffer),
		         "%s",
		         message);
	}
}
