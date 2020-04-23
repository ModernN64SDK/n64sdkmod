/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: messages.c,v $
        $Revision: 1.8 $
        $Date: 1998/11/13 12:23:49 $
 *---------------------------------------------------------------------*/
#include <ultra64.h>

#include "main.h"
#include "font.h"

u8	*message[ERROR_MES_SIZE] = {
#ifdef _ASCII_
  "Normal termination",
  "Abnormal termination",
  "Forced termination",
  "Error number",
  "Please refer to the User's Guide.",
  "[Caution] Please do not remove the disk", 
  "while the access lamp is blinking.",
  "Disk is not inserted.",
  "Please insert the disk again.",
  "Is the disk inserted?",
  "Please insert the disk.",
  "Wrong disk may have been inserted.",
  "Please confirm and",
  "set the time.",
  "Refer to the User's Guide",
  "for details",
  "Is memory expansion pak",
  "inserted correctly?",
  "Currently creating.",
  "Fail to save the previous data",
  "perfectly.",
  "Data will be deleted.",
  "Please push A button.",
  "Please remove the disk.",
  "Change the disk and",
  "insert the right disk.",
#else
"正常終了", 						
"異常終了",
"強制終了",
"エラー番号",
"取扱説明書をお読みください。",
"【注意】アクセスランプ点滅中に",
"ディスクを抜かないでください。",
"ディスクが差し込まれていません",
"ください。",
"ディスクが差し込まれていますか。",
 "ディスクを差し込んでください。", 
 "間違ったディスクが差し込まれている",
 "可能性があります。",
 "時刻を設定してください。",
 "詳しくは、取扱説明書を",
 "お読みください。",               
 "メモリー拡張パックが正しく",
 "差し込んでありますか？",
 "現在作成中です。",
 "前回データが最後まできちんと",
 "セーブできませんでした。",
 "データを消します。",
 "Ａボタンを押してください。",
 "ディスクを取り出してください。",
 "正しいディスクに交換して",
 "ディスクを差し込みなおしてください。",
#endif
};

s32	kaddr[ERROR_MES_SIZE];

