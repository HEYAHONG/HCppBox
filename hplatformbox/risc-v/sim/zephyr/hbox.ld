  SECTION_DATA_PROLOGUE(.HShellCommand,,)
  {
    . = ALIGN(8);
    PROVIDE ( __hshell_command_start = ABSOLUTE(.) );
    KEEP ( *(SORT (.HShellCommand)))
    PROVIDE ( __hshell_command_end = ABSOLUTE(.) );
  } GROUP_LINK_IN(ROMABLE_REGION)

  SECTION_DATA_PROLOGUE(.HRuntimeInit,,)
  {
    . = ALIGN(8);
    PROVIDE ( __hruntime_init_start = ABSOLUTE(.) );
    KEEP ( *(SORT (.HRuntimeInit)))
    PROVIDE ( __hruntime_init_end = ABSOLUTE(.) );
  } GROUP_LINK_IN(ROMABLE_REGION)

  SECTION_DATA_PROLOGUE(.HRuntimeLoop,,)
  {
    . = ALIGN(8);
    PROVIDE ( __hruntime_loop_start = ABSOLUTE(.) );
    KEEP ( *(SORT (.HRuntimeLoop)))
    PROVIDE ( __hruntime_loop_end = ABSOLUTE(.) );
  } GROUP_LINK_IN(ROMABLE_REGION)

  SECTION_DATA_PROLOGUE(.HRuntimeSymbol,,)
  {
    . = ALIGN(8);
    PROVIDE ( __hruntime_symbol_start = ABSOLUTE(.) );
    KEEP ( *(SORT (.HRuntimeSymbol)))
    PROVIDE ( __hruntime_symbol_end = ABSOLUTE(.) );
  } GROUP_LINK_IN(ROMABLE_REGION)
