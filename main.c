#include <stdio.h>
#include <string.h>
#include "signatures.h"
#include <ncurses.h>
#include "png_analyse.h"
#include "event_handler.h"
#include "menu.h"
#include "globals.h"
#include "stack.h"
#include "file_work.h"
#include "elf_analyse.h"
#include <elf.h>
#include <libgen.h>
#include <sys/stat.h>


change change_mod = HEX;

Point cur, ccur, inFile;

struct node *stack;

void print_with_color(Point p, unsigned char c);

void win_init();

unsigned char print_dump(int str_num);

void printIHDR(struct IHDR IHDR);

void printELF(Elf32_Ehdr_s ehdr);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("no filepath in command line\n");
        return -1;
    }
    if (argc > 2) {
        printf("to much command line parameters\n");
        return -1;
    }
    filepath = argv[1];
    struct stat stat_f;
    stat(filepath,&stat_f);
    if(!S_ISREG(stat_f.st_mode))
    {
        printf("current file is not regular file\n");
        return -1;
    }
    filename = basename(filepath);
    struct IHDR IHDR;
    Elf32_Ehdr_s ehdr;
    int key = -2;
    struct sign signatures[50];
    bool flags[10];
    signatures_init(signatures);
    const unsigned char *ext = signcheck();
    unsigned char cur_ch;
    int old_offset = 0, str_num = 0, changed = 0;
    if (!strncmp("png", (char *) ext, 3)) {
        IHDR = png_analyse();
        flags[0] = 1;
    } else if (!strncmp("elf", (char *) ext, 3)) {

        ehdr = read_elf();
        flags[1] = 1;
    }
    win_init();
    int screen_x = 0, screen_y = 0;
    start_color();
    keypad(stdscr, TRUE);
    init_pair(STYLE_CURSOR, COLOR_BLACK, COLOR_WHITE);

    inFile.x = 0;
    inFile.y = 0;
    cur.x = 12;
    cur.y = 1;
    ccur.x = cur.x + 52;
    ccur.y = cur.y;
    str_num = get_dump();
    while (true) {

        getmaxyx(stdscr, screen_y, screen_x);
        if (screen_y < 28 || screen_x < 84) {

            endwin();
            printf("min terminal size 84x28");
            return 1;
        }
        wclear(analyse_win);
        wclear(main_win);
        wclear(stdscr);
        box(main_win, 0, 0);
        box(analyse_win, 0, 0);
        if (old_offset != dump_offset || changed) {
            str_num = get_dump();
            old_offset = dump_offset;
            changed = 0;
        }
        if (inFile.y + inFile.x >= file_size) {
            int fx = cur.x;
            while (inFile.y + inFile.x >= file_size) {
                move_left();
            }
            while (fx < cur.x)move_left();
        }
        cur_ch = print_dump(str_num);

        if (flags[0] == 1) {
            printIHDR(IHDR);
        } else if (flags[1] == 1)
            printELF(ehdr);
        print_with_color(ccur, cur_ch);
        render_menu();
        wmove(main_win, cur.y, cur.x);
        refresh();
        wrefresh(analyse_win);
        wrefresh(main_win);
        flushinp();
        fflush(stdin);
        key = event();
        if (key == -1) {
            break;
        } else if (key > 0) {
            changed = change_file(key);
            key = -2;
        } else if (key == -3) {
            changed = 1;
        }

    }
    endwin();

    return 0;
}

void win_init() {
    initscr();
    cbreak();
    noecho();
    analyse_win = newwin(5, 82, 22, 1);
    main_win = newwin(22, 82, 0, 1);
}

