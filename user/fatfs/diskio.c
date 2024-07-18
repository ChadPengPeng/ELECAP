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

#define SD_CARD	 	0  		// SD卡,卷标为0
#define EX_FLASH 	1			// 外部spi flash,卷标为1
#define USB_DISK	3			// U盘,卷标为3

// 对于W25Q128
// 前8M字节给fatfs用,8M字节后,用于存放字库,字库占用6.01M.剩余部分1.99M,给客户自己用	 
#define SPI_FLASH_SECTOR_SIZE 	512	
#define SPI_FLASH_SECTOR_COUNT 	1024*8*2	// W25Q128,前8M字节给FATFS占用	
#define SPI_FLASH_BLOCK_SIZE   	8     		// 每个BLOCK有8个扇区	

FATFS fatfs_sd;              // SD卡逻辑驱动器的工作区
FATFS fatfs_flash;           // FLASH逻辑驱动器的工作区
FATFS fatfs_usb;             // USB逻辑驱动器的工作区

FIL file_sd;                 // SD卡文件对象
FIL file_flash;              // FLASH文件对象
FIL file_usb;                // USB文件对象
//-----------------------------------------------------------------
// DWORD get_fattime (void)
//-----------------------------------------------------------------
// 
// 函数功能: 用户定义函数，为fatfs模块停工当前时间
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
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
// 函数功能: 获得磁盘状态
// 入口参数: BYTE pdrv：用于标识驱动器的物理驱动器号
// 返 回 值: 无
// 注意事项: 无
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
// 函数功能: 初始化磁盘
// 入口参数: BYTE pdrv：用于标识驱动器的物理驱动器号
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
DSTATUS disk_initialize(BYTE pdrv)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD:			// SD卡
			res=SD_Init();	// SD卡初始化 
  		break;
		case EX_FLASH:		// 外部flash
			W25QXX_Init();  // W25QXX初始化
 			break;
		case USB_DISK:		// U盘  
	  		res=USBH_initialize();// U盘连接成功,则返回0.否则返回1	
			break;
		default:
			res=1; 
	}		 
	if(res)
		return  STA_NOINIT;
	else 
		return 0; 				// 初始化成功 
} 

//-----------------------------------------------------------------
// DRESULT disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
//-----------------------------------------------------------------
// 
// 函数功能: 读扇区
// 入口参数: BYTE pdrv：磁盘编号0~9
//					 BYTE *buff：数据接收缓冲首地址
//					 DWORD sector：扇区地址
//					 UINT count：需要读取的扇区数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
DRESULT disk_read (BYTE pdrv,BYTE *buff,DWORD sector,UINT count)
{
	u8 res=0; 
  if (!count)
		return RES_PARERR;// count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD:			// SD卡
			SD_ReadDisk(buff,sector,count);	 
			break;
		case EX_FLASH:		// 外部flash
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*SPI_FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		case USB_DISK:	// U盘    
			res=USBH_read(buff,sector,count);  								    
			break;
		default:
			res=1; 
	}
   // 处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res==0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	   
}

//-----------------------------------------------------------------
// DRESULT disk_write (BYTE pdrv,const BYTE *buff,DWORD sector,UINT count)
//-----------------------------------------------------------------
// 
// 函数功能: 写扇区
// 入口参数: BYTE pdrv：磁盘编号0~9
//					 BYTE *buff：发送数据首地址
//					 DWORD sector：扇区地址
//					 UINT count：需要写入的扇区数 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
DRESULT disk_write (BYTE pdrv,const BYTE *buff,DWORD sector,UINT count)
{
	u8 res=0;  
  if (!count)
		return RES_PARERR;// count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD:	// SD卡
			SD_WriteDisk((u8*)buff,sector,count);
			break;
		case EX_FLASH:// 外部flash
			for(;count>0;count--)
			{										    
				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
				sector++;
				buff+=SPI_FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		case USB_DISK:	// U盘 
			res=USBH_write((u8*)buff,sector,count); 
			break;
		default:
			res=1; 
	}
    // 处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	
} 

//-----------------------------------------------------------------
// DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff)
//-----------------------------------------------------------------
// 
// 函数功能: 其他表参数的获得
// 入口参数: BYTE pdrv：磁盘编号0~9
//					 BYTE cmd：控制代码
//					 void *buff：发送/接收缓冲区指针 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
DRESULT disk_ioctl (BYTE pdrv,BYTE cmd,void *buff)
{
	DRESULT res;						  			     
	if(pdrv==SD_CARD)// SD卡
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
	else if(pdrv==EX_FLASH)	// 外部FLASH  
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
		res=RES_ERROR;// 其他的不支持
	
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
