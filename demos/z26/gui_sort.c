/* 
** gui_sort.c -- comparator for gui_Qsort() - like strcasecmp() but knows about
**		directories ending in slash (they always sort lower than a
**		regular file) 
*/

int filename_cmp(char *left, char *right) {
	int rlen, llen;
	char llast, rlast;

	llen = strlen(left)-1;
	rlen = strlen(right)-1;
	llast = left[llen];
	rlast = right[rlen];

	if( llast == '/' && rlast == '/')
		return strcasecmp(left, right);
	else if(llast == '/')
		return -1;
	else if(rlast == '/')
		return 1;
	else
		return strcasecmp(left, right);
}

/* swap file ptrs (for gui_Qsort) */

void gui_swap(char *v[], int i, int j)
{
	char *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}


/* sort files -- Quicksort -- C.A.R. Hoare (1962) -- via K&R p. 110 */

void gui_Qsort(char *v[], int left, int right)
{
	int i, last;

	if (left >= right) return;

	gui_swap(v, left, (left + right)/2);
	last = left;
	for (i=left+1; i<=right; i++)
		if (filename_cmp(v[i], v[left]) < 0)
			gui_swap(v, ++last, i);
	gui_swap(v, left, last);
	gui_Qsort(v, left, last-1);
	gui_Qsort(v, last+1, right);
}

/*
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
