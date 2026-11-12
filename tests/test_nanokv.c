#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nanokv.h"

int main() {
    remove("test.db");
    nkv_db_t* db = nkv_open("test.db");
    assert(db != NULL);

    assert(nkv_put(db, "alpha", "one"));
    assert(nkv_put(db, "beta", "two"));

    char* v1 = nkv_get(db, "alpha");
    assert(v1 != NULL && strcmp(v1, "one") == 0);
    free(v1);

    char* v2 = nkv_get(db, "beta");
    assert(v2 != NULL && strcmp(v2, "two") == 0);
    free(v2);

    assert(nkv_delete(db, "alpha"));
    assert(nkv_get(db, "alpha") == NULL);

    nkv_close(db);
    remove("test.db");
    printf("All nano-kv tests passed successfully!\n");
    return 0;
}
