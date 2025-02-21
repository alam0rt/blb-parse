typedef struct astruct_8 astruct_8, *Pastruct_8;

struct astruct_8 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
    undefined field52_0x34;
    undefined field53_0x35;
    undefined field54_0x36;
    undefined field55_0x37;
    undefined field56_0x38;
    undefined field57_0x39;
    undefined field58_0x3a;
    undefined field59_0x3b;
    undefined field60_0x3c;
    undefined field61_0x3d;
    undefined field62_0x3e;
    undefined field63_0x3f;
    undefined field64_0x40;
    undefined field65_0x41;
    undefined field66_0x42;
    undefined field67_0x43;
    undefined field68_0x44;
    undefined field69_0x45;
    undefined field70_0x46;
    undefined field71_0x47;
    undefined field72_0x48;
    undefined field73_0x49;
    undefined field74_0x4a;
    undefined field75_0x4b;
    undefined field76_0x4c;
    undefined field77_0x4d;
    undefined field78_0x4e;
    undefined field79_0x4f;
    undefined field80_0x50;
    undefined field81_0x51;
    undefined field82_0x52;
    undefined field83_0x53;
    undefined field84_0x54;
    undefined field85_0x55;
    undefined field86_0x56;
    undefined field87_0x57;
    undefined field88_0x58;
    undefined field89_0x59;
    undefined field90_0x5a;
    undefined field91_0x5b;
    int game_blb_ptr;
    undefined1 level?; // Created by retype action
    undefined field94_0x61;
    undefined field95_0x62;
    undefined field96_0x63;
    void *load_asset_function;
};

void set_up_buffer(astruct_2 *buffer_loc)

{
  int iVar2;
  undefined *puVar1;
  
  FUN_8007bf78();
  BUF_PTR = buffer_loc;
  buffer_loc->field_0x114 = 0;
  buffer_loc->field_0x130 = 0;
  puVar1 = PTR_DAT_800a59b0;
  buffer_loc->pixel_count = uVar3;
  buffer_loc->field40_0x28 = 0;
  buffer_loc->field249_0x104 = 0;
  buffer_loc->field252_0x108 = 0;
  buffer_loc->field257_0x110 = 0;
  buffer_loc->field91_0x64 = 0;
  buffer_loc->field92_0x66 = 0;
  buffer_loc->padding = 0x1234567;
  iVar2 = *(int *)(puVar1 + 0xa650);
  padding = 0x1234567;
  buffer_loc->buffer = iVar2;
  buffer_loc->maybe_next_block_ = iVar2 + 0x1000;
  load_game_blb_asset(0,2,buffer_loc->buffer);
  initialise_blb_struct
            ((astruct_8 *)&buffer_loc->blb_reading_struct,buffer_loc->buffer,&LOAD_BLB_ASSET);
  set_blb_pointer(0,(long)&buffer_loc->blb_reading_struct);
  return;
}

// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// param 4 is 5 on bootup
// param 5 is 10

undefined4
draw_load_screen(uint offset,uint sector_count,astruct_12 *buf,short param_4,short param_5)

{
  bool bVar1;
  uint uVar2;
  u_long uVar3;
  short sVar4;
  DISPENV DStack_40;
  
  VLC_BUFFER = buf;
                    // initially the BLB header is loaded
                    // afterwards it loads the blob at 0x6000
  uVar2 = load_game_blb_asset(offset & 0xffff,sector_count & 0xffff,buf + 0x19c);
  if ((uVar2 & 0xff) != 0) {
    prepare_image_struct(256,false,0);
    VSync(0);
    SetDefDispEnv(&DStack_40,0,0x100,0x140,0x100);
    DStack_40.screen.x = 0;
    DStack_40.screen.y = 0;
    DStack_40.screen.w = 0;
    DStack_40.screen.h = 240;
    DStack_40.isrgb24 = '\0';
    PutDispEnv(&DStack_40);
    SetDispMask(1);
                    // initialise image subsystems
    DecDCTReset(0);
                    // call back at end of MDEC transmission
    DecDCToutCallback((func *)&LAB_80039e24);
    DecDCTvlcBuild(&VLC_BUFFER->vlc_table);
    output_run_level_ = (uint)(output_run_level_ == 0);
    DecDCTvlc2((u_long *)(VLC_BUFFER + 0x19c),(u_long *)(&DCT_LOAD_BUF)[output_run_level_],
               &VLC_BUFFER->vlc_table);
    DecDCTin((u_long *)(&DCT_LOAD_BUF)[output_run_level_],2);
    DecDCTout((u_long *)(&IMAGE_BUFFER_START)[IMAGE_SIZE_OFFSET],((int)bit_depth * (int)height) / 2)
    ;
    FUN_8003a194();
    VSync(0);
    SetDefDispEnv(&DStack_40,0,(uint)(_buffer_counter_ == 0) << 8,320,256);
    DStack_40.screen.x = 0;
    DStack_40.screen.y = 0;
    DStack_40.screen.w = 0;
    DStack_40.screen.h = 240;
    DStack_40.isrgb24 = '\0';
    PutDispEnv(&DStack_40);
    SetDispMask(1);
    DecDCToutCallback((func *)0x0);
    param_4 = param_4 * 60;
    sVar4 = param_5 * 60;
    if (sVar4 == 0) {
      return 1;
    }
    while (bVar1 = param_4 != 0, param_4 = param_4 + -1, bVar1) {
      sVar4 = sVar4 + -1;
      VSync(0);
    }
    while (sVar4 != 0) {
                    // perhaps to skip movie?
      uVar3 = PadRead(1);
      if (param_5 == 99) {
        if ((uVar3 & 0x10) != 0) {
          return 0;
        }
        if ((uVar3 & 0x800) != 0) {
          return 1;
        }
      }
      else {
        if ((uVar3 & 0xffff) != 0) {
          return 1;
        }
        sVar4 = sVar4 + -1;
      }
      VSync(0);
    }
  }
  return 1;
}



