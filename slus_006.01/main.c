
void main(void)

{
  byte bVar1;
  int print_stream;
  uint size;
  undefined4 name_ptr;
  undefined *movie_ptr;
  u_long controller_btn_status;
  byte i;
  uint counter;
  int buf_ptr2;
  uint level_id;
  astruct_10 *blb_ptr;
  undefined4 unaff_s4;
  code *pcVar2;
  
  __main();
  counter = 0;
  SsUtReverbOff();
  ResetCallback();
  load_files_from_cd();
  setup_graphics((graphics *)PTR_DAT_800a59b0);
  BUF_PTR = &BUFFER;
  PadInit(0);
  InitGeom();
  SetDispMask(1);
  FntLoad(0x3c0,0x100);
  print_stream = FntOpen(0x10,0x20,0x120,200,0,0x200);
  SetDumpFnt(print_stream);
  initialise_struct(PTR_DAT_800a59d8);
  load_level_(&BUFFER,pad_1);
  blb_ptr = (astruct_10 *)(BUF_PTR + 0x42);
  while( true ) {
                    // paVar8 is 8009dc9c at start up
                    // got_size is 1a
    size = ptr_end_of_blb_header(blb_ptr);
    level_id = counter & 0xff;
                    // I think this loads all the level info from the blb header
                    // 
                    // e.g. the MENU Option stuff
    if ((size & 0xff) <= level_id) break;
    name_ptr = level_name(blb_ptr,level_id);
    (&levels)[level_id] = name_ptr;
    counter_1 = counter_1 + 1;
                    // seems to reach 1a (got_size) when loop breaks
    counter_2 = counter_2 + 1;
    counter = counter + 1;
  }
  for (i = 0; bVar1 = get_movie_count(blb_ptr), i < bVar1; i = i + 1) {
    movie_ptr = (undefined *)movie_file_name(blb_ptr);
    counter = (uint)counter_1;
    counter_1 = counter_1 + 1;
    yet_another_counter = yet_another_counter + 1;
    (&sub_level_list)[counter] = movie_ptr;
  }
  DAT_800a59c8 = 0x40;
  DAT_800a59ca = 0x20;
  DAT_800a59cc = 0x80;
  DAT_800a6188 = pad_1;
  do {
    FUN_8007cc78();
    controller_btn_status = PadRead(1);
    setup_controller(pad_1,controller_btn_status & 0xffff);
    setup_controller(pad_2,controller_btn_status >> 0x10);
    print_stream = (int)BUF_PTR[1];
    if (print_stream != 0) {
      if (print_stream < 1) {
                    // executes
                    // UndefinedFunction_8007e6b0
        pcVar2 = *(code **)(BUF_PTR + 2);
      }
      else {
        buf_ptr2 = print_stream * 8 + *(int *)((int)BUF_PTR + (int)BUF_PTR[2]);
        unaff_s4 = *(undefined4 *)(buf_ptr2 + -8);
        pcVar2 = *(code **)(buf_ptr2 + -4);
      }
      buf_ptr2 = (int)*BUF_PTR;
      if (0 < print_stream << 0x10) {
        buf_ptr2 = (short)unaff_s4 + buf_ptr2;
      }
      (*pcVar2)((int)BUF_PTR + buf_ptr2);
    }
    traverse_linked_list_and_execute(BUF_PTR);
    vsync_if_required(PTR_DAT_800a59b0);
    FUN_80020e7c(BUF_PTR);
    DrawSync(0);
    (**(code **)(*(int *)(BUF_PTR + 0xc) + 0x1c))
              ((int)BUF_PTR + (int)*(short *)(*(int *)(BUF_PTR + 0xc) + 0x18));
    DrawSync(0);
    if ((DAT_800a59b4 & 6) != 0) {
      print_stream = VSync(-1);
      do {
        counter = VSync(-1);
      } while (counter < print_stream + 2U);
    }
                    // maybe not for processing levels as it occurs after main game loop
    process_level_data_();
    draw_print_stream(PTR_DAT_800a59b0);
  } while( true );
}