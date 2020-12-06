#include "main.h"

void check_malloc(void *addr)
{
	if (!addr)
	{
		printf("Error with malloc!\n");
		exit(1);
	}
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_strdup(char *str)
{
	char *ret;
	int i;

	check_malloc(ret = (char*)malloc(sizeof(char) * ft_strlen(str)));
	i = 0;
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = 0;
	return ret;
}

void fire_warrior(t_player *p)
{
	t_warrior *tmp,*tmp2;
	int i, action;

	while (1)
	{
		printf("Who do you want to fire?\n");
		i = 0;
		tmp = p->warriors;
		while (tmp)
		{
			printf("[%d] -> %s, %d HP, %d ATK, %d Gold\n", i, tmp->name, tmp->hp, tmp->atk, tmp->cost);
			tmp = tmp->next;
			i++;
		}
		scanf("%d", &action);
		if (action < 0 || action > p->warrior_count - 1)
			printf("Wrong number! Please try again!\n");
		tmp = p->warriors;
		if (action == 0)
		{
			p->warriors = p->warriors->next;
			p->gold += tmp->cost;
			free(tmp);
		}
		else
		{
			i = 0;
			while (i < action - 1)
			{
				tmp = tmp->next;
				i++;
			}
			if (tmp->next)
				tmp2 = tmp->next->next;
			else
				tmp2 = NULL;
			p->gold += tmp->next->cost;
			free(tmp->next);
			tmp->next = tmp2;
		}
		p->warrior_count--;
		
		return ;
	}
}

void create_warrior(t_player *p, int hp, int atk, int cost, char *name)
{
	t_warrior *tmp;

	if (p->warriors)
	{
		tmp = p->warriors;
		while (tmp->next)
			tmp = tmp->next;
		check_malloc(tmp->next = (t_warrior*)malloc(sizeof(t_warrior)));
		tmp = tmp->next;
	}
	else
	{
		check_malloc(tmp = (t_warrior*)malloc(sizeof(t_warrior)));
		p->warriors = tmp;
	}
	tmp->hp = hp;
	tmp->atk = atk;
	tmp->cost = cost;
	tmp->name = name;
	tmp->next = NULL;
}

void buy_warrior(t_player *p)
{
	int action;
	char *name;
	int hp = 0;
	int atk = 0;
	int cost = 0;

	while (1)
	{
		printf("You have %d gold. Please choose a warrior:\n", p->gold);
		printf("[0] -> Fighter. 50 HP, 10 ATK, 500 Gold\n");
		printf("[1] -> Ranger. 35 HP, 20 ATK, 1000 Gold\n");
		printf("[2] -> Mage. 25 HP, 25 ATK, 1500 Gold\n");
		scanf("%d", &action);
		switch (action)
		{
			case 0:
				hp = 50;
				atk = 10;
				cost = 500;
				name = ft_strdup("Fighter");
				break;
			case 1:
				hp = 35;
				atk = 20;
				cost = 1000;
				name = ft_strdup("Ranger");
				break;
			case 2:
				hp = 25;
				atk = 25;
				cost = 1500;
				name = ft_strdup("Mage");
				break;
			default:
				printf("Wrong number! Please try again!\n\n");
		}
		if (hp && atk && cost)
		{
			p->gold -= cost;
			p->warrior_count++;
			create_warrior(p, hp, atk, cost, name);
			return ;
		}
	}
}

void init_army(t_player *p)
{
	int action;
	while (1)
	{
		printf("You have %d gold and %d warriors. What do you want to do?!\n", p->gold, p->warrior_count);
		if (p->warrior_count)
			printf("[0] -> Fire a warrior. (Get all your gold back!)\n");
		printf("[1] -> Buy a warrior! (All prices and stats will be displayed)\n");
		printf("[2] -> Finish your army and go fight!\n");
		scanf("%d", &action);
		switch (action)
		{
			case 0:
				if (p->warrior_count)
					fire_warrior(p);
				else
					printf("Wrong number! Please try again!\n");
				break;
			case 1:
				buy_warrior(p);
				break;
			case 2:
				printf("%s finished with army!\n\n", p->name);
				return;
			default:
				printf("Wrong number! Please try again!\n");
		}
	}
}

t_player *init_player()
{
	t_player *p;
	char tmp[65535];

	check_malloc(p = (t_player*)malloc(sizeof(t_player)));
	p->gold = 5000;
	p->warrior_count = 0;
	p->warriors = NULL;
	printf("Hello, contestant!\nPlease enter your name: ");
	scanf("%s", tmp);
	p->name = ft_strdup(tmp);
	init_army(p);
	return p;
}

int fight(t_player *p1, t_player *p2) {
	t_warrior *w1, *w2, *tmp;
	int action;

	w1 = p1->warriors;
	w2 = p2->warriors;
	while (p1->warrior_count && p2->warrior_count)
	{
		while (w1->hp > 0 && w2->hp > 0)
		{
			w2->hp -= w1->atk;
			w1->hp -= w2->atk;
		}
		if (w2->hp <= 0 && w1->hp <= 0)
			printf("DOUBLE KILL!!! Warriors killed themselves!\n");
		if (w2->hp <= 0)
		{
			printf("%s killed on %s's side!\n", w2->name, p2->name);
			tmp = w2->next;
			free(w2->name);
			free(w2);
			w2 = tmp;
			p2->warrior_count--;
		}
		if (w1->hp <= 0)
		{
			printf("%s killed on %s's side!\n", w1->name, p1->name);
			tmp = w1->next;
			free(w1->name);
			free(w1);
			w1 = tmp;
			p1->warrior_count--;
		}
	}
	if (p1->warrior_count)
		printf("%s IS THE WINNER! %s, better luck next time!\n", p1->name, p2->name);
	else if (p2->warrior_count)
		printf("%s IS THE WINNER! %s, better luck next time!\n", p2->name, p1->name);
	else
		printf("BOTH PLAYERS LOST THEIR SOLDIERS! OMG!\n");
	printf("Play again? (0/1)\n");
	scanf("%d", &action);
	return action;
}

int main() {
	t_player *p1, *p2;

	p1 = init_player();
	p2 = init_player();
	while (fight(p1, p2));
}