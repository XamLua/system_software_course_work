: IMMEDIATE 
last_word @ cfa 1 - dup c@ 1 or swap c! ;

: 2dup >r dup r> swap >r dup r> swap ;


: if 
     ' 0branch , here_adv 0 , 
; IMMEDIATE

: then 
     here_adv swap ! 
; IMMEDIATE

: else 
     ' branch , here_adv 0 , swap here_adv swap ! 
; IMMEDIATE


: repeat 
     here_adv
; IMMEDIATE

: until 
     ' 0branch , ,
; IMMEDIATE


: for
     ' swap , ' >r , ' >r ,
     here_adv ' r> , ' r> ,
     ' 2dup , ' >r , ' >r , ' < ,
     ' 0branch ,
     here_adv 0 , swap
; IMMEDIATE

: endfor
     ' r> , ' lit , 1 , ' + , ' >r ,
     ' branch , , here_adv swap ! 
     ' r> , ' drop , 
     ' r> , ' drop ,
; IMMEDIATE