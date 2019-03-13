( a b -- 1\0 )
: is-divisor
	% if 0 else 1 then ;

( a -- 1\0 )
: parity-check
	2 is-divisor ;

( a -- a*a )
: sqr
	dup * ;

( a -- 1\0 )
: is-prime
	dup 2 <
	if drop 0 else 
		dup 2 = 
		if drop 1 else 
			1
			repeat
				1 + 2dup is-divisor
				if 2drop 0 exit else 
					2dup sqr < 
					if 2drop 1 exit else 0 then
				then
			until 
		then
	then ;

( a -- addr_with_result )
: allot-prime-result
	is-prime 1 allot 2dup ! swap drop ;

( addr_1 addr_2 -- addr_3 )
: concat
	swap 2dup count swap count + 1 + heap-alloc
	( addr_2 addr_1 addr_3 )
	swap 2dup string-copy count over +
	( addr_2 addr_3 addr_3_with_offset )
	rot string-copy ;

( a b -- n)
: divisor-power
	0 -rot
	repeat
		2dup is-divisor
		if swap over / swap rot 1 + -rot 0 
		else 2drop exit then
	until ;

( a -- 1\0 )
: is-primary
	dup 2 <
	if drop 0 else 
		1
		repeat
			1 + dup is-prime
			if 2dup divisor-power 1 >
				if 2drop 0 exit then	
			else 2dup sqr <
				if 2drop 1 exit then
			then 0
		until 
	then ; 

( a -- )
: bool-to-text
	if ." true" else ." false" then ;

( TESTS )

."                    OVERVIEW          	          \n"
." Student: N. Shishkin, Group: P3200, Var: " 
m" Shishkin" dup string-hash 3 % . heap-free cr
cr
." Parity tests:\n"
." 2 - " 2 parity-check bool-to-text cr
." 13 - " 13 parity-check bool-to-text cr
." 124 - " 124 parity-check bool-to-text cr
cr
." Prime tests:\n"
." 2 - " 2 is-prime bool-to-text cr
." -5 - " -5 is-prime bool-to-text cr
." 11 - " 11 is-prime bool-to-text cr
." 28 - " 28 is-prime bool-to-text cr
." 179426549 - " 179426549 is-prime bool-to-text cr
cr
." Allot prime tests:\n"
." 9 - " 9 allot-prime-result dup @ bool-to-text
." , address - " . cr
." 113 - " 113 allot-prime-result dup @ bool-to-text
." , address - " . cr
cr
." Concatenation tests:\n"
." 1st string - " m" Blade" dup prints cr
." 2nd string - " m" runner" dup prints cr
." Concat res - " 2dup concat dup prints heap-free heap-free heap-free cr
." \nHeap Info: \n" heap-show cr
." Primary tests:\n"
." 2 - " 2 is-primary bool-to-text cr
." 4 - " 4 is-primary bool-to-text cr
." 20 - " 20 is-primary bool-to-text cr
." 15 - " 15 is-primary bool-to-text cr
." 30030 - " 30030 is-primary bool-to-text cr