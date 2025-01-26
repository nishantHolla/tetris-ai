#include "tetris.h"

int8_t tet_hashmap_hash(const tet_Game *game, uchar_t *hash) {
  /*
     Calculates SHA256 of the board of the given game and store it in the given hash buffer. Returns
     0 if successful else returns non 0 integer
  */

  uchar_t buffer[GRID_ROW_COUNT * GRID_COL_COUNT] = {0};
  const uint16_t shape = TET_PIECE_SHAPE[game->current_piece.index][game->current_piece.rotation];
  const int32_t px = game->position.x;
  const int32_t py = game->position.y;

  for (int32_t row = 0; row < GRID_ROW_COUNT; row++) {
    for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
      if (row >= py && row < py + 4 &&
          col >= px && col < px + 4 &&
          SHAPE_BIT(shape, row-py, col-px)) {
        buffer[(row * GRID_COL_COUNT) + col] = game->current_piece.index;
      }
      else {
        buffer[(row * GRID_COL_COUNT) + col] = game->board[row][col];
      }
    }
  }

  SHA256(buffer, GRID_ROW_COUNT * GRID_COL_COUNT, hash);
  return 0;
}

int32_t tet_hashmap_index(const uchar_t *hash) {
  /*
     Given a SHA256 hash, calculate its index for hashmap and returns it
  */

  int32_t char_sum = 0;
  for (int32_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    char_sum += (int32_t) hash[i];
  }

  return char_sum % HASH_CPACITY;
}

int8_t tet_hashmap_init(tet_HashMap *map) {
  /*
     Initializes the given hash map struct. Returns 0 if initialization was successful else returns
     0
  */

  for (int32_t i = 0; i < HASH_CPACITY; i++) {
    map->buffer[i] = NULL;
  }

  map->size = 0;

  return 0;
}

int8_t tet_hashmap_get(tet_HashMap *map, const tet_Game *game, double *value) {
  /*
     Gets the value of given game from the given map and stores it in the given value container.
     Returns 0 if key is found else returns 1
  */

  uchar_t hash[SHA256_DIGEST_LENGTH];
  tet_hashmap_hash(game, hash);
  const int32_t index = tet_hashmap_index(hash);

  tet_HashBucket *bucket = map->buffer[index];
  while (bucket && memcmp(bucket->key, hash, SHA256_DIGEST_LENGTH) != 0) {
    bucket = bucket->next;
  }

  if (!bucket) {
    return 1;
  }

  if (value) {
    *value = bucket->value;
  }
  return 0;
}

int8_t tet_hashmap_set(tet_HashMap *map, const tet_Game *game, double value) {
  /*
     Sets the value of given game in the given map with the given value. Returns 0 if successful
     else returns non 0 integer. Overrides the value if game already exists in the map
  */

  uchar_t hash[SHA256_DIGEST_LENGTH];
  tet_hashmap_hash(game, hash);
  const int32_t index = tet_hashmap_index(hash);

  tet_HashBucket *bucket = map->buffer[index];
  while (bucket && memcmp(bucket->key, hash, SHA256_DIGEST_LENGTH) != 0) {
    bucket = bucket->next;
  }

  if (bucket) {
    bucket->value = value;
  }
  else {
    tet_HashBucket *new_bucket = (tet_HashBucket *) malloc(sizeof(tet_HashBucket));
    memcpy(new_bucket->key, hash, SHA256_DIGEST_LENGTH);
    new_bucket->value = value;
    new_bucket->next = map->buffer[index];
    map->buffer[index] = new_bucket;
    map->size++;
  }

  return 0;
}

int8_t tet_hashmap_remove(tet_HashMap *map, const tet_Game *game) {
  /*
     Removes a given game from the given map. Returns 0 if successful else returns non 0 integer.
     Does nothing if game is not in the map
  */

  uchar_t hash[SHA256_DIGEST_LENGTH];
  tet_hashmap_hash(game, hash);
  const int32_t index = tet_hashmap_index(hash);

  tet_HashBucket *bucket = map->buffer[index];
  tet_HashBucket *prev = NULL;
  while (bucket && memcmp(bucket->key, hash, SHA256_DIGEST_LENGTH) != 0) {
    prev = bucket;
    bucket = bucket->next;
  }

  if (bucket) {
    if (prev) {
      prev->next = bucket->next;
    }
    else {
      map->buffer[index] = bucket->next;
    }

    free(bucket);
    map->size--;
  }

  return 0;
}

void tet_hashmap_free(tet_HashMap *map) {
  /*
     Frees the given hash map
  */

  for (int32_t i = 0; i < HASH_CPACITY; i++) {
    tet_HashBucket *traverse = map->buffer[i];
    while (traverse) {
      tet_HashBucket *temp = traverse;
      traverse = traverse->next;
      free(temp);
    }

    map->buffer[i] = NULL;
  }

  map->size = 0;
}
