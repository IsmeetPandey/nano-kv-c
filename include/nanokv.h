#ifndef NANO_KV_H
#define NANO_KV_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NKV_MAX_KEY_LEN 128
#define NKV_MAX_VAL_LEN 1024

typedef struct {
    char key[NKV_MAX_KEY_LEN];
    long offset;
    uint32_t val_len;
} nkv_index_entry_t;

typedef struct {
    char* filepath;
    int fd;
    nkv_index_entry_t* index;
    size_t index_size;
    size_t index_cap;
} nkv_db_t;

nkv_db_t* nkv_open(const char* filepath);
bool nkv_put(nkv_db_t* db, const char* key, const char* value);
char* nkv_get(nkv_db_t* db, const char* key);
bool nkv_delete(nkv_db_t* db, const char* key);
void nkv_close(nkv_db_t* db);

#endif
