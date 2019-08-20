
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct bytes {
  size_t len;
  char *data;
} bytes;

bool bytes_init(bytes *b, const size_t len) {
  b->len = len;
  b->data = (char*)calloc(len, sizeof(char));
  return b->data != NULL;
}

void bytes_free(bytes *b) {
  free(b->data);
  b->data = NULL;
  b->len = 0;
}

bool bytes_equal(const bytes *const lhs, const bytes *const rhs) {
  if (lhs->len != rhs->len) {
    return false;
  }
  return memcmp(lhs->data, rhs->data, lhs->len) != 0;
}

bytes *bytes_from_c_string(bytes *b, const char str[]) {
  size_t len = strlen(str);
  char *data = (char*)malloc(len * sizeof(char));
  memcpy(data, str, len);
  b->len = len;
  b->data = data;
  return b;
}

typedef struct hash_map_pair_t {
  bytes *key;
  bytes *value;
} hash_map_pair_t;

typedef struct hash_map_t {
  int count;
  int capacity;
  hash_map_pair_t *contents;
} hash_map_t;

bool hash_map_init(hash_map_t *hash_map, int initial_capacity) {
  hash_map->count = 0;
  hash_map->capacity = initial_capacity;
  hash_map->contents = (hash_map_pair_t*)calloc(initial_capacity, sizeof(hash_map_pair_t));
  if (hash_map->contents == NULL) {
    return false;
  }
  return true;
}

void hash_map_free(hash_map_t *hash_map) {
  free(hash_map->contents);
  hash_map->contents = NULL;
  hash_map->count = 0;
  hash_map->capacity = 0;
}

bool hash_map_resize(hash_map_t *hash_map, int target_capacity) {
  return true;
}

int hash_map_hash(bytes* key) {
  unsigned int h = 17;
  for (int i = 0; i < key->len; ++i) {
    h ^= key->data[i];
    h <<= 1;
  }
  return h;
}

bool hash_map_set(hash_map_t *hash_map, bytes *key, bytes *value) {
  if ((hash_map->count + 1) * 4 > hash_map->capacity * 5) {
    if (!hash_map_resize(hash_map, hash_map->capacity * 2)) {
      return false;
    }
  }
  int pos = hash_map_hash(key) % hash_map->capacity;
  while (hash_map->contents[pos].key != NULL) {
    ++pos;
    if (pos >= hash_map->capacity) {
      pos = 0;
    }
  }
  hash_map->contents[pos].key = key;
  hash_map->contents[pos].value = value;
  return true;
}

bytes* hash_map_get(hash_map_t *hash_map, bytes *key) {
  int pos = hash_map_hash(key) % hash_map->capacity;
  hash_map_pair_t *contents_start = hash_map->contents;
  hash_map_pair_t *contents_end = hash_map->contents + hash_map->capacity;
  hash_map_pair_t *start = hash_map->contents + pos;
  hash_map_pair_t *current = start;
  while (current->key != NULL) {
    if (bytes_equal(current->key, key) == 0) {
      return current->value;
    }
    ++current;
    if (current == contents_end) {
      current = contents_start;
    }
    if (current == start) {
      return NULL;
    }
  }
  return NULL;
}

int main() {
  bytes key_a;
  bytes_from_c_string(&key_a, "key a");
  bytes key_b;
  bytes_from_c_string(&key_b, "key b");
  bytes value_a;
  bytes_from_c_string(&value_a, "value a");
  bytes value_b;
  bytes_from_c_string(&value_a, "value b");
  hash_map_t hash_map;
  hash_map_init(&hash_map, 10);
  hash_map_set(&hash_map, &key_a, &value_a);
  hash_map_set(&hash_map, &key_b, &value_b);
  bytes *c = hash_map_get(&hash_map, &key_a);
  assert(bytes_equal(&value_a, c));
  hash_map_free(&hash_map);
}

