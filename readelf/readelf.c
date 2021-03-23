/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;
		int i;


		unsigned char data = ehdr->e_ident[5];

        u_char *ptr_sh_table = NULL;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }

        // get section table addr, section header number and section header size.
		if (data == 2) {

			uint32_t temp = 0;
			uint16_t temp16 = 0;

			uint32_t e_shoff = ehdr->e_shoff;
			for (i = 0; i < 4; i++) {
				temp = (temp << 8) + (e_shoff & 255);
				e_shoff >>= 8;
			}
			e_shoff = temp;

			Elf32_Shdr *shdr = (Elf32_Shdr *)(binary + e_shoff);

			uint16_t sh_entry_count = ehdr->e_shnum;
			for (i = 0; i < 2; i++) {
				temp16 = (temp16 << 8) + (sh_entry_count & 255);
				sh_entry_count >>= 8;
			}
			sh_entry_count = temp16;

			for (Nr = 0; Nr < sh_entry_count; Nr ++) {
				// sh_addr
				uint32_t sh_addr = shdr[Nr].sh_addr;
				temp = 0;
				for (i = 0; i < 4; i++) {
					temp = (temp << 8) + (sh_addr & 255);
					sh_addr >>= 8;
				}
				sh_addr = temp;
				// sh_addr
				printf("%d:0x%x\n", Nr, sh_addr);
			}
			
		} else {
			Elf32_Phdr *phdr = (Elf32_Phdr *)(binary + ehdr->e_phoff);
			Elf32_Half ph_entry_count = ehdr->e_phnum;

			for (Nr = 0; Nr < ph_entry_count; Nr ++) {
				printf("%d:0x%x,0x%x\n", Nr, phdr[Nr].p_filesz , phdr[Nr].p_memsz);
			}

		}

        // for each section header, output section number and section addr. 
        // hint: section number starts at 0.


        return 0;
}



