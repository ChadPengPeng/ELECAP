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
// ����
//-----------------------------------------------------------------
typedef BYTE	DSTATUS;	// ���̹��ܵ�״̬
// ���̹��ܵĽ��
typedef enum {
	RES_OK = 0,	// 0���ɹ�	
	RES_ERROR,	// 1����дʧ��	
	RES_WRPRT,	// 2��д����	
	RES_NOTRDY,	// 3��û׼����	
	RES_PARERR	// 4����Ч�Ĳ���	
} DRESULT;
extern FATFS fatfs_sd;              // SD���߼��������Ĺ�����
extern FATFS fatfs_flash;           // FLASH�߼��������Ĺ�����
extern FATFS fatfs_usb;             // USB�߼��������Ĺ�����

extern FIL file_sd;                 // SD���ļ�����
extern FIL file_flash;              // FLASH�ļ�����
extern FIL file_usb;                // USB�ļ�����

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
// �궨��
//-----------------------------------------------------------------
// ����״̬λ
#define STA_NOINIT		0x01	// ������δ��ʼ��
#define STA_NODISK		0x02	// ��������û�д���
#define STA_PROTECT		0x04	// д����

// disk_ioctrl�������������
// ͨ������(Used by FatFs)
#define CTRL_SYNC					0		// ��ɹ����д�����(needed at _FS_READONLY == 0)
#define GET_SECTOR_COUNT	1		// ��ȡ���̴�С(needed at _USE_MKFS == 1)
#define GET_SECTOR_SIZE		2		// ��ȡ������С(needed at _MAX_SS != _MIN_SS)
#define GET_BLOCK_SIZE		3		// ��ȡ�������С (needed at _USE_MKFS == 1)
#define CTRL_TRIM					4		// ֪ͨ�豸����ʹ���������ϵ�����(needed at _USE_TRIM == 1)

// ͨ������ (Not used by FatFs)
#define CTRL_POWER				5		// ��ȡ/���õ�Դ״̬
#define CTRL_LOCK					6		// ����/���������Ƴ�
#define CTRL_EJECT				7		// ��������
#define CTRL_FORMAT				8		// ��ý���ϴ��������ʽ

// MMC/SDC�ض���ioctl����
#define MMC_GET_TYPE			10	// ��ȡ������
#define MMC_GET_CSD				11	// ��ȡCSD
#define MMC_GET_CID				12	// ��ȡCID
#define MMC_GET_OCR				13	// ��ȡOCR
#define MMC_GET_SDSTAT		14	// ��ȡSD״̬
#define ISDIO_READ				55	// ��SD iSDIO�Ĵ�����ȡ����
#define ISDIO_WRITE				56	// ��SD iSDIO�Ĵ���д������
#define ISDIO_MRITE				57	// ����д��SD iSDIO�Ĵ���������

// ATA/CF�ض���ioctl����
#define ATA_GET_REV				20	// ��ȡF/W revision
#define ATA_GET_MODEL			21	// ��ȡ�ͺ�����
#define ATA_GET_SN				22	// ��ȡ���к�

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------
// ��������
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
