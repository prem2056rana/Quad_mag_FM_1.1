/****************************************************************************
 * lfs_port.c
 *
 * Bare-metal or STM32CubeIDE HAL driver implementation for LittleFS
 *
 ****************************************************************************/

#include "lfs_port_ai.h"
#include "lfs.h"
#include "stm32f1xx_hal.h"
#include <string.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
//
//static int lfs_hal_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
//static int lfs_hal_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
//static int lfs_hal_erase(const struct lfs_config *c, lfs_block_t block);
//static int lfs_hal_sync(const struct lfs_config *c);
//
///****************************************************************************
// * Public Functions
// ****************************************************************************/
//
//int littlefs_mount(struct lfs_t *lfs, const struct lfs_config *config) {
//    return lfs_mount(lfs, config);
//}
//
//int littlefs_format(struct lfs_t *lfs, const struct lfs_config *config) {
//    return lfs_format(lfs, config);
//}
//
//int littlefs_open(struct lfs_t *lfs, lfs_file_t *file, const char *path, int flags) {
//    return lfs_file_open(lfs, file, path, flags);
//}
//
//int littlefs_close(struct lfs_t *lfs, lfs_file_t *file) {
//    return lfs_file_close(lfs, file);
//}
//
//ssize_t littlefs_read(struct lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t size) {
//    ssize_t ret = lfs_file_read(lfs, file, buffer, size);
//    if (ret > 0) {
//        file->pos += ret;
//    }
//    return ret;
//}
//
//ssize_t littlefs_write(struct lfs_t *lfs, lfs_file_t *file, const void *buffer, lfs_size_t size) {
//    ssize_t ret = lfs_file_write(lfs, file, buffer, size);
//    if (ret > 0) {
//        file->pos += ret;
//    }
//    return ret;
//}
//
//lfs_soff_t littlefs_seek(struct lfs_t *lfs, lfs_file_t *file, lfs_soff_t offset, int whence) {
//    lfs_soff_t ret = lfs_file_seek(lfs, file, offset, whence);
//    if (ret >= 0) {
//        file->pos = ret;
//    }
//    return ret;
//}
//
//int littlefs_sync(struct lfs_t *lfs, lfs_file_t *file) {
//    return lfs_file_sync(lfs, file);
//}
//
///****************************************************************************
// * Private Functions
// ****************************************************************************/
//
//static int lfs_hal_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
//	 LFS_ASSERT(off % cfg->read_size == 0);
//	LFS_ASSERT(size % cfg->read_size == 0);
//	LFS_ASSERT(block < cfg->block_count);
//	MT25Q_ReadData(block * cfg->block_size + off, (uint8_t *)buffer, size);
//	return LFS_ERR_OK;
//}
//
//static int lfs_hal_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
//    LFS_ASSERT(off % cfg->prog_size == 0);
//	LFS_ASSERT(size % cfg->prog_size == 0);
//	LFS_ASSERT(block < cfg->block_count);
//	MT25Q_WriteEnable();
//	MT25Q_Program(block * cfg->block_size + off, (uint8_t *)buffer, size);
//	return LFS_ERR_OK;
//}
//
//static int lfs_hal_erase(const struct lfs_config *cfg, lfs_block_t block) {
//	 LFS_ASSERT(block < cfg->block_count);
//	MT25Q_WriteEnable();
//	MT25Q_SectorErase(block * cfg->block_size);
//	return LFS_ERR_OK;
//}
//
//static int lfs_hal_sync(const struct lfs_config *cfg) {
//    // Implement hardware-specific sync function
//    // Example using HAL:
//    // HAL_FLASHEx_DATAEEPROM_Flush();
//    return 0; // Return 0 on success
//}
//
//void littlefs_get_default_config(struct lfs_config *cfg) {
//    memset(cfg, 0, sizeof(struct lfs_config));
//    cfg->read = lfs_hal_read;
//    cfg->prog = lfs_hal_prog;
//    cfg->erase = lfs_hal_erase;
//    cfg->sync = lfs_hal_sync;
//    cfg->read_size = 16;
//    cfg->prog_size = 16;
//    cfg->block_size = 4096;
//    cfg->block_count = 1024;
//    cfg->cache_size = 16;
//    cfg->lookahead_size = 16;
//    cfg->block_cycles = 100;
//}
