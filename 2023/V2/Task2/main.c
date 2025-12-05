#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

enum
{
    BLKSZ = 1024,
    LNKCNT = BLKSZ / sizeof(unsigned),
    DATACNT = 10,
};

int
check(unsigned blockNum, // block id
      unsigned *pBlocks, // pointer to an array
      int depth,         // depth of search (depth == 0 - pBlocks points to regular array)
      unsigned lnkcnt,   // number of links in block
      int *blkleft,      // nubmer of blocks remaining unchecked in file
      int fdFS           // emulates disk
)
{
    int ans = 0;
    unsigned buf[LNKCNT] = {};

    for (int i = 0; i < lnkcnt; i++) {
        if (!depth) {
            // regular array
            ans = pBlocks[i] == blockNum;
            --*blkleft;
            if (*blkleft || ans) {
                return ans;
            }
        } else {
            lseek(fdFS, (off_t) pBlocks[i] * BLKSZ, SEEK_SET);
            read(fdFS, buf, sizeof(buf));
            ans = check(blockNum, buf, depth - 1, lnkcnt, blkleft, fdFS);
            if (*blkleft || ans) {
                return ans;
            }
        }
    }
    return ans;
}


int
is_blk_in_file(unsigned blockNum, // address of observing block
               unsigned *pBlocks, // pointer to blocks array
               unsigned long sz,  // size of file in bytes
               int fdFS           // fd that emulates disk
)
{
    // obtain nubmer of blocks
    // that file occupies
    int blkleft = (sz + (BLKSZ - 1)) / BLKSZ;
    int ans = 0;

    // obtain information of first 10 blocks
    ans = check(blockNum, pBlocks, 0, 10, &blkleft, fdFS);
    if (blkleft || ans) {
        return ans;
    }

    // obtain information from 10th block - 1st level
    // need to load if from memory
    unsigned buf[LNKCNT] = {};

    lseek(fdFS, (off_t) pBlocks[10] * BLKSZ, SEEK_SET);
    read(fdFS, buf, sizeof(buf));
    ans = check(blockNum, buf, 0, LNKCNT, &blkleft, fdFS);
    if (blkleft || ans) {
        return ans;
    }

    // obtain information from 11th block - 2nd level
    lseek(fdFS, (off_t) pBlocks[11] * BLKSZ, SEEK_SET);
    read(fdFS, buf, sizeof(buf));
    ans = check(blockNum, buf, 1, LNKCNT, &blkleft, fdFS);
    if (blkleft || ans) {
        return ans;
    }

    // obtain information from 12th block 3rd level
    lseek(fdFS, (off_t) pBlocks[12] * BLKSZ, SEEK_SET);
    read(fdFS, buf, sizeof(buf));
    ans = check(blockNum, buf, 2, LNKCNT, &blkleft, fdFS);
    return ans;
}
