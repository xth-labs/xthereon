/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

#pragma once

#include "internal/cache.hpp"
#include "internal/fifo_cache_policy.hpp"
#include "internal/lru_cache_policy.hpp"

template <typename Key, typename Value>
using FIFOCache = typename caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy<Key>>;

template <typename Key, typename Value>
using LRUCache = typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy<Key>>;
