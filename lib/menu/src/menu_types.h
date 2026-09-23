#pragma once

#include <menu_types.h>


/* ASCII keycodes */
#define CTRLR 0x12
#define CTRLU 0x15
#define BS 0x08
#define TAB 0x09
#define ESC 0x1B
#define RS 0x1E
#define US 0x1F

/* clang-format off */
enum {
    ERR_BADPARM = 1,
    ERR_OUTOFRANGE,
    ERR_BADCMD
};
/* clang-format on */

/* Data types */
enum {
    ITEM_CHAR,
    ITEM_STR,
    ITEM_SHORT,
    ITEM_INT,
    ITEM_LONG,
    ITEM_BYTE_LIST,
    ITEM_SHORT_BIN,
    ITEM_TIME,
    ITEM_DATE,
    ITEM_DATETIME,
    ITEM_LAST
};
/* ITEM_BYTE_LIST Must be null terminated */


/* Item List Structure */
typedef struct {
    unsigned short data_type;
    void* address;
    unsigned long mask;
    unsigned short shift;
    unsigned short field_len;
    char* fmt;
} ItemList;

typedef struct Cmd {
    char* name;
    struct Cmd* submenu;
    void (*func)(char* cmdbuf);
    char* descrip;
    ItemList* itemList;
    void (*func2)(char* cmdbuf);
} Cmd;

typedef struct {
    long code;
    char* msg;
} CmdErr;
