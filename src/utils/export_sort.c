/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 02:47:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:12:10 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	compare_env_keys(const void *a, const void *b)
{
	t_env	*env_a;
	t_env	*env_b;
	int		len_a;
	int		len_b;
	int		max_len;

	env_a = *(t_env **)a;
	env_b = *(t_env **)b;
	len_a = ft_strlen(env_a->key);
	len_b = ft_strlen(env_b->key);
	if (len_a > len_b)
		max_len = len_a;
	else
		max_len = len_b;
	return (ft_strncmp(env_a->key, env_b->key, max_len));
}

t_env	**create_sorted_env_array(t_minishell *shell, int *count)
{
	t_env	**env_array;
	t_env	*current;
	int		i;

	*count = 0;
	current = shell->env_list;
	while (current)
	{
		(*count)++;
		current = current->next;
	}
	env_array = malloc(sizeof(t_env *) * (*count));
	if (!env_array)
		return (NULL);
	current = shell->env_list;
	i = 0;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}
	qsort(env_array, *count, sizeof(t_env *), compare_env_keys);
	return (env_array);
}

static void	print_export_entry(t_env *env)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(env->key, STDOUT_FILENO);
	if (env->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("=\"\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
}

int	print_export_env(t_minishell *shell)
{
	t_env	**env_array;
	int		count;
	int		i;

	env_array = create_sorted_env_array(shell, &count);
	if (!env_array)
		return (1);
	i = 0;
	while (i < count)
	{
		print_export_entry(env_array[i]);
		i++;
	}
	free(env_array);
	return (0);
}
