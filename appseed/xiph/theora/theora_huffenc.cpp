#include "framework.h"


/*The default Huffman codes used for VP3.1.*/
const th_huff_code TH_VP31_HUFF_CODES[TH_NHUFFMAN_TABLES][TH_NDCT_TOKENS]={
  {
    {0x002D, 6},{0x0026, 7},{0x0166, 9},{0x004E, 8},
    {0x02CE,10},{0x059E,11},{0x027D,11},{0x0008, 5},
    {0x04F9,12},{0x000F, 4},{0x000E, 4},{0x001B, 5},
    {0x0006, 4},{0x0008, 4},{0x0005, 4},{0x001A, 5},
    {0x0015, 5},{0x0007, 4},{0x000C, 4},{0x0001, 3},
    {0x0000, 3},{0x0009, 4},{0x0017, 5},{0x0029, 6},
    {0x0028, 6},{0x00B2, 8},{0x04F8,12},{0x059F,11},
    {0x009E, 9},{0x013F,10},{0x0012, 6},{0x0058, 7}
  },
  {
    {0x0010, 5},{0x0047, 7},{0x01FF, 9},{0x008C, 8},
    {0x03FC,10},{0x046A,11},{0x0469,11},{0x0022, 6},
    {0x11A1,13},{0x000E, 4},{0x000D, 4},{0x0004, 4},
    {0x0005, 4},{0x0009, 4},{0x0006, 4},{0x001E, 5},
    {0x0016, 5},{0x0007, 4},{0x000C, 4},{0x0001, 3},
    {0x0000, 3},{0x000A, 4},{0x0017, 5},{0x007D, 7},
    {0x007E, 7},{0x011B, 9},{0x08D1,12},{0x03FD,10},
    {0x046B,11},{0x11A0,13},{0x007C, 7},{0x00FE, 8}
  },
  {
    {0x0016, 5},{0x0020, 6},{0x0086, 8},{0x0087, 8},
    {0x0367,10},{0x06CC,11},{0x06CB,11},{0x006E, 7},
    {0x366D,14},{0x000F, 4},{0x000E, 4},{0x0004, 4},
    {0x0005, 4},{0x000A, 4},{0x0006, 4},{0x001A, 5},
    {0x0011, 5},{0x0007, 4},{0x000C, 4},{0x0001, 3},
    {0x0000, 3},{0x0009, 4},{0x0017, 5},{0x006F, 7},
    {0x006D, 7},{0x0364,10},{0x0D9A,12},{0x06CA,11},
    {0x1B37,13},{0x366C,14},{0x0042, 7},{0x00D8, 8}
  },
  {
    {0x0000, 4},{0x002D, 6},{0x00F7, 8},{0x0058, 7},
    {0x0167, 9},{0x02CB,10},{0x02CA,10},{0x000E, 6},
    {0x1661,13},{0x0003, 3},{0x0002, 3},{0x0008, 4},
    {0x0009, 4},{0x000D, 4},{0x0002, 4},{0x001F, 5},
    {0x0017, 5},{0x0001, 4},{0x000C, 4},{0x000E, 4},
    {0x000A, 4},{0x0006, 5},{0x0078, 7},{0x000F, 6},
    {0x007A, 7},{0x0164, 9},{0x0599,11},{0x02CD,10},
    {0x0B31,12},{0x1660,13},{0x0079, 7},{0x00F6, 8}
  },
  {
    {0x0003, 4},{0x003C, 6},{0x000F, 7},{0x007A, 7},
    {0x001D, 8},{0x0020, 9},{0x0072,10},{0x0006, 6},
    {0x0399,13},{0x0004, 3},{0x0005, 3},{0x0005, 4},
    {0x0006, 4},{0x000E, 4},{0x0004, 4},{0x0000, 4},
    {0x0019, 5},{0x0002, 4},{0x000D, 4},{0x0007, 4},
    {0x001F, 5},{0x0030, 6},{0x0011, 8},{0x0031, 6},
    {0x0005, 6},{0x0021, 9},{0x00E7,11},{0x0038, 9},
    {0x01CD,12},{0x0398,13},{0x007B, 7},{0x0009, 7}
  },
  {
    {0x0009, 4},{0x0002, 5},{0x0074, 7},{0x0007, 6},
    {0x00EC, 8},{0x00D1, 9},{0x01A6,10},{0x0006, 6},
    {0x0D21,13},{0x0005, 3},{0x0006, 3},{0x0008, 4},
    {0x0007, 4},{0x000F, 4},{0x0004, 4},{0x0000, 4},
    {0x001C, 5},{0x0002, 4},{0x0005, 4},{0x0003, 4},
    {0x000C, 5},{0x0035, 7},{0x01A7,10},{0x001B, 6},
    {0x0077, 7},{0x01A5,10},{0x0349,11},{0x00D0, 9},
    {0x0691,12},{0x0D20,13},{0x0075, 7},{0x00ED, 8}
  },
  {
    {0x000A, 4},{0x000C, 5},{0x0012, 6},{0x001B, 6},
    {0x00B7, 8},{0x016C, 9},{0x0099, 9},{0x005A, 7},
    {0x16D8,13},{0x0007, 3},{0x0006, 3},{0x0009, 4},
    {0x0008, 4},{0x0000, 3},{0x0005, 4},{0x0017, 5},
    {0x000E, 5},{0x0002, 4},{0x0003, 4},{0x000F, 5},
    {0x001A, 6},{0x004D, 8},{0x2DB3,14},{0x002C, 6},
    {0x0011, 6},{0x02DA,10},{0x05B7,11},{0x0098, 9},
    {0x0B6D,12},{0x2DB2,14},{0x0010, 6},{0x0027, 7}
  },
  {
    {0x000D, 4},{0x000F, 5},{0x001D, 6},{0x0008, 5},
    {0x0051, 7},{0x0056, 8},{0x00AF, 9},{0x002A, 7},
    {0x148A,13},{0x0007, 3},{0x0000, 2},{0x0008, 4},
    {0x0009, 4},{0x000C, 4},{0x0006, 4},{0x0017, 5},
    {0x000B, 5},{0x0016, 5},{0x0015, 5},{0x0009, 5},
    {0x0050, 7},{0x00AE, 9},{0x2917,14},{0x001C, 6},
    {0x0014, 6},{0x0290,10},{0x0523,11},{0x0149, 9},
    {0x0A44,12},{0x2916,14},{0x0053, 7},{0x00A5, 8}
  },
  {
    {0x0001, 4},{0x001D, 6},{0x00F5, 8},{0x00F4, 8},
    {0x024D,10},{0x0499,11},{0x0498,11},{0x0001, 5},
    {0x0021, 6},{0x0006, 3},{0x0005, 3},{0x0006, 4},
    {0x0005, 4},{0x0002, 4},{0x0007, 5},{0x0025, 6},
    {0x007B, 7},{0x001C, 6},{0x0020, 6},{0x000D, 6},
    {0x0048, 7},{0x0092, 8},{0x0127, 9},{0x000E, 4},
    {0x0004, 4},{0x0011, 5},{0x000C, 6},{0x003C, 6},
    {0x000F, 5},{0x0000, 5},{0x001F, 5},{0x0013, 5}
  },
  {
    {0x0005, 4},{0x003C, 6},{0x0040, 7},{0x000D, 7},
    {0x0031, 9},{0x0061,10},{0x0060,10},{0x0002, 5},
    {0x00F5, 8},{0x0006, 3},{0x0005, 3},{0x0007, 4},
    {0x0006, 4},{0x0002, 4},{0x0009, 5},{0x0025, 6},
    {0x0007, 6},{0x0021, 6},{0x0024, 6},{0x0010, 6},
    {0x0041, 7},{0x00F4, 8},{0x0019, 8},{0x000E, 4},
    {0x0003, 4},{0x0011, 5},{0x0011, 6},{0x003F, 6},
    {0x003E, 6},{0x007B, 7},{0x0000, 4},{0x0013, 5}
  },
  {
    {0x000A, 4},{0x0007, 5},{0x0001, 6},{0x0009, 6},
    {0x0131, 9},{0x0261,10},{0x0260,10},{0x0015, 6},
    {0x0001, 7},{0x0007, 3},{0x0006, 3},{0x0008, 4},
    {0x0007, 4},{0x0006, 4},{0x0012, 5},{0x002F, 6},
    {0x0014, 6},{0x0027, 6},{0x002D, 6},{0x0016, 6},
    {0x004D, 7},{0x0099, 8},{0x0000, 7},{0x0004, 4},
    {0x0001, 4},{0x0005, 5},{0x0017, 6},{0x002E, 6},
    {0x002C, 6},{0x0008, 6},{0x0006, 5},{0x0001, 5}
  },
  {
    {0x0000, 3},{0x000E, 5},{0x0017, 6},{0x002A, 6},
    {0x0010, 7},{0x00F9,10},{0x00F8,10},{0x001E, 7},
    {0x003F, 8},{0x0007, 3},{0x0006, 3},{0x0009, 4},
    {0x0008, 4},{0x0006, 4},{0x000F, 5},{0x0005, 5},
    {0x0016, 6},{0x0029, 6},{0x002B, 6},{0x0015, 6},
    {0x0050, 7},{0x0011, 7},{0x007D, 9},{0x0004, 4},
    {0x0017, 5},{0x0006, 5},{0x0014, 6},{0x002C, 6},
    {0x002D, 6},{0x000E, 6},{0x0009, 6},{0x0051, 7}
  },
  {
    {0x0002, 3},{0x0018, 5},{0x002F, 6},{0x000D, 5},
    {0x0053, 7},{0x0295,10},{0x0294,10},{0x00A4, 8},
    {0x007C, 8},{0x0000, 2},{0x0007, 3},{0x0009, 4},
    {0x0008, 4},{0x001B, 5},{0x000C, 5},{0x0028, 6},
    {0x006A, 7},{0x001E, 6},{0x001D, 6},{0x0069, 7},
    {0x00D7, 8},{0x007D, 8},{0x014B, 9},{0x0019, 5},
    {0x0016, 5},{0x002E, 6},{0x001C, 6},{0x002B, 6},
    {0x002A, 6},{0x0068, 7},{0x003F, 7},{0x00D6, 8}
  },
  {
    {0x0002, 3},{0x001B, 5},{0x000C, 5},{0x0018, 5},
    {0x0029, 6},{0x007F, 8},{0x02F0,10},{0x0198, 9},
    {0x0179, 9},{0x0000, 2},{0x0007, 3},{0x0009, 4},
    {0x0008, 4},{0x001A, 5},{0x000D, 5},{0x002A, 6},
    {0x0064, 7},{0x001E, 6},{0x0067, 7},{0x005F, 7},
    {0x00CD, 8},{0x007E, 8},{0x02F1,10},{0x0016, 5},
    {0x000E, 5},{0x002E, 6},{0x0065, 7},{0x002B, 6},
    {0x0028, 6},{0x003E, 7},{0x00BD, 8},{0x0199, 9}
  },
  {
    {0x0002, 3},{0x0007, 4},{0x0016, 5},{0x0006, 4},
    {0x0036, 6},{0x005C, 7},{0x015D, 9},{0x015C, 9},
    {0x02BF,10},{0x0000, 2},{0x0007, 3},{0x0009, 4},
    {0x0008, 4},{0x0018, 5},{0x0034, 6},{0x002A, 6},
    {0x005E, 7},{0x006A, 7},{0x0064, 7},{0x005D, 7},
    {0x00CB, 8},{0x00AD, 8},{0x02BE,10},{0x0014, 5},
    {0x0033, 6},{0x006E, 7},{0x005F, 7},{0x006F, 7},
    {0x006B, 7},{0x00CA, 8},{0x00AC, 8},{0x015E, 9}
  },
  {
    {0x000F, 4},{0x001D, 5},{0x0018, 5},{0x000B, 4},
    {0x0019, 5},{0x0029, 6},{0x00D6, 8},{0x0551,11},
    {0x0AA1,12},{0x0001, 2},{0x0000, 2},{0x0009, 4},
    {0x0008, 4},{0x001B, 5},{0x0038, 6},{0x0028, 6},
    {0x0057, 7},{0x006A, 7},{0x0068, 7},{0x0056, 7},
    {0x00E5, 8},{0x0155, 9},{0x0AA0,12},{0x0073, 7},
    {0x0069, 7},{0x00D7, 8},{0x00AB, 8},{0x00E4, 8},
    {0x00A9, 8},{0x0151, 9},{0x0150, 9},{0x02A9,10}
  },
  {
    {0x0008, 5},{0x0025, 7},{0x017A, 9},{0x02F7,10},
    {0x0BDB,12},{0x17B4,13},{0x2F6B,14},{0x001D, 5},
    {0x2F6A,14},{0x0008, 4},{0x0007, 4},{0x0001, 4},
    {0x0002, 4},{0x000A, 4},{0x0006, 4},{0x0000, 4},
    {0x001C, 5},{0x0009, 4},{0x000D, 4},{0x000F, 4},
    {0x000C, 4},{0x0003, 4},{0x000A, 5},{0x0016, 5},
    {0x0013, 6},{0x005D, 7},{0x0024, 7},{0x00BC, 8},
    {0x005C, 7},{0x05EC,11},{0x000B, 5},{0x005F, 7}
  },
  {
    {0x000F, 5},{0x0010, 6},{0x004B, 8},{0x00C6, 8},
    {0x031D,10},{0x0C71,12},{0x0C70,12},{0x0001, 4},
    {0x0C73,12},{0x0008, 4},{0x0009, 4},{0x0002, 4},
    {0x0003, 4},{0x000B, 4},{0x0006, 4},{0x0000, 4},
    {0x001C, 5},{0x0005, 4},{0x000D, 4},{0x000F, 4},
    {0x000A, 4},{0x0019, 5},{0x0013, 6},{0x001D, 5},
    {0x0030, 6},{0x0062, 7},{0x0024, 7},{0x004A, 8},
    {0x018F, 9},{0x0C72,12},{0x000E, 5},{0x0011, 6}
  },
  {
    {0x001B, 5},{0x0003, 6},{0x008D, 8},{0x0040, 7},
    {0x0239,10},{0x0471,11},{0x08E0,12},{0x0003, 4},
    {0x11C3,13},{0x000A, 4},{0x0009, 4},{0x0004, 4},
    {0x0005, 4},{0x000E, 4},{0x0007, 4},{0x0001, 4},
    {0x001E, 5},{0x0006, 4},{0x000C, 4},{0x000B, 4},
    {0x0002, 4},{0x0000, 5},{0x0041, 7},{0x001F, 5},
    {0x0022, 6},{0x0002, 6},{0x008F, 8},{0x008C, 8},
    {0x011D, 9},{0x11C2,13},{0x001A, 5},{0x0021, 6}
  },
  {
    {0x001F, 5},{0x0003, 6},{0x0003, 7},{0x0043, 7},
    {0x000B, 9},{0x0015,10},{0x0051,12},{0x0003, 4},
    {0x0050,12},{0x000D, 4},{0x000C, 4},{0x0004, 4},
    {0x0006, 4},{0x000E, 4},{0x000A, 4},{0x0001, 4},
    {0x001E, 5},{0x0005, 4},{0x0009, 4},{0x0007, 4},
    {0x0011, 5},{0x0002, 6},{0x0004, 8},{0x0002, 4},
    {0x002D, 6},{0x0020, 6},{0x0042, 7},{0x0001, 7},
    {0x0000, 7},{0x0029,11},{0x0017, 5},{0x002C, 6}
  },
  {
    {0x0003, 4},{0x001F, 6},{0x003A, 7},{0x005D, 7},
    {0x0173, 9},{0x02E4,10},{0x172D,13},{0x0004, 4},
    {0x172C,13},{0x000F, 4},{0x000E, 4},{0x0009, 4},
    {0x0008, 4},{0x000C, 4},{0x000A, 4},{0x0001, 4},
    {0x0016, 5},{0x0002, 4},{0x0005, 4},{0x001A, 5},
    {0x002F, 6},{0x0038, 7},{0x05CA,11},{0x0006, 4},
    {0x0037, 6},{0x001E, 6},{0x003B, 7},{0x0039, 7},
    {0x00B8, 8},{0x0B97,12},{0x0000, 4},{0x0036, 6}
  },
  {
    {0x0006, 4},{0x0037, 6},{0x005D, 7},{0x000C, 6},
    {0x00B9, 8},{0x02E3,10},{0x05C4,11},{0x0004, 4},
    {0x1715,13},{0x0000, 3},{0x000F, 4},{0x0008, 4},
    {0x0007, 4},{0x000C, 4},{0x0009, 4},{0x001D, 5},
    {0x0016, 5},{0x001C, 5},{0x001A, 5},{0x000B, 5},
    {0x005E, 7},{0x0170, 9},{0x1714,13},{0x000A, 4},
    {0x000A, 5},{0x0036, 6},{0x005F, 7},{0x001B, 7},
    {0x001A, 7},{0x0B8B,12},{0x0002, 4},{0x0007, 5}
  },
  {
    {0x000C, 4},{0x000B, 5},{0x0079, 7},{0x0022, 6},
    {0x00F0, 8},{0x0119, 9},{0x0230,10},{0x001D, 5},
    {0x08C4,12},{0x0001, 3},{0x0000, 3},{0x000A, 4},
    {0x0009, 4},{0x000B, 4},{0x0007, 4},{0x001C, 5},
    {0x003D, 6},{0x000D, 5},{0x0008, 5},{0x0015, 6},
    {0x008D, 8},{0x118B,13},{0x118A,13},{0x000D, 4},
    {0x0010, 5},{0x0009, 5},{0x0014, 6},{0x0047, 7},
    {0x00F1, 8},{0x0463,11},{0x001F, 5},{0x000C, 5}
  },
  {
    {0x0000, 3},{0x001A, 5},{0x0033, 6},{0x000C, 5},
    {0x0046, 7},{0x01E3, 9},{0x03C5,10},{0x0017, 5},
    {0x1E21,13},{0x0002, 3},{0x0001, 3},{0x0009, 4},
    {0x000A, 4},{0x0007, 4},{0x001B, 5},{0x003D, 6},
    {0x001B, 6},{0x0022, 6},{0x0079, 7},{0x00F0, 8},
    {0x1E20,13},{0x1E23,13},{0x1E22,13},{0x000E, 4},
    {0x0016, 5},{0x0018, 5},{0x0032, 6},{0x001A, 6},
    {0x0047, 7},{0x0789,11},{0x001F, 5},{0x0010, 5}
  },
  {
    {0x001D, 5},{0x0061, 7},{0x004E, 8},{0x009E, 9},
    {0x027C,11},{0x09F5,13},{0x09F4,13},{0x0003, 4},
    {0x0060, 7},{0x0000, 3},{0x000F, 4},{0x000B, 4},
    {0x000A, 4},{0x0009, 4},{0x0005, 4},{0x000D, 5},
    {0x0031, 6},{0x0008, 5},{0x0038, 6},{0x0012, 6},
    {0x0026, 7},{0x013F,10},{0x04FB,12},{0x000D, 4},
    {0x0002, 4},{0x000C, 5},{0x0039, 6},{0x001C, 6},
    {0x000F, 5},{0x001D, 6},{0x0008, 4},{0x0019, 5}
  },
  {
    {0x0007, 4},{0x0019, 6},{0x00AB, 8},{0x00AA, 8},
    {0x0119,10},{0x0461,12},{0x0460,12},{0x001B, 5},
    {0x0047, 8},{0x0001, 3},{0x0000, 3},{0x000C, 4},
    {0x000B, 4},{0x0009, 4},{0x0005, 4},{0x000D, 5},
    {0x0035, 6},{0x003D, 6},{0x003C, 6},{0x0018, 6},
    {0x0022, 7},{0x008D, 9},{0x0231,11},{0x000E, 4},
    {0x001F, 5},{0x0009, 5},{0x002B, 6},{0x0010, 6},
    {0x0034, 6},{0x0054, 7},{0x0008, 4},{0x0014, 5}
  },
  {
    {0x000C, 4},{0x0005, 5},{0x0008, 6},{0x005B, 7},
    {0x004D, 9},{0x0131,11},{0x0261,12},{0x001A, 5},
    {0x0012, 7},{0x0000, 3},{0x000F, 4},{0x000A, 4},
    {0x0009, 4},{0x0006, 4},{0x001B, 5},{0x0006, 5},
    {0x001C, 6},{0x002C, 6},{0x0015, 6},{0x005A, 7},
    {0x0027, 8},{0x0099,10},{0x0260,12},{0x000E, 4},
    {0x0004, 4},{0x000F, 5},{0x0007, 5},{0x001D, 6},
    {0x000B, 5},{0x0014, 6},{0x0008, 4},{0x0017, 5}
  },
  {
    {0x000F, 4},{0x0013, 5},{0x0075, 7},{0x0024, 6},
    {0x0095, 8},{0x0251,10},{0x04A0,11},{0x0010, 5},
    {0x00C8, 8},{0x0002, 3},{0x0001, 3},{0x0001, 4},
    {0x0000, 4},{0x001A, 5},{0x0011, 5},{0x002C, 6},
    {0x0065, 7},{0x0074, 7},{0x004B, 7},{0x00C9, 8},
    {0x0129, 9},{0x0943,12},{0x0942,12},{0x0003, 3},
    {0x000A, 4},{0x001C, 5},{0x0018, 5},{0x0033, 6},
    {0x0017, 5},{0x002D, 6},{0x001B, 5},{0x003B, 6}
  },
  {
    {0x0003, 3},{0x001A, 5},{0x002D, 6},{0x0038, 6},
    {0x0028, 7},{0x0395,10},{0x0E51,12},{0x0037, 6},
    {0x00E4, 8},{0x0001, 3},{0x0000, 3},{0x001F, 5},
    {0x001E, 5},{0x0017, 5},{0x003A, 6},{0x0073, 7},
    {0x002A, 7},{0x002B, 7},{0x0029, 7},{0x01CB, 9},
    {0x0729,11},{0x1CA1,13},{0x1CA0,13},{0x0004, 3},
    {0x000A, 4},{0x0004, 4},{0x0018, 5},{0x0036, 6},
    {0x000B, 5},{0x002C, 6},{0x0019, 5},{0x003B, 6}
  },
  {
    {0x0004, 3},{0x0004, 4},{0x003F, 6},{0x0017, 5},
    {0x0075, 7},{0x01F5, 9},{0x07D1,11},{0x0017, 6},
    {0x01F6, 9},{0x0001, 3},{0x0000, 3},{0x001B, 5},
    {0x001A, 5},{0x000A, 5},{0x0032, 6},{0x0074, 7},
    {0x00F8, 8},{0x00F9, 8},{0x01F7, 9},{0x03E9,10},
    {0x0FA0,12},{0x1F43,13},{0x1F42,13},{0x0003, 3},
    {0x000A, 4},{0x001E, 5},{0x001C, 5},{0x003B, 6},
    {0x0018, 5},{0x0016, 6},{0x0016, 5},{0x0033, 6}
  },
  {
    {0x0004, 3},{0x0007, 4},{0x0018, 5},{0x001E, 5},
    {0x0036, 6},{0x0031, 7},{0x0177, 9},{0x0077, 7},
    {0x0176, 9},{0x0001, 3},{0x0000, 3},{0x001A, 5},
    {0x0019, 5},{0x003A, 6},{0x0019, 6},{0x005C, 7},
    {0x00BA, 8},{0x0061, 8},{0x00C1, 9},{0x0180,10},
    {0x0302,11},{0x0607,12},{0x0606,12},{0x0002, 3},
    {0x000A, 4},{0x001F, 5},{0x001C, 5},{0x0037, 6},
    {0x0016, 5},{0x0076, 7},{0x000D, 5},{0x002F, 6}
  },
  {
    {0x0000, 3},{0x000A, 4},{0x001A, 5},{0x000C, 4},
    {0x001D, 5},{0x0039, 6},{0x0078, 7},{0x005E, 7},
    {0x0393,11},{0x0002, 3},{0x0001, 3},{0x0016, 5},
    {0x000F, 5},{0x002E, 6},{0x005F, 7},{0x0073, 8},
    {0x00E5, 9},{0x01C8,10},{0x0E4A,13},{0x1C97,14},
    {0x1C96,14},{0x0E49,13},{0x0E48,13},{0x0004, 3},
    {0x0006, 4},{0x001F, 5},{0x001B, 5},{0x001D, 6},
    {0x0038, 6},{0x0038, 7},{0x003D, 6},{0x0079, 7}
  },
  {
    {0x000B, 5},{0x002B, 7},{0x0054, 8},{0x01B7, 9},
    {0x06D9,11},{0x0DB1,12},{0x0DB0,12},{0x0002, 4},
    {0x00AB, 9},{0x0009, 4},{0x000A, 4},{0x0007, 4},
    {0x0008, 4},{0x000F, 4},{0x000C, 4},{0x0003, 4},
    {0x001D, 5},{0x0004, 4},{0x000B, 4},{0x0006, 4},
    {0x001A, 5},{0x0003, 6},{0x00AA, 9},{0x0001, 4},
    {0x0000, 5},{0x0014, 6},{0x006C, 7},{0x00DA, 8},
    {0x0002, 6},{0x036D,10},{0x001C, 5},{0x0037, 6}
  },
  {
    {0x001D, 5},{0x0004, 6},{0x00B6, 8},{0x006A, 8},
    {0x05B9,11},{0x16E1,13},{0x16E0,13},{0x0007, 4},
    {0x016F, 9},{0x000C, 4},{0x000D, 4},{0x0009, 4},
    {0x0008, 4},{0x000F, 4},{0x000A, 4},{0x0003, 4},
    {0x0017, 5},{0x0002, 4},{0x0004, 4},{0x001C, 5},
    {0x002C, 6},{0x006B, 8},{0x0B71,12},{0x0005, 4},
    {0x0003, 5},{0x001B, 6},{0x005A, 7},{0x0034, 7},
    {0x0005, 6},{0x02DD,10},{0x0000, 4},{0x000C, 5}
  },
  {
    {0x0003, 4},{0x007F, 7},{0x00A1, 8},{0x00A0, 8},
    {0x020C,10},{0x0834,12},{0x106B,13},{0x0007, 4},
    {0x0082, 8},{0x000E, 4},{0x000D, 4},{0x000B, 4},
    {0x000C, 4},{0x0000, 3},{0x0009, 4},{0x0002, 4},
    {0x0011, 5},{0x001E, 5},{0x0015, 5},{0x003E, 6},
    {0x0040, 7},{0x041B,11},{0x106A,13},{0x0006, 4},
    {0x000A, 5},{0x0029, 6},{0x007E, 7},{0x0051, 7},
    {0x0021, 6},{0x0107, 9},{0x0004, 4},{0x000B, 5}
  },
  {
    {0x0007, 4},{0x001B, 6},{0x00F6, 8},{0x00E9, 8},
    {0x03A1,10},{0x0740,11},{0x0E82,12},{0x001F, 5},
    {0x01EF, 9},{0x0001, 3},{0x0002, 3},{0x000B, 4},
    {0x000C, 4},{0x000D, 4},{0x0008, 4},{0x001C, 5},
    {0x0003, 5},{0x0012, 5},{0x0002, 5},{0x0075, 7},
    {0x01D1, 9},{0x1D07,13},{0x1D06,13},{0x000A, 4},
    {0x0013, 5},{0x003B, 6},{0x001A, 6},{0x007A, 7},
    {0x003C, 6},{0x01EE, 9},{0x0000, 4},{0x000C, 5}
  },
  {
    {0x000D, 4},{0x003D, 6},{0x0042, 7},{0x0037, 7},
    {0x00D9, 9},{0x0362,11},{0x06C6,12},{0x001F, 5},
    {0x0086, 8},{0x0001, 3},{0x0002, 3},{0x000C, 4},
    {0x000B, 4},{0x000A, 4},{0x0001, 4},{0x000F, 5},
    {0x0025, 6},{0x003C, 6},{0x001A, 6},{0x0087, 8},
    {0x01B0,10},{0x0D8F,13},{0x0D8E,13},{0x000E, 4},
    {0x0013, 5},{0x000C, 5},{0x0024, 6},{0x0020, 6},
    {0x0011, 5},{0x006D, 8},{0x0000, 4},{0x000E, 5}
  },
  {
    {0x0000, 3},{0x0012, 5},{0x0076, 7},{0x0077, 7},
    {0x014D, 9},{0x0533,11},{0x14C9,13},{0x0013, 5},
    {0x00A5, 8},{0x0002, 3},{0x0003, 3},{0x000B, 4},
    {0x000C, 4},{0x0008, 4},{0x001A, 5},{0x002B, 6},
    {0x0075, 7},{0x0074, 7},{0x00A7, 8},{0x0298,10},
    {0x14C8,13},{0x14CB,13},{0x14CA,13},{0x000F, 4},
    {0x001C, 5},{0x0007, 5},{0x002A, 6},{0x0028, 6},
    {0x001B, 5},{0x00A4, 8},{0x0002, 4},{0x0006, 5}
  },
  {
    {0x0002, 3},{0x001A, 5},{0x002B, 6},{0x003A, 6},
    {0x00ED, 8},{0x0283,10},{0x0A0A,12},{0x0004, 5},
    {0x00A1, 8},{0x0004, 3},{0x0003, 3},{0x000B, 4},
    {0x000C, 4},{0x001F, 5},{0x0006, 5},{0x0077, 7},
    {0x00A3, 8},{0x00A2, 8},{0x0140, 9},{0x1417,13},
    {0x1416,13},{0x0A09,12},{0x0A08,12},{0x0000, 3},
    {0x001E, 5},{0x0007, 5},{0x002A, 6},{0x0029, 6},
    {0x001C, 5},{0x00EC, 8},{0x001B, 5},{0x0005, 5}
  },
  {
    {0x0002, 3},{0x0002, 4},{0x0018, 5},{0x001D, 5},
    {0x0035, 6},{0x00E4, 8},{0x01CF,11},{0x001D, 7},
    {0x0072, 9},{0x0004, 3},{0x0005, 3},{0x0006, 4},
    {0x0007, 4},{0x0006, 5},{0x0073, 7},{0x0038, 8},
    {0x01CE,11},{0x039B,12},{0x0398,12},{0x0733,13},
    {0x0732,13},{0x0735,13},{0x0734,13},{0x0000, 3},
    {0x001F, 5},{0x001B, 5},{0x0034, 6},{0x000F, 6},
    {0x001E, 5},{0x00E5, 8},{0x0019, 5},{0x0038, 6}
  },
  {
    {0x0016, 5},{0x0050, 7},{0x0172, 9},{0x02E7,10},
    {0x1732,13},{0x2E67,14},{0x2E66,14},{0x0006, 4},
    {0x0051, 7},{0x0001, 3},{0x0000, 3},{0x000D, 4},
    {0x000C, 4},{0x0009, 4},{0x001C, 5},{0x0009, 5},
    {0x001C, 6},{0x001D, 6},{0x005D, 7},{0x00B8, 8},
    {0x05CD,11},{0x1731,13},{0x1730,13},{0x000F, 4},
    {0x0005, 4},{0x000F, 5},{0x0008, 5},{0x0029, 6},
    {0x001D, 5},{0x002F, 6},{0x0008, 4},{0x0015, 5}
  },
  {
    {0x0009, 4},{0x0021, 6},{0x0040, 7},{0x00AD, 8},
    {0x02B0,10},{0x1589,13},{0x1588,13},{0x001C, 5},
    {0x005F, 7},{0x0000, 3},{0x000F, 4},{0x000D, 4},
    {0x000C, 4},{0x0006, 4},{0x0011, 5},{0x002A, 6},
    {0x0057, 7},{0x005E, 7},{0x0041, 7},{0x0159, 9},
    {0x0563,11},{0x158B,13},{0x158A,13},{0x0001, 3},
    {0x0005, 4},{0x0014, 5},{0x003B, 6},{0x002E, 6},
    {0x0004, 4},{0x003A, 6},{0x0007, 4},{0x0016, 5}
  },
  {
    {0x000E, 4},{0x0007, 5},{0x0046, 7},{0x0045, 7},
    {0x0064, 9},{0x032A,12},{0x0657,13},{0x0018, 5},
    {0x000D, 6},{0x0000, 3},{0x000F, 4},{0x000A, 4},
    {0x000B, 4},{0x001A, 5},{0x0036, 6},{0x0047, 7},
    {0x0044, 7},{0x0018, 7},{0x0033, 8},{0x00CB,10},
    {0x0656,13},{0x0329,12},{0x0328,12},{0x0002, 3},
    {0x0006, 4},{0x0019, 5},{0x000E, 5},{0x0037, 6},
    {0x0009, 4},{0x000F, 5},{0x0002, 4},{0x0010, 5}
  },
  {
    {0x0003, 3},{0x0018, 5},{0x0023, 6},{0x0077, 7},
    {0x0194, 9},{0x1956,13},{0x32AF,14},{0x003A, 6},
    {0x0076, 7},{0x0002, 3},{0x0001, 3},{0x001F, 5},
    {0x001E, 5},{0x0014, 5},{0x0022, 6},{0x0064, 7},
    {0x0197, 9},{0x0196, 9},{0x032B,10},{0x0654,11},
    {0x32AE,14},{0x1955,13},{0x1954,13},{0x0000, 3},
    {0x0009, 4},{0x001C, 5},{0x0015, 5},{0x0010, 5},
    {0x000D, 4},{0x0017, 5},{0x0016, 5},{0x0033, 6}
  },
  {
    {0x0005, 3},{0x0006, 4},{0x003E, 6},{0x0010, 5},
    {0x0048, 7},{0x093F,12},{0x24FA,14},{0x0032, 6},
    {0x0067, 7},{0x0002, 3},{0x0001, 3},{0x001B, 5},
    {0x001E, 5},{0x0034, 6},{0x0066, 7},{0x0092, 8},
    {0x0126, 9},{0x024E,10},{0x049E,11},{0x49F7,15},
    {0x49F6,15},{0x24F9,14},{0x24F8,14},{0x0000, 3},
    {0x0007, 4},{0x0018, 5},{0x0011, 5},{0x003F, 6},
    {0x000E, 4},{0x0013, 5},{0x0035, 6},{0x0025, 6}
  },
  {
    {0x0005, 3},{0x0008, 4},{0x0012, 5},{0x001C, 5},
    {0x001C, 6},{0x00EA, 9},{0x1D75,14},{0x001E, 6},
    {0x0066, 7},{0x0001, 3},{0x0002, 3},{0x001B, 5},
    {0x001A, 5},{0x001F, 6},{0x003B, 7},{0x0074, 8},
    {0x01D6,10},{0x03AF,11},{0x1D74,14},{0x1D77,14},
    {0x1D76,14},{0x0EB9,13},{0x0EB8,13},{0x000F, 4},
    {0x0006, 4},{0x0013, 5},{0x003B, 6},{0x003A, 6},
    {0x0000, 3},{0x0018, 5},{0x0032, 6},{0x0067, 7}
  },
  {
    {0x0004, 3},{0x000A, 4},{0x001B, 5},{0x000C, 4},
    {0x000D, 5},{0x00E6, 8},{0x0684,11},{0x0072, 7},
    {0x00E7, 8},{0x0002, 3},{0x0001, 3},{0x0017, 5},
    {0x0016, 5},{0x0018, 6},{0x00D1, 8},{0x01A0, 9},
    {0x0686,11},{0x0D0F,12},{0x0D0A,12},{0x1A17,13},
    {0x1A16,13},{0x1A1D,13},{0x1A1C,13},{0x000F, 4},
    {0x001D, 5},{0x000E, 5},{0x0035, 6},{0x0038, 6},
    {0x0000, 3},{0x000F, 5},{0x0019, 6},{0x0069, 7}
  },
  {
    {0x0003, 3},{0x000C, 4},{0x001B, 5},{0x0000, 3},
    {0x0003, 4},{0x002E, 6},{0x0051, 9},{0x00BC, 8},
    {0x0053, 9},{0x0004, 3},{0x0002, 3},{0x0016, 5},
    {0x0015, 5},{0x0015, 7},{0x0050, 9},{0x00A4,10},
    {0x0294,12},{0x052B,13},{0x052A,13},{0x052D,13},
    {0x052C,13},{0x052F,13},{0x052E,13},{0x000E, 4},
    {0x001A, 5},{0x0004, 5},{0x0028, 6},{0x0029, 6},
    {0x000F, 4},{0x000B, 6},{0x005F, 7},{0x00BD, 8}
  },
  {
    {0x0003, 4},{0x0009, 6},{0x00D0, 8},{0x01A3, 9},
    {0x0344,10},{0x0D14,12},{0x1A2B,13},{0x0004, 4},
    {0x0015, 7},{0x0000, 3},{0x000F, 4},{0x000B, 4},
    {0x000C, 4},{0x000E, 4},{0x0009, 4},{0x001B, 5},
    {0x000A, 5},{0x0014, 5},{0x000D, 5},{0x002A, 6},
    {0x0014, 7},{0x068B,11},{0x1A2A,13},{0x0008, 4},
    {0x000B, 5},{0x002B, 6},{0x000B, 6},{0x0069, 7},
    {0x0035, 6},{0x0008, 6},{0x0007, 4},{0x000C, 5}
  },
  {
    {0x000A, 4},{0x003C, 6},{0x0032, 7},{0x0030, 7},
    {0x00C5, 9},{0x0621,12},{0x0620,12},{0x001F, 5},
    {0x0033, 7},{0x0001, 3},{0x0000, 3},{0x000E, 4},
    {0x000D, 4},{0x000C, 4},{0x0004, 4},{0x000D, 5},
    {0x0026, 6},{0x0027, 6},{0x0014, 6},{0x0063, 8},
    {0x0189,10},{0x0623,12},{0x0622,12},{0x000B, 4},
    {0x0012, 5},{0x003D, 6},{0x0022, 6},{0x0015, 6},
    {0x000B, 5},{0x0023, 6},{0x0007, 4},{0x0010, 5}
  },
  {
    {0x000F, 4},{0x000C, 5},{0x0043, 7},{0x0010, 6},
    {0x0044, 8},{0x0114,10},{0x0455,12},{0x0018, 5},
    {0x0023, 7},{0x0001, 3},{0x0000, 3},{0x000E, 4},
    {0x000D, 4},{0x0009, 4},{0x0019, 5},{0x0009, 5},
    {0x0017, 6},{0x0016, 6},{0x0042, 7},{0x008B, 9},
    {0x0454,12},{0x0457,12},{0x0456,12},{0x000B, 4},
    {0x0015, 5},{0x000A, 5},{0x0029, 6},{0x0020, 6},
    {0x000D, 5},{0x0028, 6},{0x0007, 4},{0x0011, 5}
  },
  {
    {0x0001, 3},{0x001A, 5},{0x0029, 6},{0x002A, 6},
    {0x00A0, 8},{0x0285,10},{0x1425,13},{0x0002, 5},
    {0x0000, 7},{0x0002, 3},{0x0003, 3},{0x000C, 4},
    {0x000B, 4},{0x0008, 4},{0x0012, 5},{0x0001, 6},
    {0x0051, 7},{0x0001, 7},{0x0143, 9},{0x0508,11},
    {0x1424,13},{0x1427,13},{0x1426,13},{0x000F, 4},
    {0x001C, 5},{0x0003, 5},{0x0037, 6},{0x002B, 6},
    {0x0013, 5},{0x0036, 6},{0x001D, 5},{0x0001, 5}
  },
  {
    {0x0004, 3},{0x001F, 5},{0x003D, 6},{0x0006, 5},
    {0x0016, 7},{0x0053, 9},{0x014A,11},{0x0034, 6},
    {0x002A, 8},{0x0002, 3},{0x0003, 3},{0x000B, 4},
    {0x000C, 4},{0x001C, 5},{0x0037, 6},{0x0017, 7},
    {0x002B, 8},{0x0028, 8},{0x00A4,10},{0x052D,13},
    {0x052C,13},{0x052F,13},{0x052E,13},{0x0000, 3},
    {0x001D, 5},{0x0007, 5},{0x0004, 5},{0x0035, 6},
    {0x0014, 5},{0x0036, 6},{0x0015, 5},{0x003C, 6}
  },
  {
    {0x0004, 3},{0x000A, 4},{0x0007, 5},{0x001D, 5},
    {0x0009, 6},{0x01F3, 9},{0x07C7,11},{0x0008, 6},
    {0x01F0, 9},{0x0003, 3},{0x0002, 3},{0x000D, 4},
    {0x000C, 4},{0x0017, 5},{0x007D, 7},{0x01F2, 9},
    {0x07C6,11},{0x07C5,11},{0x1F12,13},{0x3E27,14},
    {0x3E26,14},{0x1F11,13},{0x1F10,13},{0x0000, 3},
    {0x001E, 5},{0x0006, 5},{0x0039, 6},{0x0038, 6},
    {0x003F, 6},{0x002C, 6},{0x0005, 5},{0x002D, 6}
  },
  {
    {0x0002, 3},{0x0007, 4},{0x0018, 5},{0x0003, 4},
    {0x0005, 5},{0x0035, 7},{0x004F, 9},{0x0012, 7},
    {0x04E5,13},{0x0005, 3},{0x0004, 3},{0x000D, 4},
    {0x000E, 4},{0x0033, 6},{0x0026, 8},{0x009D,10},
    {0x04E4,13},{0x04E7,13},{0x04E6,13},{0x04E1,13},
    {0x04E0,13},{0x04E3,13},{0x04E2,13},{0x0000, 3},
    {0x001F, 5},{0x000C, 5},{0x003D, 6},{0x003C, 6},
    {0x0032, 6},{0x0034, 7},{0x001B, 6},{0x0008, 6}
  },
  {
    {0x0000, 3},{0x0004, 4},{0x001C, 5},{0x000F, 4},
    {0x0002, 4},{0x0007, 5},{0x0075, 7},{0x00E8, 8},
    {0x1D2A,13},{0x0005, 3},{0x0004, 3},{0x000D, 4},
    {0x000C, 4},{0x0077, 7},{0x0E96,12},{0x3A57,14},
    {0x3A56,14},{0x3A5D,14},{0x3A5C,14},{0x3A5F,14},
    {0x3A5E,14},{0x1D29,13},{0x1D28,13},{0x0003, 3},
    {0x0006, 5},{0x000A, 5},{0x002C, 7},{0x0017, 6},
    {0x0076, 7},{0x01D3, 9},{0x03A4,10},{0x002D, 7}
  },
  {
    {0x000A, 4},{0x0024, 6},{0x00BF, 8},{0x0085, 8},
    {0x0211,10},{0x0842,12},{0x1087,13},{0x0018, 5},
    {0x0020, 6},{0x0001, 3},{0x0002, 3},{0x000E, 4},
    {0x000D, 4},{0x0007, 4},{0x0013, 5},{0x0025, 6},
    {0x005E, 7},{0x0043, 7},{0x00BE, 8},{0x0109, 9},
    {0x1086,13},{0x0841,12},{0x0840,12},{0x000F, 4},
    {0x0001, 4},{0x0011, 5},{0x0000, 5},{0x002E, 6},
    {0x0019, 5},{0x0001, 5},{0x0006, 4},{0x0016, 5}
  },
  {
    {0x0002, 3},{0x000F, 5},{0x006F, 7},{0x0061, 7},
    {0x0374,10},{0x1BA8,13},{0x3753,14},{0x0012, 5},
    {0x0036, 6},{0x0000, 3},{0x0001, 3},{0x000A, 4},
    {0x000B, 4},{0x001A, 5},{0x0031, 6},{0x0060, 7},
    {0x00DC, 8},{0x01BB, 9},{0x06EB,11},{0x1BAB,13},
    {0x3752,14},{0x3755,14},{0x3754,14},{0x000E, 4},
    {0x0006, 4},{0x0013, 5},{0x000E, 5},{0x003E, 6},
    {0x0008, 4},{0x001E, 5},{0x0019, 5},{0x003F, 6}
  },
  {
    {0x0003, 3},{0x001C, 5},{0x0025, 6},{0x0024, 6},
    {0x01DA, 9},{0x1DBD,13},{0x3B7C,14},{0x003C, 6},
    {0x003D, 6},{0x0000, 3},{0x0001, 3},{0x000B, 4},
    {0x000A, 4},{0x000B, 5},{0x0077, 7},{0x00EC, 8},
    {0x03B6,10},{0x076E,11},{0x1DBF,13},{0x76FB,15},
    {0x76FA,15},{0x3B79,14},{0x3B78,14},{0x000D, 4},
    {0x001F, 5},{0x0013, 5},{0x000A, 5},{0x0008, 5},
    {0x000C, 4},{0x0008, 4},{0x0009, 5},{0x003A, 6}
  },
  {
    {0x0005, 3},{0x0003, 4},{0x0004, 5},{0x0010, 5},
    {0x008F, 8},{0x0475,11},{0x11D1,13},{0x0079, 7},
    {0x0027, 6},{0x0002, 3},{0x0003, 3},{0x0001, 4},
    {0x0000, 4},{0x0026, 6},{0x0046, 7},{0x011C, 9},
    {0x0477,11},{0x08ED,12},{0x11D0,13},{0x11D3,13},
    {0x11D2,13},{0x11D9,13},{0x11D8,13},{0x000D, 4},
    {0x001F, 5},{0x0012, 5},{0x0005, 5},{0x003D, 6},
    {0x000C, 4},{0x000E, 4},{0x0022, 6},{0x0078, 7}
  },
  {
    {0x0005, 3},{0x000C, 4},{0x001B, 5},{0x0000, 4},
    {0x0006, 6},{0x03E2,10},{0x3E3D,14},{0x000F, 7},
    {0x0034, 6},{0x0003, 3},{0x0002, 3},{0x001E, 5},
    {0x001D, 5},{0x007D, 7},{0x01F0, 9},{0x07C6,11},
    {0x3E3C,14},{0x3E3F,14},{0x3E3E,14},{0x3E39,14},
    {0x3E38,14},{0x3E3B,14},{0x3E3A,14},{0x0008, 4},
    {0x001C, 5},{0x0002, 5},{0x003F, 6},{0x0035, 6},
    {0x0009, 4},{0x0001, 3},{0x000E, 7},{0x00F9, 8}
  },
  {
    {0x0004, 3},{0x000B, 4},{0x0001, 4},{0x000A, 4},
    {0x001E, 6},{0x00E0, 9},{0x0E1E,13},{0x0071, 8},
    {0x0039, 7},{0x0007, 3},{0x0006, 3},{0x000D, 5},
    {0x000C, 5},{0x0020, 7},{0x01C2,10},{0x1C3F,14},
    {0x1C3E,14},{0x0E19,13},{0x0E18,13},{0x0E1B,13},
    {0x0E1A,13},{0x0E1D,13},{0x0E1C,13},{0x0000, 4},
    {0x0009, 5},{0x001D, 6},{0x001F, 6},{0x0011, 6},
    {0x0005, 4},{0x0001, 3},{0x0043, 8},{0x0042, 8}
  },
  {
    {0x0004, 3},{0x000D, 4},{0x0007, 4},{0x0002, 3},
    {0x0014, 5},{0x016C, 9},{0x16D1,13},{0x02DF,10},
    {0x016E, 9},{0x0000, 2},{0x0007, 3},{0x002C, 6},
    {0x002B, 6},{0x02DE,10},{0x16D0,13},{0x16D3,13},
    {0x16D2,13},{0x2DB5,14},{0x2DB4,14},{0x2DB7,14},
    {0x2DB6,14},{0x16D9,13},{0x16D8,13},{0x000C, 5},
    {0x002A, 6},{0x005A, 7},{0x001B, 6},{0x001A, 6},
    {0x0017, 5},{0x000C, 4},{0x05B7,11},{0x05B5,11}
  },
  {
    {0x0002, 2},{0x000F, 4},{0x001C, 5},{0x000C, 4},
    {0x003B, 6},{0x01AC, 9},{0x1AD8,13},{0x35B3,14},
    {0x35B2,14},{0x0001, 2},{0x0000, 2},{0x0069, 7},
    {0x0068, 7},{0x35BD,14},{0x35BC,14},{0x35BF,14},
    {0x35BE,14},{0x35B9,14},{0x35B8,14},{0x35BB,14},
    {0x35BA,14},{0x35B5,14},{0x35B4,14},{0x01A9, 9},
    {0x01A8, 9},{0x035A,10},{0x00D7, 8},{0x00D5, 8},
    {0x003A, 6},{0x001B, 5},{0x35B7,14},{0x35B6,14}
  },
  {
    {0x0000, 3},{0x0010, 5},{0x0072, 7},{0x0071, 7},
    {0x0154, 9},{0x0AAB,12},{0x0AA8,12},{0x0014, 5},
    {0x0070, 7},{0x0002, 3},{0x0003, 3},{0x000C, 4},
    {0x000B, 4},{0x0003, 4},{0x0011, 5},{0x0073, 7},
    {0x0054, 7},{0x00AB, 8},{0x02AB,10},{0x1553,13},
    {0x1552,13},{0x1555,13},{0x1554,13},{0x000D, 4},
    {0x001E, 5},{0x0012, 5},{0x003E, 6},{0x002B, 6},
    {0x0002, 4},{0x003F, 6},{0x001D, 5},{0x0013, 5}
  },
  {
    {0x0003, 3},{0x001F, 5},{0x0029, 6},{0x003D, 6},
    {0x000C, 7},{0x0069,10},{0x0345,13},{0x0002, 5},
    {0x0028, 6},{0x0002, 3},{0x0001, 3},{0x000E, 4},
    {0x000C, 4},{0x0015, 5},{0x0007, 6},{0x001B, 8},
    {0x006B,10},{0x006A,10},{0x0344,13},{0x0347,13},
    {0x0346,13},{0x01A1,12},{0x01A0,12},{0x000B, 4},
    {0x001A, 5},{0x0012, 5},{0x0000, 5},{0x003C, 6},
    {0x0008, 4},{0x001B, 5},{0x0013, 5},{0x0001, 5}
  },
  {
    {0x0004, 3},{0x0004, 4},{0x003F, 6},{0x0014, 5},
    {0x0056, 7},{0x015C, 9},{0x15D5,13},{0x003C, 6},
    {0x002A, 6},{0x0000, 3},{0x0001, 3},{0x000E, 4},
    {0x000D, 4},{0x000C, 5},{0x00AF, 8},{0x02BB,10},
    {0x15D4,13},{0x15D7,13},{0x15D6,13},{0x15D1,13},
    {0x15D0,13},{0x15D3,13},{0x15D2,13},{0x000B, 4},
    {0x0019, 5},{0x000D, 5},{0x003E, 6},{0x0031, 6},
    {0x0007, 4},{0x0005, 4},{0x003D, 6},{0x0030, 6}
  },
  {
    {0x0005, 3},{0x0008, 4},{0x001A, 5},{0x0000, 4},
    {0x0036, 6},{0x0011, 8},{0x0106,12},{0x000A, 7},
    {0x006E, 7},{0x0002, 3},{0x0003, 3},{0x0003, 4},
    {0x0002, 4},{0x006F, 7},{0x0021, 9},{0x020F,13},
    {0x020E,13},{0x0101,12},{0x0100,12},{0x0103,12},
    {0x0102,12},{0x0105,12},{0x0104,12},{0x000C, 4},
    {0x001E, 5},{0x0003, 5},{0x003E, 6},{0x003F, 6},
    {0x0009, 4},{0x000E, 4},{0x000B, 7},{0x0009, 7}
  },
  {
    {0x0002, 3},{0x000E, 4},{0x001E, 5},{0x000C, 4},
    {0x001F, 5},{0x006E, 7},{0x00AD,10},{0x00AF,10},
    {0x0014, 7},{0x0004, 3},{0x0003, 3},{0x001A, 5},
    {0x0017, 5},{0x002A, 8},{0x0576,13},{0x0AEF,14},
    {0x0AEE,14},{0x0571,13},{0x0570,13},{0x0573,13},
    {0x0572,13},{0x0575,13},{0x0574,13},{0x0003, 4},
    {0x0016, 5},{0x0004, 5},{0x0036, 6},{0x000B, 6},
    {0x000A, 4},{0x0000, 3},{0x006F, 7},{0x00AC,10}
  },
  {
    {0x0004, 3},{0x0005, 4},{0x0003, 3},{0x0001, 3},
    {0x0004, 4},{0x002F, 6},{0x0526,11},{0x1495,13},
    {0x00A6, 8},{0x0007, 3},{0x0006, 3},{0x002D, 6},
    {0x002C, 6},{0x1494,13},{0x1497,13},{0x1496,13},
    {0x1491,13},{0x1490,13},{0x1493,13},{0x1492,13},
    {0x293D,14},{0x293C,14},{0x293F,14},{0x0000, 3},
    {0x0028, 6},{0x00A5, 8},{0x0148, 9},{0x00A7, 8},
    {0x002E, 6},{0x0015, 5},{0x0A4E,12},{0x293E,14}
  },
  {
    {0x0004, 3},{0x0005, 4},{0x0003, 3},{0x0001, 3},
    {0x0004, 4},{0x002F, 6},{0x0526,11},{0x1495,13},
    {0x00A6, 8},{0x0007, 3},{0x0006, 3},{0x002D, 6},
    {0x002C, 6},{0x1494,13},{0x1497,13},{0x1496,13},
    {0x1491,13},{0x1490,13},{0x1493,13},{0x1492,13},
    {0x293D,14},{0x293C,14},{0x293F,14},{0x0000, 3},
    {0x0028, 6},{0x00A5, 8},{0x0148, 9},{0x00A7, 8},
    {0x002E, 6},{0x0015, 5},{0x0A4E,12},{0x293E,14}
  },
  {
    {0x0004, 3},{0x0005, 4},{0x0003, 3},{0x0001, 3},
    {0x0004, 4},{0x002F, 6},{0x0526,11},{0x1495,13},
    {0x00A6, 8},{0x0007, 3},{0x0006, 3},{0x002D, 6},
    {0x002C, 6},{0x1494,13},{0x1497,13},{0x1496,13},
    {0x1491,13},{0x1490,13},{0x1493,13},{0x1492,13},
    {0x293D,14},{0x293C,14},{0x293F,14},{0x0000, 3},
    {0x0028, 6},{0x00A5, 8},{0x0148, 9},{0x00A7, 8},
    {0x002E, 6},{0x0015, 5},{0x0A4E,12},{0x293E,14}
  },
  {
    {0x0003, 3},{0x0011, 5},{0x0020, 6},{0x0074, 7},
    {0x010D, 9},{0x0863,12},{0x0860,12},{0x000A, 5},
    {0x0075, 7},{0x0001, 3},{0x0000, 3},{0x000B, 4},
    {0x000A, 4},{0x0018, 5},{0x0038, 6},{0x0042, 7},
    {0x010F, 9},{0x010E, 9},{0x0219,10},{0x10C3,13},
    {0x10C2,13},{0x10C5,13},{0x10C4,13},{0x000F, 4},
    {0x0004, 4},{0x0019, 5},{0x000B, 5},{0x0039, 6},
    {0x0009, 4},{0x001B, 5},{0x001A, 5},{0x003B, 6}
  },
  {
    {0x0005, 3},{0x0001, 4},{0x003E, 6},{0x0001, 5},
    {0x00E2, 8},{0x1C6F,13},{0x38D9,14},{0x0039, 6},
    {0x001F, 6},{0x0002, 3},{0x0001, 3},{0x0009, 4},
    {0x0008, 4},{0x0000, 5},{0x0070, 7},{0x01C7, 9},
    {0x038C,10},{0x071A,11},{0x38D8,14},{0x38DB,14},
    {0x38DA,14},{0x38DD,14},{0x38DC,14},{0x000D, 4},
    {0x001D, 5},{0x000E, 5},{0x003F, 6},{0x003C, 6},
    {0x000C, 4},{0x0006, 4},{0x003D, 6},{0x001E, 6}
  },
  {
    {0x0006, 3},{0x000B, 4},{0x0011, 5},{0x001E, 5},
    {0x0074, 7},{0x03AA,10},{0x1D5C,13},{0x0001, 6},
    {0x0021, 6},{0x0001, 3},{0x0002, 3},{0x0007, 4},
    {0x0006, 4},{0x003E, 6},{0x00EB, 8},{0x01D4, 9},
    {0x0EAF,12},{0x3ABB,14},{0x3ABA,14},{0x1D59,13},
    {0x1D58,13},{0x1D5B,13},{0x1D5A,13},{0x000A, 4},
    {0x001C, 5},{0x0001, 5},{0x003F, 6},{0x003B, 6},
    {0x0001, 4},{0x0009, 4},{0x0020, 6},{0x0000, 6}
  },
  {
    {0x0004, 3},{0x000A, 4},{0x0017, 5},{0x0004, 4},
    {0x0016, 6},{0x016A, 9},{0x16B1,13},{0x0017, 7},
    {0x005B, 7},{0x0006, 3},{0x0007, 3},{0x0001, 4},
    {0x0000, 4},{0x000A, 6},{0x02D7,10},{0x0B5A,12},
    {0x16B0,13},{0x16B3,13},{0x16B2,13},{0x2D6D,14},
    {0x2D6C,14},{0x2D6F,14},{0x2D6E,14},{0x0006, 4},
    {0x000A, 5},{0x0004, 5},{0x002C, 6},{0x0017, 6},
    {0x0003, 4},{0x0007, 4},{0x0016, 7},{0x00B4, 8}
  },
  {
    {0x0005, 3},{0x000D, 4},{0x0005, 4},{0x0009, 4},
    {0x0033, 6},{0x0193, 9},{0x192C,13},{0x0061, 8},
    {0x0031, 7},{0x0000, 2},{0x0007, 3},{0x0010, 5},
    {0x0011, 5},{0x00C8, 8},{0x192F,13},{0x325B,14},
    {0x325A,14},{0x1929,13},{0x1928,13},{0x192B,13},
    {0x192A,13},{0x325D,14},{0x325C,14},{0x0018, 5},
    {0x001A, 6},{0x001B, 6},{0x0065, 7},{0x0019, 6},
    {0x0004, 4},{0x0007, 4},{0x0060, 8},{0x0324,10}
  },
  {
    {0x0006, 3},{0x0000, 3},{0x0002, 4},{0x000F, 4},
    {0x0039, 6},{0x01D9, 9},{0x1D82,13},{0x0761,11},
    {0x03BE,10},{0x0001, 2},{0x0002, 2},{0x000F, 6},
    {0x000E, 6},{0x0762,11},{0x3B07,14},{0x3B06,14},
    {0x3B1D,14},{0x3B1C,14},{0x3B1F,14},{0x3B1E,14},
    {0x3B19,14},{0x3B18,14},{0x3B1B,14},{0x0038, 6},
    {0x01DE, 9},{0x00ED, 8},{0x03BF,10},{0x00EE, 8},
    {0x003A, 6},{0x0006, 5},{0x0EC0,12},{0x3B1A,14}
  },
  {
    {0x0000, 2},{0x0002, 3},{0x000F, 5},{0x0006, 4},
    {0x001C, 6},{0x01D0,10},{0x0E8C,13},{0x1D1B,14},
    {0x1D1A,14},{0x0003, 2},{0x0002, 2},{0x00EA, 9},
    {0x00E9, 9},{0x0E89,13},{0x0E88,13},{0x0E8B,13},
    {0x0E8A,13},{0x1D65,14},{0x1D64,14},{0x1D67,14},
    {0x1D66,14},{0x1D61,14},{0x1D60,14},{0x03AD,11},
    {0x1D63,14},{0x1D62,14},{0x1D1D,14},{0x1D1C,14},
    {0x003B, 7},{0x01D7,10},{0x1D1F,14},{0x1D1E,14}
  },
  {
    {0x0002, 2},{0x000F, 4},{0x001C, 5},{0x000C, 4},
    {0x003B, 6},{0x01AC, 9},{0x1AD8,13},{0x35B3,14},
    {0x35B2,14},{0x0001, 2},{0x0000, 2},{0x0069, 7},
    {0x0068, 7},{0x35BD,14},{0x35BC,14},{0x35BF,14},
    {0x35BE,14},{0x35B9,14},{0x35B8,14},{0x35BB,14},
    {0x35BA,14},{0x35B5,14},{0x35B4,14},{0x01A9, 9},
    {0x01A8, 9},{0x035A,10},{0x00D7, 8},{0x00D5, 8},
    {0x003A, 6},{0x001B, 5},{0x35B7,14},{0x35B6,14}
  }
};



