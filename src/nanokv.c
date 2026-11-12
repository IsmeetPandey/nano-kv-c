#define _POSIX_C_SOURCE 200809L
#include "nanokv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t simple_crc(const char* s, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint8_t)s[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return ~crc;
}

nkv_db_t* nkv_open(const char* filepath) {
    nkv_db_t* db = (nkv_db_t*)malloc(sizeof(nkv_db_t));
    if (!db) return NULL;

    db->filepath = strdup(filepath);
    db->index_cap = 64;
    db->index_size = 0;
    db->index = (nkv_index_entry_t*)malloc(db->index_cap * sizeof(nkv_index_entry_t));

    FILE* f = fopen(filepath, "a+b");
    if (!f) {
        free(db->filepath);
        free(db->index);
        free(db);
        return NULL;
    }
    fclose(f);
    return db;
}

bool nkv_put(nkv_db_t* db, const char* key, const char* value) {
    if (!db || !key || !value) return false;

    FILE* f = fopen(db->filepath, "ab");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long offset = ftell(f);

    uint32_t klen = (uint32_t)strlen(key);
    uint32_t vlen = (uint32_t)strlen(value);
    uint32_t crc = simple_crc(value, vlen);

    fwrite(&crc, sizeof(uint32_t), 1, f);
    fwrite(&klen, sizeof(uint32_t), 1, f);
    fwrite(&vlen, sizeof(uint32_t), 1, f);
    fwrite(key, 1, klen, f);
    fwrite(value, 1, vlen, f);
    fclose(f);

    // Update in-memory index
    for (size_t i = 0; i < db->index_size; ++i) {
        if (strcmp(db->index[i].key, key) == 0) {
            db->index[i].offset = offset;
            db->index[i].val_len = vlen;
            return true;
        }
    }

    if (db->index_size >= db->index_cap) {
        db->index_cap *= 2;
        db->index = (nkv_index_entry_t*)realloc(db->index, db->index_cap * sizeof(nkv_index_entry_t));
    }

    strncpy(db->index[db->index_size].key, key, NKV_MAX_KEY_LEN - 1);
    db->index[db->index_size].key[NKV_MAX_KEY_LEN - 1] = '\0';
    db->index[db->index_size].offset = offset;
    db->index[db->index_size].val_len = vlen;
    db->index_size++;

    return true;
}

char* nkv_get(nkv_db_t* db, const char* key) {
    if (!db || !key) return NULL;

    for (size_t i = 0; i < db->index_size; ++i) {
        if (strcmp(db->index[i].key, key) == 0) {
            FILE* f = fopen(db->filepath, "rb");
            if (!f) return NULL;

            fseek(f, db->index[i].offset, SEEK_SET);

            uint32_t crc, klen, vlen;
            if (fread(&crc, sizeof(uint32_t), 1, f) != 1) { fclose(f); return NULL; }
            if (fread(&klen, sizeof(uint32_t), 1, f) != 1) { fclose(f); return NULL; }
            if (fread(&vlen, sizeof(uint32_t), 1, f) != 1) { fclose(f); return NULL; }

            fseek(f, klen, SEEK_CUR); // skip key

            char* val = (char*)malloc(vlen + 1);
            if (fread(val, 1, vlen, f) != vlen) {
                free(val);
                fclose(f);
                return NULL;
            }
            val[vlen] = '\0';
            fclose(f);

            if (simple_crc(val, vlen) != crc) {
                free(val);
                return NULL; // CRC integrity error
            }
            return val;
        }
    }
    return NULL;
}

bool nkv_delete(nkv_db_t* db, const char* key) {
    if (!db || !key) return false;
    for (size_t i = 0; i < db->index_size; ++i) {
        if (strcmp(db->index[i].key, key) == 0) {
            db->index[i] = db->index[db->index_size - 1];
            db->index_size--;
            return true;
        }
    }
    return false;
}

void nkv_close(nkv_db_t* db) {
    if (!db) return;
    free(db->filepath);
    free(db->index);
    free(db);
}
