#include <stdio.h>
#include <stdlib.h>
#include "nanokv.h"

int main() {
    printf("============================================================\n");
    printf("🏎️  NANO-KV-C v1.0.0\n");
    printf("├─ Append-Only Embedded Key-Value Storage Engine\n");
    printf("└─ Author: Ismeet Pandey\n");
    printf("============================================================\n");

    nkv_db_t* db = nkv_open("store.db");
    if (!db) {
        printf("Failed to open database\n");
        return 1;
    }

    nkv_put(db, "user:101", "{\"name\":\"Ismeet\",\"role\":\"engineer\"}");
    nkv_put(db, "system:status", "operational");

    char* val = nkv_get(db, "user:101");
    if (val) {
        printf("Retrieved key 'user:101' -> %s\n", val);
        free(val);
    }

    nkv_close(db);
    printf("Database closed cleanly.\n");
    return 0;
}