/*A description of a Huffman code value used when encoding the tree.*/
typedef struct{
  /*The bit pattern, left-shifted so that the MSB of all patterns is
     aligned.*/
  uint32_t pattern;
  /*The amount the bit pattern was shifted.*/
  int32_t          shift;
  /*The token this bit pattern represents.*/
  int32_t          token;
}oc_huff_entry;



/*Compares two oc_huff_entry structures by their bit patterns.
  _c1: The first entry to compare.
  _c2: The second entry to compare.
  Return: <0 if _c1<_c2, >0 if _c1>_c2.*/
static int32_t huff_entry_cmp(const void *_c1,const void *_c2){
  uint32_t b1;
  uint32_t b2;
  b1=((const oc_huff_entry *)_c1)->pattern;
  b2=((const oc_huff_entry *)_c2)->pattern;
  return b1<b2?-1:b1>b2?1:0;
}

/*Encodes a description of the given Huffman tables.
  Although the codes are stored in the encoder as flat arrays, in the bit
   stream and in the decoder they are structured as a tree.
  This function recovers the tree structure from the flat array and then
   writes it out.
  Note that the codes MUST form a Huffman code, and not merely a prefix-free
   code, since the binary tree is assumed to be full.
  _opb:   The buffer to store the tree in.
  _codes: The Huffman tables to pack.
  Return: 0 on success, or a negative value if one of the given Huffman tables
   does not form a full, prefix-free code.*/
