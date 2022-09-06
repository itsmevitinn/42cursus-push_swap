/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsergio <vsergio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 11:56:08 by vsergio           #+#    #+#             */
/*   Updated: 2022/09/05 18:02:25 by vsergio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "push_swap.h"

int main(int argc, char **argv)
{
	node_list **stack_a;
	node_list **stack_b;
	int indexargv;

	stack_a = malloc(sizeof(node_list));
	stack_b = malloc(sizeof(node_list));
	indexargv = 1;
	while (indexargv < argc)
		insert_last_node(stack_a, ft_atoi(argv[indexargv++]));
	check_errors(stack_a);
	if ((argc - 1) == 3)
	{
		sort_3(stack_a);
	}
	else if ((argc - 1) == 5)
	{
		sort_5(stack_a, stack_b, argc - 1);
	}
	else if ((argc - 1) == 16)
	{
		sort_100(stack_a, stack_b, argc - 1);
		*stack_b = NULL;
	}
	else if ((argc - 1) > 100)
	{
		get_index(stack_a);
		radix_sort(stack_a, stack_b);
	}
	print_stacks(stack_a, stack_b);
	return (0);
}

void sort_3(node_list **stack_a)
{
	int highest_value;
	int highest_position;
	int smallest_value;
	int smallest_position;

	while (!check_order(stack_a))
	{
		highest_value = pick_highest(stack_a, 3);
		highest_position = find_position(stack_a, highest_value);
		smallest_value = pick_smallest(stack_a, 3);
		smallest_position = find_position(stack_a, smallest_value);
		if (highest_position == 2 && smallest_position == 1)
		{
			reverse_rotate(stack_a, 'a');
			swap(stack_a, 'a');
		}
		else if (highest_position == 3 && smallest_position == 2)
			swap(stack_a, 'a');
		else if (highest_position == 2 && smallest_position == 3)
			reverse_rotate(stack_a, 'a');
		else if (highest_position == 1 && smallest_position == 2)
			rotate(stack_a, 'a');
		else if (highest_position == 1 && smallest_position == 3)
		{
			rotate(stack_a, 'a');
			swap(stack_a, 'a');
		}
	}
}

void sort_5(node_list **stack_a, node_list **stack_b, int len)
{
	int smallest_position;
	int smallest_value;
	int half;
	int counter;

	counter = 0;
	while (counter < 2)
	{
		half = len / 2;
		smallest_value = pick_smallest(stack_a, len);
		smallest_position = find_position(stack_a, smallest_value);
		while (smallest_position > half)
		{
			if (smallest_position == len)
			{
				reverse_rotate(stack_a, 'a');
				push(stack_a, stack_b, 'b');
				len--;
				break;
			}
			reverse_rotate(stack_a, 'a');
			smallest_position++;
		}
		while (smallest_position <= half)
		{
			if (smallest_position == 1)
			{
				push(stack_a, stack_b, 'b');
				len--;
				break;
			}
			rotate(stack_a, 'a');
			smallest_position--;
		}
		counter++;
	}
	if (!check_order(stack_a))
		sort_3(stack_a);
	while (*stack_b != NULL)
		push(stack_a, stack_b, 'a');
}

void sort_100(node_list **stack_a, node_list **stack_b, int len)
{
	int highest_position;
	int highest_value;
	int middle;
	int half;
	int counter;
	
	counter = 0;
	while (!check_order(stack_a) && len > 3)
	{
		counter = 0;
		len = count_len(stack_a);
		middle = greb_middle(stack_a, len);
		while (counter < len)
		{
			if ((*stack_a)->value < middle)
				push(stack_a, stack_b, 'b');	
			else
				rotate(stack_a, 'a');
			counter++;
		}
	}
	if (len == 3)
		sort_3(stack_a);
	len = count_len(stack_b);
	while(*stack_b != NULL && len > 0)
	{
		highest_value = pick_highest(stack_b, len);
		highest_position = find_position(stack_b, highest_value);
		while(highest_position <= half)
		{
			if (highest_position == 1)
			{
				push(stack_a, stack_b, 'a');
				len--;
				break;		
			}
			rotate(stack_b, 'b');
			highest_position--;
		}
		while(highest_position > half)
		{
			if (highest_position == len)
			{
				reverse_rotate(stack_b, 'b');
				push(stack_a, stack_b, 'a');
				len--;
				break;		
			}
			reverse_rotate(stack_b, 'b');
			highest_position++;
		}
	}
}

void radix_sort(node_list **stack_a, node_list **stack_b)
{
	int box;
	int highest_bit;
	int run_numbers;
	int next_bit;
	int len;

	next_bit = 0;
	highest_bit = get_highest_bit(stack_a);
	while (next_bit < highest_bit)
	{
		len = count_len(stack_a);
		run_numbers = 0;
		while (run_numbers < len && !check_order(stack_a))
		{
			if ((((*stack_a)->index >> next_bit) & 1) == 1)
				rotate(stack_a, 'a');
			else
				push(stack_a, stack_b, 'b');
			run_numbers++;
		}
		while (*stack_b != NULL)
			push(stack_a, stack_b, 'a');
		next_bit++;
	}
}

void bubble_sort(int *ordened, int len)
{
	int i;
	int temp;

	i = 0;
	len--;
	while (i < len)
	{
		if (ordened[i] > ordened[i + 1])
		{
			temp = ordened[i];
			ordened[i] = ordened[i + 1];
			ordened[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int greb_middle(node_list **stack_a, int len)
{
	node_list *temp;
	int *ordened;
	int middle;
	int i;
	int *new;
	int half;

	half = len / 2;
	i = 0;
	ordened = malloc(sizeof(int) * len);
	temp = *stack_a;
	while (i < len && temp)
	{
		ordened[i] = temp->value;
		temp = temp->next;
		i++;
	}
	bubble_sort(ordened, len);
	return (ordened[half - 1]);
}
