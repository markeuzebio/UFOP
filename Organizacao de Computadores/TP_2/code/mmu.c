#include "mmu.h"

#include <stdio.h>

bool canOnlyReplaceBlock(Line);

/* 
    Students' defined function. Returns -1 if there is not
    an unused line inside the cache. Otherwise, returns the
    position of the unused line inside the cache.
*/
int getUnusedLine(Line*, int);

/*
    Students' defined function. Returns the line removed from
    cache given as argument. It is established that L1 lines'
    removal always occur at the beginning (LRU).
*/
Line removeLineFIFO(Line*, int);

/*
    Students' defined function. Returns the line removed from
    cache given as argument. It is established that L2 and L3 lines'
    removal always occur randomly.
*/
Line removeLineRandomly(Line*, int, int*);

/*
    Students' defined function. Checks if a certain block in a 
    lower level cache is already contained in a higher level cache
    so that it is possible to overwrite the higher level cache's line.
*/
int blockAlreadyExistsInCache(Line*, int, int);

/*
    Modified function by students. It makes the memory mapping.
*/
int memoryCacheMapping(Machine*, MemoryBlock*, Line*, Line*, Line*);

/*
    Students' defined function.
*/
void rearrangeCacheLines(Line*, int, int);

int lineWhichWillLeave(int, Cache*);

void updateMachineInfos(Machine*, WhereWasHit*, int);

void moveLine();

char* convertToString(WhereWasHit whereWasHit) {
    switch (whereWasHit) {
        case L1Hit:
            return "CL1";
        case L2Hit:
            return "CL2";
        case L3Hit:
            return "CL3";
        case RAMHit:
            return "RAM";
    }

    return "";
}

Line* MMUSearchOnMemorys(Address add, Machine* machine, WhereWasHit* whereWasHit) {
    // Strategy => write back
    MemoryBlock* RAM = machine->ram.blocks;
    Line* cache1 = machine->l1.lines;
    Line* cache2 = machine->l2.lines;
    Line* cache3 = machine->l3.lines;
    WhereWasHit cache_hit = RAMHit;

    int cost = 0;
    int cacheLine;  // Line in cache L1 which must be returned

    // Is block in cache1?
    for(cacheLine = 0 ; cacheLine < machine->l1.size ; cacheLine++) {
        if(cache1[cacheLine].tag == add.block) {
            cache_hit = L1Hit;
            break;
        }
    }
    // If block is not in cache1, test if it is inside cache2.
    if(cache_hit != L1Hit) {
        for(cacheLine = 0 ; cacheLine < machine->l2.size ; cacheLine++) {
            if(cache2[cacheLine].tag == add.block) {
                cache_hit = L2Hit;
                break;
            }
        }
    }
    // If block is not in cache1, test if it is inside cache3.
    else if(cache_hit != L1Hit && cache_hit != L2Hit) {
        for(cacheLine = 0 ; cacheLine < machine->l3.size ; cacheLine++) {
            if(cache3[cacheLine].tag == add.block) {
                cache_hit = L3Hit;
                break;
            }
        }
    }
    
    if(cache_hit == L1Hit) {
        *whereWasHit = L1Hit;
        cost = COST_ACCESS_L1;
        // Uncomment to make the LRU method.
        /*{
            rearrangeCacheLines(cache1, cacheLine, machine->l1.size);
            // Given the rearrangement, it is necessary to change where cacheLine points to.
            cacheLine = machine->l1.size - 1;
        }*/
    } else if(cache_hit == L2Hit) {
        *whereWasHit = L2Hit;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2;
        {
            Line line = cache2[cacheLine];
            int newL1pos = memoryCacheMapping(machine, RAM, cache1, cache2, cache3);
            cache1[newL1pos] = line;
            // rearrangeCacheLines(cache2, cacheLine, machine->l2.size);
            cacheLine = newL1pos;
        }
    } else if(cache_hit == L3Hit) {
        *whereWasHit = L3Hit;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
        {
            Line line = cache3[cacheLine];
            int newL1pos = memoryCacheMapping(machine, RAM, cache1, cache2, cache3);
            cache1[newL1pos] = line;
            // rearrangeCacheLines(cache3, cacheLine, machine->l3.size);
            cacheLine = newL1pos;
        }
    } else if(cache_hit == RAMHit) {
        *whereWasHit = RAMHit;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
        {
            int newL1pos = memoryCacheMapping(machine, RAM, cache1, cache2, cache3);
            cache1[newL1pos].block = RAM[add.block];
            cache1[newL1pos].tag = add.block;
            cache1[newL1pos].updated = false;
            cacheLine = newL1pos;
        }
    }

    updateMachineInfos(machine, whereWasHit, cost);
    return &(cache1[cacheLine]);
}

