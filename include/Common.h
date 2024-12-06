#ifndef __COMMON_H__
#define __COMMON_H__

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <atomic>
#include <bitset>
#include <limits>

#include "Debug.h"
#include "HugePageAlloc.h"
#include "Rdma.h"

#include "WRLock.h"

// CONFIG_ENABLE_EMBEDDING_LOCK and CONFIG_ENABLE_CRC
// **cannot** be ON at the same time

// #define CONFIG_ENABLE_EMBEDDING_LOCK
// #define CONFIG_ENABLE_CRC

#define KEY_SIZE 8

#define LATENCY_WINDOWS 1000000

#define STRUCT_OFFSET(type, field) \
  (char *)&((type *)(0))->field - (char *)((type *)(0))

#define MAX_MACHINE 8 // number of servers

#define ADD_ROUND(x, n) ((x) = ((x) + 1) % (n))

#define MESSAGE_SIZE 96 // byte

#define POST_RECV_PER_RC_QP 128

#define RAW_RECV_CQ_COUNT 128

// { app thread
#define MAX_APP_THREAD 32

#define APP_MESSAGE_NR 96

// }

// { dir thread
#define NR_DIRECTORY 1

#define DIR_MESSAGE_NR 128
// }

void bindCore(uint16_t core);
char *getIP();
char *getMac();

inline int bits_in(std::uint64_t u) {
  auto bs = std::bitset<64>(u);
  return bs.count();
}

#include <boost/coroutine/all.hpp>

using CoroYield = boost::coroutines::symmetric_coroutine<void>::yield_type;
using CoroCall = boost::coroutines::symmetric_coroutine<void>::call_type;

struct CoroContext {
  CoroYield *yield;
  CoroCall *master;
  int coro_id;
};

namespace define {

constexpr uint64_t MB = 1024ull * 1024;
constexpr uint64_t GB = 1024ull * MB;
constexpr uint16_t kCacheLineSize = 64;

// for remote allocate
constexpr uint64_t kChunkSize = 16 * MB;

// for store root pointer
constexpr uint64_t kRootPointerStoreOffest = kChunkSize / 2;
static_assert(kRootPointerStoreOffest % sizeof(uint64_t) == 0, "XX");

// lock on-chip memory
constexpr uint64_t kLockStartAddr = 0;
constexpr uint64_t kLockChipMemSize = 256 * 1024;

// number of locks
// we do not use 16-bit locks, since 64-bit locks can provide enough concurrency.
// if you want to use 16-bit locks, call *cas_dm_mask*
constexpr uint64_t kLockSize = 16;
constexpr uint64_t kNumOfLock = kLockChipMemSize / kLockSize;

// level of tree
constexpr uint64_t kMaxLevelOfTree = 7;

constexpr uint16_t kMaxCoro = 8;
constexpr int64_t kPerThreadRdmaBuf  = 12 * MB;
constexpr int64_t kPerCoroRdmaBuf = kPerThreadRdmaBuf / kMaxCoro;

constexpr uint8_t kMaxHandOverTime = 0;

constexpr int kIndexCacheSize = 1024;  // MB
} // namespace define

static inline unsigned long long asm_rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

// constexpr uint8_t kKeySize = 64;

// For Tree
using Key = uint64_t;
using Value = uint64_t;
constexpr Key kKeyMin = std::numeric_limits<Key>::min();
constexpr Key kKeyMax = std::numeric_limits<Key>::max();
constexpr Value kValueNull = 0;

#if KEY_SIZE == 8
using InternalKey = Key;

#else
constexpr size_t arr_cnt = KEY_SIZE / sizeof(uint64_t);

struct KeyArr {
  uint64_t arr[arr_cnt];
  KeyArr() { arr[0] = 0; };
  KeyArr(const uint64_t k) {
    for (size_t i = 0; i < arr_cnt; ++i) arr[i] = k;
  };

  KeyArr &operator=(const KeyArr &other) {
    for (size_t i = 0; i < arr_cnt; ++i) arr[i] = other.arr[i];
    return *this;
  }

  operator uint64_t() const { return arr[0]; }
  bool operator==(const KeyArr &other) const { return arr[0] == other.arr[0]; }
  bool operator!=(const KeyArr &other) const { return arr[0] != other.arr[0]; }

  bool operator==(const uint64_t &other) const { return arr[0] == other; }
  bool operator!=(const uint64_t &other) const { return arr[0] != other; }

  auto operator<=>(const uint64_t &other) const { return arr[0] <=> other; }

  auto operator<=>(const KeyArr &other) const {
    return arr[0] <=> other.arr[0];
  }
} __attribute__((packed));

using InternalKey = KeyArr;

#endif

// fixed for variable length key
constexpr size_t kHeaderRawSize = 30 + 2 * sizeof(InternalKey) + 16;
constexpr size_t kHeaderSize = (kHeaderRawSize + 63) / 64 * 64;
constexpr uint32_t kPageSize =
    (kHeaderSize + 60 * (sizeof(InternalKey) + sizeof(uint64_t)) + 63) / 64 *
    64;
constexpr uint32_t kInternalPageSize = kPageSize;
constexpr uint32_t kLeafPageSize = kPageSize;

__inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

inline void mfence() { asm volatile("mfence" ::: "memory"); }

inline void compiler_barrier() { asm volatile("" ::: "memory"); }

#endif /* __COMMON_H__ */
