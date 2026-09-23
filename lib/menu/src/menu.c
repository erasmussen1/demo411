
/*
 * #include <stdlib.h>
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

#include "amenu.h"
#include "bmenu.h"
#include "tmenu.h"
#include "menu.h"


const int DASHES = 21;

static void showCommands(const char* topname, const Cmd* m);
static void showCommandEntry(const char* topname, const Cmd* p);
static int showItem(ItemList* p);
static void trimBeginBlank(char* buf, const size_t bufSize);
static Cmd* findCommand(const Cmd* m, char ch);

CmdErr* findErrorEntry(const long errcode);

/* clang-format off */
const CmdErr ME_CmdErrTable[] = {
    {ERR_BADPARM, "Bad command parameters!"},
    {ERR_OUTOFRANGE, "Out of range!"},
    {ERR_BADCMD, "Unrecognized command!"},
    {0, NULL}
};

extern Cmd AM_menu_table[];
extern Cmd BM_menu_table[];
extern Cmd TM_menu_table[];

const Cmd main_cmd_table[] = {
    {"A", (Cmd*)AM_menu_table, 0, "A Commands", NULL},
    {"B", (Cmd*)BM_menu_table, 0, "B Commands", NULL},
    {"T", (Cmd*)TM_menu_table, 0, "T Commands", NULL},
    {"?",  NULL, 0, "Help menu", NULL},
    {NULL, NULL, 0, NULL, NULL}
};
/* clang-format on */

void ME_initCommandBuffer(char* cmdbuf,  //
                          const int cmdBufferSize) {
    AM_initMenu();
    BM_initMenu();
    TM_initMenu();

    /* Clear buffer */
    for (int i = 0; i < cmdBufferSize; i++) {
        cmdbuf[i] = '\0';
    }
}

int ME_command(char* cmdbuf,                 //
               const int cmdBufferSize,      //
               void (*delay_fn)(const int),  //
               const int tick)               //
{
    int cmd_index = 0;

    putchar('\r');
    putchar('>');

    cmdbuf[0] = '\0';
    cmdbuf[cmdBufferSize] = '\0';

    int done = 0;
    int ch;
    while (!done) {
        ch = getchar();
        ch &= 0xFF;

        switch (ch) {
            case '\r':
            case '\n':
                cmdbuf[cmd_index] = '\0';

                done = 1;
                break;

            case ESC:
                cmd_index = 0;
                putchar('\r');
                putchar('>');
                break;

            case BS:
                if (cmd_index > 0) {
                    cmd_index--;
                    putchar(BS);
                    putchar(' ');
                    putchar(BS);
                }
                break;

            case ' ':
                putchar(ch);
                break;

            default:
                if ((ch >= '!') && (ch <= 'z')) {
                    if (cmd_index >= cmdBufferSize - 1) {
                        break;
                    }

                    cmdbuf[cmd_index++] = (char)ch;
                    putchar(ch);
                }
                break;
        }

        delay_fn(tick);
    }

    return cmd_index;
}

static Cmd* findCommand(const Cmd* m, char chIn) {
    const Cmd* cmd = NULL;
    const Cmd* p = m;

    int ch = toupper((int)chIn);

    while (p->name) {
        if (ch == p->name[0]) {
            cmd = p;
            break;
        }
        p++;
    }

    return (Cmd*)cmd;
}

int ME_commandProcess(char* cmdbufIn, const int cmdIndex) {
    const Cmd* p = NULL;
    const Cmd* prev = NULL;
    const Cmd* m = (const Cmd*)main_cmd_table;

    if (cmdIndex < 0) {
        return -1;
    }

    if (cmdIndex < 1) {
        return 0;
    }

    for (int i = 0; i < cmdIndex; i++) {
        prev = p;

        p = findCommand(m, cmdbufIn[i]);
        if (!p) {
            return 1;
        }

        if (*p->name == '?') {
            if (prev) {
                showCommands(prev->name, m);
            } else {
                showCommands("", m);
            }

            return 0;
        }

        if (p->submenu) {
            m = p->submenu;
            continue;
        }

        char* q = &cmdbufIn[i + 1];
        while (q && (*q == ' ')) {
            q++;
        }

        if (*q == '?') {
            printf("\n");

            if (prev && p->func2 && *prev->name == 'T') {
                // (p->func2)(cmdbufIn);
                q++;
                (p->func2)(q);
            } else if (prev) {
                showCommandEntry(prev->name, p);
            } else {
                showCommandEntry(NULL, p);
            }
            return 0;
        }

        if (p->func) {
            trimBeginBlank(cmdbufIn, 100);
            (p->func)(cmdbufIn);

            return 0;
        }
    }

    return 1;
}

