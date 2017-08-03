#ifndef SIMPLESCIM_ERROR_STRING_H
#define SIMPLESCIM_ERROR_STRING_H

/**
 * Returns an error string that contains the latest error
 * message generated by SimpleSCIM or "No error" if no
 * error string has been set.
 */
const char *simplescim_error_string_get();

/**
 * Sets the prefix of the error string with 'prefix' as the
 * format string. If a message has been set, the prefix
 * will appear before the message with a colon and a space
 * appended. If no message has been set, the prefix will
 * appear as is. NULL signifies no prefix.
 */
void simplescim_error_string_set_prefix(const char *prefix, ...);

/**
 * Sets the message of the error string with 'message' as
 * the format string. NULL signifies no message.
 */
void simplescim_error_string_set_message(const char *message, ...);

/**
 * Sets the prefix of the error string with 'prefix' as the
 * format string and sets the message of the error string
 * to strerror(errno). NULL signifies no prefix.
 */
void simplescim_error_string_set_errno(const char *prefix, ...);

/**
 * Sets the prefix of the error string to 'prefix' and sets
 * the message of the error string to 'message'.
 * 'prefix' == NULL signifies no prefix and
 * 'message' == NULL signifies no message.
 */
void simplescim_error_string_set(const char *prefix,
                                 const char *message);

#endif