/*-------------------------------------------------------------------------
 *
 * buffile_gpdb.h
 *	  The GPDB additions to buffile.
 *-------------------------------------------------------------------------
 */

#ifndef BUFFILE_GPDB_H
#define BUFFILE_GPDB_H

/* BufFile is an opaque type whose details are not known outside buffile.c. */

typedef struct BufFile BufFile;

extern BufFile *BufFileCreateFile(const char *filePrefix, bool delOnClose, bool interXact);
extern BufFile *BufFileOpenFile(const char * fileName, bool create, bool delOnClose, bool interXact);
extern BufFile *BufFileCreateTemp_ReaderWriter(const char *fileName, bool isWriter,
							   bool interXact);

extern void BufFileFlush(BufFile *file);
extern int64 BufFileGetSize(BufFile *buffile);
extern void BufFileSetWorkfile(BufFile *buffile);

#endif   /* BUFFILE_GPDB_H */