// OLD MMUSearchOnMemorys FUNCTION
//Line* MMUSearchOnMemorys(Address add, Machine* machine, WhereWasHit* whereWasHit) {
    // Direct memory map
    // int l1pos = memoryCacheMapping(add.block, &machine->l1);
    // int l2pos = memoryCacheMapping(add.block, &machine->l2);
    // int l3pos = memoryCacheMapping(add.block, &machine->l3);

    // Line* cache1 = machine->l1.lines;
    // Line* cache2 = machine->l2.lines;
    // Line* cache3 = machine->l3.lines;

    // MemoryBlock* RAM = machine->ram.blocks;
    // int cost = 0;

//     if (cache1[l1pos].tag == add.block) { 
//         /* Block is in memory cache L1 */
//         cost = COST_ACCESS_L1;
//         *whereWasHit = L1Hit;
//     } else if (cache2[l2pos].tag == add.block) { 
//         /* Block is in memory cache L2 */
//         cache2[l2pos].tag = add.block;
//         cost = COST_ACCESS_L1 + COST_ACCESS_L2;
//         *whereWasHit = L2Hit;
//         // Just works for Direct Mapping
//         {
//             Line tmp = cache1[l1pos];
//             cache1[l1pos] = cache2[l2pos];
//             int newL2pos = lineWhichWillLeave(tmp.tag, &machine->l2); /* Need to check the position of the block that will leave the L1 */
//             if (!canOnlyReplaceBlock(cache2[newL2pos]))
//                 RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
//             cache2[newL2pos] = tmp;
//         }
//     } else if (cache3[l3pos].tag == add.block) {
//         cache3[l3pos].tag = add.block;
//         cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
//         *whereWasHit = L3Hit;
//         {
//             Line tmp = cache1[l1pos];
//             cache1[l1pos] = cache3[l3pos];
//             int newL2pos = lineWhichWillLeave(tmp.tag, &machine->l2); /* Need to check the position of the block that will leave the L1 */
//             int newL3pos;
//             if (!canOnlyReplaceBlock(cache2[newL2pos])) { 
//                 newL3pos = lineWhichWillLeave(cache2[newL2pos].tag, &machine->l3);
//                 if(!canOnlyReplaceBlock(cache3[newL3pos]))
//                     RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
//                 cache3[newL3pos] = cache2[newL2pos];
//             }

//             cache2[newL2pos] = tmp;
//         }
//     } else {
//         /* Blocks only in memory RAM, need to bring it to cache and manipulate the blocks */
//         int newL2pos;
//         int newL3pos;
//         /* The block on cache L1 cannot only be replaced, the memories must be updated */
//         if (!canOnlyReplaceBlock(cache1[l1pos])) {
//             newL2pos = lineWhichWillLeave(cache1[l1pos].tag, &machine->l2); /* Need to check the position of the block that will leave the L1 */
//             /* The block on cache L2 cannot only be replaced, the memories must be updated */
//             if (!canOnlyReplaceBlock(cache2[newL2pos])) {
//                 newL3pos = lineWhichWillLeave(cache2[newL2pos].tag, &machine->l3);
//                 if (!canOnlyReplaceBlock(cache3[newL3pos]))
//                     RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
//                 cache3[newL3pos] = cache2[newL2pos];
//             }
//             cache2[newL2pos] = cache1[l1pos];
//         }
//         cache1[l1pos].block = RAM[add.block];
//         cache1[l1pos].tag = add.block;
//         cache1[l1pos].updated = false;
//         cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
//         *whereWasHit = RAMHit;
//     }
//     updateMachineInfos(machine, whereWasHit, cost);
//     return &(cache1[l1pos]);
//}

bool canOnlyReplaceBlock(Line line) {
    // Or the block is empty or
    // the block is equal to the one in memory RAM and can be replaced
    if (line.tag == INVALID_ADD || (line.tag != INVALID_ADD && !line.updated))
        return true;
    return false;
}

int getUnusedLine(Line* lines, int size) {
    for(int i = 0 ; i < size ; i++)
        if(lines[i].tag == INVALID_ADD)
            return i;
    return -1;
}

Line removeLineFIFO(Line* cache, int size_cache) {
    // Stores removed line.
    Line removed = cache[0];
    for(int i = 1 ; i < size_cache ; i++)
        cache[i - 1] = cache[i];

    return removed;
}

