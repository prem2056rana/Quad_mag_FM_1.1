///*
// * lfs_port_ai.h
// *
// *  Created on: Sep 24, 2024
// *      Author: Dell
// */
//
//#ifndef INC_LFS_PORT_AI_H_
//#define INC_LFS_PORT_AI_H_
//
//
//#include "lfs.h"
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
///****************************************************************************
// * Public Function Prototypes
// ****************************************************************************/
//
//int littlefs_mount(struct lfs_t *lfs, const struct lfs_config *config);
//int littlefs_format(struct lfs_t *lfs, const struct lfs_config *config);
//int littlefs_open(struct lfs_t *lfs, lfs_file_t *file, const char *path, int flags);
//int littlefs_close(struct lfs_t *lfs, lfs_file_t *file);
//ssize_t littlefs_read(struct lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t size);
//ssize_t littlefs_write(struct lfs_t *lfs, lfs_file_t *file, const void *buffer, lfs_size_t size);
//lfs_soff_t littlefs_seek(struct lfs_t *lfs, lfs_file_t *file, lfs_soff_t offset, int whence);
//int littlefs_sync(struct lfs_t *lfs, lfs_file_t *file);
//void littlefs_get_default_config(struct lfs_config *cfg);
//
//#ifdef __cplusplus
//}
//#endif
//
//
//#endif /* INC_LFS_PORT_AI_H_ */
