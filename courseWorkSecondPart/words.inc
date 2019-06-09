%define STATE_COMPILE 1
%define STATE_INTERPRET 0

colon "bye", bye
    dq xt_lit, 60
    TIMES 6 dq xt_lit, 0
    dq xt_syscall

%define sys_read_no 0
%define sys_write_no 1

colon "sys-read", sys_read
TIMES 3 dq xt_push_r
dq xt_lit, sys_read_no
TIMES 3 dq xt_pop_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit

colon "sys-write", sys_write
TIMES 3 dq xt_push_r
dq xt_lit, sys_write_no
TIMES 3 dq xt_pop_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit


colon  "stack-cell", stack_cell
  dq xt_lit, 0, xt_sp, xt_exit

colon "file-read-char", file_read_char
  dq xt_push_r, xt_stack_cell, xt_pop_r, xt_swap, xt_lit, 1, xt_sys_read
	branch0 .fail
  dq xt_lit, 1
.fail :
	dq xt_exit

colon "file-write-char", file_write_char
  dq xt_swap, xt_push_r, xt_push_r
  dq xt_stack_cell, xt_dup, xt_pop_r, xt_swap, xt_write_byte
  dq xt_pop_r, xt_swap, xt_lit, 1, xt_sys_write, xt_drop, xt_drop, xt_exit

colon "string-length", string_length
      dq xt_dup
.loop:
      dq xt_dup, xt_fetch_byte
      branch0 .exit
      dq xt_inc
      branch .loop
.exit:
  dq xt_swap, xt_sub, xt_exit

colon "print-cr", print_cr
  dq xt_lit, 1, xt_lit, 10, xt_file_write_char, xt_exit

colon "print-string-counted", print_string_counted
  dq xt_push_r, xt_push_r, xt_lit, 1, xt_pop_r, xt_pop_r, xt_sys_write, xt_drop, xt_exit

colon "print-string", print_string
  dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

colon "char-is-space", char_is_space
  dq xt_push_r
  dq xt_fetch_r, xt_lit,  ' ', xt_equals
  dq xt_fetch_r, xt_lit, 10, xt_equals, xt_lor
  dq xt_fetch_r, xt_lit, 13, xt_equals, xt_lor
  dq xt_pop_r,  xt_lit, '\t', xt_equals, xt_lor, xt_exit

colon "string-terminate", string_terminate
  dq xt_lit, 0, xt_swap, xt_write_byte, xt_exit

colon "file-read-word", file_read_word
  dq xt_dup, xt_push_r, xt_push_r

.read_space:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space
  branch0 .commit

  dq xt_drop
  branch .read_space

.read:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space, xt_not
  branch0 .end

.commit:
  dq xt_fetch_r, xt_write_byte
  dq xt_pop_r, xt_inc, xt_push_r
  branch .read

.end:
  dq xt_drop
.eof:
  dq xt_drop, xt_fetch_r, xt_string_terminate
  dq xt_pop_r, xt_pop_r, xt_sub, xt_exit



colon "c,", char_comma
  dq xt_here, xt_fetch, xt_write_byte
  dq xt_here, xt_fetch, xt_inc
  dq xt_here, xt_write, xt_exit

colon "2dup", ddup
  dq xt_push_r, xt_dup, xt_fetch_r, xt_swap, xt_pop_r, xt_exit

colon "2inc", dinc
  dq xt_push_r, xt_inc, xt_pop_r, xt_inc, xt_exit

colon "string-equals", string_equals
.start:
  dq xt_ddup, xt_fetch_byte, xt_swap, xt_fetch_byte, xt_equals
  branch0 .no
  dq xt_dup, xt_fetch_byte
  branch0 .yes
  dq xt_dinc
  branch .start
.yes:
  dq xt_drop, xt_drop, xt_lit, 1, xt_exit
.no:
  dq xt_drop, xt_drop, xt_lit, 0, xt_exit

colon "forth-dict-word-name", forth_dict_word_name
  dq xt_lit, 9, xt_add, xt_exit

colon "forth-find-word", forth_find_word
  dq xt_push_r, xt_last_word, xt_fetch
  dq xt_state, xt_fetch
  branch0 .loop
  dq xt_fetch
.loop:
  dq xt_dup
  branch0 .not_found

  dq xt_dup, xt_forth_dict_word_name, xt_fetch_r, xt_string_equals, xt_not
  branch0 .found

  dq xt_fetch
  branch .loop
.found:
  dq xt_lit, 1
.not_found:
  dq xt_pop_r, xt_drop, xt_exit


colon "string-copy", string_copy
.loop:
dq xt_ddup, xt_fetch_byte, xt_push_r, xt_fetch_r, xt_swap, xt_write_byte
dq xt_dinc, xt_pop_r, xt_not
  branch0 .loop
  dq xt_drop, xt_drop, xt_exit