Line removeLineRandomly(Line* cache, int size_cache, int* line_to_overwrite) {
    *line_to_overwrite = rand() % size_cache;
    return cache[*line_to_overwrite];
}

/*
    Returns the line in cache where "block_number" is present. Otherwise, that is,
    if block is not inside the cache, returns -1.
*/
int blockAlreadyExistsInCache(Line* cache, int size_cache, int block_number) {
    for(int i = 0 ; i < size_cache ; i++)
        if(cache[i].tag == block_number)
            return i;

    return -1;
}

void rearrangeCacheLines(Line* cache, int cacheLine, int size_cache) {
    Line line = cache[cacheLine];

    for(int i = cacheLine + 1 ; i < size_cache ; i++)
        cache[i - 1] = cache[i];

    cache[size_cache - 1] = line;
}

int memoryCacheMapping(Machine* machine, MemoryBlock* RAM, Line* cache1, Line* cache2, Line* cache3) {
    /* If it returns -1, there is not an unused line */
    int newL1pos = getUnusedLine(cache1, machine->l1.size);

    if(newL1pos == -1) {
        // Uncomment if it is desired to remove a line randomly.
        removeLineRandomly(cache1, machine->l1.size, &newL1pos);
        // Line removedLineL1 = removeLineFIFO(cache1, machine->l1.size);
        // newL1pos = machine->l1.size - 1;     /* New blocks in L1 are always inserted at the end, like a queue */

        if(!canOnlyReplaceBlock(cache1[newL1pos])) {
        // if(!canOnlyReplaceBlock(removedLineL1)) {
            /* If it returns -1, there is not an unused line */
            int newL2pos = blockAlreadyExistsInCache(cache2, machine->l2.size, cache1[newL1pos].tag);
            // int newL2pos = blockAlreadyExistsInCache(cache2, machine->l2.size, removedLineL1.tag);

            if(newL2pos == -1) {
                newL2pos = getUnusedLine(cache2, machine->l2.size);

                if(newL2pos == -1) {
                    /* Gets the line which is going to be overwritten.*/
                    // Uncomment if it is desired to remove a line randomly.
                    removeLineRandomly(cache2, machine->l2.size, &newL2pos);
                    // Line lineToOverwriteL2 = removeLineFIFO(cache2, machine->l2.size);
                    // newL2pos = machine->l2.size - 1;

                    if(!canOnlyReplaceBlock(cache2[newL2pos])) {
                    // if(!canOnlyReplaceBlock(lineToOverwriteL2)) {
                        /* If it returns -1, there is not an unused line */
                        int newL3pos = blockAlreadyExistsInCache(cache3, machine->l3.size, cache2[newL2pos].tag);
                        // int newL3pos = blockAlreadyExistsInCache(cache3, machine->l3.size, lineToOverwriteL2.tag);

                        if(newL3pos == -1) {
                            newL3pos = getUnusedLine(cache3, machine->l3.size);

                            if(newL3pos == -1) {
                                // Uncomment if it is desired to remove a line randomly.
                                removeLineRandomly(cache3, machine->l3.size, &newL3pos);
                                // Line lineToOverwriteL3 = removeLineFIFO(cache3, machine->l3.size);
                                // newL3pos = machine->l3.size - 1;

                                if(!canOnlyReplaceBlock(cache3[newL3pos]))
                                    RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                                // if(!canOnlyReplaceBlock(lineToOverwriteL3))
                                    // RAM[lineToOverwriteL3.tag] = lineToOverwriteL3.block;
                            }
                        }

                        cache3[newL3pos] = cache2[newL2pos];
                        // cache3[newL3pos] = lineToOverwriteL2;
                    }
                }
            }

            cache2[newL2pos] = cache1[newL1pos];
            // cache2[newL2pos] = removedLineL1;
        }
    }

    return newL1pos;
}

int lineWhichWillLeave(int address, Cache* cache) {
    return address % cache->size;
}

void updateMachineInfos(Machine* machine, WhereWasHit* whereWasHit, int cost) {
    switch (*whereWasHit) {
        case L1Hit:
            machine->hitL1 += 1;
            break;

        case L2Hit:
            machine->hitL2 += 1;
            machine->missL1 += 1;
            break;

        case L3Hit:
            machine->hitL3 += 1;
            machine->missL1 += 1;
            machine->missL2 += 1;
            break;
        
        case RAMHit:
            machine->hitRAM += 1;
            machine->missL1 += 1;
            machine->missL2 += 1;
            machine->missL3 += 1;
            break;
    }
    machine->totalCost += cost;
}