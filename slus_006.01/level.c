
void process_level_data_(void)

{
  int status;
  uint sub_index;
  ushort uVar1;
  char cVar2;
  ushort uVar3;
  
  if ((DAT_800a59b4 & 0x80) != 0) {
    uVar1 = DAT_800a611c + 0x14;
    if ((ushort)counter_1 < (ushort)(DAT_800a611c + 0x14)) {
      uVar1 = (ushort)counter_1;
    }
    uVar3 = DAT_800a611c;
    if (DAT_800a611c < uVar1) {
      do {
        FntPrint();
        uVar3 = uVar3 + 1;
        FntPrint();
        FntPrint();
      } while (uVar3 < uVar1);
    }
    if ((*(uint *)(BUF_PTR + 0x10c) & 3) == 0) {
      if ((*DAT_800a6188 & 0x1000) != 0) {
        if (DAT_800a611e != 0) {
          DAT_800a611e = DAT_800a611e - 1;
        }
        if (DAT_800a611e < DAT_800a611c) {
          DAT_800a611c = DAT_800a611e;
        }
      }
      if ((*DAT_800a6188 & 0x4000) != 0) {
        if ((int)(uint)DAT_800a611e < (int)(counter_1 - 1)) {
          DAT_800a611e = DAT_800a611e + 1;
        }
        if ((int)(uint)DAT_800a611c < (int)(DAT_800a611e - 0x13)) {
          DAT_800a611c = DAT_800a611e - 0x13;
        }
      }
    }
    if ((DAT_800a6188[1] & 0x40) != 0) {
      sub_index = (uint)DAT_800a611e;
      DAT_800a59b4 = DAT_800a59b4 & 0xff7f;
      if (sub_index < counter_2 + 10) {
        if (sub_index < 10) {
          cVar2 = (char)DAT_800a611e + '\x01';
        }
        else {
          cVar2 = 'c';
          FUN_8007a2fc(BUF_PTR + 0x84,4,(uint)(&DAT_fffffff6 + sub_index) & 0xff);
        }
      }
      else {
        status = strcmp((&sub_level_list)[sub_index],&END2_BLB);
        if (status == 0) {
          return;
        }
        cVar2 = 'c';
        FUN_8007a2fc(BUF_PTR + 0x84,1,(uint)(&DAT_fffffff6 + (sub_index - (byte)counter_2)) & 0xff);
      }
      *(char *)(BUF_PTR + 0x148) = cVar2;
    }
  }
  return;
}

