#include <stdio.h>
#include <stdlib.h>

typedef struct	s_warrior
{
	int	hp;
	int	atk;
	int cost;
	char *name;
	struct s_warrior *next;
}				t_warrior;

typedef struct	s_player
{
	char		*name;
	int			gold;
	int			warrior_count;
	t_warrior	*warriors;
}				t_player;