unsigned char print_dump(int str_num) {
    unsigned char cur_ch;
    unsigned long size = 16;
    int fx = dump_offset, cur_str = 1;
    mvwprintw(main_win, 0, 1, "%08X", inFile.y + inFile.x);
    for (int j = 0; j < str_num; j++) {
        if (j == str_num - 1 && last_size < 16) {
            size = last_size;
        }
        if (size > 0) {
            mvwprintw(main_win, cur_str, 1, "%08X: ", fx);
            for (int i = 0; i < (int) size; i++) {
                //  if (bufer[j][i] > 255)bufer[j][i] -= 255;
                mvwprintw(main_win, cur_str, i * 3 + 12, "%02X ", bufer[j][i]);
            }
            if (size < STRLEN) {
                unsigned long indent = size;
                indent -= STRLEN;
                indent *= -1;
                for (long unsigned int i = 0; i < indent; i++) {
                    wprintw(main_win, "   ");
                }
            }
            for (long unsigned int i = 0; i < size; i++) {
                if (bufer[j][i] < 32 || bufer[j][i] > '~' || bufer[j][i] == '\n') {
                    mvwprintw(main_win, cur_str, (int)i + STRLEN * 4, ".");
                    if (cur.y == cur_str && inFile.x == (int)i)cur_ch = '.';
                } else {
                    mvwprintw(main_win, cur_str, (int)i + STRLEN * 4, "%c", bufer[j][i]);
                    if (cur.y == cur_str && inFile.x ==(int)i)cur_ch = bufer[j][i];
                }
            }
            cur_str++;
            fx += STRLEN;
        }
    }
    return cur_ch;
}

void printIHDR(struct IHDR IHDR) {
    mvwprintw(analyse_win, 1, 1, "file extension is png ");
    wprintw(analyse_win, "%ld x %ld, ", IHDR.x, IHDR.y);
    wprintw(analyse_win, "%ld-bit", IHDR.bit_depth);
    switch (IHDR.color) {
        case 0:
            wprintw(analyse_win, " monochrome");
            break;
        case 2:
            wprintw(analyse_win, "/color RGB");
            break;
        case 3:
            wprintw(analyse_win, " colormap");
            break;
        case 4:
            wprintw(analyse_win, " Each pixel is a grayscale sample, followed by an alpha sample");
            break;
        case 6:
            wprintw(analyse_win, "/color RGBA");
            break;
        default:
            wprintw(analyse_win, " unknown");
    }
    if (IHDR.interplaced == 0) {
        wprintw(analyse_win, ", non-interlaced");
    } else if (IHDR.interplaced == 1) {
        wprintw(analyse_win, ", interlaced");
    } else wprintw(analyse_win, ", bad parameter");
}

