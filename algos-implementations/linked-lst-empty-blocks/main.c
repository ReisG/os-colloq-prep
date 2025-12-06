#include <stdio.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>

/*
    Запрашиваем свободный блок при файловой системе, в которой используется
    Связный список массивов из номеров свободных блоков
*/
unsigned
get_empty_block(unsigned *table,     // указатель на блок с массивом
                unsigned *table_idx, // адрес блока, который загружен в RAM
                int blk_sz,          // размер блока в байтах
                int fdFS             // индексный дескриптор, эмулирующий диск
)
{
    if (*table_idx == 0) {
        // cannot allocate memory
        return 0;
    }

    // if zero then used
    // 0th element is link to next block
    int blk_cnt = blk_sz / sizeof(unsigned);
    for (int i = 1; i < blk_cnt; i++) {
        if (table[i]) {
            // new have found requested block
            unsigned res = table[i];
            table[i] = 0;
            return res;
        }
    }

    // this block is empty and can be used to provide extra space
    unsigned res = *table_idx;

    // but now we need to load contents of
    // next array in linked list to memory
    *table_idx = table[0];
    lseek(fdFS, (off_t) (*table_idx) * blk_sz, SEEK_SET);
    read(fdFS, table, blk_sz);

    return res;
}
