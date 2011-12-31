#ifndef _GB_HEADER_
#define _GB_HEADER_

#define FILE_FORMAT_NAME "NINTENDO GameBoy ROM"

/* OFFSET */
#define HEADER_OFFSET 0x100

/* NAME */
#define ROM_NAME_LEN 16

/* NINTENDO LOGO */
static const unsigned char s_u8NintendoLogo[] =
{
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};
#define NINTENDO_LOGO_LEN sizeof(s_u8NintendoLogo)

/* Country Code */
#define CC_JAPANESE      0
#define CC_NON_JAPANESE  1

/* Licensee Code */
#define LC_SUPER_GAMEBOY 0x33
#define LC_ACCOLADE      0x79
#define LC_KONAMI        0xA4

/* Cartridge Type */
typedef enum ECartridgeType
{
  ROM_ONLY,
  ROM_MBC1,
  ROM_MBC1_RAM,
  ROM_MBC1_RAM_BATTERY,
  ROM_MBC2,
  ROM_MBC2_BATTERY
} TCartridgeType;
static const char *s_pszCartridgeType[] =
{
  "ROM ONLY",
  "ROM+MBC1",
  "ROM+MBC1+RAM",
  "ROM+MBC1+RAM+BATTERY",
  "ROM+MBC2",
  "ROM+MBC2+BATTERY"
};

#define BANK_SIZE_BIT 256
#define BANK_SIZE_BYTE 32

/* ROM */
static unsigned int s_uiRomSize[] =
{
  32, 64, 128, 256, 512, 1024, 2048, 4096
};

/* RAM */
static unsigned int s_uiRamSize[] =
{
  0, 2, 8, 32
};

/* HEADER */
#define GAMEBOY_HEADER_OFFSET 0x100
#pragma pack(push, 1)
typedef struct SGameBoyRom
{
  unsigned char   u8Start[4];
  unsigned char   u8NintendoLogo[NINTENDO_LOGO_LEN];
  char            s8RomName[ROM_NAME_LEN];
  unsigned char   u8LicenceCode[2];
  unsigned char   u8SgbFeatures;
  unsigned char   u8CartridgeType;
  unsigned char   u8RomSize;
  unsigned char   u8RamSize;
  unsigned char   u8CountryCode;
  unsigned char   u8LicenseeCode;
  unsigned char   u8VersionNumber;
  unsigned char   u8ComplementCheck;
  unsigned short  u16Checksum;
} TGameBoyRom;
#pragma pack(pop)

typedef struct    SRegisterName
{
  unsigned short  u16Addr;
  char const*     szName;
}                 TRegisterName;

#endif /* _GB_HEADER_ */
