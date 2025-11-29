/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

// Copyright...

#include <node/context.h>
#include <validation.h>
#include <banman.h>
#include <interfaces/chain.h>
#include <net.h>
#include <net_processing.h>
#include <scheduler.h>
#include <txmempool.h>     // ✅ YES — this line goes here

NodeContext::NodeContext()
{
    g_mempool = mempool.get();   // correct
}

NodeContext::~NodeContext() {}
