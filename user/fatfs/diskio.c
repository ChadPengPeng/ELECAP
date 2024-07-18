/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			
#include "bsp_sdio.h"
#include "dev_w25qxx_lz.h"
#include "usbh_diskio.h"

#define SD_CARD	 	0  		// SD��,���Ϊ0
#define EX_FLASH 	1			// �ⲿspi flash,���Ϊ1
#define USB_DISK	3			// U��,���Ϊ3

// ����W25Q128
// ǰ8M�ֽڸ�fatfs��,8M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��6.01M.ʣ�ಿ��1.99M,���ͻ��Լ���	 
#define SPI_FLASH_SECTOR_SIZE 	512	
#define SPI_FLASH_SECTOR_COUNT 	1024*8*2	// W25Q128,ǰ8M�ֽڸ�FATFSռ��	
#define SPI_FLASH_BLOCK_SIZE   	8     		// ÿ��BLOCK��8������	

FATFS fatfs_sd;              // SD���߼��������Ĺ�����
FATFS fatfs_flash;           // FLASH�߼��������Ĺ�����
FATFS fatfs_usb;             // USB�߼��������Ĺ�����

FIL file_sd;                 // SD���ļ�����
FIL file_flash;              // FLASH�ļ�����
FIL file_usb;                // USB�ļ�����
//-----------------------------------------------------------------
// DWORD get_fattime (void)
//-----------------------------------------------------------------
// 
// ��������: �û����庯����Ϊfatfsģ��ͣ����ǰʱ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DWORD get_fattime (void)
{				 
	return 0;
}	


//-----------------------------------------------------------------
// DSTATUS disk_status (BYTE pdrv)
//-----------------------------------------------------------------
// 
// ��������: ��ô���״̬
// ��ڲ���: BYTE pdrv�����ڱ�ʶ��������������������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DSTATUS disk_status(BYTE pdrv)
{ 
	return RES_OK;
} 

//-----------------------------------------------------------------
// DSTATUS disk_status (BYTE pdrv)
//-----------------------------------------------------------------
// 
// ��������: ��ʼ������
// ��ڲ���: BYTE pdrv�����ڱ�ʶ��������������������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DSTATUS disk_initialize(BYTE pdrv)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD:			// SD��
			res=SD_Init();	// SD����ʼ�� 
  		break;
		case EX_FLASH:		// �ⲿflash
			W25QXX_Init();  // W25QXX��ʼ��
 			break;
		case USB_DISK:		// U��  
	  		res=USBH_initialize();// U�����ӳɹ�,�򷵻�0.���򷵻�1	
			break;
		default:
			res=1; 
	}		 
	if(res)
		return  STA_NOINIT;
	else 
		return 0; 				// ��ʼ���ɹ� 
} 

//-----------------------------------------------------------------
// DRESULT disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
//-----------------------------------------------------------------
// 
// ��������: ������
// ��ڲ���: BYTE pdrv�����̱��0~9
//					 BYTE *buff�����ݽ��ջ����׵�ַ
//					 DWORD sector��������ַ
//					 UINT count����Ҫ��ȡ��������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DRESULT disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
{
	u8 res=0; 
  if (!count)
		return RES_PARERR;// count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD:			// SD��
			SD_ReadDisk(buff,sector,count);	 
			break;
		case EX_FLASH:		// �ⲿflash
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*SPI_FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		case USB_DISK:	// U��    
			res=USBH_read(buff,sector,count);  								    
			break;
		default:
			res=1; 
	}
   // ������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	   
}

