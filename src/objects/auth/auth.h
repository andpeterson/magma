
/**
 * @file /magma/objects/auth/auth.h
 *
 * @brief The STACIE authentication functions.
 */

#ifndef MAGMA_OBJECTS_AUTH_H
#define MAGMA_OBJECTS_AUTH_H

/***
 * @enum auth_lock_status_t
 */
typedef enum {
	 AUTH_LOCK_NONE = 0,                   /**< Normal unlocked account. */

	 AUTH_LOCK_INACTIVITY = 1,             /**< An inactive account lock, that gets cleared automatically when the user authenticates. */

	 AUTH_LOCK_EXPIRED = 2,                /**< The user subscription has expired, messages are accepted, but logins are refused. */

	 AUTH_LOCK_ADMIN = 3,                  /**< An administrative lock, and used for a variety of reasons. */
	 AUTH_LOCK_ABUSE = 4,                  /**< Abuse locks are applied to accounts that appear to be violating the acceptable use policy. */
	 AUTH_LOCK_USER = 5                    /**< The account has been locked at the request of the user. */
} auth_lock_status_t;

typedef struct {
	stringer_t *key;
	stringer_t *token;
} auth_legacy_t;

typedef struct {

	struct {
		stringer_t *master;
		stringer_t *password;
	} keys;

	struct {
		stringer_t *verification;
		stringer_t *ephemeral;
	} tokens;

} auth_stacie_t;

typedef struct {

	uint64_t usernum;
	stringer_t *username;

	struct {
		uint8_t locked;
	} status;

	struct {
		uint64_t bonus;
		stringer_t *salt;
		stringer_t *nonce;
	} seasoning;

	struct {
		stringer_t *master;
	} keys;

	struct {
		stringer_t *ephemeral;
		stringer_t *verification;
	} tokens;

	struct {
		stringer_t *key;
		stringer_t *token;
	} legacy;

} auth_t;

/// username.c
stringer_t *  auth_sanitize_address(stringer_t *username);
stringer_t *  auth_sanitize_username(stringer_t *username);

/// auth.c
auth_t *  auth_alloc(void);
auth_t *  auth_challenge(stringer_t *username);
void      auth_free(auth_t *auth);
int_t     auth_login(stringer_t *username, stringer_t *password, auth_t **output);
int_t     auth_response(auth_t *auth, stringer_t *ephemeral);

/// datatier.c
int_t   auth_data_fetch(auth_t *auth);
int_t   auth_data_update_legacy(uint64_t usernum, stringer_t *legacy, stringer_t *salt, stringer_t *verification, uint32_t bonus);
void    auth_data_update_lock(uint64_t usernum, auth_lock_status_t lock);

/// stacie.c
auth_stacie_t *  auth_stacie(uint32_t bonus, stringer_t *username, stringer_t *password, stringer_t *salt, stringer_t *verification, stringer_t *nonce);
auth_stacie_t *  auth_stacie_alloc(void);
void             auth_stacie_cleanup(auth_stacie_t *stacie);
void             auth_stacie_free(auth_stacie_t *stacie);

/// legacy.c
auth_legacy_t *  auth_legacy(stringer_t *username, stringer_t *password);
auth_legacy_t *  auth_legacy_alloc(void);
void             auth_legacy_free(auth_legacy_t *legacy);

#endif