colon "forth-create", forth_create
  dq xt_here, xt_fetch                
  dq xt_last_word, xt_fetch, xt_comma 
  dq xt_last_word, xt_write           
  dq xt_lit, 0, xt_char_comma
  dq xt_dup, xt_here, xt_fetch, xt_swap, xt_string_copy
  dq xt_string_length, xt_here, xt_fetch, xt_add, xt_inc, xt_here, xt_write
  dq xt_char_comma
  dq xt_exit

colon ":", colon
  .restart:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .restart
    dq xt_lit, 0, xt_lit, word_buf, xt_forth_create
    dq xt_lit, STATE_COMPILE, xt_state, xt_write
    dq xt_lit, i_docol, xt_comma, xt_exit

colon ";", semicolon, 1
    dq xt_lit, STATE_INTERPRET, xt_state, xt_write
    dq xt_lit, xt_exit, xt_comma, xt_exit

colon "char-is-digit", char_is_digit
  dq xt_push_r
  dq xt_fetch_r, xt_lit, '9'+1, xt_less
  dq xt_lit, '0'-1, xt_fetch_r, xt_less, xt_land
  dq xt_pop_r, xt_drop, xt_exit

colon "drop0", drop0
  dq xt_dup
  branch0 .is_zero
  dq xt_exit
  .is_zero:
  dq xt_drop, xt_exit

colon "char-to-digit", char_to_digit
  dq xt_lit, '0', xt_sub, xt_exit

colon "string-parse-unsigned-number", string_parse_unsigned_number
  dq xt_dup, xt_push_r
  dq xt_lit, 0, xt_push_r
.loop:
  dq xt_dup, xt_fetch_byte, xt_dup, xt_char_is_digit
  branch0 .end
  dq xt_char_to_digit
  dq xt_lit, 10, xt_pop_r, xt_mul, xt_add, xt_push_r
  dq xt_inc
  branch .loop
  .end:
  dq xt_drop, xt_pop_r, xt_swap, xt_pop_r, xt_sub, xt_drop0, xt_exit

colon "neg", neg
  dq xt_lit, 0, xt_swap, xt_sub, xt_exit

colon "inc", inc
  dq xt_lit, 1, xt_add, xt_exit

colon "string-parse-number", string_parse_number
  dq xt_dup, xt_fetch_byte, xt_dup, xt_char_is_digit, xt_not
  branch0 .unsigned
  dq xt_dup, xt_lit, '-', xt_equals, xt_not
  branch0 .negative

  dq xt_dup, xt_lit, '+', xt_equals, xt_not
  branch0 .positive

.negative:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc
  dq xt_swap, xt_neg, xt_swap, xt_exit
.positive:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc, xt_exit
.unsigned:
  dq xt_drop, xt_string_parse_unsigned_number, xt_exit 
.error:
  dq xt_drop, xt_lit, 0, xt_exit

colon "forth-follows-branch", forth_follows_branch
    dq xt_here, xt_fetch, xt_lit, 8, xt_sub, xt_fetch
    dq xt_dup, xt_lit, xt_branch0, xt_equals, xt_swap
    dq xt_lit, xt_branch, xt_equals, xt_lor, xt_exit

colon "forth-interpret-fd", forth_interpret_fd
    dq xt_lit, xt_fd, xt_fetch, xt_push_r
    dq xt_fd, xt_write

    .loop:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .eof

    dq xt_lit, word_buf, xt_forth_interpret

    branch .loop

    .eof:
    dq xt_pop_r, xt_fd, xt_write, xt_exit

colon "forth-xt-is-immediate", forth_xt_is_immediate
    dq xt_lit, 1, xt_sub, xt_fetch_byte, xt_exit

colon "forth-interpret", forth_interpret
.start:
    dq xt_dup, xt_forth_find_word         
    branch0 .number                       
    dq xt_swap, xt_drop, xt_cfa     

    dq xt_state, xt_fetch
    branch0 .interpreting

    dq xt_dup, xt_forth_xt_is_immediate, xt_not
    branch0 .interpreting

.compiling:
    dq xt_comma, xt_exit
.interpreting:
    dq xt_execute, xt_exit

.number:
    dq xt_dup, xt_string_parse_number      
    branch0 .no_word

    dq xt_swap, xt_drop
    dq xt_state, xt_fetch                  
    branch0 .exit

    dq xt_forth_follows_branch, xt_not
    branch0 .is_branch

.not_branch:
    dq xt_lit, xt_lit, xt_comma
.is_branch: 
    dq xt_comma, xt_exit

.no_word:
  dq xt_lit, msg.no_such_word, xt_print_string, xt_print_string, xt_print_cr

.exit:
    dq xt_exit

native "forth-stack-restore", forth_stack_restore
  mov rsp, [stack_start]
  jmp next

colon "trap", trap
    dq xt_lit, wh_trap + 9, xt_print_string, xt_print_cr, xt_exit

colon "count", count
    dq xt_pop_r, xt_drop, xt_drop, xt_exit

forth_init:
  dq xt_lit, 0, xt_forth_interpret_fd, xt_bye