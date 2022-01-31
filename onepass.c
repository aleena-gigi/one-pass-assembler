#include<stdio.h>
#include<stdlib.h>

#include<string.h>
void main()
{
    FILE* f1, * f2, * f3, * f4, * f5;
    int lc, sa, i = 0, j = 0, m[10], pgmlen, len, k, len1, l = 0,flag=0;
    char name[10], opnd[10], la[10], mne[10], s1[10], mne1[10], opnd1[10];
    char lcs[10], ms[10];
    char sym[10], symaddr[10], obj1[10], obj2[10], s2[10], q[10], s3[10];
  
    f1 = fopen("input1.txt", "r");
    f2 = fopen("optab1.txt", "r");
    f3 = fopen("symtab.txt", "w+");
    f4 = fopen("symtabnew.txt", "w+");
    f5 = fopen("output.txt", "w+");
    fscanf(f1, "%s%s%s", la, mne, opnd);
    if (strcmp(mne, "START") == 0)
    {
        sa = atoi(opnd);    //convert starting address into integer format
        strcpy(name, la);   //set pgm name
        lc = sa;            //initialise location counter to the starting address
    }
    strcpy(s1, "*");        //flag for undefined labels
    fscanf(f1, "%s%s%s", la, mne, opnd);
    while (strcmp(mne, "END") != 0)
    {
        if (strcmp(la, "-") == 0)   //if there is no symbol in the label field
        {
            fscanf(f2, "%s%s", mne1, opnd1);    //read  the mnemonic and opcode in optab
            while (!feof(f2))       //search optab for finding opcode for the mnemonic
            {
                if (strcmp(mne1, mne) == 0) 
                {
                    m[i] = lc + 1;          //storing the address for the forward reference
                    fprintf(f3, "%s\t%s\n", opnd, s1); //writing into symtab(operand and undefined flag)
                    fprintf(f5, "%s\t0000\n", opnd1);  //opcode for mnemonic in object pgm(op txt)
                    lc = lc + 3;    //setting loc ctr to the current line
                    i = i + 1;
                    break;
                }
                else
                    fscanf(f2, "%s%s", mne1, opnd1);
            }

        }

        else        //if there is a label in the label field
        {
            fseek(f3, SEEK_SET, 0);     //set to the starting point of the symtab to check whether the label is already defined
            fscanf(f3, "%s%s", sym, symaddr);
            while (!feof(f3))       //symtab: searching symtab if the label was encountered(but not defined)
            {
                if (strcmp(sym, la) == 0)
                {
                    sprintf(lcs, "%d", lc); //convert into string
                    fprintf(f4, "%s\t%s\n", la, lcs);   //write it to new symtab for defining the symbol
                    sprintf(ms, "%d", m[j]);
                    j = j + 1;
                    fprintf(f5, "%s\t%s\n", ms, lcs);  /*output: forward reference address and its corresponding value
                                                       undefined symbol:its value is first assigned as 0000 (mne-opcode 0000)
                                                       symbol definition: is encountered the address where space for 
                                                       the vvalue of symbol was left blank is taken and then will assign the 
                                                       value(addres value)*/
                    i = i + 1;
                    flag = 1;
                    break;
                }
                else
                    fscanf(f3, "%s%s", sym, symaddr);
            }  //f3
            if (flag == 0)  //if symbol is not defined in symtab
            {
                fprintf(f4, "%s\t%d\n", la, lc+3);         //new symtab: label lc+3
            }
            if (strcmp(mne, "RESW") == 0)
                lc = lc + 3 * atoi(opnd);       //reserveres the given no of words and incrment the loc countr
            else if (strcmp(mne, "BYTE") == 0)
            {
                strcpy(s2, "-");
                len = strlen(opnd);
                lc = lc + len - 2;
              
                for (k = 2; k < len; k++)
                {
                  //  q[l] = opnd[k];
                 
                   // l = l + 1;
                    fprintf(f5, "%c", opnd[k]);
                }
               
                fprintf(f5, "\t%s\n", s2);
                //fprintf(f5, "%s\t%s\n", q, s2);
               
            }
            else if (strcmp(mne, "RESB") == 0)
                lc = lc + atoi(opnd);
            else if (strcmp(mne, "WORD") == 0)
            {
                strcpy(s3, "#");
                lc = lc + 3;
                fprintf(f5, "%s\t%s\n", opnd, s3);
               
            }
            
        } // else la=-


        fseek(f2, SEEK_SET, 0);         //set to the starting of optab
        fscanf(f1, "%s%s%s", la, mne, opnd);     // scan the next instr from the input file
    }
    fseek(f5, SEEK_SET, 0);
    pgmlen = lc - sa;
    printf("H^%s^%d^0%x\n", name, sa, pgmlen);
    printf("T^");
    printf("00%d^0%x", sa, pgmlen);
    fscanf(f5, "%s%s", obj1, obj2);
    while (!feof(f5))
    {
        if (strcmp(obj2, "0000") == 0)
            printf("^%s%s", obj1, obj2);
        else if (strcmp(obj2, "-") == 0)
        {
            printf("^");
            len1 = strlen(obj1);
            for (k = 0; k < len1; k++)
                printf("%d", obj1[k]);
        }
        else if (strcmp(obj2, "#") == 0)
        {
            printf("^");
            printf("%s", obj1);
        }
        fscanf(f5, "%s%s", obj1, obj2);
    }
    fseek(f5, SEEK_SET, 0);
    fscanf(f5, "%s%s", obj1, obj2);
    while (!feof(f5))
    {
        if (strcmp(obj2, "0000") != 0)
        {
            if (strcmp(obj2, "-") != 0)
            {
                if (strcmp(obj2, "#") != 0)
                {
                    printf("\n");
                    printf("T^%s^02^%s", obj1, obj2);
                }
            }
        }
        fscanf(f5, "%s%s", obj1, obj2);
    }
    printf("\nE^00%d", sa);


}