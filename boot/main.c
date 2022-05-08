#include <uefi.h>
 
/*** ELF64 defines and structs ***/
#define ELFMAG      "\177ELF"
#define SELFMAG     4
#define EI_CLASS    4       /* File class byte index */
#define ELFCLASS64  2       /* 64-bit objects */
#define EI_DATA     5       /* Data encoding byte index */
#define ELFDATA2LSB 1       /* 2's complement, little endian */
#define ET_EXEC     2       /* Executable file */
#define PT_LOAD     1       /* Loadable program segment */
#ifdef __x86_64__
#define EM_MACH     62      /* AMD x86-64 architecture */
#endif
#ifdef __aarch64__
#define EM_MACH     183     /* ARM aarch64 architecture */
#endif

typedef struct
{
  uint8_t  e_ident[16];   /* Magic number and other info */
  uint16_t e_type;        /* Object file type */
  uint16_t e_machine;     /* Architecture */
  uint32_t e_version;     /* Object file version */
  uint64_t e_entry;       /* Entry point virtual address */
  uint64_t e_phoff;       /* Program header table file offset */
  uint64_t e_shoff;       /* Section header table file offset */
  uint32_t e_flags;       /* Processor-specific flags */
  uint16_t e_ehsize;      /* ELF header size in bytes */
  uint16_t e_phentsize;   /* Program header table entry size */
  uint16_t e_phnum;       /* Program header table entry count */
  uint16_t e_shentsize;   /* Section header table entry size */
  uint16_t e_shnum;       /* Section header table entry count */
  uint16_t e_shstrndx;    /* Section header string table index */
} Elf64_Ehdr;

typedef struct
{
  uint32_t p_type;        /* Segment type */
  uint32_t p_flags;       /* Segment flags */
  uint64_t p_offset;      /* Segment file offset */
  uint64_t p_vaddr;       /* Segment virtual address */
  uint64_t p_paddr;       /* Segment physical address */
  uint64_t p_filesz;      /* Segment size in file */
  uint64_t p_memsz;       /* Segment size in memory */
  uint64_t p_align;       /* Segment alignment */
} Elf64_Phdr;



int main (int argc, char **argv)
{
    DIR *dh;
    struct dirent *de;

    FILE *f;
    char *buff;
    long int size;
    Elf64_Ehdr *elf;
    Elf64_Phdr *phdr;
    uintptr_t entry;
    int i;


    printf("Hello, world!\n");

    if ((dh = opendir("\\"))) {
        while((de = readdir(dh)) != NULL) {
            printf("%c %04x %s\n", de->d_type == DT_DIR ? 'D' : '.', de->d_type, de->d_name);
        }
        closedir(dh);
    }

    if((f = fopen("\\kernel\\kernel.elf", "r"))) {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        buff = malloc(size + 1);
        if (!buff) {
            //fprintf(stderr, "Unable to allocate memory :(\n");
            printf("Unable to allocate memory :(\n");
            for(;;){}
        }
        fread(buff, size, 1, f);
        fclose(f);
    } else {
        //fprintf(stderr, "Unable to open file :(\n");
        printf("Unable to open file :(\n");
        for(;;){}
    }

  
    elf = (Elf64_Ehdr *)buff;
    if(!memcmp(elf->e_ident, ELFMAG, SELFMAG) &&    /* magic match? */
        elf->e_ident[EI_CLASS] == ELFCLASS64 &&     /* 64 bit? */
        elf->e_ident[EI_DATA] == ELFDATA2LSB &&     /* LSB? */
        elf->e_type == ET_EXEC &&                   /* executable object? */
        elf->e_machine == EM_MACH &&                /* architecture match? */
        elf->e_phnum > 0) {                         /* has program headers? */
            /* load segments */
            for(phdr = (Elf64_Phdr *)(buff + elf->e_phoff), i = 0;
                i < elf->e_phnum;
                i++, phdr = (Elf64_Phdr *)((uint8_t *)phdr + elf->e_phentsize)) {
                    if(phdr->p_type == PT_LOAD) {
                        printf("ELF segment %p %d bytes (bss %d bytes)\n", phdr->p_vaddr, phdr->p_filesz,
                            phdr->p_memsz - phdr->p_filesz);
                        memcpy((void*)phdr->p_vaddr, buff + phdr->p_offset, phdr->p_filesz);
                        memset((void*)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
                    }
                }
            entry = elf->e_entry;
    } else {
        //fprintf(stderr, "not a valid ELF executable for this architecture\n");
        printf("not a valid ELF executable for this architecture\n");
        for(;;){}
    }
    /* free resources */
    free(buff);


    printf("Thats an among us moment boys\n");
    printf("ELF entry point %p\n", entry);

    efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_gop_t *gop = NULL;

    BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);


    void * fb = gop->Mode->FrameBufferBase;
    //long int ppsl = gop->Mode->Info->PixelsPerScanLine;
    long int ppsl = 1920;
    exit_bs();
    (*((int(* __attribute__((sysv_abi)))(void *, long int))(entry)))(fb, ppsl);
    for(;;){}
    return 0;
}
