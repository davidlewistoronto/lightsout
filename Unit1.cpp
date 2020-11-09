//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#define puz_size	5
#define n_locations	(puz_size * puz_size)
#define n_states	(1<<(n_locations))
#define index_to_row(i)		((i)%puz_size)
#define index_to_col(i)		((i)/puz_size)
#define row_col_to_index(r,c)		((r)+(c)*puz_size)

typedef struct {
	int win_next;
	int queue_next;
	} t_state;

t_state *state_table;
#define null_state	-1

unsigned move_masks [n_locations];


//---------------------------------------------------------------------------
unsigned mask_move_bit (int irow, int icol)
{	if (irow >= 0 && irow < puz_size && icol >= 0 && icol < puz_size)
	{	return (1 << row_col_to_index (irow, icol));
	}
	else
	{	return 0;
	}
}

void init_move_masks (void)
{	int irow;
	int icol;
	int imove;
	unsigned mask;

	for (imove = 0; imove < n_locations; imove++)
	{	irow = index_to_row (imove);
		icol = index_to_col (imove);

		mask = 0;
		mask |= mask_move_bit (irow, icol);
		mask |= mask_move_bit (irow - 1, icol);
		mask |= mask_move_bit (irow, icol - 1);
		mask |= mask_move_bit (irow + 1, icol);
		mask |= mask_move_bit (irow, icol + 1);
		move_masks [imove] = mask;
	}
}

void solve (void)
{	int head;
	int tail;
	int i;
	int imove;
	unsigned this_state;
	unsigned prev_state;

	init_move_masks ();
	state_table = (t_state *) malloc (n_states * sizeof (t_state));
	for (i = 0; i < n_states; i++)
	{	state_table [i].win_next = null_state;
		state_table [i].queue_next = null_state;
	}
	head = 0;
	tail = 0;
	state_table [0].queue_next = null_state;

	while (head != null_state)
	{	this_state = (unsigned) head;
		for (imove = 0; imove < n_locations; imove++)
		{	prev_state = this_state ^ move_masks [imove];
			if (state_table [prev_state].win_next == null_state && state_table [prev_state].queue_next == null_state)
			{	state_table [prev_state].win_next = this_state;
				state_table [tail].queue_next = prev_state;
				tail = prev_state;
//				printf ("adding %08x to %08x\n", prev_state, this_state);
			}
		}
		head = state_table [head].queue_next;
	}
}


#pragma argsused
int main(int argc, char* argv[])
{   solve ();
	return 0;
}
//---------------------------------------------------------------------------
 