void initialise_blb_struct(astruct_8 *blb_loading_struct,int buffer,void *load_asset_function)

{
  blb_loading_struct->game_blb_ptr = buffer;
  blb_loading_struct->load_asset_function = load_asset_function;
  blb_loading_struct->level_ = 0xff;
  reset_structs();
  return;
}



void reset_structs(astruct_3 *param_1)

{
  reset_struct(param_1);
  reset_other_struct((astruct_4 *)param_1);
  return;
}



void reset_struct(astruct_3 *param_1)

{
  param_1->field104_0x68 = 0;
  param_1->field105_0x6c = 0;
  param_1->field106_0x70 = 0;
  param_1->field107_0x74 = 0;
  param_1->field108_0x78 = 0;
  param_1->field109_0x7c = 0;
  return;
}



void reset_other_struct(astruct_4 *param_1)

{
  param_1->field0_0x0 = 0;
  param_1->field1_0x4 = 0;
  param_1->field2_0x8 = 0;
  param_1->field8_0x20 = 0;
  param_1->field9_0x24 = 0;
  param_1->field3_0xc = 0;
  param_1->field4_0x10 = 0;
  param_1->field11_0x2c = 0;
  param_1->field5_0x14 = 0;
  param_1->field6_0x18 = 0;
  param_1->field7_0x1c = 0;
  param_1->field10_0x28 = 0;
  param_1->field14_0x38 = 0;
  param_1->field15_0x3c = 0;
  param_1->field13_0x34 = 0;
  param_1->field12_0x30 = 0;
  param_1->field16_0x40 = 0;
  param_1->field17_0x44 = 0;
  param_1->field18_0x48 = 0;
  param_1->field19_0x4c = 0;
  param_1->field20_0x50 = 0;
  param_1->field21_0x54 = 0;
  param_1->field22_0x58 = 0;
  return;
}

// param_1 + 0x60 is the value to add to the address at param_1 + 0x5c

undefined FUN_8007a254(astruct_9 *blb_parse_struct)

{
  undefined uVar1;
  byte blb_level_;
  int byte;
  
  byte = blb_parse_struct->blb_base_ptr + (uint)blb_parse_struct->blb_level_;
  if ((*(char *)(byte + 0xf36) == '\x02') &&
     (byte = strcmp((char *)((uint)*(byte *)(byte + 0xf92) * 0xc + blb_parse_struct->blb_base_ptr +
                            0xf1c),&DAT_800a60b4), byte == 0)) {
    return 0;
  }
                    // gets set to 0x100 (0xff + 1)
  blb_level_ = blb_parse_struct->blb_level_ + 1;
  blb_parse_struct->blb_level_ = blb_level_;
  if ((uint)blb_level_ < (uint)*(byte *)(blb_parse_struct->blb_base_ptr + 0xf30)) {
    uVar1 = *(undefined *)(blb_parse_struct->blb_base_ptr + (uint)blb_level_ + 0xf36);
  }
  else {
    uVar1 = 0;
  }
                    // initial game load will skip to here as iVar3 is \x0
  return uVar1;
}


undefined4 parse_blb_header(astruct_5 *something,int ptr_to_level_data)

