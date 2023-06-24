#pragma once

#ifdef CLOUD

/* key to ask if the server is available
 * iin case of update to the server,
 *  in which it will be necessary to
 *  disable the server it will answer no
*/
#define ISON 0

/* key for create new account */
#define KEY_ADD_ACCOUNT 1

/* key for log in */
#define KEY_LOG 2

/* key for write file */
#define KEY_SAVE 3

/* asks the server how much money he has in his checking account */
#define ASK_FOR_BALANCE 4

#define ADD_FRIEND 5
#define DELETE_FRIEND 6
#define BLOCK_PERSON 7

#endif // CLOUD
