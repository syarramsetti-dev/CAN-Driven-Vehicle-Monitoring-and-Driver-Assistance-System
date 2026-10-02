//lcd.h
#include "types.h"
void WriteLCD(u8);
void CmdLCD(u8);
void InitLCD(void);
void CharLCD(u8);
void StrLCD(s8 *);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32,u32);
void BuildCGRAM(s8 *,u32 );
//---- additions for this project (20x4 panel) --------------------------
void ClearLCD(void);
void GotoXYLCD(u8 row, u8 col);   //row = 0..3, col = 0..19