{
  char cVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  int ptr;
  ushort *puVar8;
  int blb_header;
  uint some_count;
  byte blb_id_;
  
  blb_header = something->blb_header_ptr;
  something->field29_0x68 = (ushort *)0x0;
  something->field30_0x6c = (uint *)0x0;
  something->field31_0x70 = 0;
  something->field32_0x74 = 0;
  something->field33_0x78 = 0;
  something->field34_0x7c = 0;
  something->field0_0x0 = 0;
  something->field1_0x4 = 0;
  something->field2_0x8 = 0;
  something->field8_0x20 = 0;
  something->field9_0x24 = 0;
  something->field3_0xc = 0;
  something->field4_0x10 = 0;
  something->field11_0x2c = 0;
  something->field5_0x14 = 0;
  something->field6_0x18 = 0;
  something->field7_0x1c = 0;
  something->field10_0x28 = 0;
  something->field14_0x38 = 0;
  something->field15_0x3c = 0;
  something->field13_0x34 = 0;
  something->field12_0x30 = 0;
  something->field16_0x40 = 0;
  something->field17_0x44 = 0;
  something->field18_0x48 = 0;
  something->field19_0x4c = 0;
  something->field20_0x50 = 0;
  something->field21_0x54 = 0;
  something->field22_0x58 = 0;
  ptr = blb_header + (uint)something->set_to_a_on_start;
  blb_id_ = *(byte *)(ptr + 0xf36);
  if (blb_id_ < 3) {
    uVar3 = 0;
  }
  else {
    uVar6 = 0;
                    // load byte located here into v1 (on boot this is 0x3)
    some_count = (uint)*(byte *)(ptr + 0xf92);
    uVar7 = 0;
    if (blb_id_ == 3) {
      puVar2 = (undefined2 *)(blb_header + some_count * 0x70);
                    // this is the first byte of the blb header
      uVar6 = *puVar2;
                    // and this is the 2nd?
      uVar7 = puVar2[1];
    }
    else if (blb_id_ == 6) {
      blb_header = blb_header + some_count * 4;
      uVar6 = *(undefined2 *)(blb_header + 0xecc);
      uVar7 = *(undefined2 *)(blb_header + 0xece);
    }
    cVar1 = (*(code *)something->field28_0x64)(uVar6,uVar7,ptr_to_level_data);
    uVar3 = 0;
    if (cVar1 != '\0') {
      something->field29_0x68 = (ushort *)ptr_to_level_data;
      if (*(char *)(something->blb_header_ptr + (uint)something->set_to_a_on_start + 0xf36) ==
          '\x03') {
        something->field30_0x6c =
             (uint *)(ptr_to_level_data +
                     *(int *)(something->blb_header_ptr + some_count * 0x70 + 8));
      }
      else {
        something->field30_0x6c = (uint *)0x0;
      }
      puVar8 = something->field29_0x68;
      some_count = 0;
      while (uVar4 = some_count & 0xffff, uVar4 < *puVar8) {
        blb_header = (int)something->field29_0x68 + *(int *)(puVar8 + uVar4 * 6 + 6);
        uVar5 = *(uint *)(puVar8 + uVar4 * 6 + 2);
        uVar3 = *(undefined4 *)(puVar8 + uVar4 * 6 + 4);
        if (uVar5 == 0x259) {
          something->field32_0x74 = blb_header;
          something->field33_0x78 = uVar3;
LAB_8007a7e4:
          some_count = some_count + 1;
        }
        else if (uVar5 < 0x25a) {
          if (uVar5 == 600) {
            something->field31_0x70 = blb_header;
            goto LAB_8007a7e4;
          }
          some_count = some_count + 1;
        }
        else {
          if (uVar5 == 0x25a) {
            something->field34_0x7c = blb_header;
            goto LAB_8007a7e4;
          }
          some_count = some_count + 1;
        }
      }
      if (*(char *)(something->blb_header_ptr + (uint)something->set_to_a_on_start + 0xf36) !=
          '\x03') {
        more_parsing_of_blb((astruct_14 *)something,1,'\x01');
      }
      uVar3 = 1;
    }
  }
  return uVar3;
}



undefined ptr_end_of_blb_header(astruct_10 *buffer)

{
  return *(undefined *)(buffer->blb_header_ptr + 0xf31);
}


byte get_blb_offset(int pointer)

{
  byte bVar1;
  byte bVar2;
  int level_;
  int pointer_to_blb_header;
  
  pointer_to_blb_header = *(int *)(pointer + 0x5c);
  level_ = pointer_to_blb_header + (uint)*(byte *)(pointer + 0x60);
  bVar1 = *(byte *)(level_ + 0xf36);
  if (bVar1 == 2) {
    bVar2 = (byte)*(undefined2 *)
                   (pointer_to_blb_header + (uint)*(byte *)(level_ + 0xf92) * 0xc + 0xf18);
  }
  else {
    bVar2 = 0;
    if (1 < bVar1) {
      if (bVar1 < 6) {
        bVar2 = 0;
        if (3 < bVar1) {
          bVar2 = (byte)*(undefined2 *)
                         (pointer_to_blb_header + (uint)*(byte *)(level_ + 0xf92) * 0x10 + 0xccc);
        }
      }
      else {
        bVar2 = 0;
      }
    }
  }
  return bVar2;
}



undefined2 get_blb_sector_count(int blb_buffer_800ae450)

