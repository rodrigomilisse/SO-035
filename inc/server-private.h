/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#include "server.h"
#include "memory.h"
#include "main.h"

static inline bool verify_wallet_ids(struct transaction *tx, struct info_container *info);

static inline bool verify_funds(struct transaction *tx, float *balances);

static inline bool verify_wallet_signature(struct transaction *tx);

static inline void sign_transaction(struct transaction *tx, int server_id);

static inline void transfer_funds(struct transaction *tx, float *balances);