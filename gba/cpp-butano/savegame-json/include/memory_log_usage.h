#ifndef MEMORY_LOG_USAGE_H
#define MEMORY_LOG_USAGE_H

#include <cstring>

#include "bn_log.h"
#include "bn_memory.h"

void memory_log_usage(const char *caption = "")
{
    uint64_t ram_ew_used_static = bn::memory::used_static_ewram();
    uint64_t ram_ew_used_alloc = bn::memory::used_alloc_ewram();
    uint64_t ram_iw_used_static = bn::memory::used_static_iwram();
    uint64_t ram_iw_used_stack = bn::memory::used_stack_iwram();

    BN_LOG(caption, strlen(caption) > 0 ? ": " : "", "Memory usage:");
    BN_LOG("EWRAM (static + alloc):", ram_ew_used_static, " + ", ram_ew_used_alloc);
    BN_LOG("IWRAM (static + stack):", ram_iw_used_static, " + ", ram_iw_used_stack);
}

#endif // MEMORY_LOG_USAGE_H