{
  byte bVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = *(int *)(blb_buffer_800ae450 + 0x5c);
  iVar3 = iVar4 + (uint)*(byte *)(blb_buffer_800ae450 + 0x60);
  bVar1 = *(byte *)(iVar3 + 0xf36);
  if (bVar1 == 2) {
    uVar2 = *(undefined2 *)(iVar4 + (uint)*(byte *)(iVar3 + 0xf92) * 0xc + 0xf1a);
  }
  else {
    uVar2 = 0;
    if (1 < bVar1) {
      if (bVar1 < 6) {
        uVar2 = 0;
        if (3 < bVar1) {
          uVar2 = *(undefined2 *)(iVar4 + (uint)*(byte *)(iVar3 + 0xf92) * 0x10 + 0xcce);
        }
      }
      else {
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}



// something fun to look at
// 

undefined get_movie_count(int blb)

{
  return *(undefined *)(*(int *)(blb + 0x5c) + 0xf32);
}



// look at me !
// 

int movie_file_name(int param_1,uint param_2)

{
  return (param_2 & 0xff) * 0x1c + *(int *)(param_1 + 0x5c) + 0xb64;
}



// this will return the address of the `DREA` `PIRA` `SCI` type  headers in the game blb.

int find_blb_asset_addr(int blb_buffer_struct)

{
  int asset_addr;
  
                    // given the pointer of 0x8009dd20, we load the address at 0x8009dd20 + 0x5c
                    // (which is 0x800ae450). Then we load the byte at 0x8009dd20 + 0x60 and add it
                    // to the address. So we end up with 0x800ae452.
  asset_addr = *(int *)(blb_buffer_struct + 0x5c) + (uint)*(byte *)(blb_buffer_struct + 0x60);
                    // we then compare the byte located at 0x800ae452 + 0xf36 with 0x01. If it
                    // equals 1, we change our return value.
  if (*(char *)(asset_addr + 0xf36) == '\x01') {
                    // on boot, the byte at base_addr + 0xf92 is 0x0, so 0x0 * 0x1c + the gameblb
                    // adddress ( struct + 0x5c) + 0xb64 ends up being 0x800aefb4
    asset_addr = (uint)*(byte *)(asset_addr + 0xf92) * 0x1c + *(int *)(blb_buffer_struct + 0x5c) +
                 0xb64;
  }
  else {
    asset_addr = 0;
  }
  return asset_addr;
}



int load_another_blb_asset_ptr(int pointer)

{
  int asset_ptr;
  
  asset_ptr = *(int *)(pointer + 0x5c) + (uint)*(byte *)(pointer + 0x60);
  if (*(char *)(asset_ptr + 0xf36) == '\x01') {
    asset_ptr = (uint)*(byte *)(asset_ptr + 0xf92) * 0x1c + *(int *)(pointer + 0x5c) + 0xb6c;
  }
  else {
    asset_ptr = 0;
  }
                    // returns the ptr to \MVDWI.STR on boot
  return asset_ptr;
}



undefined2 load_Another_asset_ptr(int pointer)

{
  undefined2 asset_ptr;
  int iVar1;
  
  iVar1 = *(int *)(pointer + 0x5c) + (uint)*(byte *)(pointer + 0x60);
  asset_ptr = 0;
  if (*(char *)(iVar1 + 0xf36) == '\x01') {
    asset_ptr = *(undefined2 *)
                 (*(int *)(pointer + 0x5c) + (uint)*(byte *)(iVar1 + 0xf92) * 0x1c + 0xb60);
  }
  return asset_ptr;
}


// this looks very interesting

int FUN_8007ae5c(astruct_13 *blb_struct)

{
  int iVar1;
  
  switch(*(undefined *)(blb_struct->blb_header_ptr + (uint)blb_struct->bytes_to_add_to_ptr + 0xf36))
  {
  case 1:
                    // points to dream works (O.DREA.DW)
    iVar1 = (uint)*(byte *)(blb_struct->blb_header_ptr + (uint)blb_struct->bytes_to_add_to_ptr +
                           0xf92) * 0x1c + blb_struct->blb_header_ptr + 0xb64;
    break;
  case 2:
    iVar1 = (uint)*(byte *)(blb_struct->blb_header_ptr + (uint)blb_struct->bytes_to_add_to_ptr +
                           0xf92) * 0xc + blb_struct->blb_header_ptr + 0xf1c;
    break;
  case 3:
    iVar1 = blb_struct->blb_header_ptr +
            (uint)*(byte *)(blb_struct->blb_header_ptr + (uint)blb_struct->bytes_to_add_to_ptr +
                           0xf92) * 0x70 + 0x56;
    break;
  case 4:
  case 5:
    iVar1 = (uint)*(byte *)(blb_struct->blb_header_ptr + (uint)blb_struct->bytes_to_add_to_ptr +
                           0xf92) * 0x10 + blb_struct->blb_header_ptr + 0xcd3;
    break;
  default:
    iVar1 = 0;
  }
  return iVar1;
}


void more_parsing_of_blb(astruct_14 *param_1,int param_2,char param_3)

{
  byte bVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint *puVar10;
  uint *puVar11;
  
  bVar1 = *(byte *)(param_1->field23_0x5c + (uint)param_1->field24_0x60 + 0xf36);
  if ((2 < bVar1) && ((bVar1 == 3 || (param_3 != '\0')))) {
    iVar8 = param_1->field23_0x5c;
    param_1->field0_0x0 = 0;
    param_1->field1_0x4 = 0;
    param_1->field2_0x8 = 0;
    param_1->field8_0x20 = 0;
    param_1->field9_0x24 = 0;
    param_1->field3_0xc = 0;
    param_1->field4_0x10 = 0;
    param_1->field11_0x2c = 0;
    param_1->field5_0x14 = 0;
    param_1->field6_0x18 = 0;
    param_1->field7_0x1c = 0;
    param_1->field10_0x28 = 0;
    param_1->field14_0x38 = 0;
    param_1->field15_0x3c = 0;
    param_1->field13_0x34 = 0;
    param_1->field12_0x30 = 0;
    param_1->field16_0x40 = 0;
    param_1->field17_0x44 = 0;
    param_1->field18_0x48 = 0;
    param_1->field19_0x4c = 0;
    param_1->field20_0x50 = 0;
    param_1->field21_0x54 = 0;
    param_1->field22_0x58 = 0;
    iVar5 = iVar8 + (uint)param_1->field24_0x60;
    if (*(char *)(iVar5 + 0xf36) == '\x03') {
      uVar6 = (uint)*(byte *)(iVar5 + 0xf92);
      if (param_3 == '\0') {
        iVar5 = iVar8 + uVar6 * 0x70 + param_2 * 2;
        uVar2 = *(undefined2 *)(iVar5 + 0x38);
        uVar3 = *(undefined2 *)(iVar5 + 0x46);
      }
      else {
        iVar5 = iVar8 + uVar6 * 0x70 + param_2 * 2;
        uVar2 = *(undefined2 *)(iVar5 + 0x1c);
        uVar3 = *(undefined2 *)(iVar5 + 0x2a);
      }
      cVar4 = (*(code *)param_1->field28_0x64)(uVar2,uVar3,param_1->field30_0x6c);
      if (cVar4 == '\0') {
        if (param_2 == 1) {
          return;
        }
        more_parsing_of_blb(param_1,1,param_3);
        return;
      }
      puVar11 = param_1->field30_0x6c;
      param_1->field0_0x0 = param_2;
    }
    else {
      puVar11 = param_1->field29_0x68;
      param_1->field0_0x0 = 1;
    }
    puVar10 = puVar11;
    for (uVar6 = 0; uVar6 < *puVar11; uVar6 = uVar6 + 1) {
      uVar7 = puVar10[1];
      uVar9 = puVar10[2];
      iVar5 = (int)puVar11 + puVar10[3];
      if (uVar7 == 0x191) {
        param_1->field9_0x24 = iVar5;
      }
      else if (uVar7 < 0x192) {
        if (uVar7 == 300) {
          param_1->field5_0x14 = iVar5;
        }
        else if (uVar7 < 0x12d) {
          if (uVar7 == 0x65) {
            param_1->field2_0x8 = iVar5;
          }
          else if (uVar7 < 0x66) {
            if (uVar7 == 100) {
              param_1->field1_0x4 = iVar5;
            }
          }
          else if (uVar7 == 200) {
            param_1->field3_0xc = iVar5;
          }
          else if (uVar7 == 0xc9) {
            param_1->field4_0x10 = iVar5;
          }
        }
        else if (uVar7 == 0x12e) {
          param_1->field7_0x1c = iVar5;
        }
        else if (uVar7 < 0x12e) {
          param_1->field6_0x18 = iVar5;
        }
        else if (uVar7 == 0x12f) {
          param_1->field10_0x28 = iVar5;
        }
        else if (uVar7 == 400) {
          param_1->field8_0x20 = iVar5;
        }
      }
      else if (uVar7 == 0x1f8) {
        param_1->field13_0x34 = iVar5;
      }
      else if (uVar7 < 0x1f9) {
        if (uVar7 == 0x1f5) {
          param_1->field14_0x38 = iVar5;
        }
        else if (uVar7 < 0x1f6) {
          if (uVar7 == 500) {
            param_1->field11_0x2c = iVar5;
          }
        }
        else if (uVar7 == 0x1f6) {
          param_1->field15_0x3c = iVar5;
        }
        else if (uVar7 == 0x1f7) {
          param_1->field12_0x30 = iVar5;
        }
      }
      else if (uVar7 == 0x259) {
        param_1->field18_0x48 = iVar5;
        param_1->field19_0x4c = uVar9;
      }
      else if (uVar7 < 0x25a) {
        if (uVar7 == 600) {
          param_1->field16_0x40 = iVar5;
          param_1->field17_0x44 = uVar9;
        }
      }
      else if (uVar7 == 0x25a) {
        param_1->field20_0x50 = iVar5;
      }
      else if (uVar7 == 700) {
        param_1->field21_0x54 = iVar5;
        param_1->field22_0x58 = uVar9;
      }
      puVar10 = puVar10 + 3;
    }
  }
  return;
}

void set_blb_pointer(long zero,long buffer_address)

{
  SET_TO_ZERO = zero;
  BLB_PARSING_STRUCT = buffer_address;
  return;
}




int FUN_8007bad0(int param_1)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  
  if ((BLB_PARSING_STRUCT == 0) || (iVar1 = FUN_8007b928(BLB_PARSING_STRUCT,param_1), iVar1 == 0)) {
    uVar2 = 0;
    if (SET_TO_ZERO == 0) {
      iVar1 = 0;
    }
    else {
      piVar3 = (int *)(SET_TO_ZERO + 0x10);
      for (; iVar1 = 0, uVar2 < *(uint *)(SET_TO_ZERO + 0xc); uVar2 = uVar2 + 1) {
        if (*piVar3 == param_1) {
          return SET_TO_ZERO +
                 *(int *)(uVar2 * 0x14 + SET_TO_ZERO + *(uint *)(SET_TO_ZERO + 0xc) * 4 + 0x18);
        }
        piVar3 = piVar3 + 1;
      }
    }
  }
  return iVar1;
}



// this function appears to draw the level / intro title screens to the screen

void load_level_(astruct_6 *param_1,undefined *param_2)

{
  char cVar1;
  undefined uVar2;
  undefined uVar3;
  byte bVar4;
  undefined2 uVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  astruct_5 *buffer;
  byte bVar9;
  
  set_up_buffer(BUF_PTR);
  load_level(param_1,0x63);
  cVar1 = FUN_8007aa0c(&BUF_PTR->blb_reading_struct);
  if (cVar1 == '\0') {
    *PTR_DAT_800a59d8 = 1;
    PTR_DAT_800a59d8[1] = 1;
  }
  else {
    *PTR_DAT_800a59d8 = cVar1;
    uVar2 = old_debug__return_input(&param_1->field68_0x84);
    PTR_DAT_800a59d8[1] = uVar2;
  }
  uVar6 = FUN_8007b43c(&param_1->field68_0x84);
  if (((uVar6 & 0x40) != 0) && (param_1->field32_0x4a == 0x100)) {
    param_1->field32_0x4a = 0xf0;
  }
  iVar7 = 0;
  do {
    *(undefined2 *)((int)&param_1[1].field31_0x48 + ((iVar7 << 0x10) >> 0xf)) = 0;
    iVar7 = iVar7 + 1;
  } while (iVar7 * 0x10000 >> 0x10 < 8);
  param_1[1].field_0x58 = 0;
  param_1[1].field_0x5b = 0;
  *(undefined *)&param_1[1].field41_0x5c = 0;
  param_1[1].field_0x5a = 0;
  param_1[1].field_0x59 = 0;
  *(undefined *)((int)&param_1[1].field20_0x2c + 1) = 0;
  param_1[1].field_0x64 = 0;
  param_1[1].field_0x65 = 0x40;
  param_1[1].field_0x66 = 0x40;
  param_1[1].field_0x67 = 0x40;
  param_1[1].field23_0x38 = 0;
  param_1[1].some_method = param_2;
  param_1[1].field_0x17 = 0;
  param_1[1].some_other_mask = 0;
  param_1[1].some_mask = 0;
  param_1[1].field_0x10 = 0;
  param_1[1].field_0x11 = 0;
  param_1[1].field_0x12 = 0;
  param_1[1].field_0x13 = 0;
  param_1[1].field_0x14 = 0;
  param_1[1].field_0x15 = 0;
  param_1[1].field_0x16 = 0;
  *(undefined *)&param_1[1].field16_0x1c = 0;
  *(undefined *)((int)&param_1[1].field16_0x1c + 1) = 0;
  *(undefined *)((int)&param_1[1].field16_0x1c + 2) = 0;
  *(undefined *)&param_1[1].field51_0x68 = 0;
  *(undefined *)((int)&param_1[1].field51_0x68 + 1) = 0;
  *(undefined *)&param_1[1].field20_0x2c = 0;
  param_1->some_mask = 0xffff0000;
  param_1->some_other_method = &LAB_8007e6b0;
  FUN_80081568(param_1);
  buffer = &param_1->field68_0x84;
  uVar8 = FUN_8007b8e4(buffer);
  param_1[1].field21_0x30 = uVar8;
  uVar5 = FUN_8007b8d0(buffer);
  *(undefined2 *)&param_1[1].field22_0x34 = uVar5;
  FUN_80081ee0(param_1);
  param_1->field64_0x7c = &DAT_8009d654;
  param_1->field65_0x80 = 0x79;
  uVar2 = FUN_8007aa0c(buffer);
  uVar3 = old_debug__return_input(buffer);
  FUN_8007ca5c(uVar2,uVar3);
  bVar9 = 0;
  *(undefined *)((int)&param_1[1].field30_0x46 + 1) = 0;
  while( true ) {
    bVar4 = ptr_end_of_blb_header((astruct_10 *)buffer);
    if (bVar4 <= bVar9) break;
    if (((*(byte *)((int)&param_1[1].field30_0x46 + 1) < 10) &&
        (cVar1 = FUN_8007a9e8(buffer,bVar9), cVar1 != '\0')) &&
       (cVar1 = FUN_8007a984(buffer,bVar9), cVar1 != '\0')) {
      *(char *)((int)&param_1[1].buffer_to_load_blb +
               *(byte *)((int)&param_1[1].field30_0x46 + 1) + 1) = cVar1;
      *(char *)((int)&param_1[1].field30_0x46 + 1) =
           *(char *)((int)&param_1[1].field30_0x46 + 1) + '\x01';
    }
    bVar9 = bVar9 + 1;
  }
  uVar2 = FUN_8007b450(&param_1->field68_0x84);
  PTR_DAT_800a59d8[4] = uVar2;
  FUN_8007df94(param_1);
  return;
}



// maybe load level?
// 
// after selecting start game, mode is 0x63 (char == '?')

bool load_level(astruct_6 *level_struct,byte state_)

{
  undefined *puVar1;
  char mode;
  byte bVar2;
  undefined uVar3;
  undefined uVar4;
  char cVar5;
  bool bVar6;
  undefined2 blb_sector_count;
  short sVar7;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  char *some_asset_ptr;
  int compare_result;
  undefined3 extraout_var_01;
  uint sector_count;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  astruct_5 *pointer;
  int iVar14;
  bool bVar15;
  
  puVar1 = PTR_DAT_800a59b0;
  PTR_DAT_800a59b0[0x1d] = 0;
  puVar1[0x1e] = 0;
  puVar1[0x1f] = 0;
  puVar1[0x505d] = 0;
  puVar1[0x505e] = 0;
  puVar1[0x505f] = 0;
  draw_print_stream(PTR_DAT_800a59b0);
  bVar15 = false;
  vsync_if_required(PTR_DAT_800a59b0);
  FUN_80020a18(level_struct);
  FUN_80020a70(level_struct);
  FUN_80022334(level_struct);
  FUN_8007c2a0();
                    // this is 'c' on start-up
  if (state_ == 0x63) {
    state_ = 1;
                    // change some global variables
    FUN_8007c2a0();
    pointer = &level_struct->field68_0x84;
    do {
      mode = FUN_8007a254((astruct_9 *)&level_struct->field68_0x84);
      switch(mode) {
      case '\0':
        FUN_8007a36c(&level_struct->field68_0x84,0,1,0);
        break;
      case '\x01':
                    // load intro movies
        some_asset_ptr = (char *)find_blb_asset_addr(pointer);
                    // if find_blb_asset_header point to END2...
        compare_result = strcmp(some_asset_ptr,&END2_BLB);
        if ((compare_result != 0) || (0x2f < (byte)PTR_DAT_800a59d8[0x1b])) {
          some_asset_ptr = (char *)load_another_blb_asset_ptr(pointer);
          compare_result = load_Another_asset_ptr(pointer);
          sVar7 = FUN_8007ae18(pointer);
          bVar6 = load_str(some_asset_ptr,compare_result,sVar7,level_struct->buffer_to_load_blb);
          if (!bVar6) {
            some_asset_ptr = (char *)FUN_8007ae5c((astruct_13 *)pointer);
            compare_result = strcmp(some_asset_ptr,&INT1_BLB);
            if (compare_result == 0) {
              FUN_8007a254((astruct_9 *)pointer);
              FUN_8007a254((astruct_9 *)pointer);
              goto LAB_8007d540;
            }
          }
        }
        break;
      case '\x02':
        bVar2 = get_blb_offset(pointer);
        sector_count = get_blb_sector_count(pointer);
        sector_count = load_screen(CONCAT31(extraout_var_01,bVar2) & 0xffff,sector_count & 0xffff,
                                   level_struct->buffer_to_load_blb);
        if ((sector_count & 0xff) == 0) {
          some_asset_ptr = (char *)FUN_8007ae5c((astruct_13 *)pointer);
          compare_result = strcmp(some_asset_ptr,&CRD1_BLB);
          if (compare_result == 0) {
LAB_8007d540:
                    // is 0x05 before initial load screen (Skullmonkeys logo) is loaded
            FUN_8007a254((astruct_9 *)pointer);
          }
        }
        break;
      case '\x04':
      case '\x05':
        some_asset_ptr = (char *)FUN_8007ae5c((astruct_13 *)pointer);
        compare_result = strcmp(some_asset_ptr,&PIRA2_BLB);
        if (compare_result != 0) {
          draw_print_stream(PTR_DAT_800a59b0);
          vsync_if_required(PTR_DAT_800a59b0);
          bVar2 = get_blb_offset(pointer);
          blb_sector_count = get_blb_sector_count(pointer);
          uVar3 = FUN_8007ab04(pointer);
          uVar4 = FUN_8007ab48(pointer);
          cVar5 = draw_load_screen(CONCAT31(extraout_var,bVar2) & 0xffff,blb_sector_count,
                                   level_struct->buffer_to_load_blb,uVar3,uVar4);
          bVar15 = cVar5 == '\0';
          if (bVar15) {
            FUN_8007a36c(pointer,0,1,0);
            mode = FUN_8007a254((astruct_9 *)pointer);
            draw_print_stream(PTR_DAT_800a59b0);
            vsync_if_required(PTR_DAT_800a59b0);
            bVar2 = get_blb_offset(pointer);
            blb_sector_count = get_blb_sector_count(pointer);
            uVar3 = FUN_8007ab04(pointer);
            uVar4 = FUN_8007ab48(pointer);
            draw_load_screen(CONCAT31(extraout_var_00,bVar2) & 0xffff,blb_sector_count,
                             level_struct->buffer_to_load_blb,uVar3,uVar4);
            FUN_8007a36c(pointer,*PTR_DAT_800a59d8,0,0);
            state_ = PTR_DAT_800a59d8[1];
            FUN_800261d0();
          }
        }
      }
    } while ((mode != '\x03') && (mode != '\x06'));
    pointer = &level_struct->field68_0x84;
    parse_blb_header(pointer,(int)level_struct->buffer_to_load_blb);
    uVar8 = FUN_8007ba38(pointer);
    uVar9 = FUN_8007ba60(pointer);
    uVar10 = FUN_8007ba10(pointer);
    FUN_8007c048(uVar8,uVar9,uVar10);
  }
  mode = FUN_8007aa0c(&level_struct->field68_0x84);
  if ((mode == '\0') && (state_ == 1)) {
    state_ = 5;
    if (DAT_800a6108 == 4) {
      DAT_800a6108 = 0;
    }
    else {
      if ((DAT_800a6108 == 0) || (state_ = 6, DAT_800a6108 == 2)) {
        state_ = 1;
      }
      DAT_800a6108 = DAT_800a6108 + 1;
    }
  }
  pointer = &level_struct->field68_0x84;
  more_parsing_of_blb((astruct_14 *)pointer,(uint)state_,'\x01');
  uVar8 = FUN_8007ba38(pointer);
  uVar9 = FUN_8007ba60(pointer);
  uVar10 = FUN_8007ba10(pointer);
  FUN_8007c048(uVar8,uVar9,uVar10);
  uVar11 = FUN_8007a58c(pointer);
  compare_result = FUN_8007afb8(pointer);
  sector_count = compare_result + 0xfU & 0xfffffff0;
  if (sector_count == 0) {
    sector_count = uVar11;
  }
  iVar14 = uVar11 - sector_count;
  compare_result = (int)level_struct->buffer_to_load_blb + sector_count;
  FUN_800143a0(PTR_DAT_800a59b0,compare_result,-0x7fe04000 - compare_result);
  compare_result = 0;
  FUN_80014850(PTR_DAT_800a59b0);
  if (iVar14 != 0) {
    compare_result = FUN_800143ec(PTR_DAT_800a59b0,1,iVar14,1);
  }
  FUN_80014964(PTR_DAT_800a59b0,0xeeeeeeee);
  iVar12 = FUN_8007b3bc(pointer,0);
  iVar13 = FUN_8007b3bc(pointer,1);
  if ((iVar12 == 0) && (iVar13 == 0)) {
    iVar13 = 2;
  }
  FUN_80013b18((graphics *)PTR_DAT_800a59b0,(ushort)iVar12 & 0xff,(byte)iVar13);
  level_struct->field69_0x104 = 0;
  level_struct->field72_0x108 = 0;
  level_struct->some_mask = 0xffff0000;
  level_struct->some_other_method = &LAB_8007e6b0;
  level_struct->some_other_mask = 0xffff0000;
  level_struct->some_method = &LAB_80020f44;
  level_struct->field16_0x1c = (undefined4 *)0x0;
  level_struct->field17_0x20 = 0;
  level_struct->field18_0x24 = 0;
  level_struct->field19_0x28 = (undefined4 *)0x0;
  level_struct->field20_0x2c = 0;
  level_struct->field21_0x30 = 0;
  level_struct->field33_0x4c = 0;
  level_struct->field34_0x50 = 0;
  level_struct->field_0x58 = 0;
  level_struct->field_0x59 = 0;
  level_struct->field_0x5a = 0;
  level_struct->field_0x5b = 0;
  level_struct->field_0x60 = 0;
  level_struct->field41_0x5c = 0;
  level_struct->field42_0x5e = 0;
  level_struct->field_0x61 = 0;
  level_struct->field_0x62 = 0;
  level_struct->field_0x11a = 0;
  level_struct->field35_0x54 = 0;
  level_struct->field36_0x56 = 0;
                    // 0x34 and 0x38 get read by the method in the struct
  level_struct->field22_0x34 = 0;
  level_struct->field23_0x38 = 0;
  level_struct->field60_0x74 = 0;
  level_struct->field61_0x78 = 0;
  level_struct->field64_0x7c = 0;
  level_struct->field65_0x80 = 0;
  level_struct->field29_0x44 = 0;
  level_struct->field30_0x46 = 0;
  level_struct->field73_0x10c = 0;
  level_struct->field74_0x110 = 0;
  level_struct->field75_0x114 = 0;
  level_struct->field_0x63 = 0;
  level_struct->field85_0x120 = 0;
  level_struct->field86_0x122 = 0;
  level_struct->field_0x12a = 0x28;
  load_all_assets_for_level((astruct_15 *)level_struct);
  more_parsing_of_blb((astruct_14 *)&level_struct->field68_0x84,(uint)state_,'\0');
  if (compare_result != 0) {
    potentially_load_asset_(PTR_DAT_800a59b0,compare_result,iVar14,1);
  }
  FUN_80014964(PTR_DAT_800a59b0,0xeeeeeeee);
  *(undefined2 *)(PTR_DAT_800a59b0 + 0xa64e) = *(undefined2 *)(PTR_DAT_800a59b0 + 0xa64c);
  FUN_8002471c(level_struct);
  FUN_80024674(level_struct);
  FUN_800246cc(level_struct);
  FUN_800250c4(level_struct);
  FUN_80024cf0(level_struct);
  FUN_80024dc0(level_struct);
  FUN_80024f30(level_struct);
  FUN_80024774(level_struct);
  return bVar15;
}

