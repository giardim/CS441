; demo.asm
;
; reduces runs of 'a's or 'b's to a single 'a' or 'b'

!main
	alpha "ab"

	cmp 'a'
	je !new_a
	cmp 'b'
	je !new_b
	halt

!new_a
	alpha 'A'
	alpha 'B'
	draw BLANK
	right
	cmp 'a'
	je !new_a

!to_end_with_a
	cmp BLANK
	je !right_of_possible_BLANK_with_a
	right
!right_of_possible_BLANK_with_a
	jne !to_end_with_a

	draw 'A'
	left
	jmp !to_first_BLANK

!new_b
	alpha 'A'
	alpha 'B'
	draw BLANK
	right
	cmp 'b'
	je !new_b

!to_end_with_b
	cmp BLANK
	je !right_of_possible_BLANK_with_b
	right
!right_of_possible_BLANK_with_b
	jne !to_end_with_b

	draw 'B'
	left
	jmp !to_first_BLANK

!to_first_BLANK
	cmp BLANK
	je !left_of_possible_BLANK
	left
!left_of_possible_BLANK
	jne !to_first_BLANK

	right

	cmp 'a'
	je !new_a
	cmp 'b'
	je !new_b

!go_right
	cmp BLANK
	je !going_right_past_possible_BLANK
	right
!going_right_past_possible_BLANK
	jne !go_right

	left

!finalize
	cmp 'A'
	je !lower_A
	cmp 'B'
	je !lower_B

	right
	halt

!finale
	cmp BLANK
	je !finale_left_of_possible_BLANK
	left
!finale_left_of_possible_BLANK
	jne !finale

	right
	halt

!lower_A
	draw 'a'
	left
	jmp !finalize

!lower_B
	draw 'b'
	left
	jmp !finalize