void printELF(Elf32_Ehdr_s ehdr) {
    switch (ehdr.class) {
        case ELFCLASS32:
            mvwprintw(analyse_win, 1, 1, " file is ELF32. ");
            break;
        case ELFCLASS64:
            mvwprintw(analyse_win, 1, 1, " file is ELF64. ");
            break;
        default:
            mvwprintw(analyse_win, 1, 1, " file is unknown.");
            return;
    }
    switch (ehdr.encoding) {
        case ELFDATA2LSB:
            wprintw(analyse_win, "2's complement, little-endian. ");
            break;
        case ELFDATA2MSB:
            wprintw(analyse_win, "2's complement, big-endian. ");
            break;
        default:
            wprintw(analyse_win, "Unknown data format. ");
            break;
    }
    switch (ehdr.version) {
        case EV_CURRENT:
            wprintw(analyse_win, "current version. ");
            break;
        default:
            wprintw(analyse_win, "invalid version. ");
    }
    switch (ehdr.osabi) {
        case ELFOSABI_SYSV:
            wprintw(analyse_win, "UNIX System V ");
            break;
        case ELFOSABI_HPUX:
            wprintw(analyse_win, "HP-UX ");
            break;
        case ELFOSABI_NETBSD:
            wprintw(analyse_win, "NetBSD ");
            break;
        case ELFOSABI_LINUX:
            wprintw(analyse_win, "Linux ");
            break;
        case ELFOSABI_SOLARIS:
            wprintw(analyse_win, "Solaris ");
            break;
        case ELFOSABI_IRIX:
            wprintw(analyse_win, "IRIX ");
            break;
        case ELFOSABI_FREEBSD:
            wprintw(analyse_win, "FreeBSD ");
            break;
        case ELFOSABI_TRU64:
            wprintw(analyse_win, "TRU64 UNIX ");
            break;
        case ELFOSABI_ARM:
            wprintw(analyse_win, "ARM ");
            break;
        default:
            wprintw(analyse_win, "UNIX System V ");
    }
    switch (ehdr.type) {
        case ET_REL:
            mvwprintw(analyse_win, 2, 1, "A relocatable file. ");
            break;
        case ET_EXEC:
            mvwprintw(analyse_win, 2, 1, "An executable file. ");
            break;
        case ET_DYN:
            mvwprintw(analyse_win, 2, 1, "A shared object. ");
            break;
        case ET_CORE:
            mvwprintw(analyse_win, 2, 1, "A core file. ");
            break;
        default:
            mvwprintw(analyse_win, 2, 1, "An unknown type. ");

    }
    switch (ehdr.machine) {
        case EM_NONE:
            mvwprintw(analyse_win, 2, 1, " No machine");
            break;
        case EM_M32:
            mvwprintw(analyse_win, 2, 1, " AT&T WE 32100");
            break;
        case EM_SPARC:
            mvwprintw(analyse_win, 2, 1, " SUN SPARC");
            break;
        case EM_386:
            mvwprintw(analyse_win, 2, 1, " Intel 80386");
            break;
        case EM_68K:
            mvwprintw(analyse_win, 2, 1, " Motorola m68k family");
            break;
        case EM_88K:
            mvwprintw(analyse_win, 2, 1, " Motorola m88k family");
            break;
        case EM_IAMCU:
            mvwprintw(analyse_win, 2, 1, " Intel MCU");
            break;
        case EM_860:
            mvwprintw(analyse_win, 2, 1, " Intel 80860");
            break;
        case EM_MIPS:
            mvwprintw(analyse_win, 2, 1, " MIPS R3000 big-endian");
            break;
        case EM_S370:
            mvwprintw(analyse_win, 2, 1, " IBM System/370");
            break;
        case EM_MIPS_RS3_LE:
            mvwprintw(analyse_win, 2, 1, " MIPS R3000 little-endian");
            break;
        case EM_PARISC:
            mvwprintw(analyse_win, 2, 1, " HPPA");
            break;
        case EM_VPP500:
            mvwprintw(analyse_win, 2, 1, " Fujitsu VPP500");
            break;
        case EM_SPARC32PLUS:
            mvwprintw(analyse_win, 2, 1, " Sun's v8plus");
            break;
        case EM_960:
            mvwprintw(analyse_win, 2, 1, " Intel 80960");
            break;
        case EM_PPC:
            mvwprintw(analyse_win, 2, 1, " PowerPC");
            break;
        case EM_PPC64:
            mvwprintw(analyse_win, 2, 1, " PowerPC 64-bit");
            break;
        case EM_S390:
            mvwprintw(analyse_win, 2, 1, " IBM S390");
            break;
        case EM_SPU:
            mvwprintw(analyse_win, 2, 1, " IBM SPU/SPC");
            break;
        case EM_V800:
            mvwprintw(analyse_win, 2, 1, " NEC V800 series");
            break;
        case EM_FR20:
            mvwprintw(analyse_win, 2, 1, " Fujitsu FR20");
            break;
        case EM_RH32:
            mvwprintw(analyse_win, 2, 1, " TRW RH-32");
            break;
        case EM_RCE:
            mvwprintw(analyse_win, 2, 1, " Motorola RCE");
            break;
        case EM_ARM:
            mvwprintw(analyse_win, 2, 1, " ARM");
            break;
        case EM_FAKE_ALPHA:
            mvwprintw(analyse_win, 2, 1, " Digital Alpha");
            break;
        case EM_SH:
            mvwprintw(analyse_win, 2, 1, " Hitachi SH");
            break;
        case EM_SPARCV9:
            mvwprintw(analyse_win, 2, 1, " SPARC v9 64-bit");
            break;
        case EM_TRICORE:
            mvwprintw(analyse_win, 2, 1, " Siemens Tricore");
            break;
        case EM_ARC:
            mvwprintw(analyse_win, 2, 1, " Argonaut RISC Core");
            break;
        case EM_H8_300:
            mvwprintw(analyse_win, 2, 1, " Hitachi H8/300");
            break;
        case EM_H8_300H:
            mvwprintw(analyse_win, 2, 1, " Hitachi H8/300H");
            break;
        case EM_H8S:
            mvwprintw(analyse_win, 2, 1, " Hitachi H8S");
            break;
        case EM_H8_500:
            mvwprintw(analyse_win, 2, 1, " Hitachi H8/500");
            break;
        case EM_IA_64:
            mvwprintw(analyse_win, 2, 1, " Intel Merced");
            break;
        case EM_MIPS_X:
            mvwprintw(analyse_win, 2, 1, " Stanford MIPS-X");
            break;
        case EM_COLDFIRE:
            mvwprintw(analyse_win, 2, 1, " Motorola Coldfire");
            break;
        case EM_68HC12:
            mvwprintw(analyse_win, 2, 1, " Motorola M68HC12");
            break;
        case EM_MMA:
            mvwprintw(analyse_win, 2, 1, " Fujitsu MMA Multimedia Accelerator");
            break;
        case EM_PCP:
            mvwprintw(analyse_win, 2, 1, " Siemens PCP");
            break;
        case EM_NCPU:
            mvwprintw(analyse_win, 2, 1, " Sony nCPU embeeded RISC");
            break;
        case EM_NDR1:
            mvwprintw(analyse_win, 2, 1, " Denso NDR1 microprocessor");
            break;
        case EM_STARCORE:
            mvwprintw(analyse_win, 2, 1, " Motorola Start*Core processor");
            break;
        case EM_ME16:
            mvwprintw(analyse_win, 2, 1, " Toyota ME16 processor");
            break;
        case EM_ST100:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronic ST100 processor");
            break;
        case EM_TINYJ:
            mvwprintw(analyse_win, 2, 1, " Advanced Logic Corp. Tinyj emb.fam");
            break;
        case EM_X86_64:
            mvwprintw(analyse_win, 2, 1, " AMD x86-64 architecture");
            break;
        case EM_PDSP:
            mvwprintw(analyse_win, 2, 1, " Sony DSP Processor");
            break;
        case EM_PDP10:
            mvwprintw(analyse_win, 2, 1, " Digital PDP-10");
            break;
        case EM_PDP11:
            mvwprintw(analyse_win, 2, 1, " Digital PDP-11");
            break;
        case EM_FX66:
            mvwprintw(analyse_win, 2, 1, " Siemens FX66 microcontroller");
            break;
        case EM_ST9PLUS:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics ST9+ 8/16 mc");
            break;
        case EM_ST7:
            mvwprintw(analyse_win, 2, 1, " STmicroelectronics ST7 8 bit mc");
            break;
        case EM_68HC16:
            mvwprintw(analyse_win, 2, 1, " Motorola MC68HC16 microcontroller");
            break;
        case EM_68HC11:
            mvwprintw(analyse_win, 2, 1, " Motorola MC68HC11 microcontroller");
            break;
        case EM_68HC08:
            mvwprintw(analyse_win, 2, 1, " Motorola MC68HC08 microcontroller");
            break;
        case EM_68HC05:
            mvwprintw(analyse_win, 2, 1, " Motorola MC68HC05 microcontroller");
            break;
        case EM_SVX:
            mvwprintw(analyse_win, 2, 1, " Silicon Graphics SVx");
            break;
        case EM_ST19:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics ST19 8 bit mc");
            break;
        case EM_VAX:
            mvwprintw(analyse_win, 2, 1, " Digital VAX");
            break;
        case EM_CRIS:
            mvwprintw(analyse_win, 2, 1, " Axis Communications 32-bit emb.proc");
            break;
        case EM_JAVELIN:
            mvwprintw(analyse_win, 2, 1, " Infineon Technologies 32-bit emb.proc");
            break;
        case EM_FIREPATH:
            mvwprintw(analyse_win, 2, 1, " Element 14 64-bit DSP Processor");
            break;
        case EM_ZSP:
            mvwprintw(analyse_win, 2, 1, " LSI Logic 16-bit DSP Processor");
            break;
        case EM_MMIX:
            mvwprintw(analyse_win, 2, 1, " Donald Knuth's educational 64-bit proc");
            break;
        case EM_HUANY:
            mvwprintw(analyse_win, 2, 1, " Harvard University machine-independent object files");
            break;
        case EM_PRISM:
            mvwprintw(analyse_win, 2, 1, " SiTera Prism");
            break;
        case EM_AVR:
            mvwprintw(analyse_win, 2, 1, " Atmel AVR 8-bit microcontroller");
            break;
        case EM_FR30:
            mvwprintw(analyse_win, 2, 1, " Fujitsu FR30");
            break;
        case EM_D10V:
            mvwprintw(analyse_win, 2, 1, " Mitsubishi D10V");
            break;
        case EM_D30V:
            mvwprintw(analyse_win, 2, 1, " Mitsubishi D30V");
            break;
        case EM_V850:
            mvwprintw(analyse_win, 2, 1, " NEC v850");
            break;
        case EM_M32R:
            mvwprintw(analyse_win, 2, 1, " Mitsubishi M32R");
            break;
        case EM_MN10300:
            mvwprintw(analyse_win, 2, 1, " Matsushita MN10300");
            break;
        case EM_MN10200:
            mvwprintw(analyse_win, 2, 1, " Matsushita MN10200");
            break;
        case EM_PJ:
            mvwprintw(analyse_win, 2, 1, " picoJava");
            break;
        case EM_OPENRISC:
            mvwprintw(analyse_win, 2, 1, " OpenRISC 32-bit embedded processor");
            break;
        case EM_ARC_COMPACT:
            mvwprintw(analyse_win, 2, 1, " ARC International ARCompact");
            break;
        case EM_XTENSA:
            mvwprintw(analyse_win, 2, 1, " Tensilica Xtensa Architecture");
            break;
        case EM_VIDEOCORE:
            mvwprintw(analyse_win, 2, 1, " Alphamosaic VideoCore");
            break;
        case EM_TMM_GPP:
            mvwprintw(analyse_win, 2, 1, " Thompson Multimedia General Purpose Proc");
            break;
        case EM_NS32K:
            mvwprintw(analyse_win, 2, 1, " National Semi. 32000");
            break;
        case EM_TPC:
            mvwprintw(analyse_win, 2, 1, " Tenor Network TPC");
            break;
        case EM_SNP1K:
            mvwprintw(analyse_win, 2, 1, " Trebia SNP 1000");
            break;
        case EM_ST200:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics ST200");
            break;
        case EM_IP2K:
            mvwprintw(analyse_win, 2, 1, " Ubicom IP2xxx");
            break;
        case EM_MAX:
            mvwprintw(analyse_win, 2, 1, " MAX processor");
            break;
        case EM_CR:
            mvwprintw(analyse_win, 2, 1, " National Semi. CompactRISC");
            break;
        case EM_F2MC16:
            mvwprintw(analyse_win, 2, 1, " Fujitsu F2MC16");
            break;
        case EM_MSP430:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments msp430");
            break;
        case EM_BLACKFIN:
            mvwprintw(analyse_win, 2, 1, " Analog Devices Blackfin DSP");
            break;
        case EM_SE_C33:
            mvwprintw(analyse_win, 2, 1, " Seiko Epson S1C33 family");
            break;
        case EM_SEP:
            mvwprintw(analyse_win, 2, 1, " Sharp embedded microprocessor");
            break;
        case EM_ARCA:
            mvwprintw(analyse_win, 2, 1, " Arca RISC");
            break;
        case EM_UNICORE:
            mvwprintw(analyse_win, 2, 1, " PKU-Unity & MPRC Peking Uni. mc series");
            break;
        case EM_EXCESS:
            mvwprintw(analyse_win, 2, 1, " eXcess configurable cpu");
            break;
        case EM_DXP:
            mvwprintw(analyse_win, 2, 1, " Icera Semi. Deep Execution Processor");
            break;
        case EM_ALTERA_NIOS2:
            mvwprintw(analyse_win, 2, 1, " Altera Nios II");
            break;
        case EM_CRX:
            mvwprintw(analyse_win, 2, 1, " National Semi. CompactRISC CRX");
            break;
        case EM_XGATE:
            mvwprintw(analyse_win, 2, 1, " Motorola XGATE");
            break;
        case EM_C166:
            mvwprintw(analyse_win, 2, 1, " Infineon C16x/XC16x");
            break;
        case EM_M16C:
            mvwprintw(analyse_win, 2, 1, " Renesas M16C");
            break;
        case EM_DSPIC30F:
            mvwprintw(analyse_win, 2, 1, " Microchip Technology dsPIC30F");
            break;
        case EM_CE:
            mvwprintw(analyse_win, 2, 1, " Freescale Communication Engine RISC");
            break;
        case EM_M32C:
            mvwprintw(analyse_win, 2, 1, " Renesas M32C");
            break;
        case EM_TSK3000:
            mvwprintw(analyse_win, 2, 1, " Altium TSK3000");
            break;
        case EM_RS08:
            mvwprintw(analyse_win, 2, 1, " Freescale RS08");
            break;
        case EM_SHARC:
            mvwprintw(analyse_win, 2, 1, " Analog Devices SHARC family");
            break;
        case EM_ECOG2:
            mvwprintw(analyse_win, 2, 1, " Cyan Technology eCOG2");
            break;
        case EM_SCORE7:
            mvwprintw(analyse_win, 2, 1, " Sunplus S+core7 RISC");
            break;
        case EM_DSP24:
            mvwprintw(analyse_win, 2, 1, " New Japan Radio (NJR) 24-bit DSP");
            break;
        case EM_VIDEOCORE3:
            mvwprintw(analyse_win, 2, 1, " Broadcom VideoCore III");
            break;
        case EM_LATTICEMICO32:
            mvwprintw(analyse_win, 2, 1, " RISC for Lattice FPGA");
            break;
        case EM_SE_C17:
            mvwprintw(analyse_win, 2, 1, " Seiko Epson C17");
            break;
        case EM_TI_C6000:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments TMS320C6000 DSP");
            break;
        case EM_TI_C2000:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments TMS320C2000 DSP");
            break;
        case EM_TI_C5500:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments TMS320C55x DSP");
            break;
        case EM_TI_ARP32:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments App. Specific RISC");
            break;
        case EM_TI_PRU:
            mvwprintw(analyse_win, 2, 1, " Texas Instruments Prog. Realtime Unit");
            break;
        case EM_MMDSP_PLUS:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics 64bit VLIW DSP");
            break;
        case EM_CYPRESS_M8C:
            mvwprintw(analyse_win, 2, 1, " Cypress M8C");
            break;
        case EM_R32C:
            mvwprintw(analyse_win, 2, 1, " Renesas R32C");
            break;
        case EM_TRIMEDIA:
            mvwprintw(analyse_win, 2, 1, " NXP Semi. TriMedia");
            break;
        case EM_QDSP6:
            mvwprintw(analyse_win, 2, 1, " QUALCOMM DSP6");
            break;
        case EM_8051:
            mvwprintw(analyse_win, 2, 1, " Intel 8051 and variants");
            break;
        case EM_STXP7X:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics STxP7x");
            break;
        case EM_NDS32:
            mvwprintw(analyse_win, 2, 1, " Andes Tech. compact code emb. RISC");
            break;
        case EM_ECOG1X:
            mvwprintw(analyse_win, 2, 1, " Cyan Technology eCOG1X");
            break;
        case EM_MAXQ30:
            mvwprintw(analyse_win, 2, 1, " Dallas Semi. MAXQ30 mc");
            break;
        case EM_XIMO16:
            mvwprintw(analyse_win, 2, 1, " New Japan Radio (NJR) 16-bit DSP");
            break;
        case EM_MANIK:
            mvwprintw(analyse_win, 2, 1, " M2000 Reconfigurable RISC");
            break;
        case EM_CRAYNV2:
            mvwprintw(analyse_win, 2, 1, " Cray NV2 vector architecture");
            break;
        case EM_RX:
            mvwprintw(analyse_win, 2, 1, " Renesas RX");
            break;
        case EM_METAG:
            mvwprintw(analyse_win, 2, 1, " Imagination Tech. META");
            break;
        case EM_MCST_ELBRUS:
            mvwprintw(analyse_win, 2, 1, " MCST Elbrus");
            break;
        case EM_ECOG16:
            mvwprintw(analyse_win, 2, 1, " Cyan Technology eCOG16");
            break;
        case EM_CR16:
            mvwprintw(analyse_win, 2, 1, " National Semi. CompactRISC CR16");
            break;
        case EM_ETPU:
            mvwprintw(analyse_win, 2, 1, " Freescale Extended Time Processing Unit");
            break;
        case EM_SLE9X:
            mvwprintw(analyse_win, 2, 1, " Infineon Tech. SLE9X");
            break;
        case EM_L10M:
            mvwprintw(analyse_win, 2, 1, " Intel L10M");
            break;
        case EM_K10M:
            mvwprintw(analyse_win, 2, 1, " Intel K10M");
            break;
        case EM_AARCH64:
            mvwprintw(analyse_win, 2, 1, " ARM AARCH64");
            break;
        case EM_AVR32:
            mvwprintw(analyse_win, 2, 1, " Amtel 32-bit microprocessor");
            break;
        case EM_STM8:
            mvwprintw(analyse_win, 2, 1, " STMicroelectronics STM8");
            break;
        case EM_TILE64:
            mvwprintw(analyse_win, 2, 1, " Tilera TILE64");
            break;
        case EM_TILEPRO:
            mvwprintw(analyse_win, 2, 1, " Tilera TILEPro");
            break;
        case EM_MICROBLAZE:
            mvwprintw(analyse_win, 2, 1, " Xilinx MicroBlaze");
            break;
        case EM_CUDA:
            mvwprintw(analyse_win, 2, 1, " NVIDIA CUDA");
            break;
        case EM_TILEGX:
            mvwprintw(analyse_win, 2, 1, " Tilera TILE-Gx");
            break;
        case EM_CLOUDSHIELD:
            mvwprintw(analyse_win, 2, 1, " CloudShield");
            break;
        case EM_COREA_1ST:
            mvwprintw(analyse_win, 2, 1, " KIPO-KAIST Core-A 1st gen.");
            break;
        case EM_COREA_2ND:
            mvwprintw(analyse_win, 2, 1, " KIPO-KAIST Core-A 2nd gen.");
            break;
        case EM_ARCV2:
            mvwprintw(analyse_win, 2, 1, " Synopsys ARCv2 ISA.");
            break;
        case EM_OPEN8:
            mvwprintw(analyse_win, 2, 1, " Open8 RISC");
            break;
        case EM_RL78:
            mvwprintw(analyse_win, 2, 1, " Renesas RL78");
            break;
        case EM_VIDEOCORE5:
            mvwprintw(analyse_win, 2, 1, " Broadcom VideoCore V");
            break;
        case EM_78KOR:
            mvwprintw(analyse_win, 2, 1, " Renesas 78KOR");
            break;
        case EM_56800EX:
            mvwprintw(analyse_win, 2, 1, " Freescale 56800EX DSC");
            break;
        case EM_BA1:
            mvwprintw(analyse_win, 2, 1, " Beyond BA1");
            break;
        case EM_BA2:
            mvwprintw(analyse_win, 2, 1, " Beyond BA2");
            break;
        case EM_XCORE:
            mvwprintw(analyse_win, 2, 1, " XMOS xCORE");
            break;
        case EM_MCHP_PIC:
            mvwprintw(analyse_win, 2, 1, " Microchip 8-bit PIC(r)");
            break;
        case EM_INTELGT:
            mvwprintw(analyse_win, 2, 1, " Intel Graphics Technology");
            break;
        case EM_KM32:
            mvwprintw(analyse_win, 2, 1, " KM211 KM32");
            break;
        case EM_KMX32:
            mvwprintw(analyse_win, 2, 1, " KM211 KMX32");
            break;
        case EM_EMX16:
            mvwprintw(analyse_win, 2, 1, " KM211 KMX16");
            break;
        case EM_EMX8:
            mvwprintw(analyse_win, 2, 1, " KM211 KMX8");
            break;
        case EM_KVARC:
            mvwprintw(analyse_win, 2, 1, " KM211 KVARC");
            break;
        case EM_CDP:
            mvwprintw(analyse_win, 2, 1, " Paneve CDP");
            break;
        case EM_COGE:
            mvwprintw(analyse_win, 2, 1, " Cognitive Smart Memory Processor");
            break;
        case EM_COOL:
            mvwprintw(analyse_win, 2, 1, " Bluechip CoolEngine");
            break;
        case EM_NORC:
            mvwprintw(analyse_win, 2, 1, " Nanoradio Optimized RISC");
            break;
        case EM_CSR_KALIMBA:
            mvwprintw(analyse_win, 2, 1, " CSR Kalimba");
            break;
        case EM_Z80:
            mvwprintw(analyse_win, 2, 1, " Zilog Z80");
            break;
        case EM_VISIUM:
            mvwprintw(analyse_win, 2, 1, " Controls and Data Services VISIUMcore");
            break;
        case EM_FT32:
            mvwprintw(analyse_win, 2, 1, " FTDI Chip FT32");
            break;
        case EM_MOXIE:
            mvwprintw(analyse_win, 2, 1, " Moxie processor");
            break;
        case EM_AMDGPU:
            mvwprintw(analyse_win, 2, 1, " AMD GPU");
            break;
        case EM_RISCV:
            mvwprintw(analyse_win, 2, 1, " RISC-V");
            break;
        case EM_BPF:
            mvwprintw(analyse_win, 2, 1, " Linux BPF -- in-kernel virtual machine");
            break;
        case EM_CSKY:
            mvwprintw(analyse_win, 2, 1, " C-SK");
            break;

    }

}


void print_with_color(Point p, unsigned char c) {
    wattron(main_win, COLOR_PAIR(STYLE_CURSOR));
    mvwprintw(main_win, p.y, p.x, "%c", c);
    wattroff(main_win, COLOR_PAIR(STYLE_CURSOR));
}