//-----------------------------------------------------------------
// DRESULT disk_write (BYTE pdrv,const BYTE *buff,DWORD sector,UINT count)
//-----------------------------------------------------------------
// 
// ��������: д����
// ��ڲ���: BYTE pdrv�����̱��0~9
//					 BYTE *buff�����������׵�ַ
//					 DWORD sector��������ַ
//					 UINT count����Ҫд��������� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DRESULT disk_write (BYTE pdrv,const BYTE *buff,DWORD sector,UINT count)
{
	u8 res=0;  
  if (!count)
		return RES_PARERR;// count���ܵ���0�����򷵻ز�������		 	 
	switch(pdrv)
	{
		case SD_CARD:	// SD��
			SD_WriteDisk((u8*)buff,sector,count);
			break;
		case EX_FLASH:// �ⲿflash
			for(;count>0;count--)
			{										    
				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
				sector++;
				buff+=SPI_FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		case USB_DISK:	// U�� 
			res=USBH_write((u8*)buff,sector,count); 
			break;
		default:
			res=1; 
	}
    // ������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	
} 

//-----------------------------------------------------------------
// DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff)
//-----------------------------------------------------------------
// 
// ��������: ����������Ļ��
// ��ڲ���: BYTE pdrv�����̱��0~9
//					 BYTE cmd�����ƴ���
//					 void *buff������/���ջ�����ָ�� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff)
{
	DRESULT res;						  			     
	if(pdrv==SD_CARD)// SD��
	{
		switch(cmd)
		{
			case CTRL_SYNC:
					res = RES_OK; 
					break;	 
			case GET_SECTOR_SIZE:
					*(DWORD*)buff = 512; 
					res = RES_OK;
					break;	 
			case GET_BLOCK_SIZE:
				  *(WORD*)buff = sd_info.BlockSize;
					res = RES_OK;
					break;	 
			case GET_SECTOR_COUNT:
					res = RES_OK;
					break;
			default:
					res = RES_PARERR;
					break;
		}
	}
	else if(pdrv==EX_FLASH)	// �ⲿFLASH  
	{
		switch(cmd)
		{
			case CTRL_SYNC:
					res = RES_OK; 
					break;	 
			case GET_SECTOR_SIZE:
					*(WORD*)buff = SPI_FLASH_SECTOR_SIZE;
					res = RES_OK;
					break;	 
			case GET_BLOCK_SIZE:
					*(WORD*)buff = SPI_FLASH_BLOCK_SIZE;
					res = RES_OK;
					break;	 
			case GET_SECTOR_COUNT:
					*(DWORD*)buff = SPI_FLASH_SECTOR_COUNT;
					res = RES_OK;
					break;
			default:
					res = RES_PARERR;
					break;
		}
	}
	else 
		res=RES_ERROR;// �����Ĳ�֧��
	
  return res;
} 

Disk_drvTypeDef disk = {{0},{0},{0},0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Links a compatible diskio driver/lun id and increments the number of active
  *         linked drivers.
  * @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits.
  * @param  drv: pointer to the disk IO Driver structure
  * @param  path: pointer to the logical drive path
  * @param  lun : only used for USB Key Disk to add multi-lun management
            else the parameter must be equal to 0
  * @retval Returns 0 in case of success, otherwise 1.
  */
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef *drv, char *path, uint8_t lun)
{
  uint8_t ret = 1;
  uint8_t DiskNum = 0;

  if(disk.nbr < _VOLUMES)
  {
    disk.is_initialized[disk.nbr] = 0;
    disk.drv[disk.nbr] = drv;
    disk.lun[disk.nbr] = lun;
    DiskNum = disk.nbr++;
    path[0] = DiskNum + '0';
    path[1] = ':';
    path[2] = '/';
    path[3] = 0;
    ret = 0;
  }

  return ret;
}

/**
  * @brief  Links a compatible diskio driver and increments the number of active
  *         linked drivers.
  * @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits
  * @param  drv: pointer to the disk IO Driver structure
  * @param  path: pointer to the logical drive path
  * @retval Returns 0 in case of success, otherwise 1.
  */
uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path)
{
  return FATFS_LinkDriverEx(drv, path, 0);
}

/**
  * @brief  Unlinks a diskio driver and decrements the number of active linked
  *         drivers.
  * @param  path: pointer to the logical drive path
  * @param  lun : not used
  * @retval Returns 0 in case of success, otherwise 1.
  */
uint8_t FATFS_UnLinkDriverEx(char *path, uint8_t lun)
{
  uint8_t DiskNum = 0;
  uint8_t ret = 1;

  if(disk.nbr >= 1)
  {
    DiskNum = path[0] - '0';
    if(disk.drv[DiskNum] != 0)
    {
      disk.drv[DiskNum] = 0;
      disk.lun[DiskNum] = 0;
      disk.nbr--;
      ret = 0;
    }
  }

  return ret;
}

/**
  * @brief  Unlinks a diskio driver and decrements the number of active linked
  *         drivers.
  * @param  path: pointer to the logical drive path
  * @retval Returns 0 in case of success, otherwise 1.
  */
uint8_t FATFS_UnLinkDriver(char *path)
{
  return FATFS_UnLinkDriverEx(path, 0);
}

/**
  * @brief  Gets number of linked drivers to the FatFs module.
  * @param  None
  * @retval Number of attached drivers.
  */
uint8_t FATFS_GetAttachedDriversNbr(void)
{
  return disk.nbr;
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
