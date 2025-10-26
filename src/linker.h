#ifndef LINKER_H
#define LINKER_H

void link_assembly(char *input_filename, char *output_filename);
void assemble_and_link(char *asm_filename, char *output_filename, int output_asm);

#endif // LINKER_H