static void showCommands(const char* topname, const Cmd* m) {
    const Cmd* p = NULL;

    printf(
        "\n"                         //
        "STM32F411\n\r"              //
        "2026 (C) Blinker inc.\n\n"  //
        "Available Commands:\n"      //
    );

    if (strcmp(topname, "")) {
        printf("\n");
    }

    if (m == NULL) {
        return;
    }

    for (p = m; p->name != 0; p++) {
        showCommandEntry(topname, p);
    }
}

static void showCommandEntry(const char* topname, const Cmd* p) {
    if (p == NULL) {
        return;
    }

    if (topname && *topname) {
        printf("%s%s = ", topname, p->name);
    } else {
        printf("%s? ", p->name);
    }

    int len = DASHES;

    ItemList* item = NULL;
    if (p->itemList != NULL) {
        for (item = p->itemList; item->data_type != ITEM_LAST; item++) {
            len -= showItem(item);
        }

        len--;
        printf(" ");
    }

    if (len > 0) {
        for (int i = 0; i < len; i++) {
            printf("-");
        }
    }

    printf(" %s\n", p->descrip);
}

static int showItem(ItemList* p) {
    unsigned short floatFlag = 0;
    unsigned long lword;

    switch (p->data_type) {
        case ITEM_CHAR:
            lword = (unsigned long)*((char*)p->address);
            break;

        case ITEM_STR:
            lword = (unsigned long)((char*)p->address);
            break;

        case ITEM_SHORT:
            lword = (unsigned long)*((short*)p->address);
            break;

        case ITEM_INT:
            lword = (unsigned long)*((int*)p->address);
            break;

        case ITEM_LONG:
            lword = *((unsigned long*)p->address);
            break;

        case ITEM_BYTE_LIST: {
            char* temp = (char*)p->address;

            floatFlag = 0;

            while (*temp != 0x00) {
                if ((floatFlag + p->field_len + 1) >= DASHES) {
                    printf("\n   ");
                    floatFlag = 0;
                }

                printf(p->fmt, *temp++);
                printf(",");
                floatFlag += 4;
            }

            putchar(BS);  /* backspace */
            putchar(' '); /* space, to clear last comma */
            putchar(BS);  /* backspace */
            return ((short)(floatFlag - 1));
            break;
        }

        case ITEM_SHORT_BIN: {
            short data = (short)*((short*)p->address);

            for (unsigned short index = p->field_len; index >= 1; index--) {
                if ((1 << (index - 1)) & data) {
                    putchar(US);
                } else {
                    putchar(RS);
                }
            }

            return (p->field_len);
            break;
        }
    }

    if (p->mask) {
        lword &= p->mask;
    }

    if (p->shift) {
        lword >>= p->shift;
    }

    switch (floatFlag) {
        case 0:
            printf(p->fmt, lword);
            break;

        case 1:
            printf(p->fmt, lword);
            break;

        default:
            break;
    }

    return (p->field_len);
}

CmdErr* findErrorEntry(const long errcode) {
    const CmdErr* p = NULL;

    for (p = ME_CmdErrTable; p->msg != NULL; p++) {
        if (p->code == errcode) {
            break;
        }
    }

    return (CmdErr*)p;
}

void ME_commandError(long errcode, char* errstr) {
    const CmdErr* p = findErrorEntry(errcode);

    printf("\n");
    if (p) {
        printf("ERR: %s\n", p->msg);
    } else {
        printf("ERR: Unknown command error!%s\n", ((errstr != 0) ? errstr : ""));
    }
}

static void trimBeginBlank(char* buf, const size_t bufSize) {
    int cn = 0;
    unsigned int flag = 1;
    char ch = 0;

    for (size_t c = 0; c < (bufSize - 1); c++) {
        ch = buf[c];

        /* Remove space from beginning of string. */
        if ((ch == ' ') && (cn == 0)) {
            continue;
        }

        if ((ch == '\r' || ch == '\n') || ch == '\0') {
            break;
        }

        if ((ch == '=') || (ch == ',') || (ch == ';')) {
            ch = ' ';
        }

        buf[cn] = ch;

        if (ch == ' ') {
            if (flag) {
                flag = 0;
                cn++;
            }
        } else {
            flag = 1;
            cn++;
        }
    }

    buf[cn] = '\0';
}