int32_t oc_huff_codes_pack(oggpack_buffer *_opb,
 const th_huff_code _codes[TH_NHUFFMAN_TABLES][TH_NDCT_TOKENS]){
  int32_t i;
  for(i=0;i<TH_NHUFFMAN_TABLES;i++){
    oc_huff_entry entries[TH_NDCT_TOKENS];
    int32_t           bpos;
    int32_t           maxlen;
    int32_t           mask;
    int32_t           j;
    /*First, find the maximum code length so we can align all the bit
       patterns.*/
    maxlen=_codes[i][0].nbits;
    for(j=1;j<TH_NDCT_TOKENS;j++){
      maxlen=OC_MAXI(_codes[i][j].nbits,maxlen);
    }
    mask=(1<<(maxlen>>1)<<((maxlen+1)>>1))-1;
    /*Copy over the codes into our temporary workspace.
      The bit patterns are aligned, and the original entry each code is from
       is stored as well.*/
    for(j=0;j<TH_NDCT_TOKENS;j++){
      entries[j].shift=maxlen-_codes[i][j].nbits;
      entries[j].pattern=_codes[i][j].pattern<<entries[j].shift&mask;
      entries[j].token=j;
    }
    /*Sort the codes into ascending order.
      This is the order the leaves of the tree will be traversed.*/
    qsort(entries,TH_NDCT_TOKENS,sizeof(entries[0]),huff_entry_cmp);
    /*For each leaf of the tree:*/
    bpos=maxlen;
    for(j=0;j<TH_NDCT_TOKENS;j++){
      int32_t bit;
      /*If this code has any bits at all.*/
      if(entries[j].shift<maxlen){
        /*Descend into the tree, writing a bit for each branch.*/
        for(;bpos>entries[j].shift;bpos--)oggpackB_write(_opb,0,1);
        /*Mark this as a leaf node, and write its value.*/
        oggpackB_write(_opb,1,1);
        oggpackB_write(_opb,entries[j].token,5);
        /*For each 1 branch we've descended, back up the tree until we reach a
           0 branch.*/
        bit=1<<bpos;
        for(;entries[j].pattern&bit;bpos++)bit<<=1;
        /*Validate the code.*/
        if(j+1<TH_NDCT_TOKENS){
          mask=~(bit-1)<<1;
          /*The next entry should have a 1 bit where we had a 0, and should
             match our code above that bit.
            This verifies both fullness and prefix-freeness simultaneously.*/
          if(!(entries[j+1].pattern&bit)||
           (entries[j].pattern&mask)!=(entries[j+1].pattern&mask)){
            return TH_EINVAL;
          }
        }
        /*If there are no more codes, we should have ascended back to the top
           of the tree.*/
        else if(bpos<maxlen)return TH_EINVAL;
      }
    }
  }
  return 0;
}
