/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#include "integer.h"
#include "ff.h"
#include "bsp_system.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 声明
//-----------------------------------------------------------------
typedef BYTE	DSTATUS;	// 磁盘功能的状态
// 磁盘功能的结果
typedef enum {
	RES_OK = 0,	// 0：成功	
	RES_ERROR,	// 1：读写失败	
	RES_WRPRT,	// 2：写保护	
	RES_NOTRDY,	// 3：没准备好	
	RES_PARERR	// 4：无效的参数	
} DRESULT;
extern FATFS fatfs_sd;              // SD卡逻辑驱动器的工作区
extern FATFS fatfs_flash;           // FLASH逻辑驱动器的工作区
extern FATFS fatfs_usb;             // USB逻辑驱动器的工作区

extern FIL file_sd;                 // SD卡文件对象
extern FIL file_flash;              // FLASH文件对象
extern FIL file_usb;                // USB文件对象

typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                     /*!< Initialize Disk Drive                     */
  DSTATUS (*disk_status)     (BYTE);                     /*!< Get Disk Status                           */
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);       /*!< Read Sector(s)                            */
#if _USE_WRITE == 1
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT); /*!< Write Sector(s) when _USE_WRITE = 0       */
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);              /*!< I/O control operation when _USE_IOCTL = 1 */
#endif /* _USE_IOCTL == 1 */

}Diskio_drvTypeDef;

/**
  * @brief  Global Disk IO Drivers structure definition
  */
typedef struct
{
  uint8_t                 is_initialized[_VOLUMES];
  const Diskio_drvTypeDef *drv[_VOLUMES];
  uint8_t                 lun[_VOLUMES];
  volatile uint8_t        nbr;

}Disk_drvTypeDef;

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
// 磁盘状态位
#define STA_NOINIT		0x01	// 驱动器未初始化
#define STA_NODISK		0x02	// 驱动器中没有磁盘
#define STA_PROTECT		0x04	// 写保护

// disk_ioctrl函数的命令代码
// 通用命令(Used by FatFs)
#define CTRL_SYNC					0		// 完成挂起的写入过程(needed at _FS_READONLY == 0)
#define GET_SECTOR_COUNT	1		// 获取磁盘大小(needed at _USE_MKFS == 1)
#define GET_SECTOR_SIZE		2		// 获取扇区大小(needed at _MAX_SS != _MIN_SS)
#define GET_BLOCK_SIZE		3		// 获取擦除块大小 (needed at _USE_MKFS == 1)
#define CTRL_TRIM					4		// 通知设备不再使用扇区块上的数据(needed at _USE_TRIM == 1)

// 通用命令 (Not used by FatFs)
#define CTRL_POWER				5		// 获取/设置电源状态
#define CTRL_LOCK					6		// 锁定/解锁磁盘移除
#define CTRL_EJECT				7		// 弹出磁盘
#define CTRL_FORMAT				8		// 在媒体上创建物理格式

// MMC/SDC特定的ioctl命令
#define MMC_GET_TYPE			10	// 获取卡类型
#define MMC_GET_CSD				11	// 获取CSD
#define MMC_GET_CID				12	// 获取CID
#define MMC_GET_OCR				13	// 获取OCR
#define MMC_GET_SDSTAT		14	// 获取SD状态
#define ISDIO_READ				55	// 从SD iSDIO寄存器读取数据
#define ISDIO_WRITE				56	// 向SD iSDIO寄存器写入数据
#define ISDIO_MRITE				57	// 屏蔽写入SD iSDIO寄存器的数据

// ATA/CF特定的ioctl命令
#define ATA_GET_REV				20	// 获取F/W revision
#define ATA_GET_MODEL			21	// 获取型号名称
#define ATA_GET_SN				22	// 获取序列号

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern DSTATUS disk_initialize (BYTE pdrv);
extern DSTATUS disk_status (BYTE pdrv);
extern DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
extern DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
extern DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef *drv, char *path, BYTE lun);
uint8_t FATFS_UnLinkDriverEx(char *path, BYTE lun);
uint8_t FATFS_GetAttachedDriversNbr(